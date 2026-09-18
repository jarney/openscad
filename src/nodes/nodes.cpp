#include "nodes/nodes.hpp"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int usage()
{
    fprintf(stderr, "Usage: nodes <command> [args...]\n");
    fprintf(stderr, "    Known processors:\n");
    fprintf(stderr, "    audit-namespace -    Checks that we have \n");
    fprintf(stderr, "                         supported all builtins and\n");
    fprintf(stderr, "                         not polluted the namespace\n");
    fprintf(stderr, "                         too much.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    process <filename>   Processes a graph file into an\n");
    fprintf(stderr, "                         openscad file so we can test it.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    edit <filename>      Open the editor for the given graph\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    parse <filename>     Reads the given openscad file and\n");
    fprintf(stderr, "                         attempts to parse it into a graph\n");
    return -1;
}

#include <QApplication>

int main(int argc, char **argv)
{
    if (argc < 1) {
	return usage();
    }
    if (!strcmp(argv[1], "audit-namespace")) {
	return main_audit_namespace(--argc, ++argv);
    }
    else if (!strcmp(argv[1], "process")) {
	return main_process(--argc, ++argv);
    }
    else if (!strcmp(argv[1], "parse")) {
	fprintf(stderr, "TODO: parser not yet implemented\n");
	return usage();
    }
    else if (!strcmp(argv[1], "edit")) {
	return main_edit(--argc, ++argv);
    }
    else if (!strcmp(argv[1], "breadcrumbs")) {
	return main_breadcrumbs(--argc, ++argv);
    }
    else {
	return usage();
    }
}

