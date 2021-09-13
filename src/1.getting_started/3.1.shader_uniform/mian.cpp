#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hello Triangle";

void processInput(GLFWwindow* window);

void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
// 通过顶点shader的输出给片元shader,在shader程序中进行传递数据
const char* vertice_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* frag_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";


int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create glfw window!" << std::endl;
        return -1;
    }
    // 绑定窗口上下文
    glfwMakeContextCurrent(window);
    // 设置视口大小

    // 监听窗口resize
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    // 加载glad，查找函数位置
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load glad!" << std::endl;
        return -1;
    }

    //    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        // 创建并编译顶点shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertice_shader_source, NULL);
    glCompileShader(vertexShader);

    // 检测shader是否编译成功
    int success = 1;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_ERROR\n"
            << infoLog << std::endl;
        return -1;
    }
    std::cout << "vertex shader compile success" << std::endl;

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &frag_shader_source, NULL);
    glCompileShader(fragmentShader);


    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_ERROR\n"
            << infoLog << std::endl;
        return -1;
    }
    std::cout << "fragment shader compile success" << std::endl;

    //  创建shader program并且link
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_ERROR\n"
            << infoLog << std::endl;
        return -1;
    }
    std::cout << "shader program link success" << std::endl;

    //  编译出program后就不再需要shader了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // 三角形顶点定义
    float vertices[] = {
           -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    // 使用0号顶点缓冲
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        float time = glfwGetTime();
        float greenValue = sin(time) / 2.0f + 0.5f;
        // 从shader中查询uniform的位置 
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // 赋值给uniform
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
