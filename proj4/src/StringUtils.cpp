#include "StringUtils.h"
#include <algorithm>

namespace StringUtils{

    std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept {

        std::string slice;

        // find lower bound equivalent for negative index
        if(start < 0) {
            start = start + str.length();
        }

        // find upper bound equivalent for negative index
        if(end < 0) {
            end = end + str.length();
        }

        // sets slice to end of string if end is 0 or larger than string
        if(end == 0 || end > str.length()) {
            end = str.length();
        }

        for(int i = start; i < end; i++) {
            slice += str[i];
        }

        return slice;
    }

    std::string Capitalize(const std::string &str) noexcept {
        
        std::string capitalized = str;

        // capitalize first letter
        capitalized[0] = toupper(capitalized[0]);

        // uncapitalize all other characters
        for(int i = 1; i < capitalized.length(); i++) {
            capitalized[i] = tolower(capitalized[i]);
        }

        return capitalized;
    }

    std::string Upper(const std::string &str) noexcept {
        
        std::string upper = str;

        for(int i = 0; i < upper.length(); i++) {
            upper[i] = toupper(upper[i]);
        }

        return upper;
    }

    std::string Lower(const std::string &str) noexcept {

        std::string lower = str;

        for(int i = 0; i < lower.length(); i++) {
            lower[i] = tolower(lower[i]);
        }

        return lower;
    }

    std::string LStrip(const std::string &str) noexcept {

        std::string stripped = str;

        // break when there is no more whitespace on left side
        int i = 0;
        while(true) {
            if(!isspace(stripped[i]) || i == stripped.length()) {
                break;
            }
            else {
                i++;
            }
        }

        // strip left side whitespaces using Slice()
        stripped = Slice(stripped, i);

        return stripped;
    }

    std::string RStrip(const std::string &str) noexcept {
        
        std::string stripped = str;

        // break when there is no more whitespace on left side
        int i = str.length() - 1;
        while(true) {
            if(!isspace(stripped[i]) || i == 0) {
                break;
            }
            else {
                i--;
            }
        }

        // strip right side whitespaces using Slice()
        stripped = Slice(stripped, 0, i + 1);

        if(stripped.length() == 1 && isspace(stripped[0])) {
            return "";
        }

        return stripped;
    }

    std::string Strip(const std::string &str) noexcept {
        
        std::string stripped = str;

        stripped = LStrip(stripped);
        stripped = RStrip(stripped);

        return stripped;
    }

    std::string Center(const std::string &str, int width, char fill) noexcept {

        int whiteSpacesRequired = width - str.length();
        if(whiteSpacesRequired < 0) {
            return str;
        }

        std::string formatted = str;

        if(whiteSpacesRequired % 2 == 0) {
            for(int i = 0; i < whiteSpacesRequired / 2; i++) {
                formatted = fill + formatted + fill;
            }
        }
        else {
            for(int i = 0; i < (whiteSpacesRequired - 1) / 2; i++) {
                formatted = fill + formatted + fill;
            }
            formatted = formatted + fill;
        }

        return formatted;
    }

    std::string LJust(const std::string &str, int width, char fill) noexcept {
        
        int whiteSpacesRequired = width - str.length();
        if(whiteSpacesRequired < 0) {
            return str;
        }

        std::string formatted = str;

        for(int i = 0; i < whiteSpacesRequired; i++) {
                formatted = formatted + fill;
            }

        return formatted;
    }

    std::string RJust(const std::string &str, int width, char fill) noexcept {
        
        int whiteSpacesRequired = width - str.length();
        if(whiteSpacesRequired < 0) {
            return str;
        }

        std::string formatted = str;

        for(int i = 0; i < whiteSpacesRequired; i++) {
                formatted = fill + formatted;
            }

        return formatted;
    }

    std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept {
        
        std::string replaced;
        std::vector<std::string> stringQueue;

        int pos;
        std::string tmpStr = str;
        while(true) {
            pos = tmpStr.find(old);

            if(pos != std::string::npos) {
                // add substring before match to queue
                if(pos > 0) {
                    stringQueue.push_back(Slice(tmpStr, 0, pos));
                    tmpStr = Slice(tmpStr, pos);
                    continue;
                }
                // remove found match at start of string and add replacement to queue
                tmpStr = Slice(tmpStr, old.length() - pos); 
                stringQueue.push_back(rep);
                //repeat
            }
            else {
                // add remainder of string to queue
                stringQueue.push_back(tmpStr);
                break;
            }

        }

        // combine queue into string
        for(std::string elm : stringQueue) {
            replaced += elm;
        }

        return replaced;
    }

