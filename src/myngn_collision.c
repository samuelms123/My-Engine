#include "myngn_collision.h"
#include "../include/myngn_physics/myngn_math.h"

#include <float.h>
#include <math.h>

myVec2 normal = (myVec2){0.0f, 0.0f};

myVec2 GET_NORMAL() {
    return normal;
}

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

myProjectionResult my_Collisions_ProjectVertices(myVec2* vertices, myVec2 normalized_axis, int vertices_lenght) {
    myProjectionResult projectionResult;
    projectionResult.min = FLT_MAX; // largest possible float
    projectionResult.max = -FLT_MAX; // smallest possible float

    for (int i = 0; i < vertices_lenght; i++ ) {
        float projection = my_Math_Dot(vertices[i], normalized_axis);
        if (projection < projectionResult.min) {projectionResult.min = projection;}
        if (projection > projectionResult.max) {projectionResult.max = projection;}
    }

    return projectionResult;
}

myProjectionResult my_Collisions_ProjectCircle(myVec2 circle_center, float radius, myVec2 normalized_axis) {
    myProjectionResult projectionResult;
    projectionResult.min = FLT_MAX;
    projectionResult.max = -FLT_MAX;

    float center = my_Math_Dot(circle_center, normalized_axis);

    projectionResult.max = center + radius;
    projectionResult.min = center - radius;

    return projectionResult;
}

int my_Collisions_FindClosestPointOnPolygon(myVec2 circle_circle_center, myVec2* vertices, int vertex_count) {
    int index = -1;
    float closest_point = FLT_MAX;

    for (int i = 0; i < vertex_count; i++) {
        float dist = my_Math_SquaredDistance(circle_circle_center, vertices[i]);
        if (dist < closest_point) {
            closest_point = dist;
            index = i;
        }
    }

    return index;
}

bool my_Collision_CheckPolygons(myRigidBody* a, myRigidBody* b, myContact* out_contact) {
    out_contact->penetration = FLT_MAX;
    out_contact->normal = (myVec2){0.0f, 0.0f};

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

        float pen_1 = projectionResult_a.max - projectionResult_b.min;
        float pen_2 = projectionResult_b.max - projectionResult_a.min;
        float smaller_pen = (pen_1 < pen_2) ? pen_1 : pen_2;
        
        if (smaller_pen < out_contact->penetration ) { 
            out_contact->penetration = smaller_pen;
            out_contact->normal = test_axis;
            normal = test_axis;
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

    // get arithmetic centr
    myVec2 center_a = my_RigidBody_GetPosition(a);
    myVec2 center_b = my_RigidBody_GetPosition(b);
    myVec2 dir = my_Math_Sub(center_b, center_a);

    if (my_Math_Dot(dir, out_contact->normal) < 0.0f) {
        out_contact->normal.x = -out_contact->normal.x;
        out_contact->normal.y = -out_contact->normal.y;
    }



    return true;
}


bool my_Collisions_CheckCirclePolygon(myRigidBody* circle, myRigidBody* polygon, myContact* out_contact) {
    out_contact->penetration = FLT_MAX;
    out_contact->normal = (myVec2){0.0f, 0.0f};


    myVec2* vertices = my_RigidBody_GetTransformedVertices(polygon);
    int vertex_count = my_RigidBody_GetVertexCount(polygon);

    myVec2 circle_center = my_RigidBody_GetPosition(circle);
    float radius = my_RigidBody_GetRadius(circle);

    myVec2 test_axis = (myVec2){0.0f, 0.0f};

    for (int i = 0; i < vertex_count; i++) {
        myVec2 edge = my_Math_Sub(vertices[(i + 1) % vertex_count ], vertices[i]);
        test_axis = my_Math_Norm((myVec2){edge.y, -edge.x}); // normalized normal = test axis

        myProjectionResult projectionResult_polygon = my_Collisions_ProjectVertices(vertices, test_axis, vertex_count);
        myProjectionResult projectionResult_circle = my_Collisions_ProjectCircle(circle_center, radius, test_axis);

        if ((projectionResult_polygon.max < projectionResult_circle.min) || (projectionResult_circle.max < projectionResult_polygon.min)) {
            return false;
        }

        float pen_1 = projectionResult_polygon.max - projectionResult_circle.min;
        float pen_2 = projectionResult_circle.max - projectionResult_polygon.min;
        float smaller_pen = (pen_1 < pen_2) ? pen_1 : pen_2;
        
        if (smaller_pen < out_contact->penetration ) { 
            out_contact->penetration = smaller_pen;
            out_contact->normal = test_axis;
        }
    }

    int closest_point_index = my_Collisions_FindClosestPointOnPolygon(circle_center, vertices, vertex_count);
    myVec2 closest_vertex = vertices[closest_point_index];

    test_axis = my_Math_Norm(my_Math_Sub(closest_vertex, circle_center));

    myProjectionResult projectionResult_polygon = my_Collisions_ProjectVertices(vertices, test_axis, vertex_count);
    myProjectionResult projectionResult_circle = my_Collisions_ProjectCircle(circle_center, radius, test_axis);

    if ((projectionResult_polygon.max < projectionResult_circle.min) || (projectionResult_circle.max < projectionResult_polygon.min)) {
        return false;
    }

    float pen_1 = projectionResult_polygon.max - projectionResult_circle.min;
    float pen_2 = projectionResult_circle.max - projectionResult_polygon.min;
    float smaller_pen = (pen_1 < pen_2) ? pen_1 : pen_2;
    
    if (smaller_pen < out_contact->penetration ) { 
        out_contact->penetration = smaller_pen;
        out_contact->normal = test_axis;
    }

    myVec2 polygon_center = my_Math_Add(my_RigidBody_GetArithmeticMean(polygon), my_RigidBody_GetPosition(polygon));
    myVec2 dir = my_Math_Sub(polygon_center, circle_center);

    if (my_Math_Dot(dir, out_contact->normal) < 0.0f) {
        out_contact->normal.x = -out_contact->normal.x;
        out_contact->normal.y = -out_contact->normal.y;
    }


    return true;
}

myCollisionType my_Collision_GetCollisionType(myRigidBodyType a_type , myRigidBodyType b_type) {
    if (a_type == MY_RIGIDBODY_CIRCLE && b_type == MY_RIGIDBODY_CIRCLE) {
        return CIRCLECIRCLE;
    }

    else if ((a_type == MY_RIGIDBODY_CIRCLE && b_type == MY_RIGIDBODY_POLYGON) || (a_type == MY_RIGIDBODY_POLYGON && b_type == MY_RIGIDBODY_CIRCLE)) {
        return CIRCLEPOLYGON;
    }
    else {
        return POLYGONPOLYGON;
    }
}
