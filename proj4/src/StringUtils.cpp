#include "StringUtils.h"

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept {
    // Treat end = 0 as NONE
    // Start & End == 0
    if (start == 0 && end == 0) {
        return str;
    }
    
    // Start is more than str length
    if (start >= (int) str.length()) {
        return "";
    }

    // Case: Start == End && Start > End
    // Mangos --> Slice(3,1) --> ""
    // Mangos --> Slice(1,1) --> ""
    if (start >= end && ((start >= 0 && end > 0) || (start < 0 && end <= 0)) && end != 0) {
        return "";
    }

    // Case: Start > 0 & end < 0, but end becomes to the left of or at start
    if (start >= 0 && end < 0 && ((int) str.length() + end <= start) && end != 0) {
        return "";
    }

    // Case: Start < 0 & end > 0, but start goes to the right of or at end
    if (start < 0 && end > 0 && ((int) str.length() + start >= end) && end != 0) {
        return "";
    }

    std::string newStr;
    int b, e;

    // No end is given
    if (start > 0 && end == 0) {
        b = start;
        e = (int) str.length();
    }
    
    // Case: End <= 0 & Start < 0 && start and end are within negative index bounds
    // Mangos --> Slice(-5, -2) --> ang
    else if (start < 0 && end <= 0 && ((int) str.length() + start >= 0) && ((int) str.length() + end >= 0) && (int) str.length() > 1) {
        b = str.length() + start;
        e = str.length() + end;
    }

    // Case: Start > 0 & End < 0 BUT negative index of End does not pass start
    else if (start >= 0 && end < 0 && ((int) str.length() + end > start) && start <= (int) str.length() && (end * - 1 <= (int) str.length())) {
        b = start;
        e = str.length() + end;
    }

    else if (start >= 0 && end > 0 && (end >= (int) str.length())) {
        b = start;
        e = str.length();
    }

    // Case: Start < 0 && End > 0 BUT negative index of Start does not pass end
    else if (start < 0 && end >= 0 && ((int) str.length() + start < end) && end <= (int) str.length() && (start * -1 <= (int) str.length())) {
        b = str.length() + start;
        e = end;
    }
    
    // Case: Start < 0, but goes beyond negative indices & end is beyond str length
    // Mangos --> Slice(-8, 8) --> "Mangos"
    else if (start < 0 && (start * -1 >= (int) str.length()) && end >= (int) str.length()) {
        b = 0;
        e = str.length();
    }

    // Case: Start < 0, but stays within negative indices & end is beyond str length
    else if (start < 0 && (start * -1 < (int) str.length()) && end >= (int) str.length() && (int) str.length() + start < end) {
        b = str.length() + start;
        e = str.length();
    }

    // Case: Start < 0, but stays within negative indices & end is within str length
    else if (start < 0 && (start * -1 < (int) str.length()) && (end < (int) str.length()) && ((int) str.length() + start < end) && end >= 0) {
        b = str.length() + start;
        e = end;
    }

    else if (start < 0 && (start * -1 >= (int) str.length()) && end < 0 && (end * -1 <= (int) str.length())) {
        b = 0;
        e = str.length() + end;
    }

    // Case Start < 0, but goes beyond negative indices & end is within str length
    // Mangos --> Slice(-10, 3) --> "Man"
    else if (start < 0 && (start * -1 >= (int) str.length()) && end <= (int) str.length() && end >= 0) {
        b = 0;
        e = end;
    }

    // Case: start & end are positive and start < end
    // Mangos --> Slice(0,3) --> Man
    else {
        b = start;
        e = end;
    }

    while (b < e) {
        newStr += str[b];
        b++;
    }

    return newStr;
}

