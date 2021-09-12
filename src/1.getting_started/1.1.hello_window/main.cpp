// GLAD的头文件包含了正确的OpenGL头文件（例如GL/gl.h），所以需要在其它依赖于OpenGL的头文件之前包含GLAD
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const char *title = "Hello Window";

void process_input(GLFWwindow *window);
void framebuffer_resize_callback(GLFWwindow *window, int width, int height);

int main()
{
    // 初始化glfw
    glfwInit();
    // 设置opengl版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 设置渲染模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建窗口
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW widnow" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }
    //  视口大小
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    //  监听窗口变化
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    // render
    // 函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了
    while (!glfwWindowShouldClose(window))
    {
        // 处理窗口的输入事件
        process_input(window);

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置
        glfwPollEvents();
        // 双缓冲模式下交换前后缓冲
        // 函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

void framebuffer_resize_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        std::cout << "press esc exit" << std::endl;
        glfwSetWindowShouldClose(window, true);
    }
}
