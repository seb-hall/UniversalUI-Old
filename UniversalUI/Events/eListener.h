//  eListener.h   first written by sebhall in April 2023
//
//  eListener is an abstract event listener


#include <UniversalUI/Core/CoreGeometry.h>

class uView;

class eListener {
public:
    uFrame frame;
    uView* target;
};