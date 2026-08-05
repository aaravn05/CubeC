# ASCII Software Renderer

A simple software-rendered 3D wireframe cube written in C++ that renders directly to the terminal using a framebuffer.

![Renderer Demo](demo.gif)


## Features

- Software framebuffer renderer
- Perspective projection
- Barycentric triangle rasterization
- Filled 3D cube
- Lambert-style ASCII face shading
- Dynamic terminal rendering
- Continuous cube rotation

## Concepts Learned

This project was built to understand the fundamentals of computer graphics without relying on external graphics libraries.

Implemented concepts include:

- Framebuffers
- Render loops
- 2D and 3D vector math
- Perspective projection
- DDA line rasterization
- Barycentric triangle rasterization
- Surface normals
- Dot products
- Basic Lambert lighting
- Software rendering pipeline

## Graphics Pipeline

Cube Vertices
      ↓
Rotation
      ↓
Perspective Projection
      ↓
Triangle Rasterization
      ↓
Face Lighting
      ↓
Framebuffer
      ↓
Terminal

## Technologies

- C++11
- Standard Library
- ANSI terminal escape sequences

## Future Improvements

- Rotation around X and Z axes
- Adjustable camera
- Filled polygons
- Z-buffer
- Lighting and shading
- Color support
- Back-face culling
- SDL2/OpenGL renderer

## Lessons Learned

This project was built from scratch from a basic idea of rotational mathematics, the main goal was to solve a math heavy project


I also built it as a learning exercise to understand how modern graphics pipelines work at a low level before using APIs such as OpenGL or DirectX.


Building each component separately made the graphics pipeline much easier to understand.


## Build

```bash
g++ main.cpp -std=c++17 -O2 -o cube
./cube
```
