//  WinRenderer.cpp   first written by sebhall in March 2023

#include <WinRenderer.h>

#include <UniversalUI/Angelo/aPixelBuffer.h>
#include <UniversalUI/Angelo/aRenderCommand.h>
#include <UniversalUI/Core/CoreGeometry.h>
#include <AngeloCoreFragment.h>
#include <AngeloCoreVertex.h>

#include <glad/glad_wgl.h>
#include <stdio.h>
#include <chrono>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>




//  compile shaders etc
bool WinRenderer::Init() {


    // build and compile CommandShader
    // ------------------------------------
    // vertex shader
    unsigned int CommandVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(CommandVertexShader, 1, &AngeloCommandVS, NULL);
    glCompileShader(CommandVertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(CommandVertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(CommandVertexShader, 512, NULL, infoLog);
        printf("ALO-ERROR: CommandShader Vertex Compilation Failed: \n%s\n", infoLog);
        return false;
    }
    // fragment shader
    unsigned int CommandFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(CommandFragmentShader, 1, &AngeloCommandFS, NULL);
    glCompileShader(CommandFragmentShader);
    // check for shader compile errors
    glGetShaderiv(CommandFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(CommandFragmentShader, 512, NULL, infoLog);
        printf("ALO-ERROR: CommandShader Fragment Compilation Failed: \n%s\n", infoLog);
        return false;
    }
    // link shaders
    CommandShader = glCreateProgram();
    glAttachShader(CommandShader, CommandVertexShader);
    glAttachShader(CommandShader, CommandFragmentShader);
    glLinkProgram(CommandShader);
    // check for linking errors
    glGetProgramiv(CommandShader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(CommandShader, 512, NULL, infoLog);
        printf("ALO-ERROR: CommandShader Linking Failed: \n%s\n", infoLog);
        return false;
    }

    printf("ALO-INFO: CommandShader compiled successfully\n");
    glDeleteShader(CommandVertexShader);
    glDeleteShader(CommandFragmentShader);

    glUseProgram(0);

    // build and compile BufferShader
    // ------------------------------------
    // vertex shader
    unsigned int BufferVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(BufferVertexShader, 1, &AngeloBufferVS, NULL);
    glCompileShader(BufferVertexShader);
    // check for shader compile errors
    glGetShaderiv(BufferVertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(BufferVertexShader, 512, NULL, infoLog);
        printf("ALO-ERROR: BufferShader Vertex Compilation Failed: \n%s\n", infoLog);
        return false;
    }
    // fragment shader
    unsigned int BufferFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(BufferFragmentShader, 1, &AngeloBufferFS, NULL);
    glCompileShader(BufferFragmentShader);
    // check for shader compile errors
    glGetShaderiv(BufferFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(BufferFragmentShader, 512, NULL, infoLog);
        printf("ALO-ERROR: BufferShader Fragment Compilation Failed: \n%s\n", infoLog);
        return false;
    }
    // link shaders
    BufferShader = glCreateProgram();
    glAttachShader(BufferShader, BufferVertexShader);
    glAttachShader(BufferShader, BufferFragmentShader);
    glLinkProgram(BufferShader);
    // check for linking errors
    glGetProgramiv(BufferShader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(BufferShader, 512, NULL, infoLog);
        printf("ALO-ERROR: BufferShader Linking Failed: \n%s\n", infoLog);
        return false;
    }

    printf("ALO-INFO: BufferShader compiled successfully\n");
    glDeleteShader(BufferVertexShader);
    glDeleteShader(BufferFragmentShader);

    glUseProgram(0);

    // Create a vertex array object and a vertex buffer object for the quad
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Define the quad vertices and texture coordinates
    float quad[] = {
        -1.0f,  1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 1.0f
    };
    // Upload the quad data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    // Enable the vertex attribute pointers for position and texture coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glDisable(GL_CULL_FACE);
    
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    return true;
}

//  render specified operations
void WinRenderer::RenderOperations(std::vector<aRenderOperation> operations) { 
    printf("ANGELO-RENDERER: Render Operations\n");

    glViewport((int)renderFrame.x, (int)renderFrame.y, (int)renderFrame.width, (int)renderFrame.height);

    aRenderCommand command;
    command.size = {renderFrame.width, renderFrame.height};

    for (aRenderOperation op : operations) {
        
        command.codes.push_back(op.code);
        command.indices.push_back(command.parameters.size());
        command.parameters.insert(command.parameters.end(), op.data.begin(), op.data.end());

    }

    int opsSize = command.codes.size(); // get the size of the vector
    uint32_t* opsArray = &command.codes[0]; // get a pointer to the first element

    int idsSize = command.indices.size(); // get the size of the vector
    uint32_t* idsArray = &command.indices[0]; // get a pointer to the first element

    int pmsSize = command.parameters.size(); // get the size of the vector
    float* pmsArray = &command.parameters[0]; // get a pointer to the first element

    unsigned int opsTex;

    glGenTextures(1, &opsTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, opsTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, opsSize, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, opsSize, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, opsArray);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int idsTex;

    glGenTextures(1, &idsTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, idsTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, idsSize, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, idsSize, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, idsArray);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int pmsTex;

    glGenTextures(1, &pmsTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, pmsTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, pmsSize, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pmsSize, 1, GL_RED, GL_FLOAT, pmsArray);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(CommandShader);

    glUniform1i(glGetUniformLocation(CommandShader, "ops"), 1);
    glUniform1i(glGetUniformLocation(CommandShader, "ids"), 2);
    glUniform1i(glGetUniformLocation(CommandShader, "pms"), 3);

    glUniform1i(glGetUniformLocation(CommandShader, "numIndices"), opsSize);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, opsTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, idsTex);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, pmsTex);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDeleteTextures(1, &opsTex);
    glDeleteTextures(1, &idsTex);
    glDeleteTextures(1, &pmsTex);

    glUseProgram(0);

    printf("done rendering ops\n");

}