std::string Capitalize(const std::string &str) noexcept {
    // Case: Empty - "" --> "" (No change)
    // Case: All letters - "abc" --> "Abc"
    // Case: Some non-letters i.e "1bc-" --> "1bc-" (No change)
    // Case: No letters i.e "123" --> "123"
    // Case: Capital exists - "AbCd" --> "Abcd"
    // Case: Capital is not first char - "aBCD"
    // Case: No capital - "hello"
    
    if (str == "") {
        return str;
    }

    std::string newStr;
    int len = str.length();
    int i = 0;

    while (i < len) {
        // Nonalphabetical Chars
        if (!(('A' <= str[i] && str[i] <= 'Z') || ('a' <= str[i] && str[i] <= 'z'))) {
            newStr += str[i];
        }
        // First Char not capital, but in alphabet
        else if ((i == 0) && !(('A' <= str[i] && str[i] <= 'Z'))) {
            newStr += str[i] - 32;
        }
        // Capital first char
        else if ((i == 0) && (('A' <= str[i] && str[i] <= 'Z'))) {
            newStr += str[i];
        }
        // Capital chars that aren't first
        else if ('A' <= str[i] && str[i] <= 'Z') {
            newStr += str[i] + 32;
        }
        // Lowercase chars that aren't first
        else {
            newStr += str[i];
        }

        i++;
    }
    return newStr;
}

std::string Upper(const std::string &str) noexcept {
    // Case: All letters - "abcde" --> "ABCDE"
    // Case: Some non-letters - "abc1de" --> "ABC1DE"
    // Case: No letters - "12345" --> "12345" (No change)
    // Case: Empty - "" --> "" (No change)
    
    if (str == "") {
        return str;
    }

    std::string newStr;
    int len = str.length();
    int i = 0;

    while (i < len) {
        // Nonalphabetical Characters --> Add
        if (!(('a' <= str[i] && str[i] <= 'z') || ('A' <= str[i] && str[i] <= 'Z'))) {
            newStr += str[i];
        }

        // Lowercase Alphabet --> Convert to upper
        else if (('a' <= str[i] && str[i] <= 'z')) {
            newStr += str[i] - 32;
        }
        
        // Uppercase Alphabet --> Add
        else {
            newStr += str[i];
        }
        i++;
    }
    return newStr;
}

