#include "../include/myngn_physics/myngn_world.h"
#include <windows.h>
#include "myngn_solver.h"
#include "myngn_collision.h"

struct myWorld {
    // myVec2 gravity;
    myRigidBody** bodies; // An array of body pointers

    int body_count;
    int body_capacity;
};

myWorld* my_World_Create() {
    myWorld* world = (myWorld*)malloc(sizeof(myWorld));
    
    //world->gravity = (myVec2){0.0f, 9.81f}; 
    world->body_capacity = 16;
    world->body_count = 0;
    
    // Allocate the array of pointers!
    world->bodies = (myRigidBody**)malloc(world->body_capacity * sizeof(myRigidBody*));
    
    return world;
}

void my_World_AddBody(myWorld* world, myRigidBody* body) {
    if (world->body_count >= world->body_capacity) {
        world->body_capacity *= 2;

        world->bodies = (myRigidBody**)realloc(
            world->bodies, 
            world->body_capacity * sizeof(myRigidBody*)
        );
    }
    world->bodies[world->body_count++] = body;
}

void my_World_Free(myWorld* world) {
    if (world->bodies != NULL) {
        free(world->bodies);
    }
    free(world);
}

void my_World_Step(myWorld* world, float delta_time) {

    for (int i = 0; i < world->body_count; i++) {
        myRigidBody* a_body =  world->bodies[i];

        myVec2 pos = my_RigidBody_GetPosition(a_body);
        myVec2 vel = my_RigidBody_GetVelocity(a_body);

        pos.x += vel.x * delta_time;
        pos.y += vel.y * delta_time;

        my_RigidBody_SetPosition(a_body, pos);
    }

    for (int i = 0; i < world->body_count; i++) {
        for (int j = 0; j < world->body_count; j++) {
            if (i <= j) continue;
            myRigidBody* a_body =  world->bodies[i];
            myRigidBody* b_body = world->bodies[j];

            myRigidBodyType i_type = my_RigidBody_GetType(a_body);
            myRigidBodyType j_type = my_RigidBody_GetType(b_body);

            myContact contact;

            if (i_type == MY_RIGIDBODY_CIRCLE && j_type == MY_RIGIDBODY_CIRCLE) {
                if(my_Collision_CheckCircles(a_body, b_body, &contact)) {
                    my_Solver_ResolveCollision(a_body, b_body, &contact);
                }
            }

        }
    }
}

int my_World_GetBodyCount(myWorld* world) {
    return world->body_count;
}

myRigidBody* my_World_GetBody(myWorld* world, int index) {
    if (index < 0 || index >= world->body_count) {
        return NULL; 
    }
    return world->bodies[index];
}