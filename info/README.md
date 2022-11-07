<h2>UniversalUI Info</h2>

This file has been written to provide info on structure and naming conventions.
For instructions on use and API reference, please see our other resources.

<h3>Class Naming Convention</h3>

There are two different categories of class names. The first begins with the prefix <i>Universal</i> such as <i>UniversalCompositor</i>.
Classes named in this way are core parts of the framework, NOT intended to be instantated by the user. They are usually designed for only one instance to be 
present, so instantiating more could produce crashing or other unexpected behaviour. Some of these classes are deigned to be called by the user and some are not.
Those designed for user interaction will be wrapped in helper functions to make the use more explicit.

<br>

Other classes are named starting with a lower case letter indicating which part of the framework they belong to. Examples of this convention include the <i>u</i>
prefix for classes belonging to the core UI framework or other miscellaneous UniverasalUI classes, as well as those beginning with <i>c</i> for classes that are
part of the UniversalCompositor module.
