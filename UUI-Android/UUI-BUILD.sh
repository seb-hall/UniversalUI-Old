#!/usr/bin/env bash

if [ "$(uname)" == "Darwin" ]; then
    PLATFORM = "mac";
    CC = "/Users/seb/Library/Android/sdk/ndk/23.1.7779620/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++";
    echo -e "\nUniversalUI Build Utility (v1 for Android, macOS Host)\n";
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    PLATFORM = "linux";
fi

declare -a XP_SRC=( 
    uApplication.cpp
    uDesktopApplication.cpp
    uSimpleApplication.cpp
    CoreHost.cpp
    uWindow.cpp
    CoreRenderer.cpp
    CoreAngelo.cpp
    CoreCompositor.cpp
    uView.cpp
    AngeloOperations.cpp
);

XP_SRC_PATH="./../UniversalUI-Source/";
XP_SRC_ARGS="${XP_SRC[@]/#/$XP_SRC_PATH}";

declare -a DP_SRC=(
    UniversalUI.cpp

);

DP_PATH="./Source/";
DP_ARGS="${DP_SRC[@]/#/$DP_PATH}";

declare -a APP_SRC=(
    main.cpp
);

APP_PATH="./App/";
APP_ARGS="${APP_SRC[@]/#/$APP_PATH}";


declare -a LINK=(
    -lepoxy
);

LINK_ARGS="${LINK[@]%,}";

EXE_NAME="main";
LIB_NAME="libUniversalUI.so";

BUILD_PATH="./Build/";
EXE_ARGS="${BUILD_PATH}${EXE_NAME}";
LIB_ARGS="${BUILD_PATH}${LIB_NAME}";

declare -a H_PATH=(
    ./../
    ./Include/
)
H_PREFIX="-I";
H_ARGS="${H_PATH[@]/#/$H_PREFIX}";

C_FLAGS=""

echo "${LIBS[@]}";

if [ "$1" == "-STATIC" ]; then
    echo "Building UniversalUI static test...";
    #g++ main.cpp CoreHost.cpp AngeloText.cpp Angelo.cpp -o main $(pkg-config --cflags gtk+-3.0 --libs) -lepoxy -lfreetype -I./include
    $CC -frtti $XP_SRC_ARGS $DP_ARGS $APP_ARGS -o $EXE_ARGS $C_FLAGS $LINK_ARGS $H_ARGS
    echo "DONE!";
elif [ "$1" == "-LIB" ]; then
    echo "Building UniversalUI library...";
    $CC $XP_SRC_ARGS $DP_ARGS -o $LIB_ARGS $C_FLAGS $LINK_ARGS $H_ARGS
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
