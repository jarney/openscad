# TODO list for node editor project:

* Process nodes into valid OpenSCAD code.

* Parse SCAD code into nodes

* Add the node library as a submodule and add configuration to CMakeLists
  so that we can optionally build with or without it.

* Add a text output to the 'nodes' editor for demo purposes.

* Loading node JSON doesn't preserve locations correctly.
  * This is in the node editor,  We need to fork it and fix the drag position update.

* Fork node editor so we can fix some bugs and add a few small features.

* Allow ports to have individual widgets so we can enter literals where needed.
