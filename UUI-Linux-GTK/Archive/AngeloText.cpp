#include <Angelo/AngeloText.h>

#include <ft2build.h>
#include <freetype/ftlcdfil.h>
#include FT_FREETYPE_H

FT_Library ft;

bool AngeloText::init() {

    if (FT_Init_FreeType(&ft)) {
        printf("ANGELO-TEXT: Freetype init failed!\n");
        return false;
    }

    printf("INFO: Freetype init OK\n");
    return true;
}

aFont* AngeloText::CreateNewFont(std::string name, std::string filePath) {

    FT_Face face;

    if (FT_New_Face(ft, filePath.c_str(), 0, &face)) {
        printf("ANGELO-TEXT: failed to find font file\n");
        return nullptr;
    }

    unsigned int index;
    unsigned long character = FT_Get_First_Char(face, &index);

    while (true) {
     printf("INFO: final character %lu\n", index);

    character = FT_Get_Next_Char(face, character, &index);
    if (!index) break; // if FT_Get_Next_Char write 0 to index then
                        // have no more characters in font face
    }

   

    return nullptr;

}