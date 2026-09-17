# TODO list for node editor project:

* Interoperability with wider OpenSCAD:
  * Process nodes into valid OpenSCAD code.
  * Parse SCAD code into nodes (use existing parse code)
  * Auto-render when graph updated?
  * We should deal with the whole tab-manager load/save thing
    and possibly implement EditorInterface.

* Add the node library as a submodule and add configuration to CMakeLists
  so that we can optionally build with or without it.
  Turns out we only need one tiny patch to the node library, everything else
  is a part of our code-base.

* Allow ports to have individual widgets so we can enter literals where needed.

* Context Menus
  * Icons for each builtin.
  * Tooltips for each builtin.
  * Translations for each tooltip/accessible text.
  * Allow extraction of node icons from module and function comment text (as base64?)
  * Allow removing nodes from groups (the group/node menu needs attention).

* UI bugs/annoyances:
  * When the UI is first loaded, the size of the view is way zoomed out to the maximum
    and not centered on our scene.
  * Saving nodes should also save the size if it's resizable.

* Translations
  * Create/find translations for nodes and categories
    so we can have a better experience cross-language.

* Control flow stuff:
  * For loop node should set a variable name for the iterator variable.
    * It should also create a variable node and an output node by default.

  * Each function, module, for loop, etc will be its own graph with an 'edit' button
    to allow you to go inside it.

  * Module and Function groups should have a text widget allowing the name to be specified.
    This is essentially a node that has no ports, but has a widget inside it.
    
  * Module and function groups should not allow connections outside of them
    and creating them should create a corresponding function or module node.

  * Module and function groups should have dedicated 'in' and 'out' nodes
    to manage what variables are currently in-scope for that module.

  * Still need variable assignment nodes so we can have named variables.
  