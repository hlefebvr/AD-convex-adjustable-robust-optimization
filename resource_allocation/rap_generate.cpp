//
// Created by henri on 28/07/22.
//

#include <iostream>
#include "instance/rap_RandomInstance.h"

using namespace rap;

int main(int argc, const char**) {

    RandomInstance instance(10, 20);

    std::cout << instance << std::endl;

    return 0;
}