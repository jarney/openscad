# TODO list for node editor project:

* Process nodes into valid OpenSCAD code.

* Parse SCAD code into nodes (use existing parse code)

* Add the node library as a submodule and add configuration to CMakeLists
  so that we can optionally build with or without it.

* Allow ports to have individual widgets so we can enter literals where needed.

* Make a better menu for adding nodes that allows some kind of icons to be displayed.

* Zoom problem in example tool when it first loads the 'node' one.

* Allow ports to specify a set of types instead of a single one
  so we can accept more than one type of data for some functions.

* Node Programs
  A node program is a collection of graphs.
  * Create a serializer for a program that includes:
    * All of the graphs
    * All of the nodes in each graph
      Including any node-specific data like internal node data or
      connections to other graphs for 'for' and 'module' things.
    * All of the connections
    * All of the groups
    * Coordinates

* Control flow stuff:
  * Each function, module, for loop, etc will be its own graph with an 'edit' button
    to allow you to go inside it.
  * Group should have a 'title/name' and some 'comment' text.
  * Each graph should have a 'comment' block to allow rich text documentation in-line.
  * Module and Function groups should have a text widget allowing the name to be specified.
  * Module and function groups should not allow connections outside of them
    and creating them should create a corresponding function or module node.
  * Module and function groups should have dedicated 'in' and 'out' nodes
    to manage what variables are currently in-scope for that module.

