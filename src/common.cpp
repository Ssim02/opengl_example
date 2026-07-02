#include "common.h"
#include <fstream>
#include <sstream>

std::optional<std::string> LoadTextFile(const std::string& filename) {
    std::ifstream fin(filename); // c++방식의 file open
    if (!fin.is_open()) {
        SPDLOG_ERROR("failed to open file: {}", filename);
        return {}; // 아무것도 없는 값 return
    }
    std::stringstream text;
    text << fin.rdbuf();
    return text.str();
}