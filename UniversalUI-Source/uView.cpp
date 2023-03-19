//  uView.cpp   first written by sebhall in Early 2023

#include <UniversalUI/Core/uView.h>

#include <stdio.h>

uView::uView(uFrame initFrame, bool initPersistent) {
    frame = initFrame;
    needsRedraw = true;
    persistent = initPersistent;
}

std::vector<aRenderOperation> uView::Draw() {
    printf("draw view!");
    return {};
}