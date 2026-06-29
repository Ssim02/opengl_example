cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME first_opengl_example)
set(CMAKE_CXX_STANDARD 17)

set(WINDOW_NAME "First OpenGL Example")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} src/main.cpp)

include(Dependency.cmake)

# MSVC에서 spdlog(내장 fmt)의 유니코드 처리를 위해 /utf-8 필요
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /utf-8)
endif()

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS}) # 실제로 어떤 라이브러리를 사용할 것인지

target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${WINDOW_NAME}"
    WINDOW_WIDTH=${WINDOW_WIDTH}
    WINDOW_HEIGHT=${WINDOW_HEIGHT}
    )

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST}) # DEP_LIST를 먼저 빌드하고 그 뒤에 우리의 프로젝트를 빌드하라는 것