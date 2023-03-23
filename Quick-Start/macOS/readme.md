<h2>UniversalUI for macOS</h2>

UniversalUI has been designed as a plug-and-play solution for developing macOS apps. 
It is packaged as a dynamic library (.dylib) so in order to get started, all you need
is to download the latest UniversalUI-macOS.zip xcode project, and add your code
in there. 

Alternatively, you can find the header files in the "UniversalUI" folder of the repository 
root, the pre-compiled dylib in the "Dylib-Standalone" subfolder, and the example Xcode 
project in the "Xcode-Project" subfolder.

Instructions for Building with a new Xcode project are as follows:
- create a new macOS app, selecting XIB as the interface and Objective-C as the language
- delete all source files and header files in the newly created project
- download libUniversalUI.dylib and import into the project, adding it in the target settings.
- download the UniversalUI header files and containing UniversalUI folder. Add this to the include path of your Xcode project
- create a main.cpp file and include UniversalUI/UniversalUI.h
- your application should now run!
