#include "utils.h"
#include <Windows.h>

std::string MASSutils::wstring2string(std::wstring& src)
{
    int len = WideCharToMultiByte(CP_ACP, 0, src.c_str(), src.size(), nullptr, 0, nullptr, nullptr);
    char* buf = (char*)calloc(len + 1, sizeof(char));
    WideCharToMultiByte(CP_ACP, 0, src.c_str(), src.size(), buf, len, nullptr, nullptr);
    buf[len] = 0;
    std::string dest(buf);
    free(buf);
    
    return dest;
}
