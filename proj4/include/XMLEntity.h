#ifndef XMLENTITY_H
#define XMLENTITY_H

#include <utility>
#include <string>
#include <vector>

struct SXMLEntity{
    using TAttribute = std::pair< std::string, std::string >;
    enum class EType{StartElement, EndElement, CharData, CompleteElement};
    EType DType;
    std::string DNameData;
    std::vector< TAttribute > DAttributes;
    
    bool AttributeExists(const std::string &name) const{
        for(auto &Attribute : DAttributes){
            if(std::get<0>(Attribute) == name){
                return true;   
            }
        }
        return false;
    };
    
    std::string AttributeValue(const std::string &name) const{
        for(auto &Attribute : DAttributes){
            if(std::get<0>(Attribute) == name){
                return std::get<1>(Attribute);   
            }
        }
        return std::string();
    };
    
    bool SetAttribute(const std::string &name, const std::string &value){
        if(name.empty()){
            return false;   
        }
        for(std::vector< TAttribute >::size_type Index = 0; Index < DAttributes.size(); Index++){
            if(std::get<0>(DAttributes[Index]) == name){
                DAttributes[Index] = std::make_pair(name,value);
                return true;
            }
        }
        DAttributes.push_back(std::make_pair(name,value));
        return true;
    };
};
   
#endif
