#ifndef MYPHYSICS_COLLISION_H
#define MYPHYSICS_COLLISION_H

#include <stdbool.h>
#include "../include/myngn_physics/myngn_body.h"
#include "myngn_solver.h"

typedef struct myProjectionResult {
    float min;
    float max;
} myProjectionResult;

typedef enum myCollisionType {
    CIRCLECIRCLE,
    CIRCLEPOLYGON,
    POLYGONPOLYGON,
    BOXBOX
} myCollisionType;

bool my_Collision_CheckCircles(myRigidBody* a, myRigidBody* b, myContact* out_contact);
bool my_Collision_CheckPolygons(myRigidBody* a, myRigidBody* b, myContact* out_contact);
bool my_Collisions_CheckCirclePolygon(myRigidBody* circle, myRigidBody* polygon, myContact* out_contact);
myProjectionResult my_Collisions_ProjectVertices(myVec2* vertices, myVec2 test_axis, int vertices_amount);
myCollisionType my_Collision_GetCollisionType(myRigidBodyType a_type , myRigidBodyType b_type);

myVec2 GET_NORMAL();

#endif