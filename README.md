<p align="center">
  <img src="uui-logo-wide.png" width="300" title="uUI Logo">
</p>

A lightweight, powerful, flexible and highly customisable immediate-mode grahics framework.

<h2>Core Features</h2>

- self-contained (no external dependancies)
- cross-platform
- M-V-C architecture
- highly flexible layout engine

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
