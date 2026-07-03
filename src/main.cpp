#include "context.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}", 
        key, scancode, 
        action == GLFW_PRESS ? "Pressed" :
        action == GLFW_RELEASE ? "Released" :
        action == GLFW_REPEAT ? "Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, const char** argv) {
    SPDLOG_INFO("Start program");

    // glfw 라이브러리 초기화, 실패하면 에러 출력 후 종료
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description); // fmt library, 타입 지정 안해줘도 알아서 적용해줌
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw 윈도우 생성, 실패하면 에러 출력 후 종료
    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 여기까지 하면 openGL context가 세팅이 된 상태임

    // glad를 활용한 OpenGL 함수 로딩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // 이게 실패하지 않아야 이 다음부터 openGL function 사용 가능함
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    auto glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    SPDLOG_INFO("OpenGL context version: {}", glVersion);

    auto context = Context::Create();
    if (!context) {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    SPDLOG_INFO("vertext shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    auto program = Program::Create({fragShader, vertShader});
    SPDLOG_INFO("program id: {}", program->Get());

    OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT); // 윈도우 최초 생성 시에는 프레임 버퍼 변경 이벤트가 발생하지 않으므로 수동 삽입
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);

    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // 윈도우와 관련한 여러 이벤트들을 수집하는 함수(창 크기가 늘어나던가 하는 등의 이벤트). 이거 없으면 너무 빨리 갱신되어서 멈춰있는것처럼 보일 것
        context->Render();
        glfwSwapBuffers(window);
    }
    context.reset(); // context = nullptr; 이 시점에서 context가 들고 있던 메모리를 해제, 안전하게 메모리 해제되면서 openGL object들도 gpu 메모리에서 날아가게 될 것. 

    // 여기서 shader등 메모리가 한꺼번에 정리되는 것이 좋음. 
    // 이걸 한꺼번에 묶어서 하기 위해 context에서 관리할 것.

    glfwTerminate();
    return 0;
}