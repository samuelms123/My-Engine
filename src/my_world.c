#include <windows.h>
#include "../include/my_world.h"
#include "../include/my_math.h"
#include "my_solver.h"
#include "my_collision.h"

struct myWorld {
    myVec2 gravity;
    float max_density;
    float min_density;
    float max_restitution;
    float min_restitution;
    myRigidBody** bodies;

    myRigidBodyAABB bounds;
    int body_count;
    int body_capacity;
};

myWorld* myWorld_CreateDefault() {
    myWorld* world = (myWorld*)malloc(sizeof(myWorld));
    
    world->gravity = (myVec2){0.0f, -9.81f}; 
    world->max_density = 22.6f; // iridium
    world->min_density = 0.00009f; // hydrogen
    world->max_restitution = 1.0f;
    world->min_restitution = 0.0f;
    world->body_capacity = 16;
    world->body_count = 0;

    world->bounds.max.x = 1000.0f;
    world->bounds.max.y = 1000.0f;
    world->bounds.min.x = 0.0f;
    world->bounds.min.y = 0.0f;

    
    world->bodies = (myRigidBody**)malloc(world->body_capacity * sizeof(myRigidBody*));
    
    return world;
}

myWorld* myWorld_Create(myWorldConf* conf) {
    myWorld* world = (myWorld*)malloc(sizeof(myWorld));
    
    world->gravity = conf->gravity; 
    world->max_density = conf->max_density; 
    world->min_density = conf->min_density; 
    world->max_restitution = conf->max_restitution;
    world->min_restitution = conf->min_restitution;
    
    world->bounds = conf->bounds;

    world->body_capacity = conf->starting_body_capacity;
    world->body_count = 0;
    
    world->bodies = (myRigidBody**)malloc(world->body_capacity * sizeof(myRigidBody*));
    
    return world;
}

float myWorld_ClampDensity(myWorld* world, float density) {
    return myMath_Clamp(density, world->min_density, world->max_density);
}

float myWorld_ClampRestitution(myWorld* world, float restitution) {
    return myMath_Clamp(restitution, world->min_restitution, world->max_restitution);
}

void myWorld_AddBody(myWorld* world, myRigidBody* body) {
    if (world->body_count >= world->body_capacity) {
        world->body_capacity *= 2;

        world->bodies = (myRigidBody**)realloc(
            world->bodies, 
            world->body_capacity * sizeof(myRigidBody*)
        );
    }
    world->bodies[world->body_count++] = body;
}

void myWorld_Free(myWorld* world) {
    if (world->bodies != NULL) {
        free(world->bodies);
    }
    free(world);
}

void myWorld_RemoveBody(myWorld* world, myRigidBody* body) {
    if (world == NULL || body == NULL) return;

    for (int i = 0; i < world->body_count; i++) {
        if (world->bodies[i] == body) {
            free(body);
            world->bodies[i] = world->bodies[world->body_count - 1];

            world->body_count--;

            return;
        }
    }
}

void myWorld_Step(myWorld* world, float delta_time) {

    // Linear motion and rotations
    for (int i = 0; i < world->body_count; i++) {
        myRigidBody* a_body =  world->bodies[i];
        myRigidBody_Step(a_body, world->gravity, delta_time);

        myRigidBodyAABB aabb = myRigidBody_GetAABB(a_body);

        if (aabb.min.y < world->bounds.min.y) {
            myWorld_RemoveBody(world, a_body);
            continue;
        }
    }



    // Collisions
    for (int i = 0; i < world->body_count; i++) {
        for (int j = 0; j < world->body_count; j++) {
            if (i <= j) continue;
            myRigidBody* a_body =  world->bodies[i];
            myRigidBody* b_body = world->bodies[j];

            if (myRigidBody_IsStatic(a_body) && myRigidBody_IsStatic(b_body)) {
                continue;
            }

            myRigidBodyType i_type = myRigidBody_GetType(a_body);
            myRigidBodyType j_type = myRigidBody_GetType(b_body);
            
            myCollisionType collision_type = myCollision_GetCollisionType(i_type, j_type);
            myContact contact;


            if (collision_type == CIRCLEPOLYGON) {
                if (i_type == MY_RIGIDBODY_CIRCLE) {
                    if (myCollision_CheckCirclePolygon(a_body, b_body, &contact)) {
                        mySolver_ResolveCollision(&contact);
                    }
                } else {
                    // b_body is the circle, a_body is the polygon
                    if (myCollision_CheckCirclePolygon(b_body, a_body, &contact)) {
                        mySolver_ResolveCollision(&contact); 
                    }
                }
            }
            else if (collision_type == POLYGONPOLYGON) {
                if (myCollision_CheckPolygons(a_body, b_body, &contact)) {
                    mySolver_ResolveCollision(&contact);
                }
            }
            else if (collision_type == CIRCLECIRCLE) {
                if (myCollision_CheckCircles(a_body, b_body, &contact)) {
                    mySolver_ResolveCollision(&contact);
                }
            }
        }
    }
}

int myWorld_GetBodyCount(myWorld* world) {
    return world->body_count;
}

myRigidBody* myWorld_GetBody(myWorld* world, int index) {
    if (index < 0 || index >= world->body_count) {
        return NULL; 
    }
    return world->bodies[index];
}