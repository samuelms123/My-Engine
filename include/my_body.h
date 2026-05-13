#ifndef MY_BODY_H
#define MY_BODY_H

#include <stdbool.h>
#include "my_math.h"

typedef struct myWorld myWorld;
typedef struct myRigidBody myRigidBody;

typedef enum myRigidBodyType {
    MY_RIGIDBODY_CIRCLE,
    MY_RIGIDBODY_BOX,
    MY_RIGIDBODY_POLYGON
} myRigidBodyType;

typedef struct myRigidBodyAABB {
    myVec2 max;
    myVec2 min;

} myRigidBodyAABB;


void myRigidBody_CreateCircle(myWorld* world, float radius, float density, float restitution, myVec2 position, bool is_static);
void myRigidBody_CreateBox(myWorld* world, float width, float height, float density, float restitution, myVec2 position, bool is_static);
void myRigidBody_CreatePolygon(myWorld* world, myVec2* vertices, int vertex_count ,float density, float restitution, myVec2 position, bool is_static);
myRigidBodyType myRigidBody_GetType(myRigidBody* body);
myRigidBodyAABB myRigidBody_GetAABB(myRigidBody* body);

void myRigidBody_Step(myRigidBody* body, myVec2 gravity, float delta_time);

float myRigidBody_GetRestitution(myRigidBody* body);
float myRigidBody_GetInvMass(myRigidBody* body);
float myRigidBody_GetMass(myRigidBody* body);

myVec2 myRigidBody_GetPosition(myRigidBody* body);
void myRigidBody_SetPosition(myRigidBody* body, myVec2 pos);

float myRigidBody_GetAngularVelocity(myRigidBody* body);
void myRigidBody_SetAngularVelocity(myRigidBody* body, float angular_velocity);

myVec2 myRigidBody_GetVelocity(myRigidBody* body);
void myRigidBody_SetVelocity(myRigidBody* body, myVec2 vel);

float myRigidBody_GetRadius(myRigidBody* body);
void myRigidBody_Rotate(myRigidBody* body, float amount);

myVec2* myRigidBody_GetTransformedVertices(myRigidBody* body);
int myRigidBody_GetVertexCount(myRigidBody* body);

void myRigidBody_Move(myRigidBody* body, myVec2 amount);
void myRigidBody_AddForce(myRigidBody* body, myVec2 amount);

float myRigidBody_CalculateArea(myVec2* vertices, int vertex_count);

bool myRigidBody_IsStatic(myRigidBody* body);
// for now

myVec2 myRigidBody_CalculateArithmeticMean(myVec2* vertices, int vertex_count);
myVec2 myRigidBody_GetArithmeticMean(myRigidBody* body);

#endif