//  render specified text
void WinRenderer::RenderText(std::string text, float size, aTextLayout layout) { 
    //printf("ANGELO-RENDERER: Render Text\n");
    glViewport((int)renderFrame.x, (int)renderFrame.y, (int)renderFrame.width, (int)renderFrame.height);
    Gdiplus::RectF boundingRect(0.0f, 0.0f, renderFrame.width, renderFrame.height);

    //  set font and string stuff
    Gdiplus::FontFamily fontFamily(L"Consolas");
    Gdiplus::Font font(&fontFamily, size, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

    //  create a wide string for windows annoyingness
    std::wstring wideString(text.begin(), text.end());

    //  create a temporary graphics context using a 1px bitmap
    Gdiplus::Bitmap tempBitmap(1, 1, PixelFormat32bppARGB);
    Gdiplus::Graphics tempGraphics(&tempBitmap);

    //  measure string and assign to rect
    Gdiplus::RectF textSize(0.0f, 0.0f, 0.0f, 0.0f);
    tempGraphics.MeasureString(wideString.c_str(), -1, &font, boundingRect, &textSize);
    printf("text size width %f, height %f\n", textSize.Width, textSize.Height);

    //  calculate layout offsets
    float dy = textSize.Height - renderFrame.height;
    float dx = textSize.Width - renderFrame.width;
    float xOffset = -dx/2;
    float yOffset = -dy/2;
    
    //  create final bitmap with new dimensions
    Gdiplus::Rect bitmapRect(0, 0, (int)textSize.Width, (int)textSize.Height);
    Gdiplus::Bitmap bitmap(bitmapRect.Width, bitmapRect.Height, PixelFormat32bppARGB);
    Gdiplus::Graphics graphics(&bitmap);
    //graphics.TranslateTransform(0, (int)textSize.Height);
    //graphics.ScaleTransform(1, -1);
    
    //  create brush and draw text
    Gdiplus::SolidBrush solidBrush(Gdiplus::Color(255, 255, 255, 255));
    graphics.DrawString(wideString.c_str(), -1, &font, Gdiplus::PointF(0.0f, 0.0f), &solidBrush);

    //  get location of raw bitmap data
    Gdiplus::BitmapData bitmapData;
    bitmap.LockBits(&bitmapRect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);
    printf("data width %d, stride %d\n", bitmapData.Width, bitmapData.Stride);

    /*
    Gdiplus::Rect bitmapRect(0, 0, 60, 25);
    int bmpWidth = 60;
    int bmpHeight = 25;


    Gdiplus::Bitmap gdiBitmap(bmpWidth, bmpHeight, PixelFormat32bppARGB);
    Gdiplus::Graphics gr(&gdiBitmap);
    gr.TranslateTransform(0, 0);
    gr.ScaleTransform(1, 1);

    Gdiplus::FontFamily  fontFamily(L"Consolas");
    Gdiplus::Font        font(&fontFamily, 20.0, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::PointF      pointF(0.0f, 0.0f);
    //                      // a  b g r
    Gdiplus::Color drawColor(255, 255, 255, 255);
    Gdiplus::SolidBrush  solidBrush(drawColor);

    std::wstring wideString(text.begin(), text.end());
    gr.DrawString(wideString.c_str(), -1, &font, pointF, &solidBrush);

    Gdiplus::BitmapData bitmapData;
    gdiBitmap.LockBits(&bitmapRect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);
    printf("data width %d, stride %d\n", bitmapData.Width, bitmapData.Stride);*/
    
    aPixelBuffer* output = new aPixelBuffer;
    output->size = {textSize.Width, textSize.Height};
    output->frame = {renderFrame.x + xOffset, renderFrame.y + yOffset, textSize.Width, textSize.Height};
    printf("2\n");
    glGenTextures(1, &output->id);
    glBindTexture(GL_TEXTURE_2D, output->id);
    printf("3\n");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmapRect.Width, bitmapRect.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, bitmapData.Scan0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    printf("4\n");

    // Delete the GDI objects
    bitmap.UnlockBits(&bitmapData);

    RenderBuffer(output);
    

    /*glUseProgram(BufferShader);

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindTexture(GL_TEXTURE_2D, output->id);
    // Set the viewport to match the input frame
    glViewport(0, 0, (int)512, (int)512);
    // Draw the quad with the input texture
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(0);*/

    glDeleteTextures(1, &output->id);
    delete output;

}

//  render specified image
void WinRenderer::RenderImage(std::string path, uSize size) { 
    //printf("ANGELO-RENDERER: Render Image\n");

    glViewport((int)renderFrame.x, (int)renderFrame.y, (int)renderFrame.width, (int)renderFrame.height);

}

//  render specified buffer
void WinRenderer::RenderBuffer(aPixelBuffer* buffer) { 
    //printf("ANGELO-RENDERER: Render Buffer\n");

    //glClearColor(0.0, 0.0, 0.0, 0.0);

    //glClear(GL_COLOR_BUFFER_BIT);

    // Use the shader program
    glUseProgram(BufferShader);

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindTexture(GL_TEXTURE_2D, buffer->id);
    // Set the viewport to match the input frame
    glViewport((int)buffer->frame.x, (int)buffer->frame.y, (int)buffer->frame.width, (int)buffer->frame.height);
    // Draw the quad with the input texture
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(0);
}
