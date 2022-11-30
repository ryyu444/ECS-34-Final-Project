#include "DSVReader.h"
#include "StringUtils.h"
#include <iostream>

struct CDSVReader::SImplementation {

    public: 
        std::shared_ptr<CDataSource> m_src;
        char m_delim;
        
        SImplementation(std::shared_ptr<CDataSource> src, char delimiter) {
            m_src = src;
            m_delim = delimiter;
        }
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter) {
    DImplementation = std::make_unique<SImplementation>(SImplementation(src, delimiter));
}

CDSVReader::~CDSVReader() {}

bool CDSVReader::End() const {
    return DImplementation->m_src->End();
}

bool CDSVReader::ReadRow(std::vector<std::string> &row) {
    // Flushes out the vector before reading in a new row
    row.clear();

    // returns false if empty src
    if(End()) {
        return false;
    }

    // Buf: String to store chars grabbed from src
    // quoteOpen: Keeps track of whether a double quote has been seen or not
    std::string buf = "";
    char tmpChar, peekChar, prevChar;
    bool quoteOpen = false, res; 
    DImplementation->m_src->Peek(tmpChar); // peek once at start to see if empty row
    
    while(true) {
        // break when reaches end of DString or row
        if(End() || tmpChar == '\n' && !quoteOpen) {
            // Checks if buf has stuff or CUR CHAR is delim
            if(!buf.empty() || tmpChar == DImplementation -> m_delim) { // add remaining buffer to row if row isn't empty
                row.push_back(buf);
            }
            break;
        }

        // Grabs char from src & keeps track of prevChar
        prevChar = tmpChar;
        DImplementation->m_src->Get(tmpChar);
        // Peaks next char & keeps track of whether it worked or not
        res = DImplementation -> m_src -> Peek(peekChar);

        // FIXME: " " surrounding str is being read incorrectly as part of the str
        // Checks if there is are " in the string
        // Case: "(delim)""
        if(tmpChar == '\"') {
            if (!quoteOpen) {
                // Checks if delim is " && checks if left & right of cur char is not "
                if (DImplementation -> m_delim == '\"' && ((prevChar != '\"') || (peekChar != '\"'))) {
                    row.push_back(buf);
                    buf.clear();
                    continue;
                }
                else {
                    quoteOpen = true;
                    continue;
                }
            }


            if (res) {
                // "" found within quotes --> Convert to single "
                if (quoteOpen && peekChar == '\"') {
                    DImplementation -> m_src -> Get(tmpChar);
                }
                else if ((peekChar == DImplementation -> m_delim || peekChar == '\n') && quoteOpen) {
                    // DImplementation -> m_src -> Get(tmpChar);
                    quoteOpen = false;
                    continue;
                }
            }
            // Peaked return false --> End of string
            else {
                continue;
            }
        }
        
        // Delim in string & two double quotes found around the string --> Push the string into row
        else if(tmpChar == DImplementation->m_delim && quoteOpen == false) {
            row.push_back(buf);
            buf.clear();
            continue;
        }

        // Adds chars that aren't new lines into the buf string
        if (tmpChar != '\n') {
            buf += tmpChar;
        }

    }

    return true;
}