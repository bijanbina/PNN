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

BOOL APIENTRY DllMain (HINSTANCE hndl, DWORD reason,
                       LPVOID reserved)
{
    (void) reserved; // to avoid not used warning
    if( reason==DLL_PROCESS_ATTACH )
    {
        hInstance = hndl;
        MessageBox(0, L"Hello From testlib!", L"Hello", MB_ICONINFORMATION);
    }
    else if( reason==DLL_PROCESS_DETACH )
    {
        ;// Try to constructor here
    }
    else if( reason==DLL_THREAD_ATTACH )
    {
        ;
    }
    else if( reason==DLL_THREAD_ATTACH )
    {
        ;
    }
    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}
