#ifndef __COMMON_H__
#define __COMMON_H__

#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>

// type alliasing
#define CLASS_PTR(klassName) \
class klassName; \
using klassName ## UPtr = std::unique_ptr<klassName>; \
using klassName ## Ptr = std::shared_ptr<klassName>; \
using klassName ## WPtr = std::weak_ptr<klassName>; 

std::optional<std::string> LoadTextFile(const std::string& filename);
// 값이 들어 있다 -> string을 꺼내기 가능
// 값이 들어 있지 않다 -> 못꺼냄

#endif // __COMMON_H__