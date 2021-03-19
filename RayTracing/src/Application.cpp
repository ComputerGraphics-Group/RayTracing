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

struct Vec
{
    float x;
    float y;
    float z;
};

Vec addVecs(Vec vec1, Vec vec2)
{
    return { vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z };
}

Vec subtractVecs(Vec vec1, Vec vec2)
{
    return { vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z };
}

Vec scaleVec(Vec vec, float scale)
{
    return { vec.x * scale, vec.y * scale, vec.z * scale };
}

void pushVertex(std::vector<float>& points, Vec vec)
{
    points.push_back(vec.x);
    points.push_back(vec.y);
    points.push_back(vec.z);
}

Vec normalize(Vec vec, float length)
{
    float dx = vec.x;
    float dy = vec.y;
    float dz = vec.z;
    float factor = length / sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    dx = dx * factor;
    dy = dy * factor;
    dz = dz * factor;
    return { dx, dy, dz };
}

void splitTriangle(std::vector<int>& indices, std::vector<float>& points, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int n_split)
{
    int offset = points.size()/6;
    int divisions = pow(2, n_split);
    Vec vec1 = { x1, y1, z1 };
    Vec vec2 = { x2, y2, z2 };
    Vec vec3 = { x3, y3, z3 };

    Vec veca = vec1;
    Vec vecb = vec1;
    Vec incra = scaleVec(subtractVecs(vec2, vec1), (float)1 / (float)divisions);
    Vec incrb = scaleVec(subtractVecs(vec3, vec1), (float)1 / (float)divisions);

    for (int i = 0; i <= divisions; ++i)
    {
        for (int j = i; j <= divisions; ++j)
        {
            pushVertex(points, normalize(vecb, 100));      //Vertex
            pushVertex(points, normalize(vecb, 1));        //Normal
            vecb = addVecs(vecb, incrb);
        }
        veca = addVecs(veca, incra);
        vecb = veca;
    }

    int a = 0;
    int ia = divisions + 1;
    for (int i = 0; i < divisions; ++i)
    {
        for (int j = 0; j < divisions - i; ++j)
        {
            indices.push_back(a + j + offset);
            indices.push_back(a + j + 1 + offset);
            indices.push_back(a + ia + j + offset);
            if (j < divisions - i - 1)
            {
                indices.push_back(a + j + 1 + offset);
                indices.push_back(a + ia + j + offset);
                indices.push_back(a + ia + j + 1 + offset);
            }
        }
        a += ia;
        ia--;
    }
}

void splitOcta(std::vector<int>& triangle_indices, std::vector<float>& triangle_points, int n_split)
{
    splitTriangle(triangle_indices, triangle_points, 10, 0, 0, 0, 10, 0, 0, 0, 10, n_split);
    splitTriangle(triangle_indices, triangle_points, -10, 0, 0, 0, 10, 0, 0, 0, 10, n_split);
    splitTriangle(triangle_indices, triangle_points, 10, 0, 0, 0, 10, 0, 0, 0, -10, n_split);
    splitTriangle(triangle_indices, triangle_points, -10, 0, 0, 0, 10, 0, 0, 0, -10, n_split);
    splitTriangle(triangle_indices, triangle_points, 10, 0, 0, 0, -10, 0, 0, 0, 10, n_split);
    splitTriangle(triangle_indices, triangle_points, -10, 0, 0, 0, -10, 0, 0, 0, 10, n_split);
    splitTriangle(triangle_indices, triangle_points, 10, 0, 0, 0, -10, 0, 0, 0, -10, n_split);
    splitTriangle(triangle_indices, triangle_points, -10, 0, 0, 0, -10, 0, 0, 0, -10, n_split);
}

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

//    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 1000, "Bresenham's line drawing", NULL, NULL);
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

    std::vector<float> triangle_points;
    std::vector<int> triangle_indices;
    int splits = 2;
    splitOcta(triangle_indices, triangle_points, splits);

    const unsigned int* triangle_indices_array = (const unsigned int*)&triangle_indices[0];
    const unsigned int* triangle_points_array = (const unsigned int*)&triangle_points[0];

    /*unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);*/

    VertexArray va;
    VertexBuffer vb(triangle_points_array, triangle_points.size() * sizeof(float));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(sizeof(float) * 3));

    /*VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(3);
    va.addBuffer(vb, layout);*/

    IndexBuffer ib(triangle_indices_array, triangle_indices.size());

    Shader shader("res/shaders/basic.shader");
    shader.bind();

    Renderer renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    glm::vec3 translationA(0, 0, 0);
    float angle_of_rotation[3] = { 0, 0, 0 };
    float lightPos[3] = { 9,25,262 };
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 mvp;
    bool computed = true;

    while (!glfwWindowShouldClose(window))
    {
        renderer.clear();
        glfwPollEvents();

        shader.bind();
        shader.setUniform4f("u_color", 1.0, 1.0f, 0.0f, 1.0f);
        shader.setUniform3f("light_pos", lightPos[0], lightPos[1], lightPos[2]);
        if (!computed)
        {
            triangle_points.clear();
            triangle_indices.clear();
            splitOcta(triangle_indices, triangle_points, splits);
            triangle_indices_array = (const unsigned int*)&triangle_indices[0];
            triangle_points_array = (const unsigned int*)&triangle_points[0];

            vb.update(triangle_points_array, triangle_points.size() * sizeof(float));
            ib.update(triangle_indices_array, triangle_indices.size());
            computed = true;
        }
        model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::translate(model, translationA);
        model = glm::rotate(model, angle_of_rotation[0], glm::vec3(1, 0, 0));
        model = glm::rotate(model, angle_of_rotation[1], glm::vec3(0, 1, 0));
        model = glm::rotate(model, angle_of_rotation[2], glm::vec3(0, 0, 1));
        view = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -500));
        projection = glm::perspective(0.78f, 1.0f, 50.0f, 1000.0f);
        shader.setUniformMat4f("model", model);
        shader.setUniformMat4f("view", view);
        shader.setUniformMat4f("projection", projection);
        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        renderer.draw(va, ib, shader);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.f, -1.f);
        shader.setUniform4f("u_color", 0.0, 0.0f, 0.0f, 1.0f);
        renderer.draw(va, ib, shader);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Testing");
            if (ImGui::Button("Increase splits"))
            {
                splits++;
                computed = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Deecrease splits") && splits>0)
            {
                splits--;
                computed = false;
            }
            ImGui::Text("Splits = %d", splits);
            ImGui::SliderFloat3("Translation", &translationA.x, -500.0f, 500.0f);
            ImGui::SliderFloat3("Rotaion", angle_of_rotation, -3.14f, 3.14f);
            ImGui::SliderFloat3("Light position", lightPos, -500.0f, 500.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
