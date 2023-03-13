#ifndef ANGELOTEXT_H
#define ANGELOTEXT_H

#include <Angelo/font.h>

#include <string>
#include <vector>

class AngeloText {

public:

    bool init();

    aFont* CreateNewFont(std::string name, std::string filePath);
    
    void RenderText(std::string text, aFont* font, float size, float x, float y);

};

#endif