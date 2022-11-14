#include "XMLReader.h"
#include <expat.h>
#include <iostream>

struct CXMLReader::SImplementation {

    public: 
        std::shared_ptr<CDataSource> m_src;
        XML_Parser m_parser;
        static std::string c_data;

        static std::vector<SXMLEntity> s_entities;

        void m_startElement(const char* name, const char* atts[]) {
            SXMLEntity tmpEntity;
            make_charData();

            tmpEntity.DNameData = name;
            tmpEntity.DType = SXMLEntity::EType::StartElement;
            for (int i = 0; atts[i]; i += 2) {
                tmpEntity.SetAttribute(atts[i], atts[i+1]);
            }
            SImplementation::s_entities.push_back(tmpEntity);
        }

        void m_endElement(const char* name) {
            SXMLEntity tmpEntity;
            make_charData();

            tmpEntity.DNameData = name;
            tmpEntity.DType = SXMLEntity::EType::EndElement;
            SImplementation::s_entities.push_back(tmpEntity);
        }

        void m_charValue(const char* s, int len) {
            for(int i = 0; i < len; i++) {
                c_data += s[i];
            }   
        }

        void make_charData() {
            if (c_data != "") {
                SXMLEntity cEntity;
                cEntity.DNameData = c_data;
                cEntity.DType = SXMLEntity::EType::CharData;
                SImplementation::s_entities.push_back(cEntity);
                c_data = "";
            }
        }

        static void m_startElementCallback(void* userData, const char* name, const char** atts);

        static void m_endElementCallback(void* userData, const char* name);

        static void m_charValueCallback(void* userData, const char* s, int len);

        SImplementation(std::shared_ptr<CDataSource> src) {
            m_src = src;
            m_parser = XML_ParserCreate(NULL);
            XML_SetUserData(m_parser, this);
            XML_SetElementHandler(m_parser, m_startElementCallback, m_endElementCallback);
            XML_SetCharacterDataHandler(m_parser, m_charValueCallback);
        }

};

std::vector<SXMLEntity> CXMLReader::SImplementation::s_entities = {};

std::string CXMLReader::SImplementation::c_data = "";

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src) {
    DImplementation = std::make_unique<SImplementation>(SImplementation(src));
}

CXMLReader::~CXMLReader() {
    XML_ParserFree(DImplementation -> m_parser);
    CXMLReader::SImplementation::s_entities.clear();
}

// Start Tag
void CXMLReader::SImplementation::m_startElementCallback(void* userData, const char* name, const char** atts) {
    SImplementation *Impl = (SImplementation *) userData;
    Impl->m_startElement(name, atts);

}

// End Tag
void CXMLReader::SImplementation::m_endElementCallback(void* userData, const char* name) {
    SImplementation *Impl = (SImplementation *) userData;
    Impl -> m_endElement(name);
}

// Handles all char data between start & end tags
void CXMLReader::SImplementation::m_charValueCallback(void* userData, const char* s, int len) {
    SImplementation *Impl = (SImplementation *) userData;
    Impl -> m_charValue(s, len);
}

bool CXMLReader::End() const {
    if(SImplementation::s_entities.size() > 0) {
        return false;
    }
    else {
        return true;
    }
}

// Utilizes the handlers to write into the entity
// skipcdata: Skips only CharData if true
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {

    // Sets entity to be passed into the m_parser
    std::string entityBuf;
    char prevChar, tmpChar, peekChar;
    bool openTag = false, closeTag = false, complete = false, end = false;

    while (!DImplementation-> m_src -> End()) {
        // Gets tags by char
        DImplementation -> m_src -> Get(tmpChar);
        entityBuf += tmpChar;
    }

    // std::cout << "entityBuf: " << entityBuf.c_str() << "\n";
    // std::cout << "entityBuf len: " << entityBuf.length() << "\n";
    // std::cout << "End?: " << !DImplementation-> m_src -> End() << "\n";

    // Throw into expat m_parser to handle the parsing
    bool parsed = XML_Parse(DImplementation -> m_parser, entityBuf.c_str(), entityBuf.length(), !DImplementation-> m_src -> End());
    // std::cout << "Parsed?: " << parsed << "\n";
    
    if(SImplementation::s_entities.size() > 0) {
        
        // std::cout << "s_entites (size " << SImplementation::s_entities.size() << "):\n";
        for(int i = 0; i < SImplementation::s_entities.size(); i++) {
            // std::cout << SImplementation::s_entities[i].DNameData << "\n";
        }

        // if skipcdata == true -> remove charData from entities vector
        if(skipcdata) {
            for(int i = 0; i < SImplementation::s_entities.size(); i++) {
                if(SImplementation::s_entities[i].DType == SXMLEntity::EType::CharData) {
                    SImplementation::s_entities.erase(SImplementation::s_entities.begin()+i);
                }
            }
            skipcdata = false;
        }

        entity = SImplementation::s_entities[0];
        SImplementation::s_entities.erase(SImplementation::s_entities.begin());

        // std::cout << "after erasing s_entities:\n";
        for(int i = 0; i < SImplementation::s_entities.size(); i++) {
            // std::cout << SImplementation::s_entities[i].DNameData << "\n";
        }

    }
    else if (entityBuf.size() == 0 || entityBuf == "<></>") {
        return false;
    }

    return true;
}