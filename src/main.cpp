
#include "AVBridge.h"

int main() {
    AVBridge *avBridge = new AVBridge();
    avBridge->Run();
    while (getchar() != '\n') {};
    return 0;
}
