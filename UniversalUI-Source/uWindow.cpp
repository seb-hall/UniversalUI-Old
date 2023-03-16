//  uWindow.cpp   first written by sebhall in Early 2022

#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Core/CoreColour.h>

uWindow::uWindow(uSize initSize, std::string initTitle) {
    size = initSize;
    title = initTitle;
    background = CORE_ALMOST_BLACK;
}
