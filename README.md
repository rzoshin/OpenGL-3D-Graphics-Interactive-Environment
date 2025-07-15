# 3D Interactive Graphics Scene with OpenGL

## Project Overview
This repository contains a 3D graphics application developed using OpenGL and GLUT for the CSE 4201 Computer Graphics and Animations course (Roll No: 1903113). The project implements an interactive 3D scene featuring a cube, sphere, and pyramid, each with distinct colors and textures, placed on a textured ground plane under a simple sky. The application supports real-time user interaction for object transformations (translation, rotation, scaling) and camera control, along with automatic animations. Lighting and shading enhance the visual realism, and procedural textures are applied to the cube and ground.

## Features
- **3D Objects**: Renders a cube, sphere, and pyramid with immediate mode OpenGL.
- **Transformations**: Supports translation, rotation, and scaling for each object, controlled via keyboard inputs.
- **Lighting and Shading**: Implements a point light source with ambient, diffuse, and specular components, using smooth shading.
- **Textures**: Applies procedurally generated chessboard textures to the cube and ground.
- **Camera Control**: Allows movement of the camera's eye and focus points for dynamic scene exploration.
- **Animations**: Automatically rotates objects (cube, sphere, pyramid) for a dynamic visual effect.
- **Environment**: Includes a textured ground plane and a simple sky background.
- **User Interaction**: Keyboard controls for object manipulation, camera movement, and program exit.

## Repository Contents
- **Source Code**:
  - `main.cpp`: Main OpenGL application implementing the 3D scene, transformations, lighting, textures, and animations.
- **Documentation**:
  - `Report.pdf`: Assignment report detailing the implementation, challenges, and solutions.
- **Dependencies**:
  - OpenGL, GLUT, GLU libraries for rendering and window management.
- **Output**:
  - Executable for rendering the interactive 3D scene (~60 FPS).

## Requirements
- **Libraries**: OpenGL, GLUT, GLU
- **Compiler**: C++ compiler (e.g., g++)
- **Operating System**: Compatible with Windows, Linux, or macOS (GLUT must be installed)

## How to Run
1. **Install Dependencies**:
   - Install OpenGL and GLUT libraries (e.g., `freeglut` on Linux, or equivalent for Windows/macOS).
   - Ensure a C++ compiler is installed (e.g., g++).
2. **Clone the Repository**:
   ```bash
   git clone https://github.com/SM-RIFAT/3D-Interactive-Graphics-OpenGL.git
   cd 3D-Interactive-Graphics-OpenGL
