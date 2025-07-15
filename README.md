# 3D Interactive Graphics Scene with OpenGL

## Project Overview
This repository contains a 3D graphics application developed using OpenGL, GLFW, and GLM for the CSE 4201 Computer Graphics and Animations course (Roll No: 1903073). The project implements an interactive 3D scene featuring a cube, sphere, pyramid, and a ground plane, enhanced with shadow mapping for realistic lighting effects. The application supports real-time user interaction for object transformations (translation, rotation, scaling) and camera control, along with automatic animations. A skybox adds depth to the environment, and textures are applied to the cube.

## Features
- **3D Objects**: Renders a cube, sphere, pyramid, and ground plane using modern OpenGL.
- **Transformations**: Supports translation, rotation, and scaling for each object, controlled via keyboard inputs.
- **Lighting and Shading**: Implements a point light source with ambient, diffuse, and specular components, enhanced by shadow mapping for realistic shadows.
- **Textures**: Applies a texture to the cube; the sphere and pyramid use colored surfaces.
- **Camera Control**: Allows dynamic scene exploration with mouse and keyboard camera movement.
- **Animations**: Automatically rotates the cube for a dynamic visual effect.
- **Shadow Mapping**: Real-time shadow generation using a depth map framebuffer.
- **Environment**: Includes a skybox and a ground plane for scene context.
- **User Interaction**: Keyboard controls for object manipulation, camera movement, and object selection.

## Repository Contents
- **Source Code**:
  - `main.cpp`: Main OpenGL application implementing the 3D scene, transformations, lighting, shadows, textures, and animations.
- **Additional Files**:
  - `Shader.h`, `Texture.h`, `Model.h`, `Transform.h`, `Camera.h`, `Utils.h`: Custom classes for managing shaders, textures, models, transformations, camera, and utilities.
  - `stb_image.h`: Library for loading textures.
  - `container.jpg`: Texture file for the cube.
- **Documentation**:
  - `Report.pdf`: Assignment report detailing the implementation, challenges, and solutions.
- **Dependencies**:
  - OpenGL, GLAD, GLFW, GLM, KHR libraries for rendering and mathematics.
- **Output**:
  - Executable for rendering the interactive 3D scene.

## Requirements
- **Libraries**: OpenGL, GLAD, GLFW, GLM, KHR
- **Compiler**: C++ compiler (e.g., g++)
- **Operating System**: Compatible with Windows, Linux, or macOS
- **Additional Files**: Ensure `container.jpg` is in the project directory.

## How to Run
1. **Install Dependencies**:
   - Install OpenGL, GLAD, GLFW, and GLM libraries (e.g., using a package manager or manual setup).
   - Ensure a C++ compiler is installed (e.g., g++).
2. **Clone the Repository**:
   ```bash
   git clone https://github.com/rzoshin/3D-Interactive-Graphics-OpenGL.git
   cd 3D-Interactive-Graphics-OpenGL