std::string Lower(const std::string &str) noexcept {
    // Case: All lower letters - "efasd" --> "efasd"
    // Case: All upper letters - "EFASD" --> "efasd"
    // Case: Mix of lower & upper - "EfAsDe" --> "efasde"
    // Case: No letters - "12345-4;" --> "12345-4;"
    // Case: Empty - "" --> ""

    if (str == "") {
        return str;
    }

    std::string newStr;
    int len = str.length();
    int i = 0;

    while (i < len) {
        // Nonalphabetical Characters --> Add
        if (!(('a' <= str[i] && str[i] <= 'z') || ('A' <= str[i] && str[i] <= 'Z'))) {
            newStr += str[i];
        }

        // Uppercase Alphabet --> Convert to lower
        else if (('A' <= str[i] && str[i] <= 'Z')) {
            newStr += str[i] + 32;
        }
        
        // Lowercase Alphabet --> Add
        else {
            newStr += str[i];
        }
        i++;
    }

    return newStr;
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
    // Case: Left Side Whitespace - "    dogs" --> "dogs"
    // Case: Right Side Whitespace - "cats    " --> "cats"
    // Case: Both Side Whitespace - "    mice    " --> "mice"
    // Case: No Whitespace - "hamster" --> "hamster"
    // Case: Empty - "" --> ""
    
    if (str == "") {
        return str;
    }

    std::string newStr;
    int first_char_idx = 0;
    int last_char_idx = 0;
    bool first = false;

    // Finds first & last char indices(
    for (int i = 0; i < (int) str.length(); i++) {
        if (!isspace(str[i]) && !first) {
            first_char_idx = i;
            first = true;
        }

        if (!isspace(str[i])) {
            last_char_idx = i;
        }
    }

    if (first_char_idx == last_char_idx && isspace(str[last_char_idx])) {
        return "";
    }
    
    // Recreates str w/o left & right whitespace
    while (first_char_idx <= last_char_idx) {
        newStr += str[first_char_idx];
        first_char_idx++;
    }

    return newStr;
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    // Case: Even Width + Even str len --> (Width - str.length()) // 2 on left & right
    // i.e Center("banana", 10, " ") --> "  banana  "
    
    // Case: Odd Width + Even str len --> 1 + (Width - str.length()) // 2 on left & (Width - str.length()) // 2 on right
    // i.e Center("banana", 9, " ") --> "  banana "
    
    // Case: Even Width + Odd str len --> (Width - str.length()) // 2 on left & 1 + (Width - str.length()) // 2 on right
    // i.e Center("hello", 10, " ") --> "  hello   "
    
    // Case: Odd Width + Odd str len --> (Width - str.length()) // 2 on left & right
    // i.e Center("hello", 11, " ") --> "   hello   "
    
    // Case: Width = 0 --> Return str
    // Case: Width < str.length() --> Return str
    // Case: Empty - "" --> Make empty str of width length
    
    if (width <= (int) str.length()) {
        return str;
    }

    std::string newStr;
    int len = str.length();
    int left, right;
    int count = 0;

    // Even width & even len || Odd width & Odd len
    if ((width % 2 == 0 && len % 2 == 0) || (width % 2 == 1 && len % 2 == 1)) {
        left = (width - len) / 2;
        right = (width - len) / 2;
    }
    // Odd width & even len
    if (width % 2 == 1 && len % 2 == 0) {
        left = 1 + (width - len) / 2;
        right = (width - len) / 2;
    }
    // Even width & odd len
    if (width % 2 == 0 && len % 2 == 1) {
        left = (width - len) / 2;
        right = 1 + (width - len) / 2;
    }

    // Adds fill char to left of word
    while (count < left) {
        newStr += fill;
        count++;
    }

    // Adds word & resets count
    newStr += str;
    count = 0;

    // Adds fill char to right of word
    while (count < right) {
        newStr += fill;
        count++;
    }

    return newStr;
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    // Case: Width <= str.length() --> Return str
    // Case: Width > str.length() --> (Width - str.length()) on right

    if (width <= (int) str.length()) {
        return str;
    }

    std::string newStr;
    int right = (width - (int) str.length());
    int count = 0;

    // Fills right side of word with fill char
    newStr += str;
    while (count < right) {
        newStr += fill;
        count++;
    }

    return newStr;
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    // Case: width <= str.length() --> Return str
    // Case: width > str.length() --> (width - str.length()) on left

    if (width <= (int) str.length()) {
        return str;
    }

    std::string newStr;
    int left = (width - (int) str.length());
    int count = 0;

    // Fills left of word w/ fill chars
    while (count < left) {
        newStr += fill;
        count++;
    }

    newStr += str;
    return newStr;
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

std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    // Case: Empty vector --> Return ""
    // Case: Nonempty vector --> Use str as separator

    // No strings --> Return empty string
    if (vect.empty()) {
        return "";
    }

    std::string newStr;
    std::vector<std::string> reverse = vect;
    std::vector<std::string> temp;

    // Reverses order of strings for popping next
    while (!reverse.empty()) {
        temp.push_back(reverse.back());
        reverse.pop_back();
    }

    // Pops strings in order & adds str as separator if string is not last in vector
    while (!temp.empty()) {
        newStr += temp.back();
        temp.pop_back();
        if (!temp.empty()) {
            newStr += str;
        }
    }

    return newStr;
}

