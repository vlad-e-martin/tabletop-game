#ifndef CSV_READER_H
#define CSV_READER_H

#include <filesystem>
#include <string>
#include <vector>

namespace GUI {
    enum CsvState {
        UnquotedField,
        QuotedField
    };
    
    class CsvReader {
    public:
        CsvReader(const std::string& csvFilePath) : m_filePath(csvFilePath) {
            m_isFileGood = std::filesystem::exists(csvFilePath); 
        }
        std::vector<std::vector<uint32_t>> readCsv_uint32();

    private:
        std::vector<std::string> readCsvRow(const std::string& row);
        
        std::string m_filePath;
        bool m_isFileGood;
    };
}

#endif // CSV_READER_H