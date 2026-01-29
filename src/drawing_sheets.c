#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * Main function to resize the drawing sheets window.
 * Takes the handle as an argument, and also the event to track creation and deletion.
 */
void resize_drawing_sheets_window(HWND hwnd, DWORD event);

/**
 * Callback function that moves and resizes the controls within the window.
 * Iterates over the list box and buttons moving and resizing.
 */
BOOL CALLBACK move_drawing_sheets_controls(HWND hwnd, LPARAM lParam);

/**
 * Function returns the Y position for each button based on it's name.
 */
int get_drawing_sheets_button_positions(const char* title);


void resize_drawing_sheets_window(HWND hwnd, DWORD event)
{
    // The isMainOpened and isDelWindowOpened variables are used to keep track of which windows are currently opened.
    // Unfortunately, the remove sheets button pos up a confirmation window that is also named 'Drawing Sheets'. 
    // I use these variables to make sure I only resize the main window and not the confirmation window.
    static bool isMainOpened = false, isDelWindowOpened = false;
    if (event == EVENT_OBJECT_CREATE)
    {
        if (!isMainOpened)
        {
            SetWindowPos(hwnd, NULL,
                (GetSystemMetrics(SM_CXSCREEN) - 600) / 2,
                (GetSystemMetrics(SM_CYSCREEN) - 700) / 2,
                600, 700,
                SWP_FRAMECHANGED | SWP_SHOWWINDOW);
            EnumChildWindows(hwnd, move_drawing_sheets_controls, 0); // Move controls within the window
            isMainOpened = true;
        }
        else if (!isDelWindowOpened)
        {
            isDelWindowOpened = true;
        }
    }
    else if (event == EVENT_OBJECT_DESTROY)
    {
        isDelWindowOpened ? isDelWindowOpened = false : (isMainOpened ? isMainOpened = false : 0);
    }
}

BOOL CALLBACK move_drawing_sheets_controls(HWND hwnd, LPARAM lParam)
{
    char className[32];
    GetClassNameA(hwnd, className, sizeof(className));
    if (strcmp(className, "Button") == 0)
    {
        char windowTitle[32];
        GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
        printf("Button found: %s\n", windowTitle);
        SetWindowPos(hwnd, NULL, 460, get_drawing_sheets_button_positions(windowTitle), 120, 40, SWP_SHOWWINDOW);
    }
    if (strcmp(className, "ListBox") == 0)
    {
        SetWindowPos(hwnd, NULL, 5, 5, 450, 650, SWP_SHOWWINDOW);
    }
    return TRUE;
}

int get_drawing_sheets_button_positions(const char* title)
{
    if (strcmp(title, "&Open") == 0)
        return 5;
    else if (strcmp(title, "&New") == 0)
        return 55;
    else if (strcmp(title, "Rena&me") == 0)
        return 105;
    else if (strcmp(title, "&Remove") == 0)
        return 155;
    else if (strcmp(title, "Move &Up") == 0)
        return 235;
    else if (strcmp(title, "Move &Dn") == 0)
        return 285;
    else if (strcmp(title, "Close") == 0)
        return 614;
}

