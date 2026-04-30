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

    typedef struct myContact {
    myVec2 normal;
    float penetration;
} myContact;
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

    out_contact->penetration = FLT_MAX;
    out_contact->normal = (myVec2){0.0f, 0.0f};

    for (int i = 0; i < vertex_a_count; i++) {
        myVec2 edge = my_Math_Sub(vertices_a[(i + 1) % vertex_a_count ], vertices_a[i]);
        myVec2 test_axis = my_Math_Norm((myVec2){edge.y, -edge.x}); // normalized normal = test axis

        myProjectionResult projectionResult_a = my_Collisions_ProjectVertices(vertices_a, test_axis, vertex_a_count);
        myProjectionResult projectionResult_b = my_Collisions_ProjectVertices(vertices_b, test_axis, vertex_b_count);

        if (projectionResult_a.max < projectionResult_b.min || projectionResult_b.max < projectionResult_a.min) {
            return false;
        }

        float pen_1 = projectionResult_a.max - projectionResult_b.min;
        float pen_2 = projectionResult_b.max - projectionResult_a.min;
        float smaller_pen = (pen_1 < pen_2) ? pen_1 : pen_2;
        
        if (smaller_pen < out_contact->penetration ) { 
            out_contact->penetration = smaller_pen;
            out_contact->normal = test_axis;
        }


    }

    for (int i = 0; i < vertex_b_count; i++) {
        myVec2 edge = my_Math_Sub(vertices_b[(i + 1) % vertex_b_count ], vertices_b[i]);
        myVec2 test_axis = my_Math_Norm((myVec2){edge.y, -edge.x}); // normalized normal = test axis

        myProjectionResult projectionResult_a = my_Collisions_ProjectVertices(vertices_a, test_axis, vertex_a_count);
        myProjectionResult projectionResult_b = my_Collisions_ProjectVertices(vertices_b, test_axis, vertex_b_count);

        if (projectionResult_a.max <= projectionResult_b.min || projectionResult_b.max <= projectionResult_a.min) {
            return false;
        }

        float pen_1 = projectionResult_a.max - projectionResult_b.min;
        float pen_2 = projectionResult_b.max - projectionResult_a.min;
        float smaller_pen = (pen_1 < pen_2) ? pen_1 : pen_2;
        
        if (smaller_pen < out_contact->penetration ) { 
            out_contact->penetration = smaller_pen;
            out_contact->normal = test_axis;
        }

    }

    myVec2 center_a = my_RigidBody_GetPosition(a);
    myVec2 center_b = my_RigidBody_GetPosition(b);
    myVec2 dir = my_Math_Sub(center_b, center_a);

    if (my_Math_Dot(dir, out_contact->normal) < 0.0f) {
        out_contact->normal.x = -out_contact->normal.x;
        out_contact->normal.y = -out_contact->normal.y;
    }



    return true;
}

