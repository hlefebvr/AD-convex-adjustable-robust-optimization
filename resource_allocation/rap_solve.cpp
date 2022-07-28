//
// Created by henri on 28/07/22.
//
#include <iostream>
#include "instance/rap_RandomInstance.h"
#include "master/rap_MasterProblem.h"

using namespace rap;

int main(int t_argc, const char** t_argv) {

    RandomInstance instance(10, 20);

    MasterProblem master(instance);

    return 0;
}
