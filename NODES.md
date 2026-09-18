# TODO list for node editor project:

* Start building test infrastructure
  * One main with lots of options/types
  * Namespace checker to make sure we've covered all the builtins and not polluted the namespace too much."
  * Evaluator to allow us to evaluate graphs and check the output.
  * 'node' editor should allow us to specify the name of a file as argv[1] and auto-create if it doesn't exist.
  * Start dogfooding the thing.

* Context Menus
  * Icons for each builtin.
  * Tooltips for each builtin.
  * Translations for each tooltip/accessible text.
  * Allow extraction of node icons from module and function comment text (as base64?)
  * Allow removing nodes from groups (the group/node menu needs attention).

* UI bugs/annoyances:
  * When the UI is first loaded, the size of the view is way zoomed out to the maximum
    and not centered on our scene.
  * Group margins seem way bigger than they should be, so we're
    not calculating them correctly, I think.

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
  * Auto-render when graph updated?
  * We should deal with the whole tab-manager load/save thing
    and possibly implement EditorInterface.

* Add the node library as a submodule and add configuration to CMakeLists
  so that we can optionally build with or without it.
  Turns out we only need one tiny patch to the node library, everything else
  is a part of our code-base.

* Allow ports to have individual widgets so we can enter literals where needed.

* Translations
  * Create/find translations for nodes and categories
    so we can have a better experience cross-language.

