#ifndef MY_WORLD_H
#define MY_WORLD_H

#include "my_body.h"

typedef struct myWorld myWorld;
typedef struct myRigidBody myRigidBody;

typedef struct myWorldConf {
    myVec2 gravity;
    float max_density;
    float min_density;
    float max_restitution;
    float min_restitution;
    int starting_body_capacity;
    myRigidBodyAABB bounds;
} myWorldConf;

myWorld* myWorld_Create(myWorldConf* conf);
myWorld* myWorld_CreateDefault();
myRigidBody* myWorld_GetBody(myWorld* world, int index);
void myWorld_Step(myWorld* world, float delta_time);
void myWorld_Free(myWorld* world);
void myWorld_AddBody(myWorld* world, myRigidBody* body);
void myWorld_RemoveBody(myWorld* world, myRigidBody* body);
int myWorld_GetBodyCount(myWorld* world);
float myWorld_ClampRestitution(myWorld* world, float restitution);
float myWorld_ClampDensity(myWorld* world, float density);

#endif