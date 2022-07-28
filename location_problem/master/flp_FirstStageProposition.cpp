//
// Created by henri on 28/07/22.
//

#include "flp_FirstStageProposition.h"
#include "../instance/flp_Instance.h"

flp::FirstStageProposition::FirstStageProposition(const flp::Instance &t_instance)  : m_x_values(t_instance.n_sites()) {}