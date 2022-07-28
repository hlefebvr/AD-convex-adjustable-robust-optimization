//
// Created by henri on 28/07/22.
//

#include "rap_FirstStageProposition.h"
#include "../instance/rap_Instance.h"

rap::FirstStageProposition::FirstStageProposition(const rap::Instance &t_instance) : m_x_values(t_instance.n_resources()) {}
