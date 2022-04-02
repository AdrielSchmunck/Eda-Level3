/*
 * Lequel?
 *
 * Copyright (C) 2022 Marc S. Ressl
 *
 * Language identification based on trigrams.
 */

#include <iostream>
#include <map>
#include <string>

#include "raylib.h"

#include "CSVData.h"
#include "Lequel.h"

using namespace std;

const string LANGUAGECODE_NAMES_FILE = "C:/Users/USUARIO/source/repos/level3/resources/languagecode_names_es.csv";
const string TRIGRAMS_PATH = "C:/Users/USUARIO/source/repos/level3/resources/trigrams/";

/*
 * Loads trigram data.
 *
 * Parameters:
 *  languageCodeNames -     Map of language code vs. language name (in user's locale).
 *  trigramProfiles -       The trigram profiles.
 *
 * Returns: whether the function succeeded.
 */
bool loadLanguagesData(map<string, string> &languageCodeNames, Languages &languages)
{
    // Reads available language codes
    cout << "Reading language codes..." << endl;

    CSVData languageCodesCSVData;
    if (!readCSV(LANGUAGECODE_NAMES_FILE, languageCodesCSVData))
        return false;

    // Reads trigram profile for each language code
    for (auto fields : languageCodesCSVData)
    {
        if (fields.size() != 2)
            continue;

        string languageCode = fields[0];
        string languageName = fields[1];

        languageCodeNames[languageCode] = languageName;

        cout << "Reading trigram profile for language code \"" << languageCode << "\"..." << endl;

        CSVData languageCSVData;
        if (!readCSV(TRIGRAMS_PATH + languageCode + ".csv", languageCSVData))
            return false;

        languages.push_back(Language());
        Language &language = languages.back();

        language.languageCode = languageCode;

        for (auto fields : languageCSVData)
        {
            if (fields.size() != 2)
                continue;

            string trigram = fields[0];
            float frequency = (float)stoi(fields[1]);

            language.trigramProfile[trigram] = frequency;
        }

        normalizeTrigramProfile(language.trigramProfile);
    }

    return true;
}

int main(int, char *[])
{
    map<string, string> languageCodeNames;
    Languages languages;

    if (!loadLanguagesData(languageCodeNames, languages))
    {
        cout << "Could not load trigram data." << endl;
        return 1;
    }

    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Lequel?");

    SetTargetFPS(60);

    string languageCode = "---";

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_V) &&
            (IsKeyDown(KEY_LEFT_CONTROL) ||
             IsKeyDown(KEY_RIGHT_CONTROL) ||
             IsKeyDown(KEY_LEFT_SUPER) ||
             IsKeyDown(KEY_RIGHT_SUPER)))
        {
            const char *clipboard = GetClipboardText();

            Text text;
            getText(clipboard, text);

            languageCode = identifyLanguage(text, languages);
        }

        if (IsFileDropped())
        {
            int count;
            char **droppedFiles = {0};
            droppedFiles = GetDroppedFiles(&count);

            Text text;
            getTextFromFile(droppedFiles[0], text);

            languageCode = identifyLanguage(text, languages);

            ClearDroppedFiles();
        }

        BeginDrawing();

        ClearBackground(BEIGE);

        DrawText("Lequel?", 80, 80, 128, BROWN);
        DrawText("Copia y pega con Ctrl+V, o arrastra un archivo...", 80, 220, 24, BROWN);

        string languageString;
        if (languageCode != "---")
        {
            if (languageCodeNames.find(languageCode) != languageCodeNames.end())
                languageString = languageCodeNames[languageCode];
            else
                languageString = "Desconocido";
        }

        int languageStringWidth = MeasureText(languageString.c_str(), 48);
        DrawText(languageString.c_str(), (screenWidth - languageStringWidth) / 2, 315, 48, BROWN);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
