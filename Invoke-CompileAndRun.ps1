# Compiles and runs the executable.

$script:CUR_DIR = Get-Item -Path . | Select-Object -ExpandProperty Name
if ($script:CUR_DIR -ne "build") {
    Write-Host "Not a build directory, aborting."
    exit 1
}

cmake --build . --config Release -- /m
.\Zhade\Release\Zhade.exe
