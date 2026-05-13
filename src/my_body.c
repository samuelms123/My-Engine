#include "../include/my_body.h"
#include "../include/my_world.h"
#include "../include/my_math.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>

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

void myRigidBody_CreateCircle(myWorld* world, float radius, float density, float restitution, myVec2 position, bool is_static) {

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
    body->density = myWorld_ClampDensity(world, density);
    body->force = (myVec2) {0.0f, 0.0f};
    body->restitution = myWorld_ClampRestitution(world, restitution);
    body->area = MY_PI * radius * radius;
    body->mass = body->area * density;
    body->is_static = is_static;
    body->vertex_count = 0;
    body->is_transform_update_required = false;
    body->angular_velocity = 0.0f;

    if (!is_static) {
        body->inv_mass = (body->mass > 0) ? 1.0f / body->mass : 0.0f;
    }
    else {
        body->inv_mass = 0.0f;
    }

    myWorld_AddBody(world, body);
}

void myRigidBody_CreateBox(myWorld* world, float width, float height, float density, float restitution, myVec2 position, bool is_static) {

    myRigidBody* body = (myRigidBody*)malloc(sizeof(myRigidBody));

    body->position = position;
    body->velocity = (myVec2) {0.0f, 0.0f};
    body->type = MY_RIGIDBODY_POLYGON;
    body->density = myWorld_ClampDensity(world, density);
    body->force = (myVec2) {0.0f, 0.0f};
    body->restitution = myWorld_ClampRestitution(world, restitution);
    body->width = width;
    body->height = height;
    body->area = width * height;
    body->mass = body->area * density;
    body->is_static = is_static;
    body->vertex_count = 4;
    body->is_transform_update_required = true;
    body->angular_velocity = 0.0f;
    body->rotation = 0.0f;

    if (!is_static) {
    body->inv_mass = (body->mass > 0) ? 1.0f / body->mass : 0.0f;
    }
    else {
        body->inv_mass = 0.0f;
    }

    // calculate box vertices
    float xh = width / 2.0f;
    float yh = height / 2.0f;

    body->vertices[0] = (myVec2){xh, yh}; // top right
    body->vertices[1] = (myVec2){-xh, yh}; // top left
    body->vertices[2] = (myVec2){-xh, -yh}; // bot left
    body->vertices[3] = (myVec2){xh, -yh}; // bot right

    myWorld_AddBody(world, body);
}

void myRigidBody_CreatePolygon(myWorld* world, myVec2* vertices, int vertex_count ,float density, float restitution, myVec2 position, bool is_static) {

    if (vertex_count < 3 || vertex_count > MY_MAX_VERTICES) {
        return;
    }
    myRigidBody* body = (myRigidBody*)malloc(sizeof(myRigidBody));

    body->position = position;
    body->velocity = (myVec2) {0.0f, 0.0f};
    body->type = MY_RIGIDBODY_POLYGON;
    body->density = myWorld_ClampDensity(world, density);
    body->force = (myVec2) {0.0f, 0.0f};
    body->restitution = myWorld_ClampRestitution(world, restitution);
    body->area = myRigidBody_CalculateArea(vertices, vertex_count);
    body->mass = body->area * density;
    body->is_static = is_static;
    body->vertex_count = vertex_count;
    body->is_transform_update_required = true;
    body->angular_velocity = 0.0f;
    body->rotation = 0.0f;

    if (!is_static) {
        body->inv_mass = (body->mass > 0) ? 1.0f / body->mass : 0.0f;
    }
    else {
        body->inv_mass = 0.0f;
    }

    // for now
    body->arithmetic_mean = myRigidBody_CalculateArithmeticMean(vertices, vertex_count);

    for (int i = 0; i < vertex_count; i++) {
        body->vertices[i].x = vertices[i].x - body->arithmetic_mean.x;
        body->vertices[i].y = vertices[i].y - body->arithmetic_mean.y;
    }

    myWorld_AddBody(world, body);
}

myRigidBodyType myRigidBody_GetType(myRigidBody* body) {return body->type;}
float myRigidBody_GetRestitution(myRigidBody* body) {return body->restitution;}
float myRigidBody_GetInvMass(myRigidBody* body) {return body->inv_mass;}
float myRigidBody_GetMass(myRigidBody* body) {return body->mass;}

