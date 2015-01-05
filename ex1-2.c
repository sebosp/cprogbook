#include <stdio.h>
// Experiment to find out what happens when printf argument string contains back-slash escapes.
// Used: perl -e 'print "#include <stdio.h>\nmain(){\n";foreach (a..z){print "\tprintf(\"\\$_\");\n"};print "}"' > ex1-2.c 
main(){
	printf("\a");
	printf("\b");
	printf("\c");
	printf("\d");
	printf("\e");
	printf("\f");
	printf("\g");
	printf("\h");
	printf("\i");
	printf("\j");
	printf("\k");
	printf("\l");
	printf("\m");
	printf("\n");
	printf("\o");
	printf("\p");
	printf("\q");
	printf("\r");
	printf("\s");
	printf("\t");
	printf("\u");
	printf("\v");
	printf("\w");
	printf("\x");
	printf("\y");
	printf("\z");
}
