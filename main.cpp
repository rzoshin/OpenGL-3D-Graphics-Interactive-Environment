#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Transform.h"
#include "Camera.h"
#include "Utils.h"

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

// Shader sources
const std::string depthVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
)";

const std::string depthFragmentShaderSource = R"(
#version 330 core
void main() {}
)";

const std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColorOrTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec3 ColorOrTexCoord;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    ColorOrTexCoord = aColorOrTexCoord;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const std::string fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 ColorOrTexCoord;
in vec4 FragPosLightSpace;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D texture1;
uniform sampler2D shadowMap;
uniform bool isSkybox;
uniform bool useTexture;

float ShadowCalculation(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normalize(Normal), normalize(lightPos - FragPos))), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    if (projCoords.z > 1.0) shadow = 0.0;
    return shadow;
}

void main() {
    if (isSkybox) {
        FragColor = vec4(0.53, 0.81, 0.92, 1.0);
    } else {
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

        float ambientStrength = 0.3;
        vec3 ambient = ambientStrength * lightColor;

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * attenuation;

        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor * attenuation;

        float shadow = ShadowCalculation(FragPosLightSpace);
        vec3 objectColor = useTexture ? texture(texture1, ColorOrTexCoord.xy).rgb : ColorOrTexCoord;
        vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
        FragColor = vec4(lighting * objectColor, 1.0);
    }
}
)";

// Model data
std::vector<float> cubeVertices = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f
};

std::vector<unsigned int> cubeIndices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    0, 1, 5, 5, 4, 0,
    2, 3, 7, 7, 6, 2,
    0, 3, 7, 7, 4, 0,
    1, 2, 6, 6, 5, 1
};

std::vector<float> pyramidVertices = {
     0.0f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f
};

std::vector<unsigned int> pyramidIndices = {
    0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1, 1, 2, 3, 1, 3, 4
};

std::vector<float> sphereVertices;
std::vector<unsigned int> sphereIndices;
void generateSphere() {
    float radius = 0.5f;
    int sectorCount = 36;
    int stackCount = 18;

    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = M_PI / 2 - i * M_PI / stackCount;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * M_PI / sectorCount;
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            sphereVertices.push_back(x);
            sphereVertices.push_back(y);
            sphereVertices.push_back(z);

            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
            sphereVertices.push_back(normal.x);
            sphereVertices.push_back(normal.y);
            sphereVertices.push_back(normal.z);

            sphereVertices.push_back(0.5f); // R
            sphereVertices.push_back(0.0f); // G
            sphereVertices.push_back(1.0f); // B

            if (i > 0 && j > 0) {
                int a = (i-1) * (sectorCount+1) + (j-1);
                int b = (i-1) * (sectorCount+1) + j;
                int c = i * (sectorCount+1) + (j-1);
                int d = i * (sectorCount+1) + j;
                sphereIndices.insert(sphereIndices.end(), {
                    static_cast<unsigned int>(a),
                    static_cast<unsigned int>(b),
                    static_cast<unsigned int>(c),
                    static_cast<unsigned int>(b),
                    static_cast<unsigned int>(d),
                    static_cast<unsigned int>(c)
                });
            }
        }
    }
}

std::vector<float> skyboxVertices = {
    -20.0f, -20.0f, -20.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
     20.0f, -20.0f, -20.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     20.0f,  20.0f, -20.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -20.0f,  20.0f, -20.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    -20.0f, -20.0f,  20.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     20.0f, -20.0f,  20.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
     20.0f,  20.0f,  20.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -20.0f,  20.0f,  20.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f
};

std::vector<unsigned int> skyboxIndices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    0, 1, 5, 5, 4, 0,
    2, 3, 7, 7, 6, 2,
    0, 3, 7, 7, 4, 0,
    1, 2, 6, 6, 5, 1
};

std::vector<float> groundVertices = {
    -5.0f, -1.0f, -5.0f,  0.0f, 1.0f, 0.0f, 0.7f, 0.7f, 0.7f,
     5.0f, -1.0f, -5.0f,  0.0f, 1.0f, 0.0f, 0.7f, 0.7f, 0.7f,
     5.0f, -1.0f,  5.0f,  0.0f, 1.0f, 0.0f, 0.7f, 0.7f, 0.7f,
    -5.0f, -1.0f,  5.0f,  0.0f, 1.0f, 0.0f, 0.7f, 0.7f, 0.7f
};

