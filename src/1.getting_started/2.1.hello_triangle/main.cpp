#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char *WINDOW_TITLE = "Hello Triangle";

void processInput(GLFWwindow *window);

void framebuffer_resize_callback(GLFWwindow *window, int width, int height);


const char *vertice_shader_source = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main()\n"
                                    "{\n"
                                    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                    "}";

const char *frag_shader_source = "#version 330 core\n"
                                 "out vec4 FragColor;"
                                 "void main()"
                                 "{"
                                 "FragColor = vec4(1.0f,0.5f,0.2f,1.0f);"
                                 "}";


int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef  __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create glfw window!" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to load glad!" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    // 三角形顶点定义
    float vertices[] = {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.0f, -0.5f, 0.0f,
    };

    // 使用缓冲对象传递数据

    // 定义顶点buffer,斌企鹅在opengl中创建和绑定buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 传输数据给buffer
    //  第一个参数指定buffer类型,第二个参数指定大小(字节) 第三参数则是数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // 创建并编译顶点shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertice_shader_source, NULL);
    glCompileShader(vertexShader);

    // 检测shader是否编译成功
    int success = 1;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_ERROR\n" << infoLog << std::endl;
        return -1;
    }
    std::cout << "vertex shader compile success" << std::endl;

    unsigned int fragmentShader;
    glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &frag_shader_source, NULL);
    glCompileShader(fragmentShader);

    success = 1;

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_ERROR\n" << infoLog << std::endl;
        return -1;
    }
    std::cout << "fragment shader compile success" << std::endl;


    //  创建shader program并且链接
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    success = 1;

    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_ERROR\n" << infoLog << std::endl;
        return -1;
    }
    std::cout << "shader program link success" << std::endl;

    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);


        glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwWindowShouldClose(window);
    }
}

void framebuffer_resize_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