    std::vector<std::string> Split(const std::string &str, const std::string &splt) noexcept {
        
        std::vector<std::string> splitStrings = {};

        std::string tmpStr = str;

        if(splt == "") {
            tmpStr = tmpStr + " ";
            tmpStr = LStrip(tmpStr);
            std::string buf = "";
                for(int i = 0; i < tmpStr.length() - 1; i++) {
                    if(isspace(tmpStr[i]) && !isspace(tmpStr[i+1])) {
                        splitStrings.push_back(buf);
                        buf = "";
                        continue;
                    }
                    else if(isspace(tmpStr[i]) && isspace(tmpStr[i+1])) {
                        continue;
                    }
                    else {
                        buf += tmpStr[i];
                    }
                }
                splitStrings.push_back(buf);
            }
        else {
            int pos;
            // we do a little bit of flag tomfoolery 
            int flag = true;
            while(true) {
                pos = tmpStr.find(splt);
                if(pos != std::string::npos) {
                    // add substring before match to vector
                    if(pos > 0) {
                        splitStrings.push_back(Slice(tmpStr, 0, pos));
                        tmpStr = Slice(tmpStr, pos);
                        flag = false;
                        continue;
                    }
                    else if(pos == 0) {
                        if(splt.length() - tmpStr.length() == 0 || 
                            Slice(tmpStr, pos + splt.length()).find(splt) == 0) {
                            flag = true;
                        }
                        if(flag) {
                            splitStrings.push_back("");
                            flag = false;
                        }
                    }
                    // remove found split match
                    tmpStr = Slice(tmpStr, splt.length() - pos); 
                    //repeat
                }
                else {
                    // add remainder of string to vector
                    if(tmpStr != ""){
                        splitStrings.push_back(tmpStr);
                    }
                    break;
                }

            }

        }

        std::string lastString = splitStrings[splitStrings.size() - 1];
        splitStrings[splitStrings.size() - 1] = Slice(lastString, 0, lastString.size());

        return splitStrings;
    }

    std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept {
        
        std::string joined;

        for(int i = 0; i < vect.size(); i++) {
                joined += vect[i];
            if(i != vect.size() - 1) {
                joined += str;
            }
        }

        return joined;
    }

    std::string ExpandTabs(const std::string &str, int tabsize) noexcept {
        
        std::string tabbed; 
        std::vector<std::string> strings = Split(str, "\t");
        int requiredSpaces;
        std::string spaces; 
        for(int i = 0; i < strings.size(); i++) {
            requiredSpaces = 0;
            spaces = "";
            if(i != strings.size() - 1) {
                requiredSpaces = tabsize - strings[i].length() % tabsize;
            }
            for(int j = 0; j < requiredSpaces; j++) {
                spaces += " ";
            }
            tabbed += strings[i] + spaces;
        } 
        return tabbed;
    }

    int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept {
        
        int editDistance = 0; 

        std::string tmpLeft = left;
        std::string tmpRight = right;
        if(ignorecase) {
            tmpLeft = Lower(left);
            tmpRight = Lower(right);
        }

        // dynamic programming solution implemented with reference to https://en.wikipedia.org/wiki/Levenshtein_distance#Example
        // under the "Iterative with full matrix" section
        // ~
        // this following video was also used as reference to understand the algorithm
        // https://www.youtube.com/watch?v=Cu7Tl7FGigQ

        // edit distance matrix
        const int row = tmpLeft.length() + 1;
        const int col = tmpRight.length() + 1;
        int EDMatrix[row][col];

        // initialize row 0 and column 0
        for(int i = 0; i < row; i++) {
            EDMatrix[i][0] = i;
        }
        for(int i = 0; i < col; i++) {
            EDMatrix[0][i] = i;
        }

        // calculates edit distance for sub-problems in matrix
        for(int i = 1; i < row; i++) {
            for(int j = 1; j < col; j++) {
                // check if characters are same
                if(tmpLeft[i-1] == tmpRight[j-1]) {
                    EDMatrix[i][j] = EDMatrix[i-1][j-1];
                }
                else {
                    int a = EDMatrix[i-1][j];
                    int b = EDMatrix[i][j-1];
                    int c = EDMatrix[i-1][j-1];

                    EDMatrix[i][j] = std::min({a, b, c}) + 1;
                }
            }
        }

        editDistance = EDMatrix[row-1][col-1];

        return editDistance;
    }

};