/*
 * Reads and writes CSV files
 *
 * Copyright (C) 2022 Marc S. Ressl
 */

#ifndef _CSVDATA_H
#define _CSVDATA_H

#include <list>
#include <string>
#include <vector>

typedef std::list<std::vector<std::string>> CSVData;

bool readCSV(const std::string path, CSVData &data);
bool writeCSV(const std::string path, CSVData &data);

#endif
