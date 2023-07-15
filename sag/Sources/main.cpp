#include "hook.h"
#include <stdio.h>
#include <QString>
#include <iostream>

static LRESULT CALLBACK lowLevelKeyboardProc(int, WPARAM, LPARAM);

// Max number of hotkeys
#define MAX_HOTKEYS 5

// Max time interval between first and second part of hotkey (ms)
#define MAX_KEYS_TIME_INTERVAL 500

static HINSTANCE hInstance;
static HWND hGDWindow;
static HHOOK hKbdHook;
HMODULE handle;

void showMessage(const WCHAR *txt);
DWORD mainThread(HMODULE mod_h);

BOOL APIENTRY DllMain (HINSTANCE hndl, DWORD reason,
                       LPVOID reserved)
{
    (void) reserved; // to avoid not used warning

    if( reason==DLL_PROCESS_ATTACH )
    {
        hInstance = hndl;
//        MessageBox(0, L"Hell!", L"Hello", MB_ICONINFORMATION);
        showMessage(L"KOKKO");
        LPTHREAD_START_ROUTINE main_th =
                (LPTHREAD_START_ROUTINE)mainThread;
        HANDLE th = CreateThread(NULL, 0, main_th, hndl, 0, NULL);
//        CloseHandle(th);
    }
    else if( reason==DLL_PROCESS_DETACH )
    {
        MessageBox(0, L"Detach Lib!", L"Hello", MB_ICONINFORMATION);
        ;// Run deconstructor here
    }
    else if( reason==DLL_THREAD_ATTACH )
    {
        MessageBox(0, L"Attach !", L"Hello", MB_ICONINFORMATION);
    }
    else if( reason==DLL_THREAD_DETACH )
    {
        MessageBox(0, L"Detach Thread!", L"Hello", MB_ICONINFORMATION);
        handle = GetModuleHandle(L"Sag.dll");
        FreeLibrary(handle);
    }
    else
    {
        MessageBox(0, L"Fuck!", L"Hello", MB_ICONINFORMATION);
    }
    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}

DWORD mainThread(HMODULE mod_h)
{
    (void) mod_h;
//    AllocConsole();
//    FILE *f = new FILE();
//    freopen_s(&f, "CONOUT$", "w", stdout);

//    std::cout << "sag tu in zendegi" << std::endl;

//    std::cout << "sag tu in zendegi" << handle << std::endl;
    return 0;
}

void showMessage(const WCHAR *txt)
{
    MessageBox(0, txt, L"DLL",
               MB_ICONINFORMATION);
}
