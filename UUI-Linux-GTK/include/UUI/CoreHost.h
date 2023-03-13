#ifndef COREHOST_H
#define COREHOST_H

#include <UUI/uWindow.h>
#include <Angelo/Angelo.h>
#include <Angelo/AngeloText.h>


class CoreHost {

public:

    uWindow* CreateNewWindow(float x, float y, float width, float height, std::string title);

    void ApplicationLaunched();

    int main(int argc, char** argv);

};

#endif

