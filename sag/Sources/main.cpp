#include "hook.h"
#include <stdio.h>
#include <QString>
#include <iostream>
#include <QFileInfo>
#include "mm_win32.h"
#include "mm_api.h"

static HHOOK hook_win;
HMODULE dll_handle = NULL;

void showMessage(const WCHAR *txt);
DWORD mainThread(HMODULE mod_h);
MmApplication sag_getApp(QString shortcut_name);

LRESULT CALLBACK CallWndProcHook(int nCode, WPARAM wParam,
                                 LPARAM lParam)
{
    printf("kuta\n");
    CWPSTRUCT* cwpStruct = reinterpret_cast<CWPSTRUCT*>(lParam);
    if( nCode>=0 )
    {
        // Process the window messages here
        HWND hWnd = cwpStruct->hwnd;
        printf("Window is being closed. Handle: %x | %x\n",
               hWnd, cwpStruct->message);
    }
    // Call the next hook in the chain
    return CallNextHookEx(hook_win, nCode, wParam, lParam);
}

BOOL APIENTRY DllMain (HINSTANCE hndl, DWORD reason,
                       LPVOID reserved)
{
    (void) reserved; // to avoid not used warning

    if( reason==DLL_PROCESS_ATTACH )
    {
        dll_handle = hndl;
        LPTHREAD_START_ROUTINE main_th =
                (LPTHREAD_START_ROUTINE)mainThread;
        CreateThread(NULL, 0, main_th, hndl, 0, NULL);
    }
    else if( reason==DLL_PROCESS_DETACH )
    {
//        MessageBox(0, L"Detach Lib!", L"Hello", MB_ICONINFORMATION);
        ;// Run deconstructor here
    }
    else if( reason==DLL_THREAD_ATTACH )
    {
//        MessageBox(0, L"Attach !", L"Hello", MB_ICONINFORMATION);
    }
    else if( reason==DLL_THREAD_DETACH )
    {
//        MessageBox(0, L"Detach Thread!", L"Hello", MB_ICONINFORMATION);
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
    AllocConsole();
    FILE *f = new FILE();
    freopen_s(&f, "CONOUT$", "w", stdout);


    // Set the hook
//    HWND  hwnd = GetForegroundWindow();
    MmApplication app = sag_getApp("Altium");
    DWORD tid  = GetWindowThreadProcessId(app.hwnd, NULL);
//    DWORD tid  = 14880;

    if( tid )
    {
        // Set the hook
        hook_win = SetWindowsHookExA(WH_GETMESSAGE, CallWndProcHook,
                                     mod_h, tid);

        if( hook_win==NULL )
        {
            printf("Failed to set the hook. Error code: %d\n",
                   GetLastError());
            return NULL;
        }
        printf("SetWindowHookEx were successful\n");
    }
    printf("mod_h:%x tid: %d hook:%x\n", mod_h, tid,
                                         hook_win);

    int loop_active = 1;
    while( loop_active )
    {
        if( GetAsyncKeyState(VK_DELETE) & 0x80000 )
        {
            loop_active = 0;
            break;
        }

        //sleep in ms
        Sleep(20);
    }

    printf("Free and Exit\n");
    FreeLibraryAndExitThread(dll_handle, true);

    return 0;
}

void showMessage(const WCHAR *txt)
{
    MessageBox(0, txt, L"DLL",
               MB_ICONINFORMATION);
}

MmApplication sag_getApp(QString shortcut_name)
{
    MmApplication app;
    app.shortcut_name = shortcut_name;
    shortcut_name += ".lnk";
    mm_getLinkPath(shortcut_name, &app);
    QFileInfo fi(app.exe_path);
    app.exe_name = fi.completeBaseName();
    app.hwnd = mm_getHWND(&app);
    return app;
}
