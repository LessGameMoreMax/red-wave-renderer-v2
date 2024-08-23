#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Renderer/Source/Shader.h"
#include "Renderer/Source/Camera.h"
#include "Renderer/Source/Model.h"

#include <iostream>
using namespace sablin;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;
const string dir = "/home/sablin/Projects/red-wave-renderer-v2/Test/Renderer/";

// camera
Camera camera(glm::vec3(-5.0f, 3.0f, -3.0f));
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Renderer", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    // set depth function to less than AND equal for skybox depth trick.
    glDepthFunc(GL_LEQUAL);
    // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // 创建IBL
    IBL ibl(dir, "Textures/hdr/night.hdr");

    // 创建G-buffer
    Gbuffer gBuffer(SCR_WIDTH, SCR_HEIGHT);

    // 创建Geo Shader
    Shader geoPassShader(dir + "Shaders/geo_pass.vs", dir + "Shaders/geo_pass.fs");

    
    // 加载模型
    Model car(dir + "Models/car/scene.gltf");
    Model road(dir + "Models/street/scene.gltf");

    // 创建dirlights
    vector<DirLight> dirLights{
        // {glm::vec3(-10.0f,  10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f)},
        {glm::vec3( 10.0f,  10.0f, 10.0f), glm::vec3(100.0f, 100.0f, 100.0f)},
        // {glm::vec3(-10.0f,  10.0f,-10.0f), glm::vec3(300.0f, 300.0f, 300.0f)},
        // {glm::vec3( 10.0f,  10.0f,-10.0f), glm::vec3(300.0f, 300.0f, 300.0f)}
    };

    // 创建pointlights
    // vector<PointLight> pointlights{
    //     {glm::vec3(-10.0f,  10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f), 0.7f, 1.8f},
    //     {glm::vec3( 10.0f,  10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f), 0.7f, 1.8f},
    //     {glm::vec3(-10.0f,  10.0f,-10.0f), glm::vec3(300.0f, 300.0f, 300.0f), 0.7f, 1.8f},
    //     {glm::vec3( 10.0f,  10.0f,-10.0f), glm::vec3(300.0f, 300.0f, 300.0f), 0.7f, 1.8f}
    // };
    
    // 创建shadow shader
    ShadowShader shadowShader(dir, camera, dirLights[0].position_, SCR_WIDTH, SCR_HEIGHT);

    // 创建PBRShader
    PBRShader pbrShader(dir);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom_), (float)SCR_WIDTH / (float)SCR_HEIGHT, camera.near_plane_, camera.far_plane_);

        // car model
        glm::mat4 car_model = glm::mat4(1.0f);
        car_model = glm::translate(car_model, glm::vec3(0.0f, 0.0f, 0.0f));
        car_model = glm::scale(car_model, glm::vec3(1.0f, 1.0f, 1.0f));
        car_model = glm::rotate(car_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        car_model = glm::rotate(car_model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // road model
        glm::mat4 road_model = glm::mat4(1.0f);
        road_model = glm::translate(road_model, glm::vec3(-0.1f, 1.1f, -3.0f));
        road_model = glm::scale(road_model, glm::vec3(1.0f, 1.0f, 1.0f));
        road_model = glm::rotate(road_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // 1. Shadow Pass: 以光源视角渲染深度
        shadowShader.BeforeDraw();

        shadowShader.setMat4("model", car_model);
        car.Draw(shadowShader);

        shadowShader.setMat4("model", road_model);
        road.Draw(shadowShader);

        shadowShader.AfterDraw();

        // 2. Geometry Pass: 将场景渲染至G-buffer
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        geoPassShader.use();
        geoPassShader.setMat4("projection", projection);
        geoPassShader.setMat4("view", view);

        geoPassShader.setMat4("model", car_model);
        car.Draw(geoPassShader);

        geoPassShader.setMat4("model", road_model);
        road.Draw(geoPassShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 3. Light Pass: PBR光照模型
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pbrShader.SetIBLMaps(ibl);
        pbrShader.SetDirLights(dirLights);
        pbrShader.SetGbufferMaps(gBuffer);
        pbrShader.SetShadowInfo(shadowShader);
        pbrShader.DeferredRender(camera.position_);

        // 4. 将深度缓冲从Gbuffer中拷贝至默认framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer.gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 5. 绘制background
        ibl.SetupBackground(projection);
        ibl.RenderBackground(view);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::MoveDirection::kForward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::MoveDirection::kBackward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::MoveDirection::kLeft, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::MoveDirection::kRight, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
 
