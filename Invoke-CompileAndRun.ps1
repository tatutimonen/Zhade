# Compiles and runs the executable.

$curDir = Get-Item -Path . | Select-Object -ExpandProperty Name
if ($curDir -ne "build") {
    Write-Host "Not a build directory, aborting."
    exit
}

cmake --build . --config Release -- /m
.\Zhade\Release\Zhade.exe
