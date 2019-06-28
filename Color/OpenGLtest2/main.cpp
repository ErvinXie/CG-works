#include <iostream>
#include <cmath>



//#include <glad/glad.h>

//local setting
#include <local_settings.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "class/Camera.h"
#include "class/Light.h"
#include "class/Shader.h"
#include "class/Material.h"
#include "class/Model.h"
#include "class/Physics/Sphere.h"


#include <glad/glad.h>
#include <glfw3.h>
#include <tools/RandMath.h>



void processInput(GLFWwindow *window); //处理输入



void InitializeGLFW();              //Ervin 初始化工作环境
GLFWwindow *InitializeGLFWwindow(); //Ervin 初始化窗口

// 摄像机参数
Camera camera;


//时间差
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
// 窗口设置参数
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

int main() {

    InitializeGLFW();

    // glfw 创建窗口
    // ------------
    GLFWwindow *window = InitializeGLFWwindow();

    // glad stand for OpenGL Loading Library
    // glad: 加载所有OpenGL函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    //set the seed of random number
    srand((unsigned) time(NULL));
    //设置Shader

    Shader lightingShader((shaders_path + "/color.vert").data(), (shaders_path + "/color.frag").data());

//    Model ourModel(object_path+"/sphere/sphere.obj");
//    Model ourModel(object_path + "/scifi tropical city/Sci-fi Tropical city.obj");

    const int sn = 20;
    Sphere s[sn];

    for (int i = 0; i < sn; i++) {
        s[i].position = glm::vec3(RAND(-10,10),RAND(-10,10),RAND(-10,10));
        s[i].velocity = glm::vec3(RAND(0,1),RAND(0,1),RAND(0,1));
    }

    //渲染循环
    //-------
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        // animation
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float G = 10.0f;
        glm::vec3 forces[sn];
        for (int i = 0; i < sn; i++) {
            forces[i] = glm::vec3(0, 0, 0);
            for (int j = 0; j < sn; j++) {
                if (i != j) {
                    s[i].hitSphere(s[j]);

                    float distance = glm::length(s[i].position - s[j].position);
                    if (distance < 0.1)
                        break;
                    forces[i] += glm::normalize(s[j].position - s[i].position) *
                                 (s[i].mass * s[j].mass * G / distance / distance);
                }
            }
        }
        for (int i = 0; i < sn; i++) {
            s[i].move(forces[i], deltaTime);
        }

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();

        //set the lighting
        Light *light = new SpotLight(camera.Position, camera.Front);
        light->setShader(lightingShader, 0);
        for (int i = 0; i < sn; i++) {
            light = new PointLight(s[i].position);
            light->light_strength = 0.1;
            light->setShader(lightingShader, i);
        }


        for (int i = 0; i < sn; i++) {
            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float) SCR_WIDTH / (float) SCR_HEIGHT,
                                                    camera.near_distance, camera.far_distance);
            glm::mat4 view = camera.View();
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);

            // render the loaded model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, s[i].position);
            model = glm::scale(model, glm::vec3(1, 1, 1) * s[i].radius);

            lightingShader.setMat4("model", model);
            s[i].Draw(lightingShader);
        }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = camera.cameraSpeed * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.translate(cameraSpeed * camera.Front);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.translate(-cameraSpeed * camera.Front);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.translate(-glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.translate(glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed);
}

//回调函数
//------
void framebuffer_size_callback(GLFWwindow *window, int width, int height); //窗口尺寸改变时的回调函数
void mouse_callback(GLFWwindow *window, double xpos, double ypos);         //鼠标事件回调函数
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);  //鼠标滚轮函数

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

//鼠标事件回调函数
//-------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    static float lastX = 400, lastY = 300;
    static bool firstMouse = true;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.rotate(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.setFov(camera.Fov - yoffset);
}

// glfw for Graphics Library Framework.
// glfw 初始化设置
// -------------
void InitializeGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
}

//初始化GLFW窗口
//------------
GLFWwindow *InitializeGLFWwindow() {
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    return window;
};

