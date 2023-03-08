<h2>UniversalUI for Windows</h2>

This is the official UniversalUI implementation for Microsoft Windows platforms. Tested on Windows 10 and Windows 11, the main dependency is OpenGL 3.3 support.
We have also tested UniversalUI on Windows 7 but did encounter errors and strange behaviour, likely related to driver issues.

This source code will produce a .dll file called UniversalUI.dll, and should be built using the MinGW port of GCC. If you wish to build the code yourself then
use the UUI-BUILD.ps1 script as guidance, otherwise use the release UniversalUI.dll provided in the Quick-Start directory.