// Concept Link: https://stackoverflow.com/questions/34546171/python-expandtabs-string-operation
std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    // Case: tabsize = 0 --> Remove tabs
    // How tabstops work: tabsize determines where the next chars should go
    // i.e tabsize 10 --> Should be 10 spaces between chars & where tab is
    // i.e ExpandTabs("hell\to", 6) --> "hell  o" (6 chars from tab: hell & 2 whitespace)

    std::string newStr;
    int i = 0;
    int count = 0;

    // Goes through string
    while (i < (int) str.length()) {
        // If tab --> Add whitespace till tabstop is hit
        if (str[i] == 9) {
            while (count < tabsize) {
                newStr += " ";
                count++;
            }
            count = 0; // Reset char count at tab
        }
        // Adds chars if not tab stop
        else {
            newStr += str[i];
            count++;
            // If char count equals tabsize, reset char count (To maintain tabstop distance)
            if (count == tabsize) {
                count = 0;
            }
        }
        i++;
    }

    return newStr;
}

// https://en.wikipedia.org/wiki/Levenshtein_distance#Definition
// NeetCode Concept Explanation: https://www.youtube.com/watch?v=XYi2-LPrwm4
int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    // Minimum number of single-char edits needed to change one word into the other

    // Base Case: Left = Empty --> Need to insert chars equal to right
    if (left == "") {
        return (int) right.length();
    }

    // Base Case: Right = Empty --> Need to insert chars equal to left
    if (right == "") {
        return (int) left.length();
    }

    // Initialize vector<vector<int>> to have matrix
    // https://stackoverflow.com/questions/12375591/vector-of-vectors-to-create-matrix

    // Allocate one extra row & col in matrix to allow for empty strings
    std::vector<std::vector<int>> matrix(left.length() + 1, std::vector<int> (right.length() + 1));

    // Compares left to empty str & sets first row - Base Case
    for (int i = 0; i <= (int) left.length(); i++) {
        matrix[i][0] = i;
    }

    // Compares right to empty str & sets first col - Base Case
    for (int j = 0; j <= (int) right.length(); j++) {
        matrix[0][j] = j;
    }

    // Start at (1,1) cause (0,0) is comparison of two empty strs, (1,0) is our first char of right compared to empty str, & (0,1) is first char of left compared to empty str
    for (int k = 1; k <= (int) left.length(); k++) {
        for (int l = 1; l <= (int) right.length(); l++) {
            // // Insert, Delete, Replace
            
            // matrix[k][l-1] - Insert (Char from right that was needed has been inserted, thus it grabs changes made to left before a char from right was inserted)
            // matrix[k-1][l] - Delete (Char from left has been deleted, thus it grabs changes made to right before the left char deletion)
            // matrix[k-1][l-1] - Replace (One of the chars has been replaced from either string, so it moves up & left to the amount of changes needed to make the substrs without those two chars)

            // Just keep on finding the minimum between insert, delete, & replace based on previous changes
            if (!ignorecase) {
                // Check if char in left is in right --> If it is, we do not need to replace that char in left
                // Need to do (k-1) & (l-1) as 1 is not the index of the first char of the string
                if (left[k - 1] == right[l - 1]) {
                    matrix[k][l] = std::min(std::min(matrix[k][l-1] + 1, matrix[k-1][l] + 1), (matrix[k-1][l-1]));
                }
                // Otherwise, it is not in right & we need to replace that char so we do matrix[k-1][l-1] + 1 to indicate that
                else {
                    matrix[k][l] = std::min(std::min(matrix[k][l-1] + 1, matrix[k-1][l] + 1), (matrix[k-1][l-1] + 1));
                }
            }

            else {
                // Does same as above, but checks for alphabetical casing
                if (left[k - 1] == right[l - 1] || left[k - 1] + 32 == right[l - 1] || left[k - 1] == right[l - 1] + 32) {
                    matrix[k][l] = std::min(std::min(matrix[k][l-1] + 1, matrix[k-1][l] + 1), (matrix[k-1][l-1]));
                }
                else {
                    matrix[k][l] = std::min(std::min(matrix[k][l-1] + 1, matrix[k-1][l] + 1), (matrix[k-1][l-1] + 1));
                }
            }
        }
    }

    // Returns last result of matrix as it will be the edit distance
    return matrix[(int) left.length()][(int) right.length()];
}

};