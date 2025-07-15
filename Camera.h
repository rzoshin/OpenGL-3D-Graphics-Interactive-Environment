#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
    float lastX, lastY;
    bool firstMouse;

    Camera(float posX, float posY, float posZ, float yaw_, float pitch_);
    glm::mat4 getViewMatrix() const;
    glm::mat4 getSkyboxViewMatrix() const;
    void processMouseMovement(double xpos, double ypos);
    void processKeyboard(GLFWwindow* window, float deltaTime);
};

#endif