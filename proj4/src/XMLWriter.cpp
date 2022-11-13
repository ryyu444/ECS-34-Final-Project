#include "XMLWriter.h"
#include "StringUtils.h"
#include <expat.h>
#include <iostream>

struct CXMLWriter::SImplementation {

    public: 
        std::shared_ptr<CDataSink> m_sink;
        std::vector<std::string> open_tags;

        SImplementation(std::shared_ptr<CDataSink> sink) {
            m_sink = sink;
        }

};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink) {
    DImplementation = std::make_unique<SImplementation>(SImplementation(sink));
}

CXMLWriter::~CXMLWriter() {};

bool CXMLWriter::Flush() {
    // Goes through any remaining Start Tags that aren't closed
    for (int i = DImplementation -> open_tags.size() - 1; i > -1; i--) {
        std::string close_tag = "</" + DImplementation -> open_tags[i] + ">";
        std::vector<char> close_tag_chars;

        // Puts close_tag into vector of chars for writing
        for (int j = 0; j < close_tag.length(); j++) {
            close_tag_chars.push_back(close_tag[j]);
        }
        // Writes the closing tag to the sink
        DImplementation -> m_sink -> Write(close_tag_chars);
    }

    // Clears all of the start tags
    DImplementation -> open_tags.clear();

    return true;
}

void replaceSpecialChars(std::string& str) {
    const std::vector<std::tuple<std::string, std::string, std::string>> specialChars = 
           {{"&amp;", "&#38;", "&"},
            {"&quot;", "&#34;", "\""}, 
            {"&apos;", "&#39;", "\'"}, 
            {"&lt;", "&#60;", "<"}, 
            {"&gt;", "&#62;", ">"},};

    for(int i = 0; i < specialChars.size(); i++) {
        str = StringUtils::Replace(str, std::get<2>(specialChars[i]), std::get<0>(specialChars[i]));
    }
}

// Write stuff in entity into the sink
bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    using EType = SXMLEntity::EType;
    std::string entity_str;
    
    // Start element: Keeps track of start tag & adds to entity_str <name atts>
    if (entity.DType == EType::StartElement) {
        DImplementation -> open_tags.push_back(entity.DNameData);
        entity_str = '<' + entity.DNameData;
    }

    // End element: </name>
    if (entity.DType == EType::EndElement) {
        // start tag != end tag --> Invalid str --> Clear all & return false
        bool start_found = false;
        for (int j = 0; j < DImplementation -> open_tags.size(); j++) {
            if (DImplementation -> open_tags[j] == entity.DNameData) {
                DImplementation -> open_tags.erase(DImplementation -> open_tags.begin() + j);
                start_found = true;
                break;
            }
        }
        
        // Reach end of StartElement tags --> EndElement doesn't match w/ existing StartElements
        if (!start_found) {
            return false;
        }

        // start tag == end tag: Adds closing tag & puts chars into vector of chars for writing
        entity_str += "</" + entity.DNameData;
    }

    // Char data: Write name directly to entity_str i.e who
    if (entity.DType == EType::CharData) {
        std::string tmpCharData = entity.DNameData;
        replaceSpecialChars(tmpCharData);
        entity_str += tmpCharData;
    }

    // Complete Element: Add <name attributes/>
    if (entity.DType == EType::CompleteElement) {
        entity_str = '<' + entity.DNameData;
    }

    // Entity has attributes: Add to the current tag
    if (entity.DAttributes.size() && entity.DType != EType::CharData && entity.DType != EType::EndElement) {
        for (auto &Att : entity.DAttributes) {
            std::string tmpVal = std::get<1>(Att);
            replaceSpecialChars(tmpVal);
            entity_str += ' ' + std::get<0>(Att) + '=' + '\"' + tmpVal + '\"';
        }
    }
    
    // Complete Element: Add / before closing the tag
    if (entity.DType == EType::CompleteElement) {
        entity_str += '/';
    }

    // Closes the tag if not CharData
    if (entity.DType != EType::CharData) {
        entity_str += '>';
    }

    // Vector of chars for entity str
    std::vector<char> ent_chars;

    // Stores chars in entity_str
    for (int i = 0; i < entity_str.length(); i++) {
        ent_chars.push_back(entity_str[i]);
    }
    
    // Writes entity_str into sink & clear strings for writing other entities
    DImplementation -> m_sink -> Write(ent_chars);

    return true;
}