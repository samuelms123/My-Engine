#ifndef MY_COLLISION_H
#define MY_COLLISION_H

#include <stdbool.h>
#include "../include/my_body.h"
#include "my_solver.h"

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

bool myCollision_CheckCircles(myRigidBody* a, myRigidBody* b, myContact* out_contact);
bool myCollision_CheckPolygons(myRigidBody* a, myRigidBody* b, myContact* out_contact);
bool myCollision_CheckCirclePolygon(myRigidBody* circle, myRigidBody* polygon, myContact* out_contact);
myProjectionResult myCollision_ProjectVertices(myVec2* vertices, myVec2 test_axis, int vertices_amount);
myCollisionType myCollision_GetCollisionType(myRigidBodyType a_type , myRigidBodyType b_type);

myVec2 GET_NORMAL();

#endif