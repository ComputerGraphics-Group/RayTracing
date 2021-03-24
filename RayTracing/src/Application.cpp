#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_opengl3.h"
#include "external/imgui/imgui_impl_glfw.h"
#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"


bool printed_error;
void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    if (!printed_error)
    {
        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
        printed_error = true;
    }

}

void MouseCallback(GLFWwindow* window, double xpos, double ypos);

void ScrollCallback(GLFWwindow* window, double dx, double dy);

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

//Window
int width = 1000;
int height = 1000;

//Imgui controls
float lightPos[3] = { 9,25,262 };

//timing
float dT = 0.0f;
float lastFrame = 0.0f;

//camera
Camera cam(glm::vec3(0,0,500),glm::vec3(0,1,0));
float lastX = width / 2;
float lastY = height / 2;
bool firstMouse = true;
bool isMovementEnabled = false;
bool isPanning = false;

void Drawstuff(Shader*);
void ImGUIsetup(void);
void ProcessInput(GLFWwindow*);

/// The driving function of the program.
/// 
/// This function initialises OpenGL windows, sets up the Array Buffer and assigns the provided data into it, sets up the shaders
/// and draws the given vertices. It also records the runtime of each passthrough render.
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 1000, "Raytracing", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, NULL);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetKeyCallback(window, KeyCallback);

    std::cout << glGetString(GL_VERSION) << std::endl;

    float points[] = {
        -50.0f, -50.0f, -50.0f,  0.0f, -1.0f,  0.0f,          //0
        -50.0f, -50.0f,  50.0f,  0.0f, -1.0f,  0.0f,          //1
         50.0f, -50.0f, -50.0f,  0.0f, -1.0f,  0.0f,          //2
         50.0f, -50.0f,  50.0f,  0.0f, -1.0f,  0.0f,          //3

         50.0f,  50.0f, -50.0f,  0.0f,  1.0f,  0.0f,          //4
         50.0f,  50.0f,  50.0f,  0.0f,  1.0f,  0.0f,          //5
        -50.0f,  50.0f, -50.0f,  0.0f,  1.0f,  0.0f,          //6
        -50.0f,  50.0f,  50.0f,  0.0f,  1.0f,  0.0f,          //7

         50.0f, -50.0f, -50.0f,  1.0f,  0.0f,  0.0f,          //8
         50.0f, -50.0f,  50.0f,  1.0f,  0.0f,  0.0f,          //9
         50.0f,  50.0f, -50.0f,  1.0f,  0.0f,  0.0f,          //10
         50.0f,  50.0f,  50.0f,  1.0f,  0.0f,  0.0f,          //11

        -50.0f, -50.0f,  50.0f, -1.0f,  0.0f,  0.0f,          //12
        -50.0f, -50.0f, -50.0f, -1.0f,  0.0f,  0.0f,          //13
        -50.0f,  50.0f, -50.0f, -1.0f,  0.0f,  0.0f,          //14
        -50.0f,  50.0f,  50.0f, -1.0f,  0.0f,  0.0f,          //15

        -50.0f, -50.0f,  50.0f,  0.0f,  0.0f,  1.0f,          //16
         50.0f, -50.0f,  50.0f,  0.0f,  0.0f,  1.0f,          //17
         50.0f,  50.0f,  50.0f,  0.0f,  0.0f,  1.0f,          //18
        -50.0f,  50.0f,  50.0f,  0.0f,  0.0f,  1.0f,          //19

        -50.0f, -50.0f, -50.0f,  0.0f,  0.0f, -1.0f,          //20
         50.0f, -50.0f, -50.0f,  0.0f,  0.0f, -1.0f,          //21
         50.0f,  50.0f, -50.0f,  0.0f,  0.0f, -1.0f,          //22
        -50.0f,  50.0f, -50.0f,  0.0f,  0.0f, -1.0f,          //23
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3,

        4, 5, 6,
        5, 6, 7,

        8, 9, 10,
        9, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    VertexArray va;
    VertexBuffer vb(points, 6 * 4 * 6 * sizeof(float));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(sizeof(float) * 3));

    VertexBufferLayout layout;
    layout.push<float>(6);
    va.addBuffer(vb, layout);

    IndexBuffer ib(indices, 12*3);

    Shader shader("res/shaders/basic.shader");

    va.unbind();
    vb.unbind();
    ib.unbind();

    Renderer renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        renderer.clear();
        glfwPollEvents();

        Drawstuff(&shader);
        shader.bind();
        renderer.draw(va, ib, shader);

        ImGUIsetup();

        ProcessInput(window);

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void Drawstuff(Shader *shader) {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    shader->setUniform4f("u_color", 1.0, 1.0f, 0.0f, 1.0f);
    shader->setUniform3f("light_pos", lightPos[0], lightPos[1], lightPos[2]);

    model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    view = cam.GetViewMatrix();
    projection = glm::perspective(radians(cam.Zoom), (float)width/(float)height, 50.0f, 1000.0f);

    shader->setUniformMat4f("model", model);
    shader->setUniformMat4f("view", view);
    shader->setUniformMat4f("projection", projection);
}

void ProcessInput(GLFWwindow *window) {
    dT = glfwGetTime() - lastFrame;
    lastFrame = glfwGetTime();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(Camera_Movement::FORWARD, dT);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(Camera_Movement::BACKWARD, dT);


}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float dx = xpos - lastX;
    float dy = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (isMovementEnabled)
        cam.ProcessMouseMovement(dx, dy, 0);
    if (isPanning)
        cam.ProcessMousePan(dx, dy);
}

void ScrollCallback(GLFWwindow* window, double dx, double dy) {
    cam.ProcessMouseScroll(dy);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
        isPanning = true;
    else if (isPanning == true)
        isPanning = false;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        isMovementEnabled = true;
    else if (isMovementEnabled == true)
        isMovementEnabled = false;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_A && action == GLFW_REPEAT)
        cam.ProcessMouseMovement(0, 0, -10.0f);

    if (key == GLFW_KEY_D && action == GLFW_REPEAT)
        cam.ProcessMouseMovement(0, 0, 10.0f);
}

void ImGUIsetup() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Testing");
        ImGui::SliderFloat3("Light position", lightPos, -500.0f, 500.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}