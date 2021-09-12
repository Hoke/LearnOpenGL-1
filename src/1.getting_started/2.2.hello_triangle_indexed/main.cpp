#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char *WINDOW_TITLE = "hello triangle 2";

void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int compileShader();

unsigned int shaderProgram;

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

    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = {
        // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    // 创建顶点数据缓冲
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 创建定点索引缓冲
    glGenVertexArrays(1, &VAO);
    // 绑定顶点索引缓冲
    glBindVertexArray(VAO);

    // 绑定定点数据缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 传输数据给buffer
    //  第一个参数指定buffer类型,第二个参数指定大小(字节) 第三参数则是数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

        int frame = 0;
    while (!glfwWindowShouldClose(window))
    {

        // just for fun
        if (frame++ % 2 == 0)
        {
            // 设置为线框模式
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // 使用索引buffer的时候需要替换成drawElement
        // 第一个参数表示绘制模式
        // 第二个参数表示顶点个数
        // 第三个参数表示索引数据类型
        // 第四个参数表示offset
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawElements函数从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取索引。
        // 这意味着我们必须在每次要用索引渲染一个物体时绑定相应的EBO，这还是有点麻烦。
        // 不过顶点数组对象同样可以保存索引缓冲对象的绑定状态。VAO绑定时正在绑定的索引缓冲对象会被保存为VAO的元素缓冲对象。
        // 绑定VAO的同时也会自动绑定EBO

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
