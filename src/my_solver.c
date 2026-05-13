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

    myVec2 normal = myMath_Norm(contact->normal);

    // Make sure normal points from A -> B
    myVec2 pos_a = myRigidBody_GetPosition(body_a);
    myVec2 pos_b = myRigidBody_GetPosition(body_b);
    myVec2 dir = myMath_Sub(pos_b, pos_a);
    if (myMath_Dot(dir, normal) < 0.0f) {
        normal = myMath_Inverse(normal);
    }

    // Positional correction: move bodies out of penetration proportionally to inverse mass
    float a_inv_mass = myRigidBody_GetInvMass(body_a);
    float b_inv_mass = myRigidBody_GetInvMass(body_b);
    float total_inv = a_inv_mass + b_inv_mass;

    if (myRigidBody_IsStatic(body_a) && myRigidBody_IsStatic(body_b)) {
        return;
    }
    else if (myRigidBody_IsStatic(body_a)) {
        myRigidBody_Move(body_b, myMath_Scale(normal, contact->penetration));
    }
    else if (myRigidBody_IsStatic(body_b)) {
        myRigidBody_Move(body_a, myMath_Scale(normal, -contact->penetration));
    }
    else if (total_inv > 0.0f) {
        float a_share = a_inv_mass / total_inv;
        float b_share = b_inv_mass / total_inv;

        myRigidBody_Move(body_a, myMath_Scale(normal, -contact->penetration * a_share));
        myRigidBody_Move(body_b, myMath_Scale(normal, contact->penetration * b_share));
    }

    // Compute relative velocity and impulse along normalized normal
    myVec2 relative_velocity = myMath_Sub(
        myRigidBody_GetVelocity(body_b),
        myRigidBody_GetVelocity(body_a)
    );

    float vel_along_normal = myMath_Dot(relative_velocity, normal);
    if (vel_along_normal > 0.0f) {
        return;
    }

    float e = myMath_Min(myRigidBody_GetRestitution(body_a), myRigidBody_GetRestitution(body_b));

    if (total_inv <= 0.0f) return;

    float j = (-(1.0f + e) * vel_along_normal) / total_inv;

    myVec2 impulse = myMath_Scale(normal, j);

    myVec2 a_vel0 = myRigidBody_GetVelocity(body_a);
    myVec2 b_vel0 = myRigidBody_GetVelocity(body_b);

    myVec2 a_vel = myMath_Sub(
        a_vel0,
        myMath_Scale(impulse, a_inv_mass)
    );

    myVec2 b_vel = myMath_Add(
        b_vel0,
        myMath_Scale(impulse, b_inv_mass)
    );

    myRigidBody_SetVelocity(body_a, a_vel);
    myRigidBody_SetVelocity(body_b, b_vel);

}