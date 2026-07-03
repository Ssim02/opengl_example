// unique_ptr : 단독 소유
// shared_ptr : 공동 소유권, 레퍼런스 카운팅을 하다가 카운팅이 0이 되면 메모리를 자동 해제
// uniq->shared 변환은 쉬움. 역은 어려움. 
// unique타입을 반환하는 함수를 SharedPtr타입의 변수가 받으면 자동으로 SharedPtr로 변환됨. 
#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) {
    auto program = ProgramUPtr(new Program());
    if (!program->Link(shaders))
        return nullptr;
    return std::move(program);
}

Program::~Program() {
    if (m_program) {
        glDeleteProgram(m_program);
    }
}

bool Program::Link(const std::vector<ShaderPtr>& shaders) {
    m_program = glCreateProgram();
    for (auto& shader: shaders)
        glAttachShader(m_program, shader->Get());
    glLinkProgram(m_program);
    
    int success = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to link program: {}", infoLog);
        return false;
    }
    return true;
}

