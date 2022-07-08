#include <iostream>
#include "location_problem/flp_Instance.h"
#include "location_problem/flp_RandomInstance.h"

int main() {

    using namespace flp;

    RandomInstance instance(10, 5, 1.5);

    std::cout << instance << std::endl;

    return 0;
}
