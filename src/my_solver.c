#include <stdbool.h>
#include "../include/my_math.h"
#include "../include/my_body.h"
#include "my_solver.h"

typedef struct myContact {
    myVec2 normal;
    float penetration;
} myContact;


void my_Solver_ResolveCollision(myRigidBody* body_a, myRigidBody* body_b, myContact* contact) {

    if (contact->penetration <= 0.0f) {
        return;
    }

    myVec2 normal = my_Math_Norm(contact->normal);

    // Make sure normal points from A -> B
    myVec2 pos_a = my_RigidBody_GetPosition(body_a);
    myVec2 pos_b = my_RigidBody_GetPosition(body_b);
    myVec2 dir = my_Math_Sub(pos_b, pos_a);
    if (my_Math_Dot(dir, normal) < 0.0f) {
        normal = my_Math_Inverse(normal);
    }

    // Positional correction: move bodies out of penetration proportionally to inverse mass
    float a_inv_mass = my_RigidBody_GetInvMass(body_a);
    float b_inv_mass = my_RigidBody_GetInvMass(body_b);
    float total_inv = a_inv_mass + b_inv_mass;

    if (my_RigidBody_IsStatic(body_a) && my_RigidBody_IsStatic(body_b)) {
        return;
    }
    else if (my_RigidBody_IsStatic(body_a)) {
        my_RigidBody_Move(body_b, my_Math_Scale(normal, contact->penetration));
    }
    else if (my_RigidBody_IsStatic(body_b)) {
        my_RigidBody_Move(body_a, my_Math_Scale(normal, -contact->penetration));
    }
    else if (total_inv > 0.0f) {
        float a_share = a_inv_mass / total_inv;
        float b_share = b_inv_mass / total_inv;

        my_RigidBody_Move(body_a, my_Math_Scale(normal, -contact->penetration * a_share));
        my_RigidBody_Move(body_b, my_Math_Scale(normal, contact->penetration * b_share));
    }

    // Compute relative velocity and impulse along normalized normal
    myVec2 relative_velocity = my_Math_Sub(
        my_RigidBody_GetVelocity(body_b),
        my_RigidBody_GetVelocity(body_a)
    );

    float vel_along_normal = my_Math_Dot(relative_velocity, normal);
    if (vel_along_normal > 0.0f) {
        return;
    }

    float e = my_Math_Min(my_RigidBody_GetRestitution(body_a), my_RigidBody_GetRestitution(body_b));

    if (total_inv <= 0.0f) return;

    float j = (-(1.0f + e) * vel_along_normal) / total_inv;

    myVec2 impulse = my_Math_Scale(normal, j);

    myVec2 a_vel0 = my_RigidBody_GetVelocity(body_a);
    myVec2 b_vel0 = my_RigidBody_GetVelocity(body_b);

    myVec2 a_vel = my_Math_Sub(
        a_vel0,
        my_Math_Scale(impulse, a_inv_mass)
    );

    myVec2 b_vel = my_Math_Add(
        b_vel0,
        my_Math_Scale(impulse, b_inv_mass)
    );

    my_RigidBody_SetVelocity(body_a, a_vel);
    my_RigidBody_SetVelocity(body_b, b_vel);

}