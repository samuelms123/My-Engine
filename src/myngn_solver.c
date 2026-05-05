#include "myngn_solver.h"
#include "../include/myngn_physics/myngn_math.h"
#include "../include/myngn_physics/myngn_body.h"
#include <stdbool.h>
typedef struct myContact {
    myVec2 normal;
    float penetration;
} myContact;


void my_Solver_ResolveCollision(myRigidBody* body_a, myRigidBody* body_b, myContact* contact) {

    float push_amount = contact->penetration / 2.0f;

    myVec2 scaled_vec = my_Math_Scale(contact->normal, push_amount);

    my_RigidBody_Move(body_a, my_Math_Inverse(scaled_vec));
    my_RigidBody_Move(body_b, scaled_vec);

    myVec2 relative_velocity = my_Math_Sub(
        my_RigidBody_GetVelocity(body_b),
        my_RigidBody_GetVelocity(body_a)
    );

    float e = my_Math_Min(my_RigidBody_GetRestitution(body_a), my_RigidBody_GetRestitution(body_b));

    float j = (-(1.0f + e) * my_Math_Dot(relative_velocity, contact->normal)) / (my_RigidBody_GetInvMass(body_a) + my_RigidBody_GetInvMass(body_b));

    myVec2 a_vel0 = my_RigidBody_GetVelocity(body_a);
    myVec2 b_vel0 = my_RigidBody_GetVelocity(body_b);
    float a_mass = my_RigidBody_GetMass(body_a);
    float b_mass = my_RigidBody_GetMass(body_b);

    myVec2 a_vel = my_Math_Sub(
        a_vel0,
        my_Math_Scale(contact->normal, j / a_mass)
    );

    myVec2 b_vel = my_Math_Add(
        b_vel0,
        my_Math_Scale(contact->normal, j / b_mass)
    );

    my_RigidBody_SetVelocity(body_a, a_vel);
    my_RigidBody_SetVelocity(body_b, b_vel);

}