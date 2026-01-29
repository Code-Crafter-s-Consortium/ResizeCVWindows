#include <windows.h>

// Print out a window with a bunch of details for debugging.
void print_window_details(HWND hwnd)
{
    char className[32], windowTitle[32];
    GetClassName(hwnd, className, sizeof(className));
    GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
    RECT r;
    GetWindowRect(hwnd, &r);
    printf("Hwnd: %ld, Window Name: %s, Window Class: %s; X = %ld, Y = %ld, W = %ld, H = %ld\n\n",
        (long int)hwnd, windowTitle, className, r.left, r.top, (r.right - r.left), (r.bottom - r.top));
}

// Print out a tree view of a window and it's children.
BOOL CALLBACK print_window(HWND hwnd, LPARAM lparam)
{
    int indent = (int)lparam;
    if (indent == 0)
        printf("\n\n\n\n\n\n\n");
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
    printf("%s", pad);

    print_window_details(hwnd);

    EnumChildWindows(hwnd, print_window, indent + 1);

    return true;
}

