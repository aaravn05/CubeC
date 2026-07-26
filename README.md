# ASCII Rotating Cube

A simple software-rendered 3D wireframe cube written in C++ that renders directly to the terminal using a framebuffer.

## Features

- Terminal framebuffer renderer
- Perspective projection
- DDA line drawing algorithm
- 3D wireframe cube
- Y-axis rotation
- Simple render loop

## Concepts Learned

This project was built to understand the fundamentals of computer graphics without relying on external graphics libraries.

Implemented concepts include:

- Framebuffers
- Render loops
- 2D and 3D vectors
- Perspective projection
- DDA line rasterization
- Wireframe rendering
- Rotation around the Y-axis
- Basic graphics pipeline

## Graphics Pipeline

```
Cube Vertices
      ↓
Rotation
      ↓
Perspective Projection
      ↓
2D Screen Coordinates
      ↓
Line Rasterization (DDA)
      ↓
Framebuffer
      ↓
Terminal
```

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

- Plotting pixels
- Drawing lines
- Projecting 3D points
- Connecting vertices
- Animating the scene

Building each component separately made the graphics pipeline much easier to understand.

##Demo 
//add gif 



## Build

```bash
g++ main.cpp -std=c++17 -O2 -o cube
./cube
```
