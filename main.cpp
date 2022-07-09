#include <iostream>
#include "location_problem/flp_Instance.h"
#include "location_problem/flp_RandomInstance.h"
#include "location_problem/flp_SeparationProblem.h"
#include "location_problem/flp_MasterProblem.h"
#include "location_problem/flp_CuttingPlaneCallback.h"

int main() {

    using namespace flp;

    const double Gamma = 2;
    const double deviation = .25;

    RandomInstance instance(10, 5, 1.5);


    SeparationProblem separation(instance, Gamma, deviation);
    CuttingPlaneCallback cb(instance, separation);

    MasterProblem master(instance, deviation);
    master.export_model("master.lp");
    master.set_callback(cb);
    master.solve();

    return 0;
}
