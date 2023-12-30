$cpuCount = (Get-WmiObject -Class Win32_Processor).NumberOfCores
cmake --build . -j $cpuCount --config Release
.\Zhade\Release\Zhade.exe
