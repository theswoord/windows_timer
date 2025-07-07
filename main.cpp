#include <windows.h> // Includes all the necessary WinAPI headers

// Forward declaration of our Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// The main entry point for a Windows GUI application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1. Register the Window Class
    WNDCLASSEX wc = {}; // Initialize to zero
    wc.cbSize        = sizeof(WNDCLASSEX);             // Size of this structure
    wc.lpfnWndProc   = WindowProc;                     // Pointer to the window procedure function
    wc.hInstance     = hInstance;                      // Handle to the application instance
    wc.lpszClassName = L"MyAlwaysOnTopWindowClass";    // Name of the window class (wide character string)
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);     // Background brush color

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Window Registration Failed!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // 2. Create the Window
    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST,                   // <<< THIS IS THE KEY EXTENDED STYLE FOR "ALWAYS ON TOP"
        L"MyAlwaysOnTopWindowClass",     // Window class name
        L"Always On Top Test Window",    // Window title
        WS_OVERLAPPEDWINDOW,             // Standard window style (title bar, border, min/max buttons)
        CW_USEDEFAULT, CW_USEDEFAULT,    // Default X, Y position
        400, 200,                        // Width, Height
        NULL,                            // No parent window
        NULL,                            // No menu
        hInstance,                       // Application instance handle
        NULL                             // No creation parameters
    );

    if (hwnd == NULL) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // 3. Show and Update the Window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 4. Message Loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); // Translates virtual-key messages into character messages
        DispatchMessage(&msg);  // Dispatches the message to the WindowProc function
    }

    return (int)msg.wParam; // Return the exit code
}

// The Window Procedure: Handles messages sent to the window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE: // When the user clicks the close button (X)
            DestroyWindow(hwnd); // Destroy the window
            break;

        case WM_DESTROY: // When the window is being destroyed
            PostQuitMessage(0); // Post a WM_QUIT message to exit the message loop
            break;

        case WM_PAINT: // When the window needs to be repainted
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps); // Get a device context for painting

                // Simple text drawing (optional, just to show something)
                // Text color
                SetTextColor(hdc, RGB(0, 0, 0)); // Black text
                // Background mode (transparent is often good for text)
                SetBkMode(hdc, TRANSPARENT);
                // Draw text
                RECT clientRect;
                GetClientRect(hwnd, &clientRect); // Get client area dimensions
                DrawText(hdc, L"This window should stay on top!", -1, &clientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                EndPaint(hwnd, &ps); // Release the device context
            }
            break;

        default:
            // Let the default window procedure handle any messages we don't handle
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0; // Message handled
}