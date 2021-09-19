#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "learnopengl/shader_s.h"
#include "learnopengl/filesystem.h"
#include <stb_image.h>

#include <ctime>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hello Triangle";

void processInput(GLFWwindow* window);

void framebuffer_resize_callback(GLFWwindow* window, int width, int height);


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

     // create shader
    Shader shader("4.7.shader.vs", "4.7.shader.fs");


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



    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool show_demo_window = false;
    bool show_another_window = false;
    // 
    ImVec4 traingle_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 clear_color = ImVec4(0.2f, 0.30f, 0.30f, 1.00f);
    

    float startTime = GetCurrentTime() / 1000.0f;
    long passedFrames = 0;
    while (!glfwWindowShouldClose(window))
    {

        float currentTime = GetCurrentTime() / 1000.0f;
        float totalTime = currentTime - startTime;
        long targetTotalFrames = totalTime / (1.0 / 60);

        while (passedFrames < targetTotalFrames)
        {
            passedFrames++;
            processInput(window);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("imgui test");                          // Create a window called "Hello, world!" and append into it.
                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
                ImGui::ColorEdit3("traingle color", (float*)&traingle_color);

                shader.setFloat("colorX", traingle_color.x);
                shader.setFloat("colorY", traingle_color.y);
                shader.setFloat("colorZ", traingle_color.z);
                shader.setFloat("colorAlpha", traingle_color.w);
                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);
                ImGui::SameLine();
                ImGui::Text("targetTotalFrames = %d  passedFrames = %d", targetTotalFrames, passedFrames);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // 3. Show another simple window.
            if (show_another_window)
            {
                ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    show_another_window = false;
                ImGui::End();
            }

            // Rendering
            ImGui::Render();

            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            shader.use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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
