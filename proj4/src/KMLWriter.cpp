#include "KMLWriter.h"
#include "XMLWriter.h"
#include "StringUtils.h"
#include <unordered_set>
#include <sstream>
#include <iomanip>

struct CKMLWriter::SImplementation{
    std::shared_ptr<CXMLWriter> DXMLWriter;
    std::unordered_set<std::string> DPointStyles;
    std::unordered_set<std::string> DLineStyles;
    std::size_t DIndentionLevel;

    static const std::string DKMLTag;
    static const std::string DDocumentTag;
    static const std::string DNameTag;
    static const std::string DPointTag;
    static const std::string DDescriptionTag;
    static const std::string DStyleTag;
    static const std::string DLineStyleTag;
    static const std::string DColorTag;
    static const std::string DWidthTag;
    static const std::string DStyleURLTag;
    static const std::string DPlacemarkTag;
    static const std::string DLineStringTag;
    static const std::string DCoordinatesTag;
    static const std::string DTessellateTag;
    static const std::string DAltitudeModeTag;
    static const std::string DAltitudeModeRelativeToGround;
    static const std::string DIDKey;
    static const std::string DXMLNSKey;
    static const std::string DXMLNSValue;

    bool OutputIndent(){
        SXMLEntity Entity;
        Entity.DType = SXMLEntity::EType::CharData;
        Entity.DNameData = std::string("\n") + std::string(DIndentionLevel*2,' ');
        return DXMLWriter->WriteEntity(Entity);
    }

    bool StartTag(const std::string &name, const std::vector<SXMLEntity::TAttribute> &attributes){
        if(OutputIndent()){
            SXMLEntity Entity;
            Entity.DType = SXMLEntity::EType::StartElement;
            Entity.DNameData = name;
            Entity.DAttributes = attributes;
            if(DXMLWriter->WriteEntity(Entity)){
                DIndentionLevel++;
                return true;
            }
        }
        return false;
    }

    bool EndTag(const std::string &name){
        if(DIndentionLevel){
            DIndentionLevel--;
            if(OutputIndent()){
                SXMLEntity Entity;
                Entity.DType = SXMLEntity::EType::EndElement;
                Entity.DNameData = name;
                if(DXMLWriter->WriteEntity(Entity)){
                    return true;
                }
            }
        }
        return false;
    }

    bool StartTagDataEndTag(const std::string &name, const std::string &data){
        if(OutputIndent()){
            SXMLEntity TagEntity, DataEntity;
            TagEntity.DType = SXMLEntity::EType::StartElement;
            TagEntity.DNameData = name;
            DataEntity.DType = SXMLEntity::EType::CharData;
            DataEntity.DNameData = data;
            if(DXMLWriter->WriteEntity(TagEntity) && DXMLWriter->WriteEntity(DataEntity)){
                TagEntity.DType = SXMLEntity::EType::EndElement;
                return DXMLWriter->WriteEntity(TagEntity);
            }
        }
        return false;
    }

    bool IndentedData(const std::vector<std::string> &datalines){
        SXMLEntity Entity;
        Entity.DType = SXMLEntity::EType::CharData;
        Entity.DNameData = std::string("\n") + std::string(DIndentionLevel*2,' ') + StringUtils::Join(std::string("\n") + std::string(DIndentionLevel*2,' '),datalines);
        return DXMLWriter->WriteEntity(Entity);
    }

    SImplementation(std::shared_ptr< CDataSink > sink, const std::string &name, const std::string &desc){
        const std::string XMLEncoding = "<?xml version='1.0' encoding='UTF-8'?>";
        sink->Write(std::vector<char>(XMLEncoding.begin(),XMLEncoding.end()));
        DXMLWriter = std::make_shared<CXMLWriter>(sink);
        DIndentionLevel = 0;

        if(StartTag(DKMLTag,{{DXMLNSKey,DXMLNSValue}}) && StartTag(DDocumentTag,{}) && StartTagDataEndTag(DNameTag,name) && StartTagDataEndTag(DDescriptionTag,desc)){
           // Good  
        }
    }

    ~SImplementation(){
        EndTag(DDocumentTag);
        EndTag(DKMLTag);
    }

    bool CreatePointStyle(const std::string &stylename, unsigned int color){
        std::stringstream TempStream;
        TempStream<<std::setw(8);
        TempStream<<std::setfill('0');
        TempStream<<std::hex<<color;
        if(!DPointStyles.count(stylename) && 
            StartTag(DStyleTag,{{DIDKey,stylename}}) && 
            StartTag(DPointTag,{}) && 
            StartTagDataEndTag(DColorTag,TempStream.str()) && 
            EndTag(DPointTag) && 
            EndTag(DStyleTag)){

            DPointStyles.insert(stylename);
            return true;
        }
        return false;
    }

