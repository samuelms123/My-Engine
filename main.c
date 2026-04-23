#include "engine.h"
#include "mathmy.h"
#include "win_handlers.h"
#include <windowsx.h>
#include <windows.h>

AppState app_state = {0};

void engine_cleanup(EngineState* state) {
    if (state->rects != NULL) {
        free(state->rects);
        state->rects = NULL;
    }
}

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

    case WM_MOUSEMOVE:
        on_mousemove(lParam);
        return 0;

    case WM_LBUTTONUP:
        on_leftmouseup(lParam);
        return 0;

    case WM_RBUTTONDOWN:
        on_rightmousedown(lParam);
        return 0;

    case WM_RBUTTONUP:
        on_rightmouseup(lParam);
        return 0;

    case WM_ERASEBKGND:
        return 1;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // init app state  --Refactor to master "app_init"--
    engine_init(&app_state.physics);
    app_state.window_w = 600;
    app_state.window_h = 600;
    app_state.mouse_pos = (Vec2){0, 0};
    //


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
        CW_USEDEFAULT, CW_USEDEFAULT,  app_state.window_w, app_state.window_h,

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

        // for debugging: if the delta time is too big, we can just clamp it to a reasonable value (e.g., 0.25 seconds)
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

        // PHYSICS PROCESS
        while (physics_accumulator >= TARGET_PHYSICS_DELTA) {
            engine_update(&app_state.physics, TARGET_PHYSICS_DELTA);
            physics_accumulator -= TARGET_PHYSICS_DELTA;
        }

        // RENDERING PROCESS
        if (render_accumulator >= TARGET_RENDER_DELTA) {
            engine_render(hwnd);
            render_accumulator -= TARGET_RENDER_DELTA; 
        }

    }
    engine_cleanup(&app_state.physics);
    return (int)msg.wParam;
}
