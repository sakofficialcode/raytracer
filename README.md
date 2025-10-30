# Ray Tracer


<p float="left">
    <img src="assets\\full_render.jpg" alt="Full Render" width="450"/>
    <img src="assets\\partial_render.jpg" alt="Partial Render" width="450"/>
</p>

## Description

Graphics ray tracer written in C++ with realistic renders of textures and a camera supporting 3-axis movement. Keyboard events and display handled via SDL3.

## Getting Started

### Dependencies

* CMake
* SDL3 included in subdirectory

### Build Steps
```
cmake -B build
cmake --build build
```

Copy the appropriate `.dll` for your architecture from `external\SDL3\lib` to the `build\Debug` folder

### Executing program

Call the executable from the terminal:

```
./build/Debug/raytracer.exe
```

### Controls
#### Position Movements
`q` / `e` - Up / Down \
`w` / `s` - Forward / Backward \
`a` / `d` - Right / Left

#### Camera View Movements
`↑` / `↓` - Look up / down \
`←` / `→` - Look right / left



## Acknowledgments

* [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)