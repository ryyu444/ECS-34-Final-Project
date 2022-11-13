#include "FileDataFactory.h"
#include "FileDataSource.h"
#include "FileDataSink.h"
#include <filesystem>

CFileDataFactory::CFileDataFactory(const std::string &path){
    if(path.empty()){
        DBasePath = "./";
    }
    else{
        DBasePath = DBasePath.back() == '/' ? path : path + "/";
    }
}

std::shared_ptr< CDataSource > CFileDataFactory::CreateSource(const std::string &name) noexcept{
    return std::make_shared<CFileDataSource>(DBasePath + name);
}

std::shared_ptr< CDataSink > CFileDataFactory::CreateSink(const std::string &name) noexcept{
    std::error_code ErrorCode;
    if(!std::filesystem::create_directories(DBasePath,ErrorCode) && ErrorCode){
        return nullptr;
    }
    return std::make_shared<CFileDataSink>(DBasePath + name);
}
