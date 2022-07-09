//
// Created by henri on 09/07/22.
//

#include "flp_CuttingPlaneCallback.h"
#include "flp_SeparationProblem.h"
#include "flp_Instance.h"

flp::CuttingPlaneCallback::CuttingPlaneCallback(flp::Instance& t_instance, flp::SeparationProblem &t_separation)
    : Callback(t_instance, t_separation) {}

void flp::CuttingPlaneCallback::callback() {
    if (where != GRB_CB_MIPSOL) { return; }

    auto proposition = get_proposition();
    m_separation.update(proposition);
    m_separation.solve();

    std::cout << "DONE" << std::endl;
}
