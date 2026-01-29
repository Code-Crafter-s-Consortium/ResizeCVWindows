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

/**
 * Function handles the creation and destruction of windows. 
 * Registered with a windows event hook.
 */
void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

/**
 * This function handles the messgaes for the tray icon.
 */
LRESULT CALLBACK WinWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/**
 * Creates the main window instance. The window is not visible, but is neccessary for the messaging process.
 */
void initialize_main_window(HINSTANCE instance);

/**
 * Creates an icon in the tray.
 */
void initialize_tray(HINSTANCE instance);

/**
 * Print out a window tree for debugging.
 */
BOOL CALLBACK print_window(HWND hwnd, LPARAM lparam);

void print_window_details(HWND hwnd);


/**
 * Main entry point for the application.
 */
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
    if (strcmp(windowTitle, "Drawing Sheets") == 0)
        resize_drawing_sheets_window(hwnd, event);
    else if (!strcmp(windowTitle, "Select Tools"))
        resize_tool_selection_window(hwnd, event);
    else if (!strcmp(windowTitle, "Tool"))
        print_window(hwnd, 0);

    //printf("Window Title: %s; Window Class: %s\n", windowTitle, windowClass);
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

void initialize_main_window(HINSTANCE instance)
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

void initialize_tray(HINSTANCE instance)
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

BOOL CALLBACK print_window(HWND hwnd, LPARAM lparam)
{
    int indent = (int)lparam;
    if (indent == 0)
        printf("\n\n\n\n\n\n\n");

    char className[32], windowTitle[32];
    GetClassName(hwnd, className, sizeof(className));
    GetWindowText(hwnd, windowTitle, sizeof(windowTitle));

    // build padding
    char pad[64];
    int i = 0;
    while (i < indent * 4)
    {
        pad[i++] = ' ';
        pad[i++] = ' ';
        pad[i++] = ' ';
        pad[i++] = ' ';
    }
    pad[i] = 0; // null terminate

    RECT r;
    GetWindowRect(hwnd, &r);

    printf("%sHwnd: %ld, Window Name: %s, Window Class: %s; X = %ld, Y = %ld, W = %ld, H = %ld\n\n"
        , pad, hwnd, windowTitle, className, r.left, r.top, (r.right - r.left), (r.bottom - r.top));

    EnumChildWindows(hwnd, print_window, indent + 1);

    return true;
}


void print_window_details(HWND hwnd)
{
    char className[32], windowTitle[32];
    GetClassName(hwnd, className, sizeof(className));
    GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
    RECT r;
    GetWindowRect(hwnd, &r);
    printf("Hwnd: %ld, Window Name: %s, Window Class: %s; X = %ld, Y = %ld, W = %ld, H = %ld\n\n"
        , hwnd, windowTitle, className, r.left, r.top, (r.right - r.left), (r.bottom - r.top));
}