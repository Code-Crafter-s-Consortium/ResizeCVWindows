#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#include <stdbool.h>
#include "drawing_sheets.c"
#include "tool_selection.c"
#include "..\resources\resource.h"

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_APP_ICON 1001
#define ID_TRAY_EXIT 1002

static NOTIFYICONDATA nid;
static HWND mainWindow;

void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
LRESULT CALLBACK WinWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void initialize_main_window(HINSTANCE instance);
static void initialize_tray(HINSTANCE instance);
BOOL CALLBACK print_window(HWND hwnd, LPARAM lparam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    initialize_main_window(hInstance);
    initialize_tray(hInstance);

    // Create a hook to listen for window creation and destruction events.
    HWINEVENTHOOK hook = SetWinEventHook(EVENT_MIN, EVENT_MAX, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    if (hook == NULL)
        return 1;

    // Message loop handles all application messages.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWinEvent(hook);
    return 0;
}

void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
    if (event != EVENT_OBJECT_CREATE && event != EVENT_OBJECT_DESTROY && event != EVENT_OBJECT_NAMECHANGE)
        return;
    if (idObject != OBJID_WINDOW)
        return;
    char windowTitle[32], windowClass[32];
    GetClassName(hwnd, windowClass, sizeof(windowClass));
    GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
    if (!strcmp(windowTitle, "Drawing Sheets"))
        resize_drawing_sheets_window(hwnd, event);
    else if (!strcmp(windowTitle, "Select Tools"))
        resize_tool_selection_window(hwnd);
    else if (!strcmp(windowTitle, "Tool"))
        resize_tool_selection_sub_window(hwnd);

    //print_window_details(hwnd);
}

LRESULT CALLBACK WinWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_TRAYICON:
        if (lParam == WM_RBUTTONUP)
        {
            HMENU hMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, TEXT("Exit"));
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);
            TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
        }
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_TRAY_EXIT)
        {
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
        }
        break;
    case WM_DESTROY:
        Shell_NotifyIcon(NIM_DELETE, &nid);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

static void initialize_main_window(HINSTANCE instance)
{
    // Register the window class.
    // We have to create a window class so we can process messgaes with our tray icon.
    const char* className = "ResizeCVWindowsClass";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WinWindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = className;
    RegisterClass(&wc);
    mainWindow = CreateWindowEx(WS_EX_TOOLWINDOW, wc.lpszClassName, TEXT("Resize CV Windows"), 
        WS_OVERLAPPEDWINDOW, 800, 600, 300, 200, NULL, NULL, instance, NULL);
}

// Initialize the tray.
static void initialize_tray(HINSTANCE instance)
{
    // Create and show the system tray icon.
    nid.hIcon = LoadIcon(instance, MAKEINTRESOURCE(SMALL_ICON));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = mainWindow;
    nid.uID = ID_TRAY_APP_ICON;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    lstrcpy(nid.szTip, TEXT("Resize CV Windows"));
    Shell_NotifyIcon(NIM_ADD, &nid);
}
