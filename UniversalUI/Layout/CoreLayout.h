//  CoreLayout.h    first written by sebhall in April 2023
//
//  CoreLayout is a platform-independant module responsible
//  for automating layouts and designing responsive interfaces

#include <UniversalUI/Layout/lContainer.h>
#include <UniversalUI/Core/CoreGeometry.h>


class CoreLayout {
    public:

    void ContainerResize(lContainer* container, uSize toSize, uDirection direction);
    
};