<p align="center">
  <img src="uui-logo-wide.png" width="300" title="uUI Logo">
</p>

A high performance graphics framework for cross-platform applications.

<h2>Core Features</h2>

- self-contained (no external dependancies)
- native performance
- M-V-C architecture
- modular design
- rapid & low code development support

<br> <br>

<h2>Main Components</h2>

- UniversalUICore <br> A lightweight UI library
- UniversalSandbox <br> A platform-independant rutime environment
- UniversalConnect <br> Native interfaces for multiple different platforms 
- Angelo Geometry Engine <br> A parametric, highly configurable geometry engine
- UniversalCompositor <br> A cross-platform compositor powered by OpenGL

**Example:** the following C++ code launches a window with the title "hello github!", and a red background:

~~~
  #include "uUI.h"
  using namespace uUI;
  
  [...]
  
  uWindow window(800, 600, "hello guthib!");
  window.backgroundColour = uColour(1.0, 0.0, 0.0, 1.0);
  
  [...]
  
~~~

<h2>Other Features</h2>

- asynchronous rendering system for high performance
- modern UI element library included as standard
- support for VR and AR applications
- custom font support
- embedded OpenGL support
- animation support
