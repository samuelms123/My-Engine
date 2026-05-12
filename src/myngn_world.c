#include <windows.h>
#include "../include/myngn_physics/myngn_world.h"
#include "../include/myngn_physics/myngn_math.h"
#include "myngn_solver.h"
#include "myngn_collision.h"
#include <windows.h>

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

myWorld* my_World_CreateDefault() {
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

myWorld* my_World_Create(myWorldConf* conf) {
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

float my_World_ClampDensity(myWorld* world, float density) {
    return my_Math_Clamp(density, world->min_density, world->max_density);
}

float my_World_ClampRestitution(myWorld* world, float restitution) {
    return my_Math_Clamp(restitution, world->min_restitution, world->max_restitution);
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

void my_World_RemoveBody(myWorld* world, myRigidBody* body) {
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

void my_World_Step(myWorld* world, float delta_time) {

    // Linear motion and rotations
    for (int i = 0; i < world->body_count; i++) {
        myRigidBody* a_body =  world->bodies[i];
        my_RigidBody_Step(a_body, world->gravity, delta_time);

        myRigidBodyAABB aabb = my_RigidBody_GetAABB(a_body);

        if (aabb.min.y < world->bounds.min.y) {
            my_World_RemoveBody(world, a_body);
            continue;
        }
    }



    // Collisions
    for (int i = 0; i < world->body_count; i++) {
        for (int j = 0; j < world->body_count; j++) {
            if (i <= j) continue;
            myRigidBody* a_body =  world->bodies[i];
            myRigidBody* b_body = world->bodies[j];

            if (my_RigidBody_IsStatic(a_body) && my_RigidBody_IsStatic(b_body)) {
                continue;
            }

            myRigidBodyType i_type = my_RigidBody_GetType(a_body);
            myRigidBodyType j_type = my_RigidBody_GetType(b_body);
            
            myCollisionType collision_type = my_Collision_GetCollisionType(i_type, j_type);
            myContact contact;


            if (collision_type == CIRCLEPOLYGON) {
                if (i_type == MY_RIGIDBODY_CIRCLE) {
                    if (my_Collisions_CheckCirclePolygon(a_body, b_body, &contact)) {
                        my_Solver_ResolveCollision(a_body, b_body, &contact);
                    }
                } else {
                    // b_body is the circle, a_body is the polygon
                    if (my_Collisions_CheckCirclePolygon(b_body, a_body, &contact)) {
                        my_Solver_ResolveCollision(b_body, a_body, &contact); 
                    }
                }
            }
            else if (collision_type == POLYGONPOLYGON) {
                if (my_Collision_CheckPolygons(a_body, b_body, &contact)) {
                    my_Solver_ResolveCollision(a_body, b_body, &contact);
                }
            }
            else if (collision_type == CIRCLECIRCLE) {
                if (my_Collision_CheckCircles(a_body, b_body, &contact)) {
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