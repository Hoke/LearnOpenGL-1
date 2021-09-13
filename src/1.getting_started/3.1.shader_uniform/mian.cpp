#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hello Triangle";

void processInput(GLFWwindow* window);

void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
// ͨ������shader�������ƬԪshader,��shader�����н��д�������
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
    // �󶨴���������
    glfwMakeContextCurrent(window);
    // �����ӿڴ�С

    // ��������resize
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    // ����glad�����Һ���λ��
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load glad!" << std::endl;
        return -1;
    }

    //    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        // ���������붥��shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertice_shader_source, NULL);
    glCompileShader(vertexShader);

    // ���shader�Ƿ����ɹ�
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

    //  ����shader program����link
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

    //  �����program��Ͳ�����Ҫshader��
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // �����ζ��㶨��
    float vertices[] = {
           -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
    };

    unsigned int VBO;
    unsigned int VAO;
    // �����������ݻ���
    glGenBuffers(1, &VBO);


    // ����������������
    glGenVertexArrays(1, &VAO);
    // �󶨶�����������
    glBindVertexArray(VAO);

    // �󶨶������ݻ���
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // �������ݸ�buffer
    //  ��һ������ָ��buffer����,�ڶ�������ָ����С(�ֽ�) ����������������
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // ������������
    // ��һ��������ʾ��������shader��layout(location = 0)
    // �ڶ����Ƕ��嶨�����ݵĴ�С������ֵ��ɵ�vec3
    // �������������ƶ���������
    // ���ĸ��������ƶ��Ƿ����Normalize
    // ������������Ա���Ϊ�ڶ��㻺����ȡֵ��ʱ��Ĳ���
    // ������������ʾofffset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    // ʹ��0�Ŷ��㻺��
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
        // ��shader�в�ѯuniform��λ�� 
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // ��ֵ��uniform
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
