#ifndef MYPHYSICS_WORLD_H
#define MYPHYSICS_WORLD_H

#include "myngn_body.h"

typedef struct myWorld myWorld;
typedef struct myRigidBody myRigidBody;

myWorld* my_World_Create();
void my_World_Step(myWorld* world, float delta_time);
void my_World_Free(myWorld* world);
void my_World_AddBody(myWorld* world, myRigidBody* body);
int my_World_GetBodyCount(myWorld* world);

myRigidBody* my_World_GetBody(myWorld* world, int index);

#endif