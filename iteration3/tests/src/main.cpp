//
//  main.cpp
//

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "TestManager.h"


int main(int argc, char* argv[]) {
    std::cout << "Running FlashPhoto tests..." << std::endl;
    TestManager *testing = new TestManager(argc, argv);
    int tres, fres, cres;
    tres = testing->testTools(0, -1);
    if (tres != 0) std::cout << "Failed FlashPhoto tool test with errno: " << tres << std::endl;
    fres = testing->testFilters(0, 10);
    if (fres != 0) std::cout << "Failed FlashPhoto filter test with errno: " << fres << std::endl;
    cres = testing->testCLI();
    if (cres != 0) std::cout << "Failed Mia CLI test with errno: " << cres << std::endl;


    delete testing;
    std::cout << "FlashPhoto and CLI tests complete";
    if (tres + fres + cres == 0) std::cout << " with no errors";
    else {
        if (tres > 0 && fres > 0) std::cout << " with FlashPhoto tool and filter tests failed";
        else if (tres > 0) std::cout << " with FlashPhoto tools test failed";
        else if (fres > 0) std::cout << " with FlashPhoto filter test failed";
        if (cres > 0) std::cout << ", with CLI tests failed";
    }
    std::cout << std::endl;
    exit(0);
}
