#include <windows.h>
#include "win32_handlers.h"
#include "../include/myngn_physics/myngn_world.h" 
#include "../include/myngn_physics/myngn_body.h"
#include "../include/myngn_physics/myngn_math.h"

extern myWorld* world;
const float SPEED = 150.0f;
const float ANGULAR_SPEED = 5.0f;

void on_paint(HWND hwnd) {
    PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);


        RECT rect;
        GetClientRect(hwnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        FillRect(memDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

        if (world != NULL) {
            int body_count = my_World_GetBodyCount(world);

            for (int i = 0; i < body_count; i++) {
                myRigidBody* body =  my_World_GetBody(world, i);

                if (my_RigidBody_GetType(body) == MY_RIGIDBODY_CIRCLE) {
                    float r = my_RigidBody_GetRadius(body);
                    myVec2 pos = my_RigidBody_GetPosition(body);
                    Ellipse(
                        memDC,
                        (int)(pos.x - r),
                        (int)(pos.y - r),
                        (int)(pos.x + r),
                        (int)(pos.y + r)
                    );
                }

                if (my_RigidBody_GetType(body) == MY_RIGIDBODY_BOX) {
                    myVec2* vertices = my_RigidBody_GetTransformedVertices(body);
                    int count = my_RigidBody_GetVertexCount(body);

                    MoveToEx(memDC, (int)vertices[0].x, (int)vertices[0].y, NULL);

                    for (int v = 1; v < count; v++) {
                        LineTo(memDC, (int)vertices[v].x, (int)vertices[v].y);
                    }

                    LineTo(memDC, (int)vertices[0].x, (int)vertices[0].y);
                }
            }
        }

        BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hwnd, &ps);
}

void on_key_w_down() {
    myRigidBody* body =  my_World_GetBody(world, 0);
    if (body != NULL) {
        myVec2 vel = my_RigidBody_GetVelocity(body);
        vel.y = -SPEED;
        vel.x = 0;
        my_RigidBody_SetVelocity(body, vel);
    }
}

void on_key_a_down() {
    myRigidBody* body =  my_World_GetBody(world, 0);
    if (body != NULL) {
        myVec2 vel = my_RigidBody_GetVelocity(body);
        vel.x = -SPEED;
        vel.y = 0;
        my_RigidBody_SetVelocity(body, vel);
    }
}
void on_key_s_down() {
    myRigidBody* body =  my_World_GetBody(world, 0);
    if (body != NULL) {
        myVec2 vel = my_RigidBody_GetVelocity(body);
        vel.x = 0;
        vel.y = SPEED;
        my_RigidBody_SetVelocity(body, vel);
    }
}
void on_key_d_down() {
    myRigidBody* body =  my_World_GetBody(world, 0);
    if (body != NULL) {
        myVec2 vel = my_RigidBody_GetVelocity(body);
        vel.x = SPEED;
        vel.y = 0;
        my_RigidBody_SetVelocity(body, vel);
    }
}

void on_key_space_down() {
    myRigidBody* body =  my_World_GetBody(world, 0);

    if (body != NULL) {
        float vel = my_RigidBody_GetAngularVelocity(body);
        vel = ANGULAR_SPEED;
        my_RigidBody_SetAngularVelocity(body, vel);
    }
}

void on_no_press() {
    myRigidBody* body =  my_World_GetBody(world, 0);
    if (body != NULL) {
        my_RigidBody_SetVelocity(body, (myVec2){0.0f, 0.0f});
        my_RigidBody_SetAngularVelocity(body, 0.0f);
    }
}
/*;
void on_mousemove(LPARAM lparam);
void on_leftmouseup(LPARAM lparam);
void on_rightmousedown(LPARAM lparam);
void on_rightmouseup(LPARAM lparam);
*/