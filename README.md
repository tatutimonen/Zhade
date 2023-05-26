# Zhade

(images here)

----

## Running Locally

   1. Clone the repository, along with its submodules (```--recurse-submodules```)

   2. Do ```./vcpkg/vcpkg/bootstrap-vcpkg.sh -disableMetrics``` or ```.\vcpkg\bootstrap-vcpkg.bat -disableMetrics``` depending on your platform

   3. Do ```mkdir build; cd build; cmake ..``` to generate platform-specific project files

   4. Build with, e.g., ```cmake --build .```. Note that some compilers such as MSVC support different build configurations. You may provide these as further options. For example, amend the previous with ```--config Release``` to build with Release configuration on MSVC.

   5. Run the executable
