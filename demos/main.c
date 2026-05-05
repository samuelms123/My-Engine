#include "../include/myngn_physics/myngn_math.h"
#include "../include/myngn_physics/myngn_world.h"
#include "win32_handlers.h"
#include "../include/myngn_physics/myngn_world.h"
#include <windowsx.h>
#include <windows.h>
#include <stdbool.h>

myWorld* world = NULL;
bool key_w_down = false;
bool key_a_down = false;
bool key_s_down = false;
bool key_d_down = false;
bool key_space_down = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        on_paint(hwnd);
        return 0;

    case WM_KEYDOWN:
        if (wParam == 0x57) key_w_down = true; // W
        if (wParam == 0x41) key_a_down = true; // A
        if (wParam == 0x53) key_s_down = true; // S
        if (wParam == 0x44) key_d_down = true; // D
        if (wParam == 0x20) key_space_down = true; // Space
        return 0;

    case WM_KEYUP:
        if (wParam == 0x57) key_w_down = false; // W
        if (wParam == 0x41) key_a_down = false; // A
        if (wParam == 0x53) key_s_down = false; // S
        if (wParam == 0x44) key_d_down = false; // D
        if (wParam == 0x20) key_space_down = false; // Space
        return 0;

    case WM_ERASEBKGND:
        return 1;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    world = my_World_Create();
    myVec2 box_vertices[4] = {
        (myVec2){-150.0f, -150.0f},
        (myVec2){-150.0f, 150.0f},
        (myVec2){150.0f, 150.0f},
        (myVec2){150.0f, -150.0f},
    };
    my_RigidBody_CreateCircleBody(world, 50.0f, 1.0f, 0.5f, (myVec2){350.0f, 100.0f}, false);
    my_RigidBody_CreatePolygonBody(
    world,
    box_vertices,
    4,
    1.0f,             
    1.0f,                     
    (myVec2){650.0f, 150.0f}, 
    false                   
    );
/*
    myVec2 triangle_vertices[3] = {
        (myVec2){0.0f, -50.0f},
        (myVec2){-50.0f, 50.0f},
        (myVec2){50.0f, 50.0f},
    };
    my_RigidBody_CreateCircleBody(world, 50.0f, 1.0f, 0.5f, (myVec2){350.0f, 100.0f}, false);
    my_RigidBody_CreatePolygonBody(
    world,
    box_vertices,
    4,
    1.0f,             
    1.0f,                     
    (myVec2){650.0f, 150.0f}, 
    false                   
    );
        my_RigidBody_CreatePolygonBody(
    world,
    triangle_vertices,
    3,
    1.0f,             
    1.0f,                     
    (myVec2){650.0f, 150.0f}, 
    false                   
    );
    
    my_RigidBody_CreateCircleBody(world, 50.0f, 1.0f, 0.5f, (myVec2){350.0f, 500.0f}, false);
    my_RigidBody_CreatePolygonBody(
        world,
        triangle_vertices,
        4,
        1.0f,             
        1.0f,                     
        (myVec2){450.0f, 100.0f}, 
        false                   
    );
    //my_RigidBody_CreateBoxBody(world, 50.0f, 50.0f, 1.0f, 1.0f, (myVec2){550.0f, 100.0f}, false);
    /*
    my_RigidBody_CreateCircleBody(world, 40.0f, 1.0f, 0.5f, (myVec2){600.0f, 100.0f}, false);
    my_RigidBody_CreateCircleBody(world, 40.0f, 1.0f, 0.5f, (myVec2){900.0f, 100.0f}, false);
    my_RigidBody_CreateCircleBody(world, 40.0f, 1.0f, 0.5f, (myVec2){100.0f, 100.0f}, false);
    my_RigidBody_CreateCircleBody(world, 40.0f, 1.0f, 0.5f, (myVec2){400.0f, 100.0f}, false);
    */
    // app_state.mouse_pos = (myVec2){0, 0};


    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"My-Engine",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        400, 10,  1000, 1000,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );


    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    bool running = true;
    MSG msg = { 0 };

    const float FPS_CAP = 80.0f;
    const float TPS = 80.0f;

    LARGE_INTEGER frequency, last_time, current_time;
    QueryPerformanceFrequency(&frequency); // How many "counts" per second, machine dependent
    QueryPerformanceCounter(&last_time);    // Starting timestamp

    float physics_accumulator = 0.0f;
    float render_accumulator = 0.0f;

    const float TARGET_PHYSICS_DELTA = 1.0f / TPS;      // every 0.01666 seconds 
    const float TARGET_RENDER_DELTA = 1.0f / FPS_CAP;   // every 0.01250 seconds


    // MAIN ENGINE LOOP
    while (running) {
        QueryPerformanceCounter(&current_time);

        // Calculate how much time passed since we were here last time
        float delta_time = (float)(current_time.QuadPart - last_time.QuadPart) / frequency.QuadPart;

        if (delta_time > 0.25f) {
            delta_time = TARGET_PHYSICS_DELTA; 
        }
        // Update last time for the next loop
        last_time = current_time;

        // Add the dt to the separate accumulator "buckets" 
        physics_accumulator += delta_time;
        render_accumulator += delta_time;

        // OS
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (key_w_down) on_key_w_down();
        if (key_a_down) on_key_a_down();
        if (key_s_down) on_key_s_down();
        if (key_d_down) on_key_d_down();
        if (key_space_down) on_key_space_down();

        // PHYSICS PROCESS
        while (physics_accumulator >= TARGET_PHYSICS_DELTA) {
            my_World_Step(world, TARGET_PHYSICS_DELTA);
            physics_accumulator -= TARGET_PHYSICS_DELTA;
        }

        // RENDERING PROCESS
        if (render_accumulator >= TARGET_RENDER_DELTA) {
            InvalidateRect(hwnd, NULL, TRUE); 
            UpdateWindow(hwnd);
            render_accumulator -= TARGET_RENDER_DELTA; 
        }

    }
    my_World_Free(world);
    return (int)msg.wParam;
}