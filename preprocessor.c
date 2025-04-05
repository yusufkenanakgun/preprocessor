#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "P_header.h"

void declare(char* line, FILE *file) {
    if (sscanf(line, "@%s %s(%s,%s)", PT.oper, AT[aCount].name, AT[aCount].size1, AT[aCount].size2) == 4) {
        aCount++;
        AT[aCount-1].dim = 2;
    }
    if (sscanf(line, "@%s %s(%s)", PT.oper, AT[aCount].name, AT[aCount].size1) == 3) {
        aCount++;
        AT[aCount-1].dim = 1;
    }
    if (AT[aCount-1].dim == 2) {
        fprintf(file, "%s[%s][%s];\n", AT[aCount-1].name, AT[aCount-1].size2, AT[aCount-1].size1);
    } else {
        fprintf(file, "%s[%s];\n", AT[aCount-1].name, AT[aCount-1].size1);
    }
}

void read(char* line, FILE *file) {
    sscanf(line, "@%s %s < %s", PT.oper, PT.lhs, PT.rhs1);
    int k;
    int i = 0;
    while (i < aCount) {
        if (strcmp(PT.lhs, AT[i].name) == 0) {
            k = i;
            break;
        }
        i++;
    }
    if (AT[k].dim == 1) {
        fprintf(file, "int size = sizeof(%s) / sizeof(%s[0]);\nFILE *f = fopen(\"%s\", \"r\");\nfor(int i = 0; i < size; i++) {\tfscanf(f, \"%%d\", &%s[i]);\n}\nfclose(f);\n", PT.lhs, PT.lhs, PT.rhs1, PT.lhs);
    } else {
        fprintf(file, "int sizeRows = sizeof(%s) / sizeof(%s[0]);\nint sizeCols = sizeof(%s[0]) / sizeof(%s[0][0]);\nFILE *f = fopen(\"%s\", \"r\");\nfor(int i = 0; i < sizeRows; i++) {\n\tfor(int j = 0; j < sizeCols; j++) {\n\t\tfscanf(f, \"%%d\", &%s[i][j]);\n\t}\n}\nfclose(f);\n", PT.lhs, PT.lhs, PT.lhs, PT.lhs, PT.rhs1, PT.lhs);
    }
}


void copy(char* line, FILE *file) {
    sscanf(line, "@%s %s = %s", PT.oper, PT.lhs, PT.rhs1);
    int curDim;
    int k;
    int i = 0;
    while (i < aCount) {
        if (strcmp(PT.lhs, AT[i].name) == 0 || strcmp(PT.rhs1, AT[i].name) == 0) {
            k = i;
            break;
        }
        i++;
    }
    if (AT[k].dim == 2) {
        fprintf(file, "int sizeRows = sizeof(%s) / sizeof(%s[0]);\nint sizeCols = sizeof(%s[0]) / sizeof(%s[0][0]);\nfor(int i = 0; i < sizeRows; i++) {\n\tfor(int j = 0; j < sizeCols; j++) {\n\t\t%s[i][j] = %s[i][j];\n\t}\n}\n", PT.lhs, PT.lhs, PT.lhs, PT.lhs, PT.lhs, PT.rhs1);
    } else {
        fprintf(file, "int size = sizeof(%s) / sizeof(%s[0]);\nfor(int i = 0; i < size; i++) {\n\t%s[i] = %s[i];\n}\n", PT.lhs, PT.lhs, PT.lhs, PT.rhs1);
    }
}

void initialize(char* line, FILE* file) {
    sscanf(line, "@%s %s = %s", PT.oper, PT.lhs, PT.rhs1);
    int k;
    int i = 0;
    while (i < aCount) {
        if (strcmp(PT.lhs, AT[i].name) == 0) {
            k = i;
            break;
        }
        i++;
    }
    if (AT[k].dim == 2) {
        fprintf(file, "int sizeRows = sizeof(%s) / sizeof(%s[0]);\nint sizeCols = sizeof(%s[0]) / sizeof(%s[0][0]);\nfor(int i = 0; i < sizeRows; i++) {\n\tfor(int j = 0; j < sizeCols; j++) {\n\t\t%s[i][j] = %s;\n\t}\n}\n", PT.lhs, PT.lhs, PT.lhs, PT.lhs, PT.lhs, PT.rhs1);
    } else {
        fprintf(file, "int size = sizeof(%s) / sizeof(%s[0]);\nfor(int i = 0; i < size; i++) {\n\t%s[i] = %s;\n}\n", PT.lhs, PT.lhs, PT.lhs, PT.rhs1);
    }
}

void print(char *line, FILE *file) {
    sscanf(line, "@%s %s", PT.oper, PT.lhs);
    int k;
    int i = 0;
    while (i < aCount) {
        if (strcmp(PT.lhs, AT[i].name) == 0) {
            k = i;
            break;
        }
        i++;
    }
    if (AT[k].dim == 2) {
        fprintf(file, "int sizeRows = sizeof(%s) / sizeof(%s[0]);\nint sizeCols = sizeof(%s[0]) / sizeof(%s[0][0]);\nfor(int i = 0; i < sizeRows; i++) {\n\tfor(int j = 0; j < sizeCols; j++) {\n\t\tprintf(\"%%d\", %s[i][j]);\n\t\tprintf(\" \");\n\t}\n\tprintf(\"\\n\");\n}\n", PT.lhs, PT.lhs, PT.lhs, PT.lhs, PT.lhs);
    } else {
        fprintf(file, "int size = sizeof(%s) / sizeof(%s[0]);\nfor(int i = 0; i < size; i++) {\n\tprintf(\"%%d\", %s[i]);\n\tprintf(\" \");\n}\nprintf(\"\\n\");\n", PT.lhs, PT.lhs, PT.lhs);
    }
}

void parse(FILE *in, FILE *out) {
    char line[150];
    while (fgets(line, sizeof(line), in)) {
        if (line[0] == '@') {
            if (strstr(line, "declare")) {
                declare(line, out);
            } else if (strstr(line, "read")) {
                read(line, out);
            } else if (strstr(line, "copy")) {
                copy(line, out);
            } else if (strstr(line, "initialize")) {
                initialize(line, out);
            } else if (strstr(line, "print")) {
                print(line, out);
            }
        } else {
            fprintf(out, "%s", line);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("Error opening input file");
        return 1;
    }

    FILE *outputFile = fopen("expanded.c", "w");
    if (!outputFile) {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    parse(inputFile, outputFile);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
