#ifndef __SHADER_H__
#define __SHADER_H__

#include "common.h"

CLASS_PTR(Shader);
class Shader {
public:
    static ShaderUPtr CreateFromFile(const std::string & filename, GLenum shaderType);

    ~Shader();
    uint32_t Get() const { return m_shader; }
private:
    Shader() {} // constructor가 private
    bool LoadFile(const std::string& filename, GLenum shaderType); // 초기화하는 함수도 private
    uint32_t m_shader { 0 }; // 멤버변수 : openGL이 리턴해 줄 shaderObject의 id
};

/*
    생성자가 private인 이유 
    - 클래스의 public인 static function을 통하지 않고서는 만들 수 없게끔 만드려고
    - 이대로라면 Shader를 uniq_ptr로만 만들 수 있게 강제됨
    
    클래스 생성 시 지켜야 할 원칙 
    -> 외부에 값을 노출시키는 것을 최대한 막아야 함

    
    m_shader : openGL을 호출해 얻어진 숫자. 이 숫자는 openGL만 설정해야함. 
                그래서 Get()만 있고 Set()이 없음.
                Shader object는 shader 내부에서만 관리하기 위해서 이렇게 하는 것. 
    
    LoadFile()이 bool을 리턴하는 이유 : 생성 실패 시 false를 리턴하기 위해
*/

#endif // __SHADER_H__