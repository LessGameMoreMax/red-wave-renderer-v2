#ifndef R_STATUS_H
#define R_STATUS_H
#include <cstdint>
#include <string>
#include "../Misc/MacroTools.h"
namespace sablin{

static constexpr int32_t RSTATUS_OK = 0;
static constexpr int32_t RSTATUS_ERROR = -1;
static constexpr int32_t RSTATUS_CRASH = -101;

class RStatus{
private:
    int32_t error_code_;
    std::string error_info_;
    std::string error_location_;
public:
    explicit RStatus();
    explicit RStatus(const std::string& error_info, const std::string& error_location = STRING_NULL);
    explicit RStatus(int32_t error_code, const std::string& error_info, const std::string& error_location = STRING_NULL);
    RStatus(const RStatus& other);
    RStatus(const RStatus&& other) noexcept;
    RStatus& operator=(const RStatus& other);
    RStatus& operator+=(const RStatus& cur);

    inline int32_t GetErrorCode() const{
        return error_code_;
    }

    inline const std::string& GetErrorInfo() const{
        return error_info_;
    }

    inline const std::string& GetErrorLocation() const{
        return error_location_;
    }

    inline bool IsOk() const{
        return error_code_ == RSTATUS_OK;
    }

    inline bool IsError() const{
        return error_code_ < RSTATUS_OK;
    }

    inline bool IsCrash() const{
        return error_code_ == RSTATUS_CRASH;
    }
};
}
#endif
