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
#include "Vec.h"
#include "Octasphere.h"
#include "IcoSphere.h"
#include "Cone.h"
#include "Camera.h"

bool printed_error;
void GLAPIENTRY MessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
)
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

//Window
int width = 1000;
int height = 1000;

//Imgui controls
float lightPos[3] = { 9,25,262 };
int splits = 0;
bool computed = true;

//timing
float dT = 0.0f;
float lastFrame = 0.0f;

//camera
Camera cam(glm::vec3(0, 0, 500), glm::vec3(0, 1, 0));
float lastX = width / 2;
float lastY = height / 2;
bool firstMouse = true;
bool isMovementEnabled = true;
bool isPanning = false;

void Drawstuff(Shader*);
void ImGUIsetup(void);
void ProcessInput(GLFWwindow*);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

//    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Ray Tracing", NULL, NULL);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    std::vector<float> triangle_vertices;
    std::vector<int> triangle_indices;
    IcoSphere ico_sphere(splits);
    ico_sphere.generateSphere(triangle_indices, triangle_vertices);
//    Cone cone(100, 50, splits);
//    cone.generate(triangle_indices, triangle_vertices);
    VertexArray va;
    VertexBuffer vb((const unsigned int*)triangle_vertices.data(), triangle_vertices.size() * sizeof(float));

    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(3);
    va.addBuffer(vb, layout);

    IndexBuffer ib((const unsigned int*)triangle_indices.data(), triangle_indices.size());

    Shader shader("res/shaders/basic.shader");
    shader.bind();

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
        shader.setUniform3f("light_pos", lightPos[0], lightPos[1], lightPos[2]);
        if (!computed)
        {
            triangle_indices.clear();
            triangle_vertices.clear();
            ico_sphere.setSplits(splits);
            ico_sphere.generateSphere(triangle_indices, triangle_vertices);
//            cone.setParams(100, 50, splits);
//            cone.generate(triangle_indices, triangle_vertices);
            vb.update((const unsigned int*)triangle_vertices.data(), triangle_vertices.size() * sizeof(float));
            ib.update((const unsigned int*)triangle_indices.data(), triangle_indices.size());
            computed = true;
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader.setUniform4f("u_color", 1.0, 1.0f, 0.0f, 1.0f);
        renderer.draw(va, ib, shader);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.f, -1.f);
        shader.setUniform4f("u_color", 1.0, 0.0f, 0.0f, 1.0f);
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

void Drawstuff(Shader* shader) {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    shader->setUniform4f("u_color", 1.0, 1.0f, 0.0f, 1.0f);
    shader->setUniform3f("light_pos", lightPos[0], lightPos[1], lightPos[2]);

    model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    view = cam.GetViewMatrix();
    projection = glm::perspective(radians(cam.Zoom), (float)width / (float)height, 50.0f, 1000.0f);

    shader->setUniformMat4f("model", model);
    shader->setUniformMat4f("view", view);
    shader->setUniformMat4f("projection", projection);
}

void ProcessInput(GLFWwindow* window) {
    dT = glfwGetTime() - lastFrame;
    lastFrame = glfwGetTime();

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        isMovementEnabled = !isMovementEnabled;
        if (isMovementEnabled) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(Camera_Movement::FORWARD, dT);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(Camera_Movement::BACKWARD, dT);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.ProcessKeyboard(Camera_Movement::LEFT, dT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.ProcessKeyboard(Camera_Movement::RIGHT, dT);

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
        cam.ProcessMouseMovement(dx, dy);
    if (isPanning)
        cam.ProcessMousePan(dx, dy);
}

void ScrollCallback(GLFWwindow* window, double dx, double dy) {
    cam.ProcessMouseScroll(dy);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        isPanning = true;
        isMovementEnabled = false;
    }
    else if (isPanning == true) {
        isPanning = false;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        isMovementEnabled = true;
    }
    else if (isMovementEnabled == true) {
        isMovementEnabled = false;
    }
}

void ImGUIsetup() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Testing");
        ImGui::SliderFloat3("Light position", lightPos, -500.0f, 500.0f);
        if (ImGui::Button("Increase splits"))
        {
            splits++;
            computed = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Decrease splits") && splits > 0)
        {
            splits--;
            computed = false;
        }
        ImGui::Text("Splits = %d", splits);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}