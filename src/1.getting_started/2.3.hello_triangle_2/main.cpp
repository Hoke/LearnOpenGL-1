#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char *WINDOW_TITLE = "hello triangle 2";

void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int compileShader();
int compileShader2();

unsigned int shaderProgram;
unsigned int shaderProgram2;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create glfw widnow" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load GLAD" << std::endl;
        return -1;
    }

    // 创建编译链接shader
    if (compileShader() != 0)
    {
        std::cout << "Failed to compile shader" << std::endl;
        return -1;
    }
    if (compileShader2() != 0)
    {
        std::cout << "Failed to compile shader" << std::endl;
        return -1;
    }

    // 顶点
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f,  // right
        0.0f, 0.5f, 0.0f    // top
    };

    // float vertices2[] = {
    //     -0.5f, 0.5f, 0.0f, // left
    //     0.5f, 0.5f, 0.0f,  // right
    //     0.0f, -0.5f, 0.0f  // top
    // };

    float vertices2[] = {
        -1.0f, -1.0f, 0.0f, // left
        1.0f, -1.0f, 0.0f,  // right
        0.0f, 1.0f, 0.0f    // top
    };

    unsigned int VBO;
    unsigned int VAO;
    // 创建顶点数据缓冲
    glGenBuffers(1, &VBO);

    // 创建定点索引缓冲
    glGenVertexArrays(1, &VAO);
    // 绑定顶点索引缓冲
    glBindVertexArray(VAO);

    // 绑定定点数据缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 传输数据给buffer
    //  第一个参数指定buffer类型,第二个参数指定大小(字节) 第三参数则是数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 解析顶点数据
    // 第一个参数表示传给顶点shader的layout(location = 0)
    // 第二个是定义定点数据的大小，三个值组成的vec3
    // 第三个参数是制定数据类型
    // 第四个参数是制定是否进行Normalize
    // 蒂五个参数可以表述为在定点缓冲中取值的时候的步长
    // 第六个参数表示offfset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *)0);

    // 使用0号顶点缓冲
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int VBO2;
    unsigned int VAO2;
    // 创建顶点数据缓冲
    glGenBuffers(1, &VBO2);

    // 创建定点索引缓冲
    glGenVertexArrays(1, &VAO2);
    // 绑定顶点索引缓冲
    glBindVertexArray(VAO2);

    // 绑定定点数据缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    // 传输数据给buffer
    //  第一个参数指定buffer类型,第二个参数指定大小(字节) 第三参数则是数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    // 解析顶点数据
    // 第一个参数表示传给顶点shader的layout(location = 0)
    // 第二个是定义定点数据的大小，三个值组成的vec3
    // 第三个参数是制定数据类型
    // 第四个参数是制定是否进行Normalize
    // 蒂五个参数可以表述为在定点缓冲中取值的时候的步长
    // 第六个参数表示offfset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *)0);

    // 使用0号顶点缓冲
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void frameBufferResizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int compileShader()
{

    const char *vertice_shader_source = "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                        "}\0";
    const char *frag_shader_source = "#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                     "}\n\0";

    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertice_shader_source, NULL);
    glShaderSource(fragmentShader, 1, &frag_shader_source, NULL);

    int success = 1;
    char infoLog[512];
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "vertex shader compile failed" << infoLog << std::endl;
        return -1;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "fragment shader compile failed" << infoLog << std::endl;
        return -1;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "shader link failed" << infoLog << std::endl;
        return -1;
    }

    std::cout << "shader compile success" << std::endl;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
}

int compileShader2()
{

    const char *vertice_shader_source = "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                        "}\0";
    const char *frag_shader_source = "#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
                                     "}\n\0";

    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertice_shader_source, NULL);
    glShaderSource(fragmentShader, 1, &frag_shader_source, NULL);

    int success = 1;
    char infoLog[512];
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "vertex shader compile failed" << infoLog << std::endl;
        return -1;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "fragment shader compile failed" << infoLog << std::endl;
        return -1;
    }

    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader);
    glLinkProgram(shaderProgram2);
    glGetShaderiv(shaderProgram2, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "shader link failed" << infoLog << std::endl;
        return -1;
    }

    std::cout << "shader compile success" << std::endl;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
}
