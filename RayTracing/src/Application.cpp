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
#include "Grid.h"
#include "ModelLoader.h"

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

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void ScrollCallback(GLFWwindow* window, double dx, double dy);

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

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
bool isMovementEnabled = false;
bool isPanning = false;

void Drawstuff(Shader*, glm::mat4 model);
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
    std::vector<float> cow_points;
    std::vector<int> cow_indices;
    ModelLoader modelLoader;
    modelLoader.loadModel("res/models/cow.obj", cow_indices, cow_points);
    VertexArray vacow;
    VertexBuffer vbcow((const unsigned int*)cow_points.data(), cow_points.size() * sizeof(float));
    VertexBufferLayout layoutcow;
    layoutcow.push<float>(3);
    vacow.addBuffer(vbcow, layoutcow);
    IndexBuffer ibcow((const unsigned int*)cow_indices.data(), cow_indices.size());

    std::vector<float> teddy_points;
    std::vector<int> teddy_indices;
    modelLoader.loadModel("res/models/teddy.obj", teddy_indices, teddy_points);
    std::cout << teddy_points.size() << std::endl;
    VertexArray vateddy;
    VertexBuffer vbteddy((const unsigned int*)teddy_points.data(), teddy_points.size() * sizeof(float));
    VertexBufferLayout layoutteddy;
    layoutteddy.push<float>(3);
    vateddy.addBuffer(vbteddy, layoutteddy);
    IndexBuffer ibteddy((const unsigned int*)teddy_indices.data(), teddy_indices.size());

    std::vector<float> grid_points;
    std::vector<int> grid_indices;
    Grid grid(-1000, 1000, -1000, 1000, 50, 0);
    grid.generateGrid(grid_indices, grid_points);
    VertexArray vag;
    VertexBuffer vbg((const unsigned int*)grid_points.data(), grid_points.size() * sizeof(float));
    VertexBufferLayout layoutg;
    layoutg.push<float>(3);
    vag.addBuffer(vbg, layoutg);
    IndexBuffer ibg((const unsigned int*)grid_indices.data(), grid_indices.size());

    std::vector<float> octa_sphere_vertices;
    std::vector<int> octa_sphere_indices;
    OctaSphere octa_sphere(10, splits);
    octa_sphere.generateSphere(octa_sphere_indices, octa_sphere_vertices);
    VertexArray vaos;
    VertexBuffer vbos((const unsigned int*)octa_sphere_vertices.data(), octa_sphere_vertices.size() * sizeof(float));
    VertexBufferLayout layoutos;
    layoutos.push<float>(3);
    vaos.addBuffer(vbos, layoutos);
    IndexBuffer ibos((const unsigned int*)octa_sphere_indices.data(), octa_sphere_indices.size());

    std::vector<float> ico_sphere_vertices;
    std::vector<int> ico_sphere_indices;
    IcoSphere ico_sphere(splits);
    ico_sphere.generateSphere(ico_sphere_indices, ico_sphere_vertices);
    VertexArray vais;
    VertexBuffer vbis((const unsigned int*)ico_sphere_vertices.data(), ico_sphere_vertices.size() * sizeof(float));
    VertexBufferLayout layoutis;
    layoutis.push<float>(3);
    vais.addBuffer(vbis, layoutis);
    IndexBuffer ibis((const unsigned int*)ico_sphere_indices.data(), ico_sphere_indices.size());

    std::vector<float> cone_vertices;
    std::vector<int> cone_indices;
    Cone cone(100, 50, 0);
    cone.generate(cone_indices, cone_vertices);
    VertexArray vac;
    VertexBuffer vbc((const unsigned int*)cone_vertices.data(), cone_vertices.size() * sizeof(float));
    VertexBufferLayout layoutc;
    layoutc.push<float>(3);
    vac.addBuffer(vbc, layoutc);
    IndexBuffer ibc((const unsigned int*)cone_indices.data(), cone_indices.size());

    Shader shader("res/shaders/basic.shader");

    Renderer renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    glm::mat4 model;

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        renderer.clear();
        glfwPollEvents();

        Drawstuff(&shader, model);
        shader.bind();
        if (!computed)
        {
            octa_sphere_indices.clear();
            octa_sphere_vertices.clear();
            octa_sphere.setSplits(splits);
            octa_sphere.generateSphere(octa_sphere_indices, octa_sphere_vertices);
            vbos.update((const unsigned int*)octa_sphere_vertices.data(), octa_sphere_vertices.size() * sizeof(float));
            ibos.update((const unsigned int*)octa_sphere_indices.data(), octa_sphere_indices.size());

            ico_sphere_indices.clear();
            ico_sphere_vertices.clear();
            ico_sphere.setSplits(splits);
            ico_sphere.generateSphere(ico_sphere_indices, ico_sphere_vertices);
            vbis.update((const unsigned int*)ico_sphere_vertices.data(), ico_sphere_vertices.size() * sizeof(float));
            ibis.update((const unsigned int*)ico_sphere_indices.data(), ico_sphere_indices.size());
            
            cone_indices.clear();
            cone_vertices.clear();
            cone.setParams(100, 50, splits);
            cone.generate(cone_indices, cone_vertices);
            vbc.update((const unsigned int*)cone_vertices.data(), cone_vertices.size() * sizeof(float));
            ibc.update((const unsigned int*)cone_indices.data(), cone_indices.size());

            computed = true;
        }
        //grid
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.f, -1.f);
        model = glm::translate(model, glm::vec3(0, -50, 0));
        shader.setUniformMat4f("model", model);
        shader.setUniform4f("u_color", 0.0, 0.0f, 1.0f, 1.0f);
        renderer.draw(vag, ibg, shader);

        //octasphere
        /*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        model = glm::translate(model, glm::vec3(-150, 100, -100));        
        shader.setUniformMat4f("model", model);
        shader.setUniform4f("u_color", 1.0, 1.0f, 0.0f, 1.0f);
        renderer.draw(vaos, ibos, shader);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.f, -1.f);
        shader.setUniform4f("u_color", 1.0, 0.0f, 0.0f, 1.0f);
        renderer.draw(vaos, ibos, shader);

        //icosphere
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        model = glm::translate(model, glm::vec3(300, 0, 0));
        shader.setUniformMat4f("model", model);
        shader.setUniform4f("u_color", 1.0, 1.0f, 0.0f, 1.0f);
        renderer.draw(vais, ibis, shader);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.f, -1.f);
        shader.setUniform4f("u_color", 1.0, 0.0f, 0.0f, 1.0f);
        renderer.draw(vais, ibis, shader);

        //cone
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        model = glm::translate(model, glm::vec3(-150, 0, 0));
        shader.setUniformMat4f("model", model);
        shader.setUniform4f("u_color", 1.0, 1.0f, 0.0f, 1.0f);
        renderer.draw(vac, ibc, shader);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.f, -1.f);
        shader.setUniform4f("u_color", 1.0, 0.0f, 0.0f, 1.0f);
        renderer.draw(vac, ibc, shader);*/

        ////cow
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        model = glm::translate(model, glm::vec3(0, 180, 0));
        model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
        shader.setUniformMat4f("model", model);
        shader.setUniform4f("u_color", 1.0, 1.0f, 0.0f, 1.0f);
        renderer.draw(vacow, ibcow, shader);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.f, -1.f);
        shader.setUniform4f("u_color", 1.0, 0.0f, 0.0f, 1.0f);
        renderer.draw(vacow, ibcow, shader);

        //teddy
        /*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        model = glm::translate(model, glm::vec3(0, 300, -300));
        model = glm::scale(model, glm::vec3(10, 10, 10));
        shader.setUniformMat4f("model", model);
        shader.setUniform4f("u_color", 1.0, 1.0f, 0.0f, 1.0f);
        renderer.draw(vateddy, ibteddy, shader);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.f, -1.f);
        shader.setUniform4f("u_color", 1.0, 0.0f, 0.0f, 1.0f);
        renderer.draw(vateddy, ibteddy, shader);*/

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

void Drawstuff(Shader* shader, glm::mat4 model) {
    glm::mat4 view;
    glm::mat4 projection;

    view = cam.GetViewMatrix();
    projection = glm::perspective(radians(cam.Zoom), (float)width / (float)height, 1.0f, 10000.0f);

    shader->setUniformMat4f("model", model);
    shader->setUniformMat4f("view", view);
    shader->setUniformMat4f("projection", projection);
}

void ProcessInput(GLFWwindow* window) {
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
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        isPanning = true;
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