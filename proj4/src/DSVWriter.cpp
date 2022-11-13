#include "DSVWriter.h"
#include "StringUtils.h"
#include <iostream>

struct CDSVWriter::SImplementation {
    public:
        // Variables
        std::shared_ptr<CDataSink> m_sink;
        char m_delimiter;
        bool m_quoteall;
        int times_written = 0;
        // Constructor to set our struct variables
        SImplementation(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall) {
            m_sink = sink;
            m_delimiter = delimiter;
            m_quoteall = quoteall;
        }
};

CDSVWriter::CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall) {
    // Creates instance of struct to hide class members
    DImplementation = std::make_unique<SImplementation>(sink, delimiter, quoteall);
}

CDSVWriter::~CDSVWriter() {}

bool CDSVWriter::WriteRow(const std::vector<std::string> &row) {
    std::vector<std::string> all_qtd;
    bool quote = false, delim = false, new_l = false;

    // Go through row to check if quotes are needed
    for (int k = 0; k < (int) row.size(); k++) {
        // quoteall = true --> Quote all strings in row
        std::string temp;

        // Goes through string from row & checks if delim, new line, or quotes are in it
        for (int j = 0; j < row[k].length(); j++) {
            // Quotes in string --> Replace '"' with ""
            if (row[k][j] == '\"') {
                temp += std::string(2, '"');
                quote = true;
                continue;
            }
            // Delim in string --> Add "" at the end when the string is formed
            else if (row[k][j] == DImplementation -> m_delimiter) {
                delim = true;
            }
            // Newline in string --> Add "" at the end when the string is formed
            else if (row[k][j] == '\n') {
                new_l = true;
            }
            // No matching critera --> Just add the char as is
            temp += row[k][j];
        }

        // If new line or delim or quote exists, add "" to entire string
        // If quoteall --> Add "" to entire string
        if (quote || delim || new_l || DImplementation -> m_quoteall) {
            temp = '"' + temp + '"';
        }

        // Reset bools & push the string into the vector for the sink
        delim = quote = new_l = false;
        all_qtd.push_back(temp);
    }

    std::string res;
    std::vector<char> res_letters;

    // Joins row into a single string w/ delimiter
    res = StringUtils::Join(std::string(1, DImplementation -> m_delimiter), all_qtd);
    // Gets letters from result of joining row
    
    // Newline signals end of row
    if (DImplementation -> times_written != 0) {
        res_letters.push_back('\n');
    }

    // Empty vector --> Empty Row --> Add newline
    if (row.empty()) {
        return false;
    }
    
    // Adds chars from row into a vector of chars for writing
    for (int i = 0; i < (int) res.length(); i++) {
        res_letters.push_back(res[i]);
    }

    // Writes the letters into Sink
    DImplementation -> m_sink -> Write(res_letters);
    DImplementation -> times_written++;

    return true;
}
