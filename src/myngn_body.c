#include "../include/myngn_physics/myngn_body.h"
#include "../include/myngn_physics/myngn_world.h"
#include "../include/myngn_physics/myngn_math.h"
#include <stdlib.h>
#include <math.h>

#define MY_MAX_VERTICES 8

typedef struct myRigidBody {
    myVec2 position; // center of mass position
    myVec2 velocity;
    myTransform transform;
    myRigidBodyType type;
    myVec2 vertices[MY_MAX_VERTICES];
    myVec2 transformed_vertices[MY_MAX_VERTICES];
    myVec2 arithmetic_mean;
    int vertex_count;
    float angular_velocity;
    float width;
    float height;
    float rotation; // rad
    float radius;
    float density;
    float restitution;
    float area;
    float mass;
    float inv_mass;
    bool is_static;
    bool is_transform_update_required;
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
    body->vertex_count = 0;
    body->is_transform_update_required = false;
    body->angular_velocity = 0.0f;

    my_World_AddBody(world, body);
}

void my_RigidBody_CreateBoxBody(myWorld* world, float width, float height, float density, float restitution, myVec2 position, bool is_static) {

    myRigidBody* body = (myRigidBody*)malloc(sizeof(myRigidBody));

    body->position = position;
    body->velocity = (myVec2) {0.0f, 0.0f};
    body->type = MY_RIGIDBODY_BOX;
    body->density = density;
    body->restitution = restitution;
    body->width = width;
    body->height = height;
    body->area = width * height;
    body->mass = body->area * density;
    body->inv_mass = (body->mass > 0) ? 1.0f / body->mass : 0.0f;
    body->is_static = is_static;
    body->vertex_count = 4;
    body->is_transform_update_required = true;
    body->angular_velocity = 0.0f;

    // calculate box vertices
    float xh = width / 2.0f;
    float yh = height / 2.0f;

    body->vertices[0] = (myVec2){xh, yh}; // top right
    body->vertices[1] = (myVec2){-xh, yh}; // top left
    body->vertices[2] = (myVec2){-xh, -yh}; // bot left
    body->vertices[3] = (myVec2){xh, -yh}; // bot right

    my_World_AddBody(world, body);
}

myRigidBodyType my_RigidBody_GetType(myRigidBody* body) {return body->type;}

myVec2 my_RigidBody_GetPosition(myRigidBody* body) {return body->position;}
void my_RigidBody_SetPosition(myRigidBody* body, myVec2 pos) {
    body->position.x = pos.x;
    body->position.y = pos.y;
    body->is_transform_update_required = true;
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

void my_RigidBody_Rotate(myRigidBody* body, float amount) {
    body->rotation += amount;
    body->is_transform_update_required = true;
}

void my_RigidBody_Move(myRigidBody* body, myVec2 amount) {
    body->position = my_Math_Add(body->position, amount);
    body->is_transform_update_required = true;
}

float my_RigidBody_GetAngularVelocity(myRigidBody* body) {
    return body->angular_velocity;
}

void my_RigidBody_SetAngularVelocity(myRigidBody* body, float angular_velocity) {
    body->angular_velocity = angular_velocity;
}

int my_RigidBody_GetVertexCount(myRigidBody* body) {
    return body->vertex_count;
}

myVec2* my_RigidBody_GetTransformedVertices(myRigidBody* body) {
    if (body->is_transform_update_required) {
        
        myTransform temp_transform;

        temp_transform.p = body->position;
        temp_transform.q.cos = cosf(body->rotation);
        temp_transform.q.sin = sinf(body->rotation);
        
        for (int i = 0; i < body->vertex_count; i++) {
            body->transformed_vertices[i] = my_Math_Transform(body->vertices[i], temp_transform);
        }
        
        body->is_transform_update_required = false;
    }

    return body->transformed_vertices;
}
