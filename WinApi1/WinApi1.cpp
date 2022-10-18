// WinApi1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WinApi1.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND pressMeButton;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                CreateAndShowWindow(HINSTANCE, int);
LRESULT CALLBACK    WindowProcedure(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID                PaintSquare(HDC, int, int);
HWND                CreateSquareButton(HMENU, HWND, LPWSTR, int, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDR_MENU, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!CreateAndShowWindow(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MENU));
    MSG msg;

    // Main message loop. Runs until GetMessage returns 0
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg); // translate vkey messages (numbers) into character messages
            DispatchMessage(&msg); // send message to WindowProcedure
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProcedure;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(190, 210, 190)); // background color.
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL CreateAndShowWindow(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowEx(
        0,
        szWindowClass, // main window class name
        szTitle,
        WS_OVERLAPPEDWINDOW, // default window
        CW_USEDEFAULT, // default x
        CW_USEDEFAULT, // default y
        CW_USEDEFAULT, // default width
        CW_USEDEFAULT, // default height
        HWND_DESKTOP, // parent window: desktop
        nullptr, // no menu // LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU))
        hInstance, // program instance handle
        nullptr // no window creation data
    );

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WindowProcedure(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const int randomNumber = rand() % 300;
    const int randomNumberTwo = rand() % 300;

    switch (message)
    {
    case WM_CREATE: 
    {
        pressMeButton = CreateSquareButton(
            (HMENU)ID_PRESS_ME_BUTTON, hWnd, 
            (LPWSTR)L"PRESS ME", 
            10 + randomNumber, 
            10 + randomNumberTwo
        );
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT: // Defined in WinApi.rc and Resource.h
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd); // Sends WM_DESTROY
            break;
        case ID_PRESS_ME_BUTTON:
            DestroyWindow(pressMeButton);
            UpdateWindow(hWnd); // forces an immediate redraw for all currently accumulated invalidated areas

            pressMeButton = CreateSquareButton(
                (HMENU)ID_PRESS_ME_BUTTON, 
                hWnd, (LPWSTR)L"PRESS ME",
                10 + randomNumber, 
                10 + randomNumberTwo
            );

            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        break;
    }
    case WM_PAINT: // Allows to change how window is drawn
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        PaintSquare(hdc, 10 + randomNumber, 10 + randomNumberTwo);
        EndPaint(hWnd, &ps);

        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

VOID PaintSquare(HDC hdc, int positionX, int positionY) 
{
    RECT rectangle;
    rectangle.top = positionY;
    rectangle.right = positionX;
    rectangle.bottom = 100 + positionY;
    rectangle.left = 100 + positionX;
    FillRect(hdc, &rectangle, CreateSolidBrush(RGB(100, 10, 500)));
}

HWND CreateSquareButton(HMENU buttonId, HWND hWnd, LPWSTR text, int positionX, int positionY) {
    return CreateWindow(
        TEXT("BUTTON"),  // Predefined class; Unicode assumed 
        TEXT("Press me"),      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        positionX,         // x position 
        positionY,         // y position 
        100,        // Button width
        100,        // Button height
        hWnd,       // Parent window
        (HMENU)buttonId,       // Optional (can pass NULL). ID of button
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), // Optional (can pass NULL). Gets info about the hWnd window and GWLP_HINSTANCE app instance
        NULL
    );
}
