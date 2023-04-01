//  CoreEvents.h   first written by sebhall in April 2023
//
//  CoreEvents is a module controlling the UniversalUI 
//  events system. CoreEvents is designed to have an
//  instance for each uWindow created, and contains all
//  the submodules neeeded to control events such as 
//  mouse and keyboard events and drag and drop events.

#include <UniversalUI/Core/CoreGeometry.h>
#include <UniversalUI/Events/eEventListener.h>
#include <vector>

class CoreEvents {
public:

    std::vector<eEventListener> listeners;

    void 

};