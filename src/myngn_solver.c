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

}