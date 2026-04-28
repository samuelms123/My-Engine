#include "myngn_collision.h"
#include "../include/myngn_physics/myngn_math.h"

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