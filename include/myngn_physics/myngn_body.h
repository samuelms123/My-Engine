#ifndef MYPHYSICS_BODY_H
#define MYPHYSICS_BODY_H

#include "myngn_math.h"
#include <stdbool.h>

typedef struct myWorld myWorld;
typedef struct myRigidBody myRigidBody;

typedef enum myRigidBodyType {
    MY_RIGIDBODY_CIRCLE,
    MY_RIGIDBODY_BOX,
    MY_RIGIDBODY_POLYGON
} myRigidBodyType;


void my_RigidBody_CreateCircleBody(myWorld* world, float radius, float density, float restitution, myVec2 position, bool is_static);
void my_RigidBody_CreateBoxBody(myWorld* world, float width, float height, float density, float restitution, myVec2 position, bool is_static);
void my_RigidBody_CreatePolygonBody(myWorld* world, myVec2* vertices, int vertex_count ,float density, float restitution, myVec2 position, bool is_static);
myRigidBodyType my_RigidBody_GetType(myRigidBody* body);

void my_RigidBody_Step(myRigidBody* body, float delta_time);

float my_RigidBody_GetRestitution(myRigidBody* body);
float my_RigidBody_GetInvMass(myRigidBody* body);
float my_RigidBody_GetMass(myRigidBody* body);

myVec2 my_RigidBody_GetPosition(myRigidBody* body);
void my_RigidBody_SetPosition(myRigidBody* body, myVec2 pos);

float my_RigidBody_GetAngularVelocity(myRigidBody* body);
void my_RigidBody_SetAngularVelocity(myRigidBody* body, float angular_velocity);

myVec2 my_RigidBody_GetVelocity(myRigidBody* body);
void my_RigidBody_SetVelocity(myRigidBody* body, myVec2 vel);

float my_RigidBody_GetRadius(myRigidBody* body);
void my_RigidBody_Rotate(myRigidBody* body, float amount);

myVec2* my_RigidBody_GetTransformedVertices(myRigidBody* body);
int my_RigidBody_GetVertexCount(myRigidBody* body);

void my_RigidBody_Move(myRigidBody* body, myVec2 amount);
void my_RigidBody_AddForce(myRigidBody* body, myVec2 amount);

float my_RigidBody_CalculateArea(myVec2* vertices, int vertex_count);
// for now

myVec2 my_RigidBody_CalculateArithmeticMean(myVec2* vertices, int vertex_count);
myVec2 my_RigidBody_GetArithmeticMean(myRigidBody* body);

#endif