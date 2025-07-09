#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <mmsystem.h>
#define TIMER_1 1
#define PAUSE_BUTTON 2
#define RESET_BUTTON 3
#define EDIT_BUTTON 4
#define TEXT_EDIT 5

#define WIDTH 400
#define HEIGHT 200

#define CLASS_NAME L"timer2h"

// compiled using g++ E:\timer\windows_timer\timer.cpp -o E:\timer\windows_timer\timer.exe -luser32 -lgdi32 -lkernel32 -lwinmm -std=c++17 -g -D_UNICODE -DUNICODE ; E:\timer\windows_timer\timer.exe
// learn CMAKE
// #include <cstring.h>

int i = 5; // set timer
int original = i;
bool is_paused = true;
HWND hwndEditText; // temp
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
                               WIDTH,
                               HEIGHT,
                               NULL,
                               NULL,
                               hInstance,
                               NULL);

    HWND hwndPauseButton = CreateWindow(
        L"BUTTON",                                             // Predefined class; Unicode assumed
        L"resume",                                              // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        10,                                                    // x position  FRONT END SUX
        HEIGHT - 85,                                           // y position
        70,                                                    // Button width
        50,                                                    // Button height
        hwnd,                                                  // Parent window
        (HMENU)PAUSE_BUTTON,                                   // pass the ID
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.

    HWND hwndResetButton = CreateWindow(
        L"BUTTON",                                             // Predefined class; Unicode assumed
        L"RESET",                                              // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        80,                                                    // x position  FRONT END SUX
        HEIGHT - 85,                                           // y position
        50,                                                    // Button width
        50,                                                    // Button height
        hwnd,                                                  // Parent window
        (HMENU)RESET_BUTTON,                                   // pass the ID
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.

    hwndEditText = CreateWindow(
        L"EDIT",                                       // Predefined class; Unicode assumed
        L"",                                           // Button text
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, // Styles
        WIDTH / 2 - 40,                                           // x position  FRONT END SUX
        HEIGHT - 85 ,                                   // y position
        80,                                            // Button width
        20,                                            // Button height
        hwnd,                                          // Parent window
        (HMENU)TEXT_EDIT,                              // pass the ID
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.

    HWND hwndEdit = CreateWindow(
        L"BUTTON",                                      // Predefined class; Unicode assumed
        L"SET",                                         // Button text
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, // Styles
        WIDTH / 2 + 40,                                            // x position  FRONT END SUX
        HEIGHT - 85,                                    // y position
        40,                                             // Button width
        20,                                             // Button height
        hwnd,                                           // Parent window
        (HMENU)EDIT_BUTTON,                             // pass the ID
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.

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
    case WM_COMMAND:
        if (wParam == PAUSE_BUTTON)
        {
            is_paused = !is_paused;
            HWND hpause = GetDlgItem(hwnd, PAUSE_BUTTON);
            SetWindowText(hpause, is_paused ? L"resume" : L"pause");
        }
        if (wParam == RESET_BUTTON)
        {
            // if( )
            KillTimer(hwnd, TIMER_1);
            SetTimer(hwnd,             // handle to main window
                     TIMER_1,          // timer identifier
                     1000,             // 10-second interval
                     (TIMERPROC)NULL); // no timer callback
            i = original;
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            // is_paused = !is_paused;
        }
        if (wParam == EDIT_BUTTON)
        {
            KillTimer(hwnd, TIMER_1);
            SetTimer(hwnd,             // handle to main window
                     TIMER_1,          // timer identifier
                     1000,             // 10-second interval
                     (TIMERPROC)NULL); // no timer callback
            TCHAR buff[100];
            GetWindowText(hwndEditText, buff, sizeof(buff) / sizeof(TCHAR));
            i = _wtoi(buff);
            original = i;
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
        }
        break;

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
        if (wParam == TIMER_1 && is_paused == false) // if wparam was called by TIMER_1 | if i == 0 make sound hehe
        {
            if (i > 0)
            {
                // int temp  = i ;
                if (i == original / 2)
                {
                    PlaySoundW(L"E:\\timer\\windows_timer\\half.wav", NULL, SND_ALIAS | SND_SYNC);
                }
                i--;
            }
            else
            {
                // PlaySoundW(L"?", NULL, SND_ALIAS | SND_SYNC); ///// eeeeehhh ! lmohim anchof chi cool sound
                PlaySoundW(L"E:\\timer\\windows_timer\\end.wav", NULL, SND_ALIAS | SND_SYNC);
                printf("end of the timer\n"); // andir  sound hnayaaa
                KillTimer(hwnd, TIMER_1);
            }
            int err = InvalidateRect(hwnd, NULL, TRUE);
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
        ZeroMemory(&lf, sizeof(LOGFONT)); // memset(&lf,0,sizeof(LOGFONT));

        RECT clientRect;
        GetClientRect(hwnd, &clientRect); // Get client area dimensions
        int desiredFontHeight = clientRect.bottom * 45 / 100;
        lf.lfHeight = desiredFontHeight;
        lf.lfWeight = FW_EXTRABOLD;                     // bold italic o l3aybat
        wcscpy_s(lf.lfFaceName, LF_FACESIZE, L"Arial"); // n9DR NSMIH BO3O

        HFONT hfont = CreateFontIndirect(&lf);
        HFONT hOldFont = (HFONT)SelectObject(hdc, hfont);

        long long hours = i / 3600; // 2
        long long minutes = (i % 3600) / 60;
        long long seconds = i % 60;
        std::wstringstream sstream;

        sstream << std::setw(2) << std::setfill(L'0') << hours << L":"
                << std::setw(2) << std::setfill(L'0') << minutes << L":"
                << std::setw(2) << std::setfill(L'0') << seconds;
        std::wstring todraw = sstream.str();

        DrawText(hdc, todraw.c_str(), -1, &clientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        SelectObject(hdc, hOldFont); // kanrj3 font l9dim hehe bach maykbtch lia kolchi b same BIG FONT

        DeleteObject(hfont); // li creatit

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
