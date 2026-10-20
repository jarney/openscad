# TODO list for node editor project:

* Implement other languages:
  * Implement some basic common-lisp
  * Implement some C stuff.

* Add back per-node widgets for the built-in nodes
  that need to be implemented in editor.
  Register them with the editor interface by node type (global name).

* Testing:
  * Start making some actual graphs/drawings with it.

* Context Menus
  * Derive 'add' menu from loaded packages.
      * Icons for each builtin. (from metadata as b64 strings in module)
  * Translations for each tooltip/accessible text.
  * Allow extraction of node icons from module and function comment text (as base64?)

* UI bugs/annoyances:
  * When the UI is first loaded, the size of the view is way zoomed out to the maximum
    and not centered on our scene.

* Control flow stuff:
  * For loop should create a variable node and an output node by default.

  * Each function, module, for loop, etc will be its own graph with an 'edit' button
    to allow you to go inside it.

  * Module and Function groups should have a text widget allowing the name to be specified.
    This is essentially a node that has no ports, but has a widget inside it.
    
  * Module and function groups should have dedicated 'in' and 'out' nodes
    to manage what variables are currently in-scope for that module.

* Interoperability with wider OpenSCAD:
  * Process nodes into valid OpenSCAD code.
  * Parse SCAD code into nodes (use existing parse code)
  * We should deal with the whole tab-manager load/save thing
    and possibly implement EditorInterface.

* Allow ports to have individual widgets so we can enter literals where needed.

