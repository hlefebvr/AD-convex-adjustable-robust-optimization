#include <iostream>
#include "location_problem/flp_Instance.h"
#include "location_problem/flp_RandomInstance.h"
#include "location_problem/flp_SeparationProblem.h"

int main() {

    using namespace flp;

    RandomInstance instance(100, 50, 1.5);


    SeparationProblem separation(instance, 2, .25);
    separation.export_model("separation.lp");
    separation.solve();

    // SeparationProblem(const Instance&);
    // -> update(const FirstStageProposition&)
    // -> solve()
    // -> RobustCertificate get_solution()

    // MasterProblem(const Instance&)
    // -> update(const RobustCertificate&)
    // -> solve()
    // -> FirstStageProposition get_solution()

    return 0;
}
