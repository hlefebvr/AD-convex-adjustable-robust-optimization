//
// Created by henri on 09/07/22.
//

#ifndef CONVEX_ARO_FLP_SCENARIOGENERATIONCALLBACK_H
#define CONVEX_ARO_FLP_SCENARIOGENERATIONCALLBACK_H

#include "flp_Callback.h"

namespace flp {
    class ScenarioGenerationCallback;
}

class flp::ScenarioGenerationCallback : public flp::Callback {
protected:
    void add_cut(const FirstStageProposition &t_proposition, const RobustCertificate &t_certificate) override;
public:
    ScenarioGenerationCallback(Instance& t_instance, SeparationProblem& t_separation);
};


#endif //CONVEX_ARO_FLP_SCENARIOGENERATIONCALLBACK_H
