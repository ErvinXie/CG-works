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
#include "class/Physics/Sphere.h"
#include "class/Physics/Plane.h"
#include "class/Physics/Box.h"


#include <glad/glad.h>
#include <glfw3.h>
#include <tools/RandMath.h>


void processInput(GLFWwindow *window, Sphere &player); //处理输入



void InitializeGLFW();              //Ervin 初始化工作环境
GLFWwindow *InitializeGLFWwindow(); //Ervin 初始化窗口

// 摄像机参数
Camera camera;
int pause = 0;
float pauseTime = 0;
int reset = 0;


//时间差
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间


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

    Shader targetShader((shaders_path + "/color.vert").data(), (shaders_path + "/color.frag").data());
    Shader skyGroundShader((shaders_path + "/color.vert").data(), (shaders_path + "/color.frag").data());


    //加载模型
    Plane sky(glm::vec3(0, 50, 0), glm::vec3(0, -1, 0));
    Plane ground(glm::vec3(0, -50, 0), glm::vec3(0, 1, 0));
    Sphere player;
    const int box_num = 10;
    Box box[box_num];

    //initialize
    int score = 0;
    player.center = glm::vec3(0, 20, 0);
    player.velocity = glm::vec3(0, 0, 0);
    player.roMat = glm::mat4(1.0f);
    player.omega = glm::vec3(0, 0, 0);
    player.resist = 2;
    player.normal_loss = 0.0;
    int player_at = 0;
    int target_box = 1;
    box[0] = Box(glm::vec3(0, -5, 0), 3, 1, 3);

    for (int i = 1; i < box_num; i++) {
        box[i] = box[i - 1].getNext(score);
    }
    //initialize end


    //引力区
    const int sphere_num = 0;
    Sphere sphere[sphere_num];
    for (int i = 0; i < sphere_num; i++) {
        sphere[i].center = glm::vec3(RAND(-10, 10), RAND(-10, 10), RAND(-10, 10));
        sphere[i].velocity = glm::vec3(RAND(0, 1), RAND(0, 1), RAND(0, 1));
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
        if (pause >= 2||pauseTime > 0) {
            if (pauseTime > 0) {
                pauseTime -= deltaTime;
                if (pauseTime <= 0) {
                    reset = 1;
                    pauseTime = 0;
                }
            }
            deltaTime = 0;
        }
        lastFrame = currentFrame;

        //reset
        if (reset == 1) {
            int score = 0;
            player.center = glm::vec3(0, 20, 0);
            player.velocity = glm::vec3(0, 0, 0);
            player.roMat = glm::mat4(1.0f);
            player.omega = glm::vec3(0, 0, 0);
            player_at = 0;
            target_box = 1;
            box[0] = Box(glm::vec3(0, -5, 0), 3, 3, 3);
            for (int i = 1; i < box_num; i++) {
                box[i] = box[i - 1].getNext(score);
            }
            reset = 2;
            continue;
        }
        //reset


        camera.Position = glm::vec3(0, player.radius, 0) + player.center - 15.0f * camera.Front;
        float G = 100.0f;
        glm::vec3 forces[sphere_num];
        for (int i = 0; i < sphere_num; i++) {
            forces[i] = glm::vec3(0, 0, 0);
            for (int j = 0; j < box_num; j++) {
                sphere[i].hitBox(box[j]);
            }
            for (int j = 0; j < sphere_num; j++) {
                if (i != j) {
                    sphere[i].hitSphere(sphere[j]);
                    float distance = glm::length(sphere[i].center - sphere[j].center);
                    if (distance < 0.1)
                        break;
                    forces[i] += glm::normalize(sphere[j].center - sphere[i].center) *
                                 (sphere[i].mass * sphere[j].mass * G / distance / distance);
                }
            }
        }
        for (int i = 0; i < sphere_num; i++) {
            sphere[i].hitPlane(ground);
            sphere[i].move(forces[i], deltaTime);
        }


        //About the player
        for (int i = 0; i < box_num; i++) {
            if (player.hitBox(box[i])) {
                player_at = i;
                if (target_box == i) {
                    score += 1;
//                    target_box = (target_box + (int) RAND(1.1, box_num + 0.1)) % box_num;
                    box[((target_box - 1) % box_num + box_num) % box_num] = box[((target_box - 2) % box_num + box_num) %
                                                                                box_num].getNext(score);
                    target_box = (target_box + 1) % box_num;
                }
            }
        }
        player.hitPlane(sky);
        if (pauseTime == 0 && player.hitPlane(ground)) {
            pauseTime = 1;
        }
        player.move(glm::vec3(0, -50, 0), deltaTime);


        // input
        // -----
        processInput(window, player);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects

        //set the shader
        skyGroundShader.use();
        PointLight *pointLight = new PointLight(player.center);
        pointLight->light_strength = 1;
        pointLight->constant = 1.5;
        pointLight->quadratic = 0.00001;
        pointLight->linear = 0.00005;
        pointLight->setShader(skyGroundShader, 0);

        SpotLight *spotLight = new SpotLight(player.center - glm::vec3(0, player.radius, 0), glm::vec3(0, -1, 0));
        spotLight->constant = 1;
        spotLight->quadratic = 0;
        spotLight->linear = 0;
        spotLight->specular = glm::vec3(-0.5, -0.5, -0.5);
        spotLight->setShader(skyGroundShader, 0);




        //render the sky and the ground
        sky.Draw(skyGroundShader, camera);
//        ground.Draw(skyGroundShader, camera);

        //Draw the plantes
//        lightingShader.use();
//        Light *light;
//        light = new SpotLight(camera.Position, camera.Front);
//        light->setShader(lightingShader, -1);
//        for (int i = 0; i < sphere_num; i++) {
//            light = new PointLight(sphere[i].center);
//            light->light_strength = 0.33;
//            light->setShader(lightingShader, i);
//        }
//        for (int i = 0; i < sphere_num; i++) {
//            sphere[i].Draw(lightingShader, camera);
//        }

        for (int i = 0; i < sphere_num; i++) {
            sphere[i].Draw(skyGroundShader, camera);
        }
        //Draw the boxes

        for (int i = 0; i < box_num; i++) {
            if (i != target_box)
                box[i].Draw(skyGroundShader, camera);
        }
        player.Draw(skyGroundShader, camera);



        //Draw the target Box
        targetShader.use();
        pointLight->specular = glm::vec3(sinf(currentFrame) * 0.3 + 0.3, cosf(currentFrame) * 0.3 + 0.3,
                                         (sinf(currentFrame) + cosf(currentFrame)) / sqrt(2) * 0.3 + 0.3);
        pointLight->setShader(targetShader, 0);
        box[target_box].Draw(targetShader, camera);

        pointLight->specular = glm::vec3(1, 1, 1) -
                               glm::vec3(sinf(currentFrame) * 0.3 + 0.3, cosf(currentFrame) * 0.3 + 0.3,
                                         (sinf(currentFrame) + cosf(currentFrame)) / sqrt(2) * 0.3 + 0.3);
        pointLight->setShader(targetShader, 0);
        ground.Draw(targetShader, camera);



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

float jump_cd = 0;

void processInput(GLFWwindow *window, Sphere &player) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

//    float cameraDelta = camera.cameraSpeed * deltaTime; // adjust accordingly
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera.translate(cameraDelta * camera.Front);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera.translate(-cameraDelta * camera.Front);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera.translate(-glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraDelta);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera.translate(glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraDelta);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (pause == 0)pause = 2;
        if (pause == 3)pause = 1;
    } else {
        if (pause == 1)pause = 0;
        if (pause == 2)pause = 3;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (reset == 0)reset = 1;
    } else {
        if (reset == 2)reset = 0;
    }

    float playerDelta = deltaTime * 40;
    glm::vec3 r = glm::normalize(glm::cross(camera.Front, camera.Up));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.velocity += playerDelta * -glm::normalize(glm::cross(r, camera.Up));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.velocity += playerDelta * glm::normalize(glm::cross(r, camera.Up));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.velocity += playerDelta * -r;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.velocity += playerDelta * r;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && jump_cd < 0) {
        player.velocity += glm::vec3(0, 25, 0);
        jump_cd = 1;
    }
    jump_cd -= deltaTime;
//    cout<<"FPS:"<<1.0/deltaTime<<endl;

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
    GLFWwindow *window = glfwCreateWindow(camera.screen_width, camera.screen_height, "LearnOpenGL", NULL, NULL);
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

