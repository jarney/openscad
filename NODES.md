# TODO list for node editor project:

* Process nodes into valid OpenSCAD code.

* Parse SCAD code into nodes (use existing parse code)

* Add the node library as a submodule and add configuration to CMakeLists
  so that we can optionally build with or without it.
  Turns out we only need one tiny patch to the node library, everything else
  is a part of our code-base.

* Allow ports to have individual widgets so we can enter literals where needed.

* Better menu/UI semantics
  * Make or get a whole bunch of icons for each of the node types.
  * Allow extraction of node icons from module comment text (as base64?)
  * Allow removing nodes from groups (the group/node menu needs attention).
  * Allow categories to also have icons

* UI bugs/annoyances:
  * When the UI is first loaded, the size of the view is way zoomed out to the maximum
    and not centered on our scene.

* Control flow stuff:
  * Each function, module, for loop, etc will be its own graph with an 'edit' button
    to allow you to go inside it.  They should completely own their sub-graphs
    and express things recursively inside each node.

  * Module and Function groups should have a text widget allowing the name to be specified.
    This is essentially a node that has no ports, but has a widget inside it.
    
  * Module and function groups should not allow connections outside of them
    and creating them should create a corresponding function or module node.

  * Module and function groups should have dedicated 'in' and 'out' nodes
    to manage what variables are currently in-scope for that module.

  * Still need variable assignment nodes so we can have named variables.
  