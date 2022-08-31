#include <stdio.h>
#include "./files/flag-controller/flag-controller.c"
#include "./files/file-handler/file-handler.c"
int main(int argc, char* argv[]) {
    struct Processo* processo = ProcessarFlagsIniciais(argc, argv);
    HandleProcesso(processo);
    return 0;
}