std::vector<unsigned int> groundIndices = {
    0, 1, 2, 2, 3, 0
};

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Objects with Shadows", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set initial viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // Initialize Shaders
    Shader depthShader(depthVertexShaderSource, depthFragmentShaderSource);
    Shader mainShader(vertexShaderSource, fragmentShaderSource);

    // Initialize Textures
    Texture cubeTexture("container.jpg");

    // Generate sphere data
    generateSphere();

    // Initialize Models
    Model cube(cubeVertices, cubeIndices, 8, {3, 3, 2}); // Position, Normal, TexCoord
    Model pyramid(pyramidVertices, pyramidIndices, 9, {3, 3, 3}); // Position, Normal, Color
    Model sphere(sphereVertices, sphereIndices, 9, {3, 3, 3}); // Position, Normal, Color
    Model skybox(skyboxVertices, skyboxIndices, 8, {3, 3, 2}); // Position, Normal, TexCoord (not used)
    Model ground(groundVertices, groundIndices, 9, {3, 3, 3}); // Position, Normal, Color

    // Initialize Transforms
    Transform cubeTransform;
    cubeTransform.position = glm::vec3(-1.0f, 0.0f, 0.0f);
    cubeTransform.scale = glm::vec3(0.5f);

    Transform pyramidTransform;
    pyramidTransform.position = glm::vec3(1.0f, 0.0f, 0.0f);
    pyramidTransform.scale = glm::vec3(0.5f);

    Transform sphereTransform;
    sphereTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    sphereTransform.scale = glm::vec3(0.5f);

    Transform skyboxTransform; // No transformation needed
    Transform groundTransform; // No transformation needed

    // Initialize Camera
    Camera camera(0.0f, 1.0f, 3.0f, -90.0f, 0.0f);

    // Initialize Depth Map FBO
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Lighting
    glm::vec3 lightPos(2.0f, 4.0f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    // Animation Variables
    float cubeRotationAngle = 0.0f;
    int selectedObject = 0;

    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Timing
        static float lastFrame = 0.0f;
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input Processing
        camera.processKeyboard(window, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) selectedObject = 0;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) selectedObject = 1;
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) selectedObject = 2;

        Transform* transform = nullptr;
        switch (selectedObject) {
            case 0: transform = &cubeTransform; break;
            case 1: transform = &pyramidTransform; break;
            case 2: transform = &sphereTransform; break;
        }
        if (transform) {
            float translationSpeed = 2.5f * deltaTime;
            if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
                transform->position.y += translationSpeed;
            if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
                transform->position.y -= translationSpeed;
            if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
                transform->position.x -= translationSpeed;
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
                transform->position.x += translationSpeed;
            if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
                transform->position.z -= translationSpeed;
            if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
                transform->position.z += translationSpeed;

            float rotationSpeed = 1.5f * deltaTime;
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                transform->rotation.x += rotationSpeed;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                transform->rotation.x -= rotationSpeed;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                transform->rotation.y += rotationSpeed;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                transform->rotation.y -= rotationSpeed;
            if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
                transform->rotation.z += rotationSpeed;
            if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
                transform->rotation.z -= rotationSpeed;

            float scaleSpeed = 1.0f * deltaTime;
            if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
                transform->scale -= scaleSpeed;
            if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
                transform->scale += scaleSpeed;
        }

        // Animation
        cubeRotationAngle += deltaTime;
        cubeTransform.rotation.y = cubeRotationAngle;

        // 1. Render Depth Map
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        depthShader.use();
        depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        depthShader.setMat4("model", cubeTransform.getModelMatrix());
        cube.draw();

        depthShader.setMat4("model", pyramidTransform.getModelMatrix());
        pyramid.draw();

        depthShader.setMat4("model", sphereTransform.getModelMatrix());
        sphere.draw();

        depthShader.setMat4("model", groundTransform.getModelMatrix());
        ground.draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. Render Scene
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainShader.use();
        mainShader.setVec3("lightPos", lightPos);
        mainShader.setVec3("lightColor", lightColor);
        mainShader.setVec3("viewPos", camera.position);
        mainShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        mainShader.setInt("shadowMap", 1);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        mainShader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        cubeTexture.bind();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        // Draw Skybox
        glDepthFunc(GL_LEQUAL);
        mainShader.setBool("isSkybox", true);
        mainShader.setBool("useTexture", false);
        mainShader.setMat4("view", camera.getSkyboxViewMatrix());
        mainShader.setMat4("model", skyboxTransform.getModelMatrix());
        skybox.draw();
        mainShader.setBool("isSkybox", false);
        glDepthFunc(GL_LESS);
        mainShader.setMat4("view", camera.getViewMatrix());

        // Draw Ground
        mainShader.setBool("useTexture", false);
        mainShader.setMat4("model", groundTransform.getModelMatrix());
        ground.draw();

        // Draw Cube (textured)
        mainShader.setBool("useTexture", true);
        mainShader.setMat4("model", cubeTransform.getModelMatrix());
        cube.draw();

        // Draw Pyramid (no texture)
        mainShader.setBool("useTexture", false);
        mainShader.setMat4("model", pyramidTransform.getModelMatrix());
        pyramid.draw();

        // Draw Sphere (no texture)
        mainShader.setBool("useTexture", false);
        mainShader.setMat4("model", sphereTransform.getModelMatrix());
        sphere.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthMap);

    glfwTerminate();
    return 0;
}