myRigidBodyAABB myRigidBody_GetAABB(myRigidBody* body) {
    myRigidBodyAABB aabb;
    aabb.max.x = -FLT_MAX;
    aabb.max.y = -FLT_MAX;
    aabb.min.x = FLT_MAX;
    aabb.min.y = FLT_MAX;

    myVec2 pos = myRigidBody_GetPosition(body);

    if (body->type == MY_RIGIDBODY_CIRCLE) {
        float r = myRigidBody_GetRadius(body);
        aabb.max.x = pos.x + r;
        aabb.max.y = pos.y + r;
        aabb.min.x = pos.x - r;
        aabb.min.y = pos.y - r;
    }

    else if (body->type == MY_RIGIDBODY_POLYGON || body->type == MY_RIGIDBODY_BOX) {
        myVec2* vertices = myRigidBody_GetTransformedVertices(body);
        int count = myRigidBody_GetVertexCount(body);

        for (int i = 0; i < count; i++) {
            if (vertices[i].x > aabb.max.x) {
                aabb.max.x = vertices[i].x;
            }
            if (vertices[i].x < aabb.min.x) {
                aabb.min.x = vertices[i].x;
            }

            if (vertices[i].y > aabb.max.y) {
                aabb.max.y = vertices[i].y;
            }
            if (vertices[i].y < aabb.min.y) {
                aabb.min.y = vertices[i].y;
            }
        }
    }

    return aabb;
}

void myRigidBody_Step(myRigidBody* body, myVec2 gravity, float delta_time) {
    if (body->is_static) {return;}


    body->velocity = myMath_Add(
        body->velocity,
        myMath_Scale(gravity, delta_time)
    );

    body->position = myMath_Add(
        body->position, 
        myMath_Scale(body->velocity, delta_time)
    );

    body->rotation = body->rotation + body->angular_velocity * delta_time;

    body->force = (myVec2) {0.0f, 0.0f};
    body->is_transform_update_required = true;
}

float myRigidBody_CalculateArea(myVec2* vertices, int vertex_count) {
    if (vertex_count < 3) {return 0;}

    float a = 0.0f;
    float b = 0.0f;

    for (int i = 0; i < vertex_count; i++) {
        a += vertices[i].x * vertices[(i+1) % vertex_count].y;
        b += vertices[i].y * vertices[(i+1) % vertex_count].x;
    }

    return 0.5 * myMath_Abs((a-b));
}

myVec2 myRigidBody_GetPosition(myRigidBody* body) {return body->position;}
void myRigidBody_SetPosition(myRigidBody* body, myVec2 pos) {
    body->position.x = pos.x;
    body->position.y = pos.y;
    body->is_transform_update_required = true;
}

myVec2 myRigidBody_GetVelocity(myRigidBody* body) {return body->velocity;}
void myRigidBody_SetVelocity(myRigidBody* body, myVec2 vel) {
    body->velocity.x = vel.x;
    body->velocity.y = vel.y;
}


float myRigidBody_GetRadius(myRigidBody* body) {
    if (body->type == MY_RIGIDBODY_CIRCLE) {
        return body->radius;
    }
    return 0.0f;
}

bool myRigidBody_IsStatic(myRigidBody* body) {
    return body->is_static;
}

void myRigidBody_Rotate(myRigidBody* body, float amount) {
    body->rotation += amount;
    body->is_transform_update_required = true;
}

void myRigidBody_Move(myRigidBody* body, myVec2 amount) {
    body->position = myMath_Add(body->position, amount);
    body->is_transform_update_required = true;
}

float myRigidBody_GetAngularVelocity(myRigidBody* body) {
    return body->angular_velocity;
}

void myRigidBody_SetAngularVelocity(myRigidBody* body, float angular_velocity) {
    body->angular_velocity = angular_velocity;
}

int myRigidBody_GetVertexCount(myRigidBody* body) {
    return body->vertex_count;
}

myVec2* myRigidBody_GetTransformedVertices(myRigidBody* body) {
    if (body->is_transform_update_required) {
        
        myTransform temp_transform;

        temp_transform.p = body->position;
        temp_transform.q.cos = cosf(body->rotation);
        temp_transform.q.sin = sinf(body->rotation);
        
        for (int i = 0; i < body->vertex_count; i++) {
            body->transformed_vertices[i] = myMath_Transform(body->vertices[i], temp_transform);
        }
        
        body->is_transform_update_required = false;
    }

    return body->transformed_vertices;
}

myVec2 myRigidBody_CalculateArithmeticMean(myVec2* vertices, int vertex_count) {
    float x_total = 0.0f;
    float y_total = 0.0f;

    for (int i = 0; i < vertex_count; i++) {
        x_total += vertices[i].x;
        y_total += vertices[i].y;
    }

    return (myVec2){x_total / (float)vertex_count, y_total / (float)vertex_count};
}

myVec2 myRigidBody_GetArithmeticMean(myRigidBody* body) {
    if (myRigidBody_GetType(body) == MY_RIGIDBODY_CIRCLE) {
        return body->position;
    }

    return body->arithmetic_mean;
}

void myRigidBody_AddForce(myRigidBody* body, myVec2 amount) {
    body->force = amount;
}
