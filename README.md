# Zhade

A WIP rasterization-based renderer utilizing modern OpenGL. Currently under development:

- Rendering queue based on encoding OpenGL state change requirements and objects' average depth in a bit field, and sorting the rendering tasks with respect to these keys before performing them. See, e.g., [this blog post](https://realtimecollisiondetection.net/blog/?p=86) and [this talk](https://www.youtube.com/watch?v=-bCeNzgiJ8I&t=1920s) for more details. The depth value is included to aid early z-culling.
    
- Separate attribute format specification to decouple the buffers from the vertex array. This facilitates minimizing the overhead induced by changing vertex format state and is generally more intuitive.

- Shadow maps with sampling enhancements such as fitting, cascaded; and filtering, such as PCSS.

----

## Running Locally

   1. Ensure you have CMake version ≥ 3.17

   2. Clone the repository, along with its submodules

   3. Do ```mkdir build; cd build; cmake ..``` to generate platform-specific project files. Note that on Windows you need to specify the target architecture for CMake as ```Win32```.

   4. Build with, e.g., ```cmake --build .```. Note that some compilers such as MSVC support different build configurations. You may provide these as further options. For example, amend the previous with ```--config Release``` to build with Release configuration on MSVC.

   5. Run the executable
