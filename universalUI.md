<h1>universalUI</h1>

A lightweight, highly customisable immediate-mode grahics library.

<h2>Core Features</h2>

- self-contained (no external dependancies)
- cross-platform
- M-V-C architecture
- very fast implementation

**Example:** the following code launches a window with the title "hello github!", and a red background:

~~~
  #include "uui.h"
  using namespace uui;
  
  [...]
  
  uWindow window(800, 600, "hello guthib!");
  window.backgroundColour = uColour(1.0, 0.0, 0.0, 1.0)
  
  [...]
  
~~~

<h2>Other Features</h2>

- flexible constraint system
- modern UI elements included
- powerful classes included
- custom font support
- embedded OpenGL support
- animation support
