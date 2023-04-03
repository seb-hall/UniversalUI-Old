//  CoreRenderer.cpp   first written by Seb H in March 2023

#include <UniversalUI/Angelo/CoreRenderer.h>
#include <UniversalUI/Angelo/aRenderOperation.h>

//  setup framebuffer etc
bool CoreRenderer::Init() { return true; }

//  render specified operations
void CoreRenderer::RenderOperations(std::vector<aRenderOperation> operations) { }

//  render specified text
void CoreRenderer::RenderText(std::string text, float size) { }

//  render specified image
void CoreRenderer::RenderImage(std::string path, uSize size) { }

//  render specified buffer
void CoreRenderer::RenderBuffer(aPixelBuffer* buffer) { }


/*
#include <UniversalUI/Core/uWindow.h>
#include <UniversalUI/Core/uView.h>

#include <vector>
#include <iterator>
#include <map>

// update drawing commands for view
void CoreRenderer::SetupViewForRendering(uView* view) {

    // run function for each child view
    for (uView* subview : view->subviews) {
        SetupViewForRendering(subview);
    }

    // if there is no pixelbuffer associated with the view and there should be: make a new one
    if (!buffers.count(view) && view->persistent) {
        buffers[view] = NewPixelBuffer({view->frame.width, view->frame.height});
    }

    // if there is no rendercommand associated withthe view, it has requested a redraw 
    // or it's command was generated for a different size, regenerate new command
    if (!commands.count(view) || view->needsRedraw || (view->frame.width != commands[view].size.width || view->frame.height != commands[view].size.height)) {

        aRenderCommand renderCommand;
        renderCommand.size = {view->frame.width, view->frame.height};

        std::vector<aRenderOperation> viewOps = view->Draw();

        for (aRenderOperation op : viewOps) {
            
            renderCommand.codes.push_back(op.code);
            renderCommand.indices.push_back(renderCommand.parameters.size());
            renderCommand.parameters.insert(renderCommand.parameters.end(), op.data.begin(), op.data.end());
        }

	    //renderCommand.codes[0] = renderCommand.codes.size();
	    //renderCommand.codes[0] -= 1;

        renderCommand.parameters[0] = 0.0f;
        renderCommand.parameters[1] = 1.0f;
        renderCommand.parameters[2] = 0.0f;
        renderCommand.parameters[3] = 1.0f;

        commands[view] = renderCommand;

        if (!view->persistent) {
            view->needsRedraw = false;
        }
    }
    
}

//  initialise window resources and fetch render commands
void CoreRenderer::SetupWindowForRendering(uWindow* window) {

	//printf("FUNCTION STARTING\n");
    if (window->rootView == nullptr) {
        printf("UUI-ERROR: window '%s' contains no view for rendering\n", window->title.c_str());
        return;
    }

    SetupViewForRendering(window->rootView);

    aRenderCommand renderCommand = commands[window->rootView];
}

*/