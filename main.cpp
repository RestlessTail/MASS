#include "MyArknightsScriptScript.h"
#include <stdio.h>

#pragma warning(disable:4996)

int main(int argc, char* argv[]) {
#ifdef _DEBUG
	MyArknightsScriptScript m("D:\\c\\Arknights\\MyArinightsScriptScript\\demo\\demo.mass");
	m.exec();
	return 0;
#else
	if (argc >= 2) {
		MyArknightsScriptScript m(argv[1]);
		m.exec();
		return 0;
	}
	else {
		printf("Fatal error: no input file.\n");
		return -1;
	}
#endif
}