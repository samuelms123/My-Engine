#include <windows.h>
#include "win32_handlers.h"
#include "../include/myngn_physics/myngn_world.h" 
#include "../include/myngn_physics/myngn_body.h"
#include "../include/myngn_physics/myngn_math.h"
#include <stdlib.h>


extern myWorld* world;
const float FORCE_MAGNITUDE = 500.0f;
const float ANGULAR_SPEED = 10.0f;

void on_paint(HWND hwnd, float PPM) {
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
                    int cx = (int)(pos.x * PPM);
                    int cy = (int)(height - (pos.y * PPM));
                    myRigidBodyAABB aabb = my_RigidBody_GetAABB(body);

                    Rectangle(memDC, aabb.min.x * PPM, height - (aabb.max.y * PPM), aabb.max.x * PPM, height - (aabb.min.y  * PPM)); // aabb test
                    Ellipse(
                        memDC,
                        cx - (int)(r * PPM),
                        cy - (int)(r * PPM),
                        cx + (int)(r * PPM),
                        cy + (int)(r * PPM)
                    );
                }

                // separate later
                if ((my_RigidBody_GetType(body) == MY_RIGIDBODY_BOX) || (my_RigidBody_GetType(body) == MY_RIGIDBODY_POLYGON)) {
                    myVec2* vertices = my_RigidBody_GetTransformedVertices(body);
                    int count = my_RigidBody_GetVertexCount(body);
                    myRigidBodyAABB aabb = my_RigidBody_GetAABB(body);

                    Rectangle(memDC, aabb.min.x * PPM, height - (aabb.max.y * PPM), aabb.max.x * PPM, height - (aabb.min.y  * PPM)); // aabb test

                    MoveToEx(memDC, (int)(vertices[0].x * PPM), (int)(height - vertices[0].y * PPM), NULL);

                    for (int v = 1; v < count; v++) {
                        LineTo(memDC, (int)(vertices[v].x * PPM), (int)(height - vertices[v].y * PPM));
                    }

                    // Connect the last vertex back to the first
                    LineTo(memDC, (int)(vertices[0].x * PPM), (int)(height - vertices[0].y * PPM));
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
        myVec2 force_dir;
        force_dir.x = 0;
        force_dir.y = FORCE_MAGNITUDE;

        my_RigidBody_AddForce(body, force_dir);
    }
}

void on_key_a_down() {
    myRigidBody* body =  my_World_GetBody(world, 0);
    if (body != NULL) {
        myVec2 force_dir;
        force_dir.x = -FORCE_MAGNITUDE;;
        force_dir.y = 0;

        my_RigidBody_AddForce(body, force_dir);
    }
}
void on_key_s_down() {
    myRigidBody* body =  my_World_GetBody(world, 0);
    if (body != NULL) {
        myVec2 force_dir;
        force_dir.x = 0;
        force_dir.y = -FORCE_MAGNITUDE;

        my_RigidBody_AddForce(body, force_dir);
    }
}
void on_key_d_down() {
    myRigidBody* body =  my_World_GetBody(world, 0);
    if (body != NULL) {
        myVec2 force_dir;
        force_dir.x = FORCE_MAGNITUDE;;
        force_dir.y = 0;

        my_RigidBody_AddForce(body, force_dir);
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
    return;
}

void on_lmouse_click(myVec2 mouse_pos) {
    my_RigidBody_CreateCircleBody(world, 0.5f, 10.0f, 0.5f, mouse_pos, false);

}

void on_rmouse_click(myVec2 mouse_pos) {
    my_RigidBody_CreateBoxBody(world, 1.0f, 1.0f, 10.0f, 0.5f, mouse_pos, false);
}