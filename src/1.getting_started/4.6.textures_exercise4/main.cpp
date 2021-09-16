#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "learnopengl/shader_s.h"
#include "learnopengl/filesystem.h"
#include <stb_image.h>

//ʹ��һ��uniform������Ϊmix�����ĵ������������ı���������ɼ��ȣ�ʹ���Ϻ��¼����ı����ӻ�Ц���Ŀɼ��ȣ�

// ��չ:ʹ�����Ҽ����ı�Ц������


void frameBufferResize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Textures";

float mixValue = 0.2f;
// Ц������
float dirValue = 1.0f;

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

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferResize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load glad!" << std::endl;
        return -1;
    }

    // create shader
    Shader shader("4.6.shader.vs", "4.6.shader.fs");

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
       0, 1, 3, // first triangle
       1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // �������
    // �����Ʒ�ʽ
    // �������
    // �༶��Զ����


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ��������ͼƬ
    // �����������
    unsigned int texture;
    // ����opengl����
    glGenTextures(1, &texture);
    // ������
    glBindTexture(GL_TEXTURE_2D, texture);
    // ��������Ļ���,���˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    int width, height, nrChannels;
    unsigned char* data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        // ������������
        // ��һ������ָ������Ŀ��
        // �ڶ�������ָ���༶����ļ���
        // ��������������OpenGL����ϣ����������Ϊ���ָ�ʽ
        // ���ĸ��͵���������������յ�����Ŀ�Ⱥ͸߶�
        // �¸�����Ӧ�����Ǳ���Ϊ0����ʷ���������⣩
        // ���ߵڰ˸�����������Դͼ�ĸ�ʽ���������͡�����ʹ��RGBֵ�������ͼ�񣬲������Ǵ���Ϊchar(byte)���飬���ǽ��ᴫ���Ӧֵ��
        // ��������������
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��Ȼ����Ŀǰֻ�л�������(Base - level)������ͼ�񱻼����ˣ����
        //Ҫʹ�ö༶��Զ�������Ǳ����ֶ��������в�ͬ��ͼ�񣨲��ϵ����ڶ��������������ߣ�ֱ������������֮�����glGenerateMipmap����
        //��Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }
    // �Ѿ���ȡ�����ݣ��ͷż����ڴ�
    stbi_image_free(data);

    // �ߵ�һ��
    // ��ΪOpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ����������ˣ�stb_image.h�ܹ���ͼ�����ʱ�������Ƿ�תy��
    stbi_set_flip_vertically_on_load(true);
    unsigned int texture2;
    // ����opengl����
    glGenTextures(1, &texture2);
    // ������
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        // ������������
        // ��һ������ָ������Ŀ��
        // �ڶ�������ָ���༶����ļ���
        // ��������������OpenGL����ϣ����������Ϊ���ָ�ʽ
        // ���ĸ��͵���������������յ�����Ŀ�Ⱥ͸߶�
        // �¸�����Ӧ�����Ǳ���Ϊ0����ʷ���������⣩
        // ���ߵڰ˸�����������Դͼ�ĸ�ʽ���������͡�����ʹ��RGBֵ�������ͼ�񣬲������Ǵ���Ϊchar(byte)���飬���ǽ��ᴫ���Ӧֵ��
        // ��������������
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��Ȼ����Ŀǰֻ�л�������(Base - level)������ͼ�񱻼����ˣ����
        //Ҫʹ�ö༶��Զ�������Ǳ����ֶ��������в�ͬ��ͼ�񣨲��ϵ����ڶ��������������ߣ�ֱ������������֮�����glGenerateMipmap����
        //��Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }
    // �Ѿ���ȡ�����ݣ��ͷż����ڴ�
    stbi_image_free(data);

    shader.use(); // don't forget to activate/use the shader before setting uniforms!
    // ͨ��ʹ��glUniform1i����ÿ���������ķ�ʽ����OpenGLÿ����ɫ�������������ĸ�����Ԫ��
    // ����ֻ��Ҫ����һ�μ��ɣ���������������Ⱦѭ����ǰ��
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 0);
    // or set it via the texture class
    shader.setInt("texture2", 1);
    // ����0�ź�����1��

    



    while (!glfwWindowShouldClose(window))
    {


            processInput(window);
        
            
            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
           

            // bind Texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
        
            // ���û�ϵ�ֵ
            shader.setFloat("mixValue", mixValue);
            // ���÷���ֵ��uniform
            shader.setFloat("dir", dirValue);
            // render container
            shader.use();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

   
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

}



void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.0001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue >= 1.0f)
        {
            mixValue = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.0001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue <= 0.0f)
        {
            mixValue = 0.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        dirValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        dirValue = -1.0f;
    }
}

void frameBufferResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
