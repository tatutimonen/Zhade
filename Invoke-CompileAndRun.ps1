# Compiles and runs the executable.

$curDir = Get-Item -Path . | Select-Object -ExpandProperty Name
if ($curDir -ne "build") {
    Write-Host "Not a build directory, aborting."
    exit
}

$cpuCount = (Get-WmiObject -Class Win32_Processor).NumberOfCores
cmake --build . -j $cpuCount --config Release
.\Zhade\Release\Zhade.exe
