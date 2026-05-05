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
    myVec2 force;
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
    body->density = 15.0f;
    body->force = (myVec2) {0.0f, 0.0f};
    body->restitution = 0.5f;
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
    body->force = (myVec2) {0.0f, 0.0f};
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

void my_RigidBody_CreatePolygonBody(myWorld* world, myVec2* vertices, int vertex_count ,float density, float restitution, myVec2 position, bool is_static) {

    if (vertex_count < 3 || vertex_count > MY_MAX_VERTICES) {
        return;
    }
    myRigidBody* body = (myRigidBody*)malloc(sizeof(myRigidBody));

    body->position = position;
    body->velocity = (myVec2) {0.0f, 0.0f};
    body->type = MY_RIGIDBODY_POLYGON;
    body->density = 15.0f;
    body->force = (myVec2) {0.0f, 0.0f};
    body->restitution = 0.5f;
    body->area = my_RigidBody_CalculateArea(vertices, vertex_count);
    body->mass = body->area * density;
    body->inv_mass = (body->mass > 0) ? 1.0f / body->mass : 0.0f;
    body->is_static = is_static;
    body->vertex_count = vertex_count;
    body->is_transform_update_required = true;
    body->angular_velocity = 0.0f;

    // for now
    body->arithmetic_mean = my_RigidBody_CalculateArithmeticMean(vertices, vertex_count);

    for (int i = 0; i < vertex_count; i++) {
        body->vertices[i].x = vertices[i].x - body->arithmetic_mean.x;
        body->vertices[i].y = vertices[i].y - body->arithmetic_mean.y;
    }

    my_World_AddBody(world, body);
}

myRigidBodyType my_RigidBody_GetType(myRigidBody* body) {return body->type;}
float my_RigidBody_GetRestitution(myRigidBody* body) {return body->restitution;}
float my_RigidBody_GetInvMass(myRigidBody* body) {return body->inv_mass;}
float my_RigidBody_GetMass(myRigidBody* body) {return body->mass;}

void my_RigidBody_Step(myRigidBody* body, float delta_time) {
    body->velocity = my_Math_Add(
        body->velocity,
        my_Math_Scale(body->force, delta_time)
    );

    body->position = my_Math_Add(
        body->position, 
        my_Math_Scale(body->velocity, delta_time)
    );

    body->rotation = body->rotation + body->angular_velocity * delta_time;

    body->force = (myVec2) {0.0f, 0.0f};
    body->is_transform_update_required = true;
}

float my_RigidBody_CalculateArea(myVec2* vertices, int vertex_count) {
    if (vertex_count < 3) {return 0;}

    float a = 0.0f;
    float b = 0.0f;

    for (int i = 0; i < vertex_count; i++) {
        a += vertices[i].x * vertices[(i+1) % vertex_count].y;
        b += vertices[i].y * vertices[(i+1) % vertex_count].x;
    }

    return 0.5 * my_Math_Abs((a-b));
}

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

myVec2 my_RigidBody_CalculateArithmeticMean(myVec2* vertices, int vertex_count) {
    float x_total = 0.0f;
    float y_total = 0.0f;

    for (int i = 0; i < vertex_count; i++) {
        x_total += vertices[i].x;
        y_total += vertices[i].y;
    }

    return (myVec2){x_total / (float)vertex_count, y_total / (float)vertex_count};
}

myVec2 my_RigidBody_GetArithmeticMean(myRigidBody* body) {
    if (my_RigidBody_GetType == MY_RIGIDBODY_CIRCLE) {
        return body->position;
    }

    return body->arithmetic_mean;
}

void my_RigidBody_AddForce(myRigidBody* body, myVec2 amount) {
    body->force = amount;
}
