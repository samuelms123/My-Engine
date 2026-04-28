#ifndef MYPHYSICS_COLLISION_H
#define MYPHYSICS_COLLISION_H

#include <stdbool.h>
#include "../include/myngn_physics/myngn_body.h"
#include "myngn_solver.h"

bool my_Collision_CheckCircles(myRigidBody* a, myRigidBody* b, myContact* out_contact);

#endif