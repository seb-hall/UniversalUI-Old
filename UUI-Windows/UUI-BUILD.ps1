# this is a script to build Universal UI on PowerShell with GCC

# Write-Output "TODO: CHANGE SUBSYSTEM TO WINDOWS NOT CONSOLE FOR RELEASE BUT LEAVE DEBUG??"

$MODE=$args[0]

if ($MODE -eq "-STATIC") {
    Write-Output "STATIC Building UniversalUI program..."
   g++ .\App\main.cpp .\Source\glad.c -o .\Build\main.exe -Wall -g -mwindows --static -static-libgcc -static-libstdc++ -lglfw3 -lgdi32 -lopengl32  -I.\Include -I.\..\ -L.\Lib
    Write-Output "DONE!"
} elseif ($MODE -eq "-DLL") {
    Write-Output "DLL Building UniversalUI library..."
    g++ -DUUI_BUILD .\Source\UniversalUI.cpp .\Source\glad.c .\..\UniversalUI-Source\uSimpleApplication.cpp .\..\UniversalUI-Source\uDesktopApplication.cpp -shared -mconsole -o .\Build\UniversalUI.dll -Wall -lgcc -lstdc++ -lglfw3 -lgdi32 -lopengl32  -I.\Include -I.\..\ -L.\Lib --static -static-libgcc -static-libstdc++
    Write-Output "DONE!"
} elseif ($MODE -eq "-DEBUG") {
    Write-Output "DEBUG Building UniversalUI application..."
    g++ .\App\main.cpp -o .\Build\main.exe -Wall -mconsole -I.\..\ -L.\Build -lUniversalUI
    Write-Output "DONE!"
} elseif ($MODE -eq "-RELEASE") {
    Write-Output "DEBUG Building UniversalUI application..."
    g++ .\App\main.cpp -o .\Build\main.exe -Wall -mwindows -I.\..\ -L.\Build -lUniversalUI
    Write-Output "DONE!"
} else {
    Write-Output "No arguments supplied.`nHelp:`n`t-STATIC for building a program all in one`n`t-DLL for building the UniversalUI DLL`n`t-DEBUG for building a debug UniversalUI application`n`t-STANDARD for building a standard UniversalUI application.`n"
}