#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Transform.h"
#include "Camera.h"

extern int selectedObject;
extern Transform cubeTransform;
extern Transform pyramidTransform;
extern Transform sphereTransform;
extern Camera camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

#endif