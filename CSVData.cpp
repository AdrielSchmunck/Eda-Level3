/*
 * Reads and writes CSV files
 *
 * Copyright (C) 2022 Marc S. Ressl
 */

#include <fstream>

#include "CSVData.h"

using namespace std;

bool readCSV(const string path, CSVData &data)
{
    ifstream file(path);

    if (!file.is_open())
        return false;

    file.seekg(0, ios::end);
    int fileSize = file.tellg();
    char *fileData = new char[fileSize];
    file.seekg(0);
    file.read(&fileData[0], fileSize);

    bool inQuotes = false;
    bool lastQuote = false;

    string field;
    vector<string> fields;

    for (int i = 0; i < fileSize; i++)
    {
        char c = fileData[i];

        if (lastQuote && c != '"')
            inQuotes = !inQuotes;

        if (c == '"')
        {
            if (lastQuote)
            {
                field += c;
                lastQuote = false;
            }
            else
                lastQuote = true;
        }
        else if (c == ',')
        {
            if (inQuotes)
                field += c;
            else
            {
                fields.push_back(field);
                field.clear();
            }

            lastQuote = false;
        }
        else if ((c == '\n') || (c == '\r'))
        {
            if (field.size())
                fields.push_back(field);
            field.clear();

            if (fields.size())
                data.push_back(fields);
            fields.clear();

            inQuotes = false;
            lastQuote = false;
        }
        else
        {
            field += c;
            lastQuote = false;
        }
    }

    if (field.size())
        fields.push_back(field);
    if (fields.size())
        data.push_back(fields);

    delete[] fileData;

    return true;
}

bool writeCSV(const string path, CSVData &data)
{
    ofstream file(path);

    if (!file.is_open())
        return false;

    for (auto fields : data)
    {
        string line;

        bool isFirstField = true;
        for (auto field : fields)
        {
            if (!isFirstField)
                line += ',';
            else
                isFirstField = false;

            // Replaces " with ""
            size_t pos = 0;
            while ((pos = field.find('"', pos)) != std::string::npos)
            {
                field.replace(pos, 1, "\"\"");
                pos += 2;
            }

            line += '"' + field + '"';
        }

        line += '\n';

        file.write(line.c_str(), line.size());

        if (!file.good())
            return false;
    }

    return true;
}
