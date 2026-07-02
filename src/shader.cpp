#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType) {
    auto shader = ShaderUPtr(new Shader());
    if (!shader->LoadFile(filename, shaderType)) // 만약 초기화 실패 시
        return nullptr; // nullptr리턴 --> shader가 가지고 있던 메모리 자동 반환하고 스코프가 정리됨
    return std::move(shader); // 그게 아니라면 소유권 이전
}

Shader::~Shader() {
    if (m_shader) {
        glDeleteShader(m_shader); // 소멸 시 shader에 정수값이 들어있다면 제거
    }
}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType) {
    auto result = LoadTextFile(filename);
    if (!result.has_value())
        return false;

    auto& code = result.value(); // std::string& code = result.value();
    const char* codePtr = code.c_str();
    int32_t codeLength = (int32_t)code.length();

    // create and compile shader
    m_shader = glCreateShader(shaderType);
    glShaderSource(m_shader, 1, (const GLchar* const*)&codePtr, &codeLength);
    glCompileShader(m_shader);

    // check compile error
    int success = 0;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success); // i : integer, v : 뒤에 포인터 타입이 붙어야 한다는 뜻?
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
        SPDLOG_ERROR("reason: {}", infoLog);
        return false;
    }
    return true;
}