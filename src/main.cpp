#include <iostream>
#include <string>
#include "../include/TrafficManager.h"
#include "../include/BackendProtocol.h"

int main(int argc, char* argv[]) {
    bool backendMode = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--backend") {
            backendMode = true;
            break;
        }
    }

    if (backendMode) {
        BackendProtocol bp;
        bp.run();
    } else {
        TrafficManager tm;
        tm.runMenu();
    }
    
    return 0;
}
