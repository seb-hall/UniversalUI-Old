//  CoreRenderer.cpp   first written by Seb H in March 2023

#include <UniversalUI/Angelo/CoreRenderer.h>
#include <UniversalUI/Angelo/aRenderOperation.h>
#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Core/uView.h>

#include <vector>
#include <iterator>
#include <map>

const std::map<unsigned short, unsigned short> paramNumberMap = {
    {0, 4},     // clear colour
    {2, 4},     // drawing colour
    {3, 1},     // drawing weight
    {10, 4},	// line
    {11, 3},	// circle
    {12, 5},	// arc
    {13, 4},	// aligned rectangle
    {14, 5}		// aligned, rounded rectangle
};

// register view
void CoreRenderer::SetupViewForRendering(uView* view) {

    for (uView* subview : view->subviews) {
        SetupViewForRendering(subview);
    }

    if (!buffers.count(view)) {
        buffers[view] = NewPixelBuffer({view->frame.width, view->frame.height});
    }
}

// return a combined vector of all render operations for all subviews within a specified view (inclusive)
std::vector<aRenderOperation> OpsForView(uView* view) {
    std::vector<aRenderOperation> renderOps;

    for (uView* subview : view->subviews) {
        std::vector<aRenderOperation> subviewOps = OpsForView(subview);
        renderOps.insert(renderOps.end(), subviewOps.begin(), subviewOps.end());
    }

    std::vector<aRenderOperation> viewOps = view->Draw();
    renderOps.insert(renderOps.end(), viewOps.begin(), viewOps.end());
    return renderOps;
} 

//  initialise window resources and fetch render commands
void CoreRenderer::SetupWindowForRendering(uWindow* window) {

	//printf("FUNCTION STARTING\n");
    if (window->rootView == nullptr) {
        printf("UUI-ERROR: window '%s' contains no view for rendering\n", window->title.c_str());
        return;
    }

    SetupViewForRendering(window->rootView);

    aRenderCommand* renderCommand = new aRenderCommand;
    commands[window] = renderCommand;

    std::vector<aRenderOperation> ops = OpsForView(window->rootView);

    //printf("\nBegin ROP-DUMP\n\n");
    
    for (aRenderOperation op : ops) {
        
        /*printf("ROP: %d : ", op.code);

        for (float f : op.data) {
            printf("%f ", f);
        }

        printf("\n");
        */

        renderCommand->codes.push_back(op.code);
		renderCommand->indices.push_back(renderCommand->parameters.size());
        renderCommand->parameters.insert(renderCommand->parameters.end(), op.data.begin(), op.data.end());
    }

	renderCommand->codes[0] = renderCommand->codes.size();
	renderCommand->codes[0] -= 1;

    /*
    printf("\nBegin RCOM-DUMP\n");

    printf("\nCODES\n");

    for (unsigned short code : renderCommand->codes) {
        printf("%d ", code);
    }

    printf("\n\nPARAMS\n");

    for (float param : renderCommand->parameters) {
        printf("%f ", param);
    }

	printf("\n\nINDICES\n");

    for (unsigned short index : renderCommand->indices) {
        printf("%d ", index);
    }

    printf("\n\nDONE\n");

    printf("UUI-INFO: window '%s' setup for Angelo rendering\n", window->title.c_str()); */
}

//  *** PLACEHOLDER FUNCTIONS ***

//  compiler shaders and prepare for rendering
bool CoreRenderer::InitialiseRenderer() { return true; }

//  render a window, override by platform for OpenGL/Metal functionality
void CoreRenderer::RenderWindow(uWindow* window) { }

//  render a view, override by platform for OpenGL/Metal functionality
void CoreRenderer::RenderView(uView* view) { }

//  generate a new pixel buffer in GPU memory with a given size
aPixelBuffer CoreRenderer::NewPixelBuffer(uSize size) { aPixelBuffer a; return a; }

//  resize an existing pixel buffer
void CoreRenderer::ResizePixelBuffer(aPixelBuffer buffer, uSize size) { }

//  fill a buffer with a given colour - use mainly for testing purposes
void CoreRenderer::ClearBuffer(aPixelBuffer buffer, uColour colour) { }
