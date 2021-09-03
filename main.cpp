#include "MyArknightsScriptScript.h"
#include <stdio.h>

#pragma warning(disable:4996)

int main(int argc, char* argv[]) {
	if (argc >= 2) {
		MyArknightsScriptScript m(argv[1]);
		m.exec();
		return 0;
	}
	else {
		printf("Fatal error: no input file.\n");
		return -1;
	}
}
