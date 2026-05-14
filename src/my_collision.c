#include "../include/my_math.h"
#include "my_collision.h"

#include <float.h>
#include <math.h>

myVec2 normal = (myVec2){0.0f, 0.0f};

myVec2 GET_NORMAL() {
    return normal;
}

bool myCollision_CheckCircles(myRigidBody* a, myRigidBody* b, myContact* out_contact) {
    myVec2 pos_a = myRigidBody_GetPosition(a);
    myVec2 pos_b = myRigidBody_GetPosition(b);
    float radius_a = myRigidBody_GetRadius(a);
    float radius_b = myRigidBody_GetRadius(b);
    float radii = radius_a + radius_b;

    // Straight to broad phase for now

    float dx = pos_a.x - pos_b.x;
    float dy = pos_a.y - pos_b.y;
    float distance = myMath_Distance(pos_a, pos_b);

    if (distance >= radii) {return false;}

    myVec2 normal = myMath_Norm(myMath_Sub(pos_b, pos_a));
    out_contact->penetration = radii - distance;
    out_contact->normal = normal;
    out_contact->a = a;
    out_contact->b = b;

    return true;
}

myProjectionResult myCollision_ProjectVertices(myVec2* vertices, myVec2 normalized_axis, int vertices_lenght) {
    myProjectionResult projectionResult;
    projectionResult.min = FLT_MAX; // largest possible float
    projectionResult.max = -FLT_MAX; // smallest possible float

    for (int i = 0; i < vertices_lenght; i++ ) {
        float projection = myMath_Dot(vertices[i], normalized_axis);
        if (projection < projectionResult.min) {projectionResult.min = projection;}
        if (projection > projectionResult.max) {projectionResult.max = projection;}
    }

    return projectionResult;
}

myProjectionResult myCollision_ProjectCircle(myVec2 circle_center, float radius, myVec2 normalized_axis) {
    myProjectionResult projectionResult;
    projectionResult.min = FLT_MAX;
    projectionResult.max = -FLT_MAX;

    float center = myMath_Dot(circle_center, normalized_axis);

    projectionResult.max = center + radius;
    projectionResult.min = center - radius;

    return projectionResult;
}

int myCollision_FindClosestPointOnPolygon(myVec2 circle_circle_center, myVec2* vertices, int vertex_count) {
    int index = -1;
    float closest_point = FLT_MAX;

    for (int i = 0; i < vertex_count; i++) {
        float dist = myMath_SquaredDistance(circle_circle_center, vertices[i]);
        if (dist < closest_point) {
            closest_point = dist;
            index = i;
        }
    }

    return index;
}

bool myCollision_CheckPolygons(myRigidBody* a, myRigidBody* b, myContact* out_contact) {
    out_contact->penetration = FLT_MAX;
    out_contact->normal = (myVec2){0.0f, 0.0f};

    myVec2* vertices_a = myRigidBody_GetTransformedVertices(a);
    int vertex_a_count = myRigidBody_GetVertexCount(a);

    myVec2* vertices_b = myRigidBody_GetTransformedVertices(b);
    int vertex_b_count = myRigidBody_GetVertexCount(b);

    for (int i = 0; i < vertex_a_count; i++) {
        myVec2 edge = myMath_Sub(vertices_a[(i + 1) % vertex_a_count ], vertices_a[i]);
        myVec2 test_axis = myMath_Norm((myVec2){edge.y, -edge.x}); // normalized normal = test axis

        myProjectionResult projectionResult_a = myCollision_ProjectVertices(vertices_a, test_axis, vertex_a_count);
        myProjectionResult projectionResult_b = myCollision_ProjectVertices(vertices_b, test_axis, vertex_b_count);

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
        myVec2 edge = myMath_Sub(vertices_b[(i + 1) % vertex_b_count ], vertices_b[i]);
        myVec2 test_axis = myMath_Norm((myVec2){edge.y, -edge.x}); // normalized normal = test axis

        myProjectionResult projectionResult_a = myCollision_ProjectVertices(vertices_a, test_axis, vertex_a_count);
        myProjectionResult projectionResult_b = myCollision_ProjectVertices(vertices_b, test_axis, vertex_b_count);

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

    out_contact->a = a;
    out_contact->b = b;

    return true;
}


bool myCollision_CheckCirclePolygon(myRigidBody* circle, myRigidBody* polygon, myContact* out_contact) {
    out_contact->penetration = FLT_MAX;
    out_contact->normal = (myVec2){0.0f, 0.0f};


    myVec2* vertices = myRigidBody_GetTransformedVertices(polygon);
    int vertex_count = myRigidBody_GetVertexCount(polygon);

    myVec2 circle_center = myRigidBody_GetPosition(circle);
    float radius = myRigidBody_GetRadius(circle);

    myVec2 test_axis = (myVec2){0.0f, 0.0f};

    for (int i = 0; i < vertex_count; i++) {
        myVec2 edge = myMath_Sub(vertices[(i + 1) % vertex_count ], vertices[i]);
        test_axis = myMath_Norm((myVec2){edge.y, -edge.x}); // normalized normal = test axis

        myProjectionResult projectionResult_polygon = myCollision_ProjectVertices(vertices, test_axis, vertex_count);
        myProjectionResult projectionResult_circle = myCollision_ProjectCircle(circle_center, radius, test_axis);

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

    int closest_point_index = myCollision_FindClosestPointOnPolygon(circle_center, vertices, vertex_count);
    myVec2 closest_vertex = vertices[closest_point_index];

    test_axis = myMath_Norm(myMath_Sub(closest_vertex, circle_center));

    myProjectionResult projectionResult_polygon = myCollision_ProjectVertices(vertices, test_axis, vertex_count);
    myProjectionResult projectionResult_circle = myCollision_ProjectCircle(circle_center, radius, test_axis);

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

    out_contact->a = circle;
    out_contact->b = polygon;
    
    return true;
}

myCollisionType myCollision_GetCollisionType(myRigidBodyType a_type , myRigidBodyType b_type) {
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
