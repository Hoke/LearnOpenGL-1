#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "learnopengl/filesystem.h"
#include "learnopengl/shader_s.h"

#include <iostream>


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "transformations_exercise1";

void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
float mixValue = 0.2f;
float transX = 0.0f;
float transY = 0.0f;

int main()
{
	// init and config
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // !__APPLE__

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);


	// glad:加载所有opengl函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	// create shader
	Shader shader("5.3.shader.vs", "5.3.shader.fs");


	// 顶点shader输入
	float vertices[] = {
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};

	// 索引
	unsigned int indices[] = {
	 0, 1, 3, // first triangle
	 1, 2, 3  // second triangle
	};
    
    unsigned int VBO, VAO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // 顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);
    // 纹理坐标属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // 纹理1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 纹理过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    
    int width, height , nChannel;
    stbi_set_flip_vertically_on_load(true);// 反转Y轴
    unsigned char* data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nChannel, 0);
    
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture." << std::endl;
        return -1;
    }
    
    stbi_image_free(data);
    
    // 纹理2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nChannel, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "failed to load texture." << std::endl;
        return -1;
    }
    
    stbi_image_free(data);
    
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

	
    
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
		shader.setFloat("mixValue", mixValue);
        shader.use();
        
        //     GLM库从0.9.9版本起，默认会将矩阵类型初始化为一个零矩阵（所有元素均为0），而不是单位矩阵（对角元素为1，其它元素为0）。如果你使用的是0.9.9或0.9.9以上的版本，你需要将所有的矩阵初始化改为 glm::mat4 mat = glm::mat4(1.0f)
        glm::mat4 transform = glm::mat4(1.0f);
        // 缩放0.5 绕z轴旋转90度
//        trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
//        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        
        
        transform  = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        
        
        // 查询shader中变换矩阵uniform的位置,传值
        unsigned int transformLocation = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        // 第一个参数是uniform的位置
        // 第二个参数是发送几个
        // 第三个参数以矩阵是否需要行列交换，GLM默认就是列主序的，这里不需要
        // 数据，GLM提供的转换函数转换为OPENGL的形式
        
        
		
        
        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        
        // 画第二个箱子
        
        
        transform = glm::mat4(1.0f);
        glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scale = cos(glfwGetTime());
        transform = glm::scale(transform, glm::vec3(scale, scale, scale));
//        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &transform[0][0]);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
}


void frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.01f;
        transY += 0.01f;
        if(mixValue >= 1.0f)
        {
            mixValue = 1.0f;
        }
        if(transY >= 0.5f)
        {
            transY = 0.5f;
        }
        
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.01f;
        
        transY -= 0.01f;
        if(mixValue <= 0.0f)
        {
            mixValue = 0.0f;
        }
        if(transY <= -0.5f)
        {
            transY = -0.5f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        mixValue += 0.01f;
        transX -= 0.01f;
        if(mixValue >= 1.0f)
        {
            mixValue = 1.0f;
        }
        if(transX <= -0.5f)
        {
            transX = -0.5f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        mixValue += 0.01f;
        transX += 0.01f;
        if(mixValue >= 1.0f)
        {
            mixValue = 1.0f;
        }
        if(transX >= 0.5f)
        {
            transX = 0.5f;
        }
    }
}
