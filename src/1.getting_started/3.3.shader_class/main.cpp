#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <learnopengl/shader_s.h>


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Shader Class";

void processInput(GLFWwindow* window);

void framebuffer_resize_callback(GLFWwindow* window, int width, int height);

const char* vertice_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* frag_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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

    Shader ourShader("3.3.shader.vs", "3.3.shader.fs");


    // �����ζ��㶨��
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
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
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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
