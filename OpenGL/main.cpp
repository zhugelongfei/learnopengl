//
//  main.cpp
//  OpenGL
//
//  Created by 诸葛龙飞 on 2020/11/24.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// glfw and glad
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// imgui
#include "ImGui/imgui.h"
#include "ImGui/impl/imgui_impl_glfw.h"
#include "ImGui/impl/imgui_impl_opengl3.h"

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// myself class
#include "Object/Object.hpp"
#include "World/World.hpp"
#include "Mesh/Mesh.hpp"
#include "Shader/ShaderProg.hpp"
#include "Camera/Camera.hpp"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void showSimpleWindow();
void showInspectorWindow();

int windowWidth = 800;
int windowHeight = 600;

float deltaTime = 0.0f;
float lastTime = 0.0f;

bool enableCursor = true;
bool firstIn = true;
float lastMouseX = 0;
float lastMouseY = 0;

Camera cam(45.0f, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

int main(int argc, char **argv)
{
    // init glfw
    glfwInit();
    
    // set opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    //use core_profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //向前兼容
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //create glfw window(width, height, window's name, null, null)
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glViewport(0, 0, windowWidth, windowHeight);
    
    // cursor
    if (enableCursor)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    // event call back
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // setup dear imgui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    //(void)io;//???????
    
    // setup imgui style
    ImGui::StyleColorsDark();
    
    // setup platform/renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    glEnable(GL_DEPTH_TEST);

    // create mesh
    Mesh mesh;

    // create shader program
    ShaderProg shader("./Res/Shaders/vertShader.vsh", "./Res/Shaders/fragShader.fsh");
    shader.setTexture("./Res/Images/test1.jpeg", "mainTex", 0);
    shader.setTexture("./Res/Images/test.jpeg", "secTex", 1);

    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//GL_LINE
    
    World& world = World::GetInstance();
    
    Object light("Light");
    Object cube("Cube", &light);
    
    std::cout << "Name : " << *light.name << std::endl;
    
    while(!glfwWindowShouldClose(window))
    {
        // calculate delta time
        float curTime = glfwGetTime();
        deltaTime = curTime - lastTime;
        lastTime = curTime;

        // process input
        processInput(window);
        
        // start the dear imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        showSimpleWindow();
        
        showInspectorWindow();
        
        ImGui::Render();

        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        
        // update camera pos
        float x = sin(glfwGetTime()) * 5;
        float y = cos(glfwGetTime()) * 5;
        
        glm::mat4 viewMat = glm::lookAt(glm::vec3(x, 0, y), glm::vec3(0, 0, 0), glm::vec3(0, 1.0f, 0));
        
        // set projection matrix
        glm::mat4 projection = cam.GetProjectionMat(windowWidth, windowHeight);
        shader.setMatrix4("projectionMat", glm::value_ptr(projection));
        
        // set view matrix
        glm::mat4 view = cam.GetViewMat();
        shader.setMatrix4("viewMat", glm::value_ptr(viewMat));
        
        // draw cube
        int count = sizeof(cubePositions) / sizeof(glm::vec3);
        for (int i = 0; i < count; ++i) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMatrix4("modelMat", glm::value_ptr(model));
            
            mesh.draw();
        }

        // draw dear imgui
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(ECameraDir::Forward, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(ECameraDir::Backward, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.ProcessKeyboard(ECameraDir::Left, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.ProcessKeyboard(ECameraDir::Right, deltaTime);
    else if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        enableCursor = !enableCursor;
        if (enableCursor)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstIn)
    {
        lastMouseX = xPos;
        lastMouseY = yPos;
        firstIn = false;
    }
    float deltaX = xPos - lastMouseX;
    float deltaY = yPos - lastMouseY;
    lastMouseX = xPos;
    lastMouseY = yPos;
    
    if (!enableCursor)
    {
        cam.ProcessMouseMove(deltaX, deltaY, deltaTime);
    }
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    //fov = fmax(1.0f, fmin(45.0f, fov - yOffset * deltaTime));
}

bool showFrameCount = false;
float sliderVal = 0.2f;
float lightCol[4];

void showSimpleWindow()
{
    ImGui::ShowDemoWindow();
    ImGui::Begin("Simple window");
    
    ImGui::Text("Camera Pos : %.2f \t %.2f \t %.2f", cam.pos.x, cam.pos.y, cam.pos.z);
    
    ImGui::Text("Pitch : %.2f", cam.pitch);
    ImGui::Text("Yaw : %.2f", cam.yaw);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    
    ImGui::ColorEdit4("Light", lightCol);

    ImGui::End();
}

void showInspectorWindow()
{
    ImGui::Begin("Inspector");
    
    World& world = World::GetInstance();
    for (std::vector<Object*>::iterator it = world.totalObjVec.begin(); it != world.totalObjVec.end(); it++) {
        Object* obj = *it;
        if(obj->parent == NULL)
        {
            // draw it.
            ImGui::TreeNode(obj->name->c_str());
        }
    }
    
    ImGui::End();
}
