<h2>UniversalUI Core</h2>

UniversalUI Core contains the completely cross-platform parts of the framework.

In here, you will find abstract classes for building UniversalUI applications as well as protocols 
to which parts of the platform-specific modules conform.

It is advised to follow our examples and guides for which classes to implement and which to ignore.
They have been specifically designed to get you and your app up and running as soon as possible.

<h3>Core/Interface</h3>

This folder contains classes specifally for interface design such as windows, buttons and text boxes.
They are all designed to be used by app developers so feel free to implement them as much as you wish.
These classes also have the most detailed documentation out of all the framwork so please see the 
API reference for all the information you might need!

<h3>Core/UTB</h3>

This folder contains methods and types for reading and parsing UTB files. As explained in the documentation,
UTB files are used by the framwork as persistent storage of interface elements and layouts and other app information.
They have been designed for extremely fast parsing, forward/backwards compatability and flexible parameter assignment.

Most of the methods and types defined in this folder aren't designed to be used by app developers so please 
see the documentation for the best ways to use this part of the framework.

<h3>Core/Protocol</h3>

This folder contains protocols to which modules of the host implementation conform. These types make inheritance and
includes much easier and cleaner, and they are designed to have no includes apart from standard C++ library components
such as std::string and std::vector. 

<h3>Core/Graphics3D</h3>

This folder contains methods and types for 3D applications to use. They are not required for UniversalUI but some
may be used for core features in future versions.
