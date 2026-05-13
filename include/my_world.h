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

myWorld* my_World_Create(myWorldConf* conf);
myWorld* my_World_CreateDefault();
void my_World_Step(myWorld* world, float delta_time);
void my_World_Free(myWorld* world);
void my_World_AddBody(myWorld* world, myRigidBody* body);
void my_World_RemoveBody(myWorld* world, myRigidBody* body);
int my_World_GetBodyCount(myWorld* world);
float my_World_ClampRestitution(myWorld* world, float restitution);
float my_World_ClampDensity(myWorld* world, float density);

myRigidBody* my_World_GetBody(myWorld* world, int index);

#endif