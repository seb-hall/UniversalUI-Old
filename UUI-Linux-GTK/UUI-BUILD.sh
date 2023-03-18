#!/bin/bash
# this is a script to build UniversalUI on Linux using bash and GCC

echo -e "\nUniversalUI Build Utility (v1 for Linux-GTK)\n";

declare -a XP_SRC=( 
    uApplication.cpp
    uDesktopApplication.cpp
    uSimpleApplication.cpp
    CoreHost.cpp
    uWindow.cpp
    CoreRenderer.cpp
    uView.cpp
);

XP_SRC_PATH="./../UniversalUI-Source/";
XP_SRC_ARGS="${XP_SRC[@]/#/$XP_SRC_PATH}";

declare -a DP_SRC=(
    UniversalUI.cpp
    LinuxGTKHost.cpp
    #second.cpp
);

DP_PATH="./Source/";
DP_ARGS="${DP_SRC[@]/#/$DP_PATH}";

declare -a APP_SRC=(
    main.cpp
    #second.cpp
);

APP_PATH="./App/";
APP_ARGS="${APP_SRC[@]/#/$APP_PATH}";

declare -a LINK=(
    #-lfreetype
    -lepoxy
);

LINK_ARGS="${LINK[@]%,}";

EXE_NAME="main";
LIB_NAME="UniversalUI.so";

BUILD_PATH="./Build/";
EXE_ARGS="${BUILD_PATH}${EXE_NAME}";
LIB_ARGS="${BUILD_PATH}${LIB_NAME}";

declare -a H_PATH=(
    ./../
    ./Include/
)
H_PREFIX="-I";
H_ARGS="${H_PATH[@]/#/$H_PREFIX}";

C_FLAGS="$(pkg-config --cflags gtk+-3.0 --libs)"

echo "${LIBS[@]}";

if [ "$1" == "-STATIC" ]; then
    echo "Building UniversalUI static test...";
    #g++ main.cpp CoreHost.cpp AngeloText.cpp Angelo.cpp -o main $(pkg-config --cflags gtk+-3.0 --libs) -lepoxy -lfreetype -I./include
    g++ -frtti $XP_SRC_ARGS $DP_ARGS $APP_ARGS -o $EXE_ARGS $C_FLAGS $LINK_ARGS $H_ARGS
    echo "DONE!";
elif [ "$1" == "-LIB" ]; then
    echo "Building UniversalUI library...";

    echo "DONE!";
elif [ "$1" == "-DEBUG" ]; then
    echo "Building DEBUG UniversalUI application...";

    echo "DONE!";
elif [ "$1" == "-RELEASE" ]; then
    echo "Building RELEASE UniversalUI application...";

    echo "DONE!";
else 
    echo -e "No arguments supplied. \n\n\t HELP:\n\n\t-STATIC \tbuild a single executable with UUI and App\n\t-LIB \t\tbuild UUI as a dynamic/shared library\n\t-DEBUG \t\tbuild a UUI client application\n\t-STANDARD \tbuild a UUI client application\n\n";
fi

#g++ main.cpp CoreHost.cpp  AngeloText.cpp Angelo.cpp -o main $(pkg-config --cflags gtk+-3.0 --libs) -lepoxy -lfreetype -I./include
