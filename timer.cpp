#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <iomanip>
#define TIMER_1 1

#define CLASS_NAME L"timer2h"

// compiled using g++ E:\timer\timer.cpp -o E:\timer\timer.exe -luser32 -lgdi32 -lkernel32 -std=c++17 -g -D_UNICODE -DUNICODE
// learn CMAKE
// #include <cstring.h>

int i = 7200; // set timer

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t timer[] = L"my timer";
    WNDCLASSEX wc = {};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassExW(&wc);

    HWND hwnd = CreateWindowEx(WS_EX_TOPMOST,
                               CLASS_NAME,
                               L"miwmiw",
                               WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               400,
                               200,
                               NULL,
                               NULL,
                               hInstance,
                               NULL);

    ShowWindow(hwnd, nCmdShow);
    // i++;
    // printf("%d",i);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg); // Translates virtual-key messages into character messages
        DispatchMessage(&msg);  // Dispatches the message to the WindowProc function
    }

    return (int)msg.wParam; // Return the exit code
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
    case WM_CLOSE:           // When the user clicks the close button (X)
        DestroyWindow(hwnd); // Destroy the window
        KillTimer(hwnd, TIMER_1); 
        break;

    case WM_CREATE:                // When the window is being destroyed
        SetTimer(hwnd,             // handle to main window
                 TIMER_1,          // timer identifier
                 1000,             // 10-second interval
                 (TIMERPROC)NULL); // no timer callback
        break;
    case WM_TIMER:
            if(wParam == TIMER_1) // if wparam was called by TIMER_1
            {
                i--;
                int err = InvalidateRect(hwnd,NULL,TRUE);
                // printf("%d ", err);
                UpdateWindow(hwnd);

            }
            break;


    case WM_DESTROY:        // When the window is being destroyed
        PostQuitMessage(0); // Post a WM_QUIT message to exit the message loop
        break;

    case WM_PAINT: // When the window needs to be repainted
    {
        PAINTSTRUCT ps;
        
        HDC hdc = BeginPaint(hwnd, &ps); // Get a device context for painting
        
        // Simple text drawing (optional, just to show something)
        SetTextColor(hdc, RGB(0, 0, 0)); // Black text
        // Background mode (transparent is often good for text)
        SetBkMode(hdc, TRANSPARENT);
        LOGFONT lf;
        ZeroMemory(&lf,sizeof(LOGFONT)); // memset(&lf,0,sizeof(LOGFONT));



        RECT clientRect;
        GetClientRect(hwnd, &clientRect); // Get client area dimensions
        int desiredFontHeight = clientRect.bottom * 45 / 100;
        lf.lfHeight = desiredFontHeight;
        lf.lfWeight  = FW_EXTRABOLD; // bold italic o l3aybat 
        wcscpy_s(lf.lfFaceName, LF_FACESIZE, L"Arial"); // n9DR NSMIH BO3O 

        HFONT hfont = CreateFontIndirect(&lf);
        HFONT hOldFont = (HFONT)SelectObject(hdc, hfont);

        long long hours = i / 3600; // 2
        long long minutes = (i % 3600) / 60; 
        long long seconds = i % 60;
        std::wstringstream sstream;

        sstream << std::setw(2) << std::setfill(L'0') << hours << L":"
        << std::setw(2) << std::setfill(L'0') << minutes << L":" 
        << std::setw(2) << std::setfill(L'0') << seconds  ;
        std::wstring todraw = sstream.str();

        DrawText(hdc, todraw.c_str(), -1, &clientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        
        SelectObject(hdc, hOldFont); // kanrj3 font l9dim hehe bach maykbtch lia kolchi b same BIG FONT 

        DeleteObject(hfont); //li creatit

        EndPaint(hwnd, &ps); // Release the device context
    }
    break;

    default:
        // Let the default window procedure handle any messages we don't handle
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0; // Message handled
}

// const wchar_t timer[] = L"my timer";
// int main(){

//     WNDCLASS wc = { };

//     wc.lpfnWndProc   = WindowProc;
//     wc.hInstance     = hInstance;
//     wc.lpszClassName = timer;
// }
