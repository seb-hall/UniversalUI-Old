//  CoreCompositor.cpp   first written by Seb H in March 2023

#include <UniversalUI/Angelo/CoreCompositor.h>

bool CoreCompositor::SetupForCompositing() {
    return true;
}

aPixelBuffer* CoreCompositor::CompositeBuffers(uSize extents, std::vector<aPixelBuffer*> inputBuffers) {
    return new aPixelBuffer;
}