#ifndef MYPHYSICS_SOLVER_H
#define MYPHYSICS_SOLVER_H

#include "../include/myngn_physics/myngn_math.h"
#include "../include/myngn_physics/myngn_body.h"

typedef struct myContact {
    myVec2 normal;
    float penetration;
} myContact;


void my_Solver_ResolveCollision(myRigidBody* body_a, myRigidBody* body_b, myContact* contact);

#endif