@echo off
echo Building PhysicalTherapyRobot...

REM Try to use qmake if available
qmake PhysicalTherapyRobot.pro -spec win32-msvc2017 -r
if %errorlevel% neq 0 (
    echo qmake failed, trying to build directly...
    
    REM Try to build with MSBuild if available
    msbuild PhysicalTherapyRobot.sln /p:Configuration=Release /p:Platform=x64 /m
    if %errorlevel% neq 0 (
        echo MSBuild failed. Please check Visual Studio installation.
        echo You may need to:
        echo 1. Open the project in Visual Studio 2017
        echo 2. Restore NuGet packages
        echo 3. Build from within Visual Studio
        echo 4. Check for missing dependencies (Qt, OpenCV, etc.)
    )
) else (
    echo qmake succeeded, trying nmake...
    nmake
    if %errorlevel% neq 0 (
        echo nmake failed. Check compilation errors above.
    )
)

echo Build process completed.