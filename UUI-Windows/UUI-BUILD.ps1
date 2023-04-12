# this is a script to build UniversalUI on PowerShell with GCC

# Write-Output "TODO: CHANGE SUBSYSTEM TO WINDOWS NOT CONSOLE FOR RELEASE BUT LEAVE DEBUG??"

# Declare an array of strings
[String[]] $XP_SRC = 
    "uApplication.cpp", 
    "uDesktopApplication.cpp",
    "uSimpleApplication.cpp",
    "CoreHost.cpp", 
    "uWindow.cpp", 
    "CoreRenderer.cpp", 
    "CoreAngelo.cpp", 
    "CoreCompositor.cpp", 
    "uView.cpp", 
    "AngeloOperations.cpp" 


# Prefix each element with a path
$XP_SRC_PATH = ".\..\UniversalUI-Source\"
$XP_SRC_ARGS = $XP_SRC | ForEach-Object { "$XP_SRC_PATH$_" }

# Declare an array of strings
[String[]] $DP_SRC = 
    "UniversalUI.cpp",
    "glad.c",
    "glad_wgl.c",
    "WinAngelo.cpp",
    "WinRenderer.cpp",
    "WinHost.cpp"


# Prefix each element with a path
$DP_SRC_PATH = ".\Source\"
$DP_SRC_ARGS = $DP_SRC | ForEach-Object { "$DP_SRC_PATH$_" }


$MODE=$args[0]

if ($MODE -eq "-STATIC") {
    Write-Output "building static UniversalUI program..."
    g++ -DUUI_BUILD .\App\main.cpp @XP_SRC_ARGS @DP_SRC_ARGS -o .\Build\main.exe -Wall -g -mwindows -lglu32 -lgdi32 -lopengl32  -I.\Include -I.\..\ --static -static-libgcc -static-libstdc++
    Write-Output "DONE!"
} elseif ($MODE -eq "-DYLIB") {
    Write-Output "building UniversalUI dynamic library..."
    g++ -DUUI_BUILD .\Source\UniversalUI.cpp .\Source\aRenderer.cpp .\Source\glad.c .\Source\uWindowManager.cpp .\..\UniversalUI-Source\uSimpleApplication.cpp .\..\UniversalUI-Source\uWindow.cpp .\..\UniversalUI-Source\uDesktopApplication.cpp .\..\UniversalUI-Source\uApplication.cpp -shared -mconsole -o .\Build\UniversalUI.dll -Wall -lgcc -lstdc++ -lglfw3 -lgdi32 -lopengl32  -I.\Include -I.\..\ -L.\Lib --static -static-libgcc -static-libstdc++  
    Write-Output "DONE!"
} elseif ($MODE -eq "-LIB") {
    Write-Output "building UniversalUI static library..."
    g++ -DUUI_BUILD .\Source\UniversalUI.cpp .\Source\aRenderer.cpp .\Source\glad.c .\Source\uWindowManager.cpp .\..\UniversalUI-Source\uSimpleApplication.cpp .\..\UniversalUI-Source\uWindow.cpp .\..\UniversalUI-Source\uDesktopApplication.cpp .\..\UniversalUI-Source\uApplication.cpp -shared -mconsole -o .\Build\UniversalUI.dll -Wall -lgcc -lstdc++ -lglfw3 -lgdi32 -lopengl32  -I.\Include -I.\..\ -L.\Lib --static -static-libgcc -static-libstdc++  
    Write-Output "DONE!"
} elseif ($MODE -eq "-DEBUG") {
    Write-Output "DEBUG Building UniversalUI application..."
    g++ .\App\main.cpp -o .\Build\main.exe -Wall -mconsole -I.\..\ -L.\Build -lUniversalUI.dll --static -static-libgcc -static-libstdc++
    Write-Output "DONE!"
} elseif ($MODE -eq "-RELEASE") {
    Write-Output "RELEASE Building UniversalUI application..."
    g++ .\App\main.cpp -o .\Build\main.exe -Wall -mwindows -I.\..\ -L.\Build --static -static-libgcc -static-libstdc++ -lUniversalUI 
    Write-Output "DONE!"
} else {
    Write-Output "No arguments supplied.`nHelp:`n`t-STATIC for building a program all in one`n`t-DYLIB for building the UniversalUI DLL`n`t-DEBUG for building a debug UniversalUI application`n`t-STANDARD for building a standard UniversalUI application.`n"
}