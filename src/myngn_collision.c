#include "myngn_collision.h"
#include "../include/myngn_physics/myngn_math.h"

#include <float.h>

bool my_Collision_CheckCircles(myRigidBody* a, myRigidBody* b, myContact* out_contact) {
    myVec2 pos_a = my_RigidBody_GetPosition(a);
    myVec2 pos_b = my_RigidBody_GetPosition(b);
    float radius_a = my_RigidBody_GetRadius(a);
    float radius_b = my_RigidBody_GetRadius(b);
    float radii = radius_a + radius_b;

    // Straight to broad phase for now

    float dx = pos_a.x - pos_b.x;
    float dy = pos_a.y - pos_b.y;
    float distance = my_Math_Distance(pos_a, pos_b);

    if (distance >= radii) {return false;}

    myVec2 normal = my_Math_Norm(my_Math_Sub(pos_b, pos_a));
    out_contact->penetration = radii - distance;
    out_contact->normal = normal;

    return true;
}
/*
typedef struct myRigidBody {
    myVec2 position; // center of mass position
    myVec2 velocity;
    myTransform transform;
    myRigidBodyType type;
    myVec2 vertices[MY_MAX_VERTICES];
    myVec2 transformed_vertices[MY_MAX_VERTICES];
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

    body->vertices[0] = (myVec2){xh, yh}; // top right
    body->vertices[1] = (myVec2){-xh, yh}; // top left
    body->vertices[2] = (myVec2){-xh, -yh}; // bot left
    body->vertices[3] = (myVec2){xh, -yh}; // bot right
    */

myProjectionResult my_Collisions_ProjectVertices(myVec2* vertices, myVec2 test_axis, int vertices_lenght) {
    myProjectionResult projectionResult;
    projectionResult.min = FLT_MAX; // largest possible float
    projectionResult.max = -FLT_MAX; // smallest possible float

    for (int i = 0; i < vertices_lenght; i++ ) {
        float projection = my_Math_Dot(vertices[i], test_axis);
        if (projection < projectionResult.min) {projectionResult.min = projection;}
        if (projection > projectionResult.max) {projectionResult.max = projection;}
    }

    return projectionResult;
}

bool my_Collision_CheckPolygons(myRigidBody* a, myRigidBody* b, myContact* out_contact) {
    myVec2* vertices_a = my_RigidBody_GetTransformedVertices(a);
    int vertex_a_count = my_RigidBody_GetVertexCount(a);

    myVec2* vertices_b = my_RigidBody_GetTransformedVertices(b);
    int vertex_b_count = my_RigidBody_GetVertexCount(b);

    for (int i = 0; i < vertex_a_count; i++) {
        myVec2 edge = my_Math_Sub(vertices_a[(i + 1) % vertex_a_count ], vertices_a[i]);
        myVec2 test_axis = my_Math_Norm((myVec2){edge.y, -edge.x}); // normalized normal = test axis

        myProjectionResult projectionResult_a = my_Collisions_ProjectVertices(vertices_a, test_axis, vertex_a_count);
        myProjectionResult projectionResult_b = my_Collisions_ProjectVertices(vertices_b, test_axis, vertex_b_count);

        if (projectionResult_a.max < projectionResult_b.min || projectionResult_b.max < projectionResult_a.min) {
            return false;
        }  
    }

    for (int i = 0; i < vertex_b_count; i++) {
        myVec2 edge = my_Math_Sub(vertices_b[(i + 1) % vertex_a_count ], vertices_b[i]);
        myVec2 test_axis = my_Math_Norm((myVec2){edge.y, -edge.x}); // normalized normal = test axis

        myProjectionResult projectionResult_a = my_Collisions_ProjectVertices(vertices_a, test_axis, vertex_a_count);
        myProjectionResult projectionResult_b = my_Collisions_ProjectVertices(vertices_b, test_axis, vertex_b_count);

        if (projectionResult_a.max <= projectionResult_b.min || projectionResult_b.max <= projectionResult_a.min) {
            return false;
        }  
    }

    return true; // todo: fill the contact struct
}

