#include "../include/myngn_physics/myngn_body.h"
#include "../include/myngn_physics/myngn_world.h"
#include "../include/myngn_physics/myngn_math.h"
#include <stdlib.h>

typedef struct myRigidBody {
    myVec2 position;
    myVec2 velocity;
    myRigidBodyType type;
    float radius;
    float density;
    float restitution;
    float area;
    float mass;
    float inv_mass;
    bool is_static;
} myRigidBody;

void my_RigidBody_CreateCircleBody(myWorld* world, float radius, float density, float restitution, myVec2 position, bool is_static) {

    myRigidBody* body = (myRigidBody*)malloc(sizeof(myRigidBody));
    /*
    if (body == NULL) {
        return NULL;
    }
        */
    body->position = position;
    body->velocity = (myVec2) {0.0f, 0.0f};
    body->type = MY_RIGIDBODY_CIRCLE;
    body->radius = radius;
    body->density = density;
    body->restitution = restitution;
    body->area = MY_PI * radius * radius;
    body->mass = body->area * density;
    body->inv_mass = (body->mass > 0) ? 1.0f / body->mass : 0.0f;
    body->is_static = is_static;

    my_World_AddBody(world, body);
}

myRigidBodyType my_RigidBody_GetType(myRigidBody* body) {return body->type;}

myVec2 my_RigidBody_GetPosition(myRigidBody* body) {return body->position;}
void my_RigidBody_SetPosition(myRigidBody* body, myVec2 pos) {
    body->position.x = pos.x;
    body->position.y = pos.y;
}

myVec2 my_RigidBody_GetVelocity(myRigidBody* body) {return body->velocity;}
void my_RigidBody_SetVelocity(myRigidBody* body, myVec2 vel) {
    body->velocity.x = vel.x;
    body->velocity.y = vel.y;
}


float my_RigidBody_GetRadius(myRigidBody* body) {
    if (body->type == MY_RIGIDBODY_CIRCLE) {
        return body->radius;
    }
    return 0.0f;
}

void my_RigidBody_Move(myRigidBody* body, myVec2 amount) {
    myVec2 new_pos = my_Math_Add(body->position, amount);
    body->position = new_pos;
}
