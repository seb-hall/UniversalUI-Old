//  CoreCompositor.cpp   first written by Seb H in March 2023

#include <UniversalUI/Angelo/CoreCompositor.h>
#include <UniversalUI/Angelo/CoreAngelo.h>
#include <UniversalUI/Angelo/aPixelBuffer.h>
#include <UniversalUI/Core/uView.h>
#include <UniversalUI/Core/uWindow.h>

#include <chrono>

//bool CoreCompositor::Init() {
//    return true;
//}


void CoreCompositor::CompositeView(uView* view, aPixelBuffer* ontoBuffer) {
    
    CoreAngelo* angelo = parent->angelo;

    //  render view before rendering children
    if (view->isPersistant) {

        if (!angelo->buffers.count(view)) {
            
            //  buffer doesn't exist so make new one and render to buffer
            aPixelBuffer* viewBuffer = angelo->GeneratePixelBuffer(view);
            angelo->buffers[view] = viewBuffer;
            angelo->BindRenderTarget(viewBuffer);
            angelo->renderer->renderFrame = {0.0f, 0.0f, view->frame.width, view->frame.height};
            view->Draw(angelo->renderer);
            //angelo->UnBindRenderTarget();
            angelo->BindRenderTarget(ontoBuffer);

        } else if (view->needsRedraw) {

            //  buffer exists but is out of date, regenerate and render to buffer
            angelo->DestroyPixelBuffer(angelo->buffers[view]);
            aPixelBuffer* viewBuffer = angelo->GeneratePixelBuffer(view);
            angelo->buffers[view] = viewBuffer;
            angelo->BindRenderTarget(viewBuffer);
            angelo->renderer->renderFrame = {0.0f, 0.0f, view->frame.width, view->frame.height};
            view->Draw(angelo->renderer);
            //angelo->UnBindRenderTarget();
            angelo->BindRenderTarget(ontoBuffer);
           
            view->needsRedraw = false;
        }
        
        //  render buffer to screen
        angelo->renderer->renderFrame = view->globalFrame;
        angelo->renderer->RenderBuffer(angelo->buffers[view]);

    } else {
        
        // render directly to screen
        angelo->renderer->renderFrame = view->globalFrame;
        view->Draw(angelo->renderer);
        
    }
    
    //  render each subview to screen
    for (uView* subview : view->subviews) {
        CompositeView(subview, ontoBuffer);
    }

}

aPixelBuffer* CoreCompositor::CompositeRootView(uView* view) {

    printf("COMPOSITOR ROOT VIEW: UNBIND NOT CALLED, REMOVE\n");

    aPixelBuffer* outputBuffer = parent->angelo->GeneratePixelBuffer(view);
    parent->angelo->BindRenderTarget(outputBuffer);
    CompositeView(view, outputBuffer);
    //parent->angelo->UnBindRenderTarget();
    return outputBuffer;
    
}

aPixelBuffer* CoreCompositor::CompositeBuffers(uSize extents, std::vector<aPixelBuffer*> inputBuffers) {
    
    return new aPixelBuffer;
}

