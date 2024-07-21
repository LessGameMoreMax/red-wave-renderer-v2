#include "RStatus.h"
namespace sablin{

RStatus::RStatus():error_code_(RSTATUS_OK){}

RStatus::RStatus(const std::string& error_info, const std::string& error_location):
    error_code_(RSTATUS_ERROR),
    error_info_(error_info),
    error_location_(error_location){
}

RStatus::RStatus(int32_t error_code, const std::string& error_info, const std::string& error_location):
    error_code_(error_code),
    error_info_(error_info),
    error_location_(error_location){
}

RStatus::RStatus(const RStatus& other):
    error_code_(other.error_code_),
    error_info_(other.error_info_),
    error_location_(other.error_location_){
}

RStatus::RStatus(const RStatus&& other) noexcept:
    error_code_(other.error_code_),
    error_info_(other.error_info_),
    error_location_(other.error_location_){
}

RStatus& RStatus::operator=(const RStatus& other){
    error_code_ = other.error_code_;
    error_info_ = other.error_info_;
    error_location_ = other.error_location_;
    return *this;
}

RStatus& RStatus::operator+=(const RStatus& cur){
    if(IsOk() && cur.IsError()){
        error_code_ = cur.error_code_;
        error_info_ = cur.error_info_;
        error_location_ = cur.error_location_;
    }
    return *this;
}

}
