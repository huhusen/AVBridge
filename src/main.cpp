
#include "AVBridge.h"

int main() {
    AVBridge *avBridge = new AVBridge();
    avBridge->Run();
    while (getchar() != '\n') {};
    delete avBridge;
    return 0;
}