    bool CreateLineStyle(const std::string &stylename, unsigned int color, int width){
        std::stringstream TempStream;
        TempStream<<std::setw(8);
        TempStream<<std::setfill('0');
        TempStream<<std::hex<<color;
        if(!DLineStyles.count(stylename) && 
            StartTag(DStyleTag,{{DIDKey,stylename}}) && 
            StartTag(DLineStyleTag,{}) && 
            StartTagDataEndTag(DColorTag,TempStream.str()) && 
            StartTagDataEndTag(DWidthTag,std::to_string(width)) && 
            EndTag(DLineStyleTag) && 
            EndTag(DStyleTag)){
                
            DLineStyles.insert(stylename);
            return true;
        }
        return false;
    }

    bool CreatePoint(const std::string &name, const std::string &desc, const std::string &stylename, CStreetMap::TLocation point){
        if(DPointStyles.count(stylename) && 
            StartTag(DPlacemarkTag,{}) && 
            StartTagDataEndTag(DNameTag,name) && 
            StartTagDataEndTag(DDescriptionTag,desc) && 
            StartTagDataEndTag(DStyleURLTag,std::string("#") + stylename) && 
            StartTag(DPointTag,{}) && 
            StartTagDataEndTag(DTessellateTag,"1") && 
            StartTagDataEndTag(DAltitudeModeTag,DAltitudeModeRelativeToGround) && 
            StartTag(DCoordinatesTag,{}) && 
            IndentedData({std::to_string(std::get<1>(point)) + "," + std::to_string(std::get<0>(point))});
            EndTag(DCoordinatesTag) && 
            EndTag(DPointTag) && 
            EndTag(DPlacemarkTag)){

            return true;
        }
        return false;
    }

    bool CreatePath(const std::string &name, const std::string &stylename, const std::vector< CStreetMap::TLocation > &points){
        std::vector<std::string> PointStrings;
        PointStrings.reserve(points.size());
        for(auto &Point : points){
            PointStrings.push_back(std::to_string(std::get<1>(Point)) + "," + std::to_string(std::get<0>(Point)));
        }
        if(DLineStyles.count(stylename) && 
            StartTag(DPlacemarkTag,{}) && 
            StartTagDataEndTag(DNameTag,name) && 
            StartTagDataEndTag(DStyleURLTag,std::string("#") + stylename) && 
            StartTag(DLineStringTag,{}) && 
            StartTagDataEndTag(DTessellateTag,"1") && 
            StartTagDataEndTag(DAltitudeModeTag,DAltitudeModeRelativeToGround) && 
            StartTag(DCoordinatesTag,{}) && 
            IndentedData(PointStrings);
            EndTag(DCoordinatesTag) && 
            EndTag(DLineStringTag) && 
            EndTag(DPlacemarkTag)){

            return true;
        }
        return false;
    }
};

const std::string CKMLWriter::SImplementation::DKMLTag = "kml";
const std::string CKMLWriter::SImplementation::DDocumentTag = "Document";
const std::string CKMLWriter::SImplementation::DNameTag = "name";
const std::string CKMLWriter::SImplementation::DPointTag = "Point";
const std::string CKMLWriter::SImplementation::DDescriptionTag = "description";
const std::string CKMLWriter::SImplementation::DStyleTag = "Style";
const std::string CKMLWriter::SImplementation::DLineStyleTag = "LineStyle";
const std::string CKMLWriter::SImplementation::DColorTag = "color";
const std::string CKMLWriter::SImplementation::DWidthTag = "width";
const std::string CKMLWriter::SImplementation::DStyleURLTag = "styleUrl";
const std::string CKMLWriter::SImplementation::DPlacemarkTag = "Placemark";
const std::string CKMLWriter::SImplementation::DLineStringTag = "LineString";
const std::string CKMLWriter::SImplementation::DCoordinatesTag = "coordinates";
const std::string CKMLWriter::SImplementation::DTessellateTag = "tessellate";
const std::string CKMLWriter::SImplementation::DAltitudeModeTag = "altitudeMode";
const std::string CKMLWriter::SImplementation::DAltitudeModeRelativeToGround = "relativeToGround";
const std::string CKMLWriter::SImplementation::DIDKey = "id";
const std::string CKMLWriter::SImplementation::DXMLNSKey = "xmlns";
const std::string CKMLWriter::SImplementation::DXMLNSValue = "http://www.opengis.net/kml/2.2";
        
CKMLWriter::CKMLWriter(std::shared_ptr< CDataSink > sink, const std::string &name, const std::string &desc){
    DImplementation = std::make_unique<SImplementation>(sink, name, desc);
}

CKMLWriter::~CKMLWriter(){

}

bool CKMLWriter::CreatePointStyle(const std::string &stylename, unsigned int color){
    return DImplementation->CreatePointStyle(stylename,color);
}

bool CKMLWriter::CreateLineStyle(const std::string &stylename, unsigned int color, int width){
    return DImplementation->CreateLineStyle(stylename,color,width);
}

bool CKMLWriter::CreatePoint(const std::string &name, const std::string &desc, const std::string &stylename, CStreetMap::TLocation point){
    return DImplementation->CreatePoint(name,desc,stylename,point);
}

bool CKMLWriter::CreatePath(const std::string &name, const std::string &stylename, const std::vector< CStreetMap::TLocation > &points){
    return DImplementation->CreatePath(name,stylename,points);
}
