

// Command entry-points.

// Audit/dump the namespace
int main_audit_namespace(int argc, char *argv[]);

// Process a node file into different output.
int main_process(int argc, char *argv[]);

// Invoke the standalone node editor.
int main_edit(int argc, char *argv[]);

// This one can probably be deleted.
// This was a test app for a single 'helper'
// widget used as the container for a program
// when editing it.
int main_breadcrumbs(int argc, char *argv[]);
