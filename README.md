<p align="center">
  <img src="Media/UUI-Logo-Wide.png" width="300" title="uUI Logo">
</p>

A high performance, modular framework for cross-platform app development.

<h2>Main Features</h2>

- native performance
- suitable for rapid & low code development
- fully modular architecture
- multiple UI design methods
- self-contained (no external dependancies)

<br>

<h2>Core Modules</h2>

- UniversalUICore - <i> a lightweight UI library</i>
- UniversalSandbox - <i> a platform-independant rutime environment</i>
- UniversalConnect - <i> native interface modules for all major platforms </i>
- Angelo Geometry Engine - <i> a parametric, highly configurable geometry engine</i>
- UniversalCompositor - <i> a cross-platform compositor powered by OpenGL</i>
- Universal3D - <i> a lightweight 3D engine designed for on-demand rendering and auxiliary 3D implementations (e.g CAD and modelling)</i>

<br>

<h2>Example Code</h2>

**Example:** the following C++ code launches a window with the title "hello github!", and a red background:

~~~
  #include "UUICore.h"
  using namespace UUI;
  
  [...]
  
  uWindow window(800, 600, "hello githib!");
  window.backgroundColour = uColour(1.0, 0.0, 0.0, 1.0);
  PresentNewWindow(&window);
  
  [...]
  
~~~

<br>

<h2>Supported Platforms</h2>

- Windows 7 onwards

- MacOS 10.12 onwards

- iOS & iPadOS 12 onwards

- Android 9 onwards

- Linux, see compatability notes for more details

- Modern Web Browsers, see compatability notes for more details

<br>

<h2>Intended Use Cases</h2>

Many, including:

- Cross-platform and native mobile apps
- Lightweight 3D applications
- Porting existing C++ frameworks to other platforms
- Creative and production software
- Manufacturing software

<br>

<h2>Licence</h2>

This work is licensed under the Creative Commons Attribution-NonCommercial 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

In short, persons are permitted to share or use UniversalUI in any way they chose, provided they give appropriate credit, and use it for non-commercial projects. If you would like to build a commercial application with UniversalUI, please contact us directly.
