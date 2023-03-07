# this is a script to build Universal UI on PowerShell with GCC

$MODE=$args[0]

if ($MODE -eq "-STATIC") {
    Write-Output "STATIC Building UniversalUI program..."
    g++ main.cpp glad.c -o main.exe -mwindows -lopengl32 -lgdi32 -lglfw3 -I.\include -L.\lib
    Write-Output "DONE!"
} else {
    Write-Output "No arguments supplied.`nHelp:`n`t-STATIC for building a program all in one`n`t-DLL for building the UniversalUI DLL`n`t-DEBUG for building a debug UniversalUI application`n`t-STANDARD for building a standard UniversalUI application.`n"
}



#g++ ${path}main.cpp ${path}glad.c -o ${path}build/main.exe -mwindows -lopengl32 -lgdi32 -lglfw3dll -lfreetype `pkg-config --cflags freetype2` -I./source/include

#g++ -DUUI_BUILD .\UniversalUI.cpp .\uApplication.cpp -shared -mconsole -o UniversalUI.dll -Wall -I $(Get-Location) -L $(Get-Location) -lglfw3 -lgdi32 -lopengl32