#include "hook.h"
#include <stdio.h>

static LRESULT CALLBACK lowLevelKeyboardProc(int, WPARAM, LPARAM);

// Max number of hotkeys
#define MAX_HOTKEYS 5

// Max time interval between first and second part of hotkey (ms)
#define MAX_KEYS_TIME_INTERVAL 500

static HINSTANCE hInstance;
static HWND hGDWindow;
static HHOOK hKbdHook;

__declspec (dllexport) void removeHook()
{
    if( hKbdHook )
    {
        UnhookWindowsHookEx( hKbdHook );
        hKbdHook = 0;
    }
}

__declspec (dllexport) BOOL setHook( HWND hwnd )
{
    hGDWindow = hwnd;
    removeHook();
    hKbdHook = SetWindowsHookEx( WH_KEYBOARD_LL, lowLevelKeyboardProc, hInstance, 0 );
    return hKbdHook != 0;
}

__declspec (dllexport) void clearHotkeys()
{
        ;
}

static BOOL isModifiersPressed( DWORD modifiers )
{
    int n = GetAsyncKeyState( VK_MENU ) & 0x8000;
    if( ( ( modifiers & MOD_ALT ) && n == 0 )
            || ( ( modifiers & MOD_ALT ) == 0 && n ) )
        return FALSE;

    n = GetAsyncKeyState( VK_SHIFT ) & 0x8000;
    if( ( ( modifiers & MOD_SHIFT ) && n == 0 )
            || ( ( modifiers & MOD_SHIFT ) == 0 && n ) )
        return FALSE;

    n = GetAsyncKeyState( VK_CONTROL ) & 0x8000;
    if( ( ( modifiers & MOD_CONTROL ) && n == 0 )
            || ( ( modifiers & MOD_CONTROL ) == 0 && n ) )
        return FALSE;

    n = ( GetAsyncKeyState( VK_LWIN ) & 0x8000 ) | ( GetAsyncKeyState( VK_RWIN ) & 0x8000 );
    if( ( ( modifiers & MOD_WIN ) && n == 0 )
            || ( ( modifiers & MOD_WIN ) == 0 && n ) )
        return FALSE;

    return TRUE;
}

static LRESULT CALLBACK lowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{

}

BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ )
{
    (void) reserved;
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        hInstance = hInst;
        break;

    case DLL_PROCESS_DETACH:
        removeHook();
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;
    }

    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}
