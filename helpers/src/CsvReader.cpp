#include <helpers/CsvReader.h>

#include <fstream>
#include <iostream>

std::vector<std::string> GUI::CsvReader::readCsvRow(const std::string &row) {
    GUI::CsvState state = GUI::CsvState::UnquotedField;
    std::vector<std::string> fieldList;
    std::string currentField = "";
    for (const char& c : row) {
        // Reached a comma, save previous field and create a new one
        if (c == ',') {
            if (!currentField.empty()) {
                fieldList.push_back(currentField);
            }
            currentField = "";
        }
        else if (c != '"') {
            currentField.push_back(c);
        }
    }

    // Store the contents of the final column (no closing comma)
    if (!currentField.empty()) {
        fieldList.push_back(currentField);
    }

    return fieldList;
}

/// Read CSV file, Excel dialect. Accept "quoted fields", ""with quotes"""
std::vector<std::vector<uint32_t>> GUI::CsvReader::readCsv_uint32() {
    std::string fileRow; // Stores raw string containing single row of CSV data
    
    std::vector<std::vector<uint32_t>> processedMatrix; // Stores final processed data
    std::vector<std::string> processedRow; // Stores processed data of current row of CSV data
    std::vector<uint32_t> convertedRow; // Stores processed data, converted to uint32, of current row of CSV data

    // Return empty result if there's something wrong with the file
    if (!m_isFileGood) {
        return processedMatrix;
    }

    std::ifstream inFileStream(m_filePath);
    while (!inFileStream.eof()) {
        std::getline(inFileStream, fileRow);
        if (inFileStream.bad() || inFileStream.fail()) {
            // Return whatever was processed, then give up
            return processedMatrix;
        }
        processedRow = readCsvRow(fileRow);
        for (const std::string& processedStr : processedRow) {
            // Skip empty rows
            if (processedRow.empty()) {
                continue;
            }
            // Convert string into unsigned int
            convertedRow.push_back(strtoul(processedStr.c_str(), 0, 10));
        }
        processedMatrix.push_back(convertedRow);
        convertedRow = {};
    }
    return processedMatrix;
}