#include "Utils.h"

int selectedObject = 0;
Transform cubeTransform;
Transform pyramidTransform;
Transform sphereTransform;
Camera camera(0.0f, 1.0f, 3.0f, -90.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Transform* transform = nullptr;
    switch (selectedObject) {
        case 0: transform = &cubeTransform; break;
        case 1: transform = &pyramidTransform; break;
        case 2: transform = &sphereTransform; break;
    }
    if (transform) {
        transform->scale += (float)yoffset * 0.1f;
        transform->scale = glm::max(transform->scale, glm::vec3(0.1f));
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.processMouseMovement(xpos, ypos);
}