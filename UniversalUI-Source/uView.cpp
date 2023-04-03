//  uView.cpp   first written by sebhall in Early 2023

#include <UniversalUI/Core/uView.h>

#include <stdio.h>

uView::uView(uFrame initFrame) {
    frame = initFrame;
    needsRedraw = true;
    isPersistant = false;
}

void uView::Draw(CoreRenderer* renderer) { }