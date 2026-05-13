#ifndef MY_SOLVER_H
#define MY_SOLVER_H

#include "../include/my_math.h"
#include "../include/my_body.h"

typedef struct myContact {
    myVec2 normal;
    float penetration;
} myContact;


void my_Solver_ResolveCollision(myRigidBody* body_a, myRigidBody* body_b, myContact* contact);

#endif