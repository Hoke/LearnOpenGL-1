
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_s.h>

#include <iostream>


const int SCREEN_WIDHT = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "transformations";

void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main()
{
	// init and config
	glfwInit();
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // !__APPLE__

	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDHT, SCREEN_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);


	// 绑定函数指针查找库
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	// create shader
	Shader shader("5.1.shader.vs", "5.1.shader.fs");


	// 顶点
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


}
