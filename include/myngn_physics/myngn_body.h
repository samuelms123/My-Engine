#ifndef MYPHYSICS_BODY_H
#define MYPHYSICS_BODY_H

#include "myngn_math.h"
#include <stdbool.h>

typedef struct myWorld myWorld;
typedef struct myRigidBody myRigidBody;

typedef enum myRigidBodyType {
    MY_RIGIDBODY_CIRCLE,
    MY_RIGIDBODY_RECTANGLE
} myRigidBodyType;

//void myRigidBody_AddForce(myRigidBody* body, myVec2 force);


void my_RigidBody_CreateCircleBody(myWorld* world, float radius, float density, float restitution, myVec2 position, bool is_static);
myRigidBodyType my_RigidBody_GetType(myRigidBody* body);

myVec2 my_RigidBody_GetPosition(myRigidBody* body);
void my_RigidBody_SetPosition(myRigidBody* body, myVec2 pos);

myVec2 my_RigidBody_GetVelocity(myRigidBody* body);
void my_RigidBody_SetVelocity(myRigidBody* body, myVec2 vel);

float my_RigidBody_GetRadius(myRigidBody* body);
void my_RigidBody_Move(myRigidBody* body, myVec2 amount);
// myRigidBodyType my_RigidBody_GetType(myRigidBody* body);
// float my_RigidBody_GetRadius(myRigidBody* body);

#endif