#pragma once

int ParseArgs(int argc, char* argv[]);

int ParseExport(int argc, char* argv[], int curArg);

int ParseHelp(int argc, char* argv[], int curArg);

int ExecMASS();

int ShowHelp();