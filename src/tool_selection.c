#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "misc.c"

void resize_tool_selection_window(HWND hwnd);
void resize_tool_selection_sub_window(HWND hwnd);
static BOOL CALLBACK enum_tool_selection_controls(HWND hwnd, LPARAM lParam);
int get_button_positions(const char* name);


void resize_tool_selection_window(HWND hwnd)
{
    SetWindowPos(hwnd, HWND_BOTTOM,
        (GetSystemMetrics(SM_CXSCREEN) - 700) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - 600) / 2,
        700, 600,
        SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    EnumChildWindows(hwnd, enum_tool_selection_controls, 0); // Move controls within the window
}

void resize_tool_selection_sub_window(HWND hwnd)
{
    SetWindowPos(hwnd, HWND_BOTTOM, 20, 20, 600, 470, SWP_SHOWWINDOW);
    EnumChildWindows(hwnd, enum_tool_selection_controls, 0); // Move controls within the window
}

static BOOL CALLBACK enum_tool_selection_controls(HWND hwnd, LPARAM lParam)
{
    char className[32], windowTitle[32];
    GetClassName(hwnd, className, sizeof(className));
    GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
    printf("Enumerating... %s - %s\n", className, windowTitle);

    if (!strcmp(className, "SysTabControl32"))
    {
        SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_SHOWWINDOW);
        return TRUE;
    }
    else if (!strcmp(className, "Static"))
    {
        if (!strcmp(windowTitle, "Tool Profile"))
        {
            SetWindowPos(hwnd, HWND_TOP, 300, 20, 100, 14, SWP_SHOWWINDOW);
            return TRUE;
        }
        else if (!strcmp(windowTitle, "Tools"))
        {
            SetWindowPos(hwnd, HWND_TOP, 20, 20, 100, 14, SWP_SHOWWINDOW);
            return TRUE;
        }
    }
    else if (!strcmp(className, "AfxOleControl140u"))
    {
        // some random text
        if (!strcmp(windowTitle, "v 2023.0")
            || !strcmp(windowTitle, "v 2024.0")
            || !strcmp(windowTitle, "v 2025.0")
            || !strcmp(windowTitle, "v 2026.0"))
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

        // must be the tool preview
        SetWindowPos(hwnd, HWND_TOP, 300, 40, 300, 400, SWP_SHOWWINDOW);
        return TRUE;
    }
    else if (!strcmp(className, "Button"))
    {
        SetWindowPos(hwnd, HWND_TOP, get_button_positions(windowTitle), 520, 100, 30, SWP_SHOWWINDOW);
        return TRUE;
    }
    // else if (!strcmp(className, "Button"))
    // {
    //     static int buttonIndex = 0;
    //     SetWindowPos(hwnd, HWND_TOP, 560 - (buttonIndex * 120), 520, 100, 30, SWP_SHOWWINDOW);
    //     buttonIndex += buttonIndex < 3 ? 1 : -3;
    //     return TRUE;
    // }
    return TRUE;
}

int get_button_positions(const char* name)
{
    if (strcmp(name, "OK") == 0)
        return 560;
    else if (strcmp(name, "Cancel") == 0)
        return 440;
    else if (strcmp(name, "Help") == 0)
        return 140;
    else if (strcmp(name, "&Apply") == 0)
        return 20;
}