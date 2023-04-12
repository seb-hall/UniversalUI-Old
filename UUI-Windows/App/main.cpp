#include <UniversalUI/UniversalUI.h>
#include <UniversalUI/Core/uDesktopApplication.h>
#include <UniversalUI/Core/CoreColour.h>
#include <UniversalUI/Core/uView.h>
#include <UniversalUI/Angelo/AngeloOperations.h>

#include <stdio.h>
#include <string>

class uButton: public uView {
    public:

    std::string title;

    void Draw(CoreRenderer* renderer) override {
        printf("UUI-INFO: Render uButton\n");
        renderer->RenderOperations({
            AngeloColour({0.125, 0.125, 0.125, 1.0}),
            AngeloRoundedRectangle({5.0f, 5.0f}, {frame.width - 5.0f, frame.height - 5.0f}, 5.0f)
        });
        renderer->RenderText(title, 15.0f);
    }
};

class BackgroundView: public uView {
    public: 

    void Draw(CoreRenderer* renderer) override {
        printf("UUI-INFO: Render BackgroundView\n");
        //renderer->RenderText("hi text here", 20.0f);
        
        renderer->RenderOperations({
            AngeloColour({0.75f, 0.75f, 0.75f, 1.0f}),
            AngeloRoundedRectangle({15.0f, 15.0f}, {frame.width - 15.0f, frame.height - 15.0f}, 10.0f)

        });
        
    }
};


class App: public uDesktopApplication {
    public:

    uWindow* window1;
    uWindow* window2;

    BackgroundView* background;
   

    App() : uDesktopApplication("MYAPP", 1, 0) {

    }

    void FinishedLaunching() override {
        printf("MyApp finished launching!\n");
        
        window1 = new uWindow({1000, 750}, "App - Main");
        window2 = new uWindow({250, 750}, "App - Tools");

        window2->rootView = new uView;

        background = new BackgroundView;
        window1->rootView = background;
        window1->rootView->frame = {0.0, 0.0, 250.0, 250.0};
        window1->rootView->globalFrame = {0.0, 0.0, 250.0, 250.0};
        window1->rootView->backgroundColour = {0.0, 0.0, 0.0, 1.0};

        for (int i = 0; i < 5; i++) {
            uButton* button = new uButton;
            button->frame = {10.0f + (65.0f*(float)i), 10.0f, 60.0f, 25.0f};
            button->globalFrame = {10.0f + (65.0f*(float)i), 10.0f, 60.0f, 25.0f};
            button->isPersistant = true;
            button->needsRedraw = true;
            button->title = "File";

            if (i == 1) {
                button->title = "Edit";
            }

            if (i == 2) {
                button->title = "View";
            }

            if (i == 3) {
                button->title = "Window";
            }

            if (i == 4) {
                button->title = "Help";
            }
            

            background->subviews.push_back(button);
        }
        


        window1->background = CORE_BLACK;

        ShowWindow(window1);
        //ShowWindow(window2);
        
    }

    void WindowResized(uWindow* window, uSize toSize) override {
        if (window == window1) {
            std::string title = "App - Main (";
            title += std::to_string(toSize.width);
            title += ", ";
            title += std::to_string(toSize.height);
            title += ")";
            SetWindowTitle(window1, title);
        }

        if (window == window2) {
            std::string title = "App - Tools (";
            title += std::to_string(toSize.width);
            title += ", ";
            title += std::to_string(toSize.height);
            title += ")";
            SetWindowTitle(window2, title);
        }
    }

};

int main(int argc, char* argv[]) {

    App* myApp = new App();
    return UniversalUI(myApp);
}