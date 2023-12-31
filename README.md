# Zhade

(images here)

----

## Running Locally

   1. Clone the repository, along with its submodules (`--recurse-submodules`)

   2. Do `.\vcpkg\bootstrap-vcpkg.bat -disableMetrics` or `./vcpkg/bootstrap-vcpkg.sh -disableMetrics` depending on your platform

   3. Do `mkdir build; cd build; cmake ..` to generate platform-specific project files

   4. Do `..\Invoke-CompileAndRun.ps1` or `../compile-and-run.sh` depending on your platform
