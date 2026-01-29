#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * Main function to resize the tool selection window.
 * Takes the handle as an argument, and also the event to track creation and deletion.
 */
void resize_tool_selection_window(HWND hwnd, DWORD event);

void resize_tool_selection_sub_window(HWND hwnd, DWORD event);

/**
 * Callback function that moves and resizes the controls within the window.
 */
static BOOL CALLBACK enum_tool_selection_controls(HWND hwnd, LPARAM lParam);

static BOOL CALLBACK enum_tab_controls(HWND hwnd, LPARAM lParam);


void resize_tool_selection_window(HWND hwnd, DWORD event)
{
    SetWindowPos(hwnd, HWND_BOTTOM,
        (GetSystemMetrics(SM_CXSCREEN) - 700) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - 600) / 2,
        700, 600,
        SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    EnumChildWindows(hwnd, enum_tool_selection_controls, 0); // Move controls within the window
}

void resize_tool_selection_sub_window(HWND hwnd, DWORD event)
{
    SetWindowPos(hwnd, HWND_BOTTOM,
        10, 10,
        680, 480,
        SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    EnumChildWindows(hwnd, enum_tool_selection_controls, 0); // Move controls within the window
}

static BOOL CALLBACK enum_tool_selection_controls(HWND hwnd, LPARAM lParam)
{
    char className[32], windowTitle[32];
    GetClassName(hwnd, className, sizeof(className));
    GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
    printf("Enumerating... %s - %s\n", className, windowTitle);

    if (strcmp(windowTitle, "Tool") == 0)
    {
        SetWindowPos(hwnd, HWND_BOTTOM,
        20, 30,
        600, 470,
        SWP_FRAMECHANGED | SWP_SHOWWINDOW);
        EnumChildWindows(hwnd, enum_tool_selection_controls, 0); // Move controls within the window
    }
    else if (strcmp(className, "SysTabControl32") == 0)
    {
        SetWindowPos(hwnd, HWND_BOTTOM, 10, 10, 0, 0, SWP_SHOWWINDOW);
    }
    else if (strcmp(className, "Static") == 0)
    {
        if (strcmp(windowTitle, "Tool Profile") == 0)
        {
            SetWindowPos(hwnd, HWND_TOP, 300, 20, 100, 14, SWP_SHOWWINDOW);
        }
        else if (strcmp(windowTitle, "Tools") == 0)
        {
            SetWindowPos(hwnd, HWND_TOP, 20, 20, 100, 14, SWP_SHOWWINDOW);
        }
    }
    else if (strcmp(className, "AfxOleControl140u") == 0)
    {
        // some random text
        if (!strcmp(windowTitle, "v 2023.0"))
        {
            SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW);
            return TRUE;
        }

        HWND child = FindWindowEx(hwnd, NULL, "ListBox", NULL);
        if (child)
        {
            SetWindowPos(hwnd, HWND_TOP, 20, 40, 300, 400, SWP_SHOWWINDOW);
            SetWindowPos(child, HWND_TOP, 0, 0, 250, 400, SWP_SHOWWINDOW);
            SetWindowRgn(hwnd, NULL, TRUE);
            return TRUE;
        }

        SetWindowPos(hwnd, HWND_TOP, 300, 40, 300, 400, SWP_SHOWWINDOW);
        return TRUE;
    }
    // else if (!strcmp("ListBox", className))
    // {
    //     SetWindowPos(hwnd, HWND_TOP, 0, 0, 250, 400, SWP_SHOWWINDOW);
    // }
    else if (strcmp(className, "Button") == 0)
    {
        int x;
        if (strcmp(windowTitle, "OK") == 0)
            x = 580;
        else if (strcmp(windowTitle, "Cancel") == 0)
            x = 450;
        else
            x = 20;
        SetWindowPos(hwnd, HWND_TOP, x, 520, 100, 30, SWP_SHOWWINDOW);
    }
    return TRUE;
}

static BOOL CALLBACK enum_tab_controls(HWND hwnd, LPARAM lParam)
{
    char className[32], windowTitle[32];;
    GetClassName(hwnd, className, sizeof(className));
    GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
    //printf("Enumerating... %s - %s\n", className, windowTitle);
    // if (strcmp(className, "SysTabControl32") == 0)
    // {
    //     SetWindowPos(hwnd, NULL, 10, 10, 570, 500, SWP_SHOWWINDOW);
    //     EnumChildWindows(hwnd, enum_tool_selection_controls, 0);
    // }
    return TRUE;
}
