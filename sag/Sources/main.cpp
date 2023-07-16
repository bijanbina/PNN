#include "hook.h"
#include <stdio.h>
#include <QString>
#include <iostream>
#include <QFileInfo>
#include <QTextStream>
#include "mm_win32.h"
#include "mm_api.h"

static HHOOK hook_msg;
static HHOOK hook_win;
HMODULE dll_handle = NULL;
QTextStream out;

void  setupHook(HMODULE dll_h);
void  showMessage(const WCHAR *txt);
DWORD mainThread(HMODULE mod_h);
MmApplication sag_getApp(QString shortcut_name);

LRESULT CALLBACK msgCallback(int nCode, WPARAM wParam,
                                 LPARAM lParam)
{
    printf("kuta\n");
    out << "My Text\n";
    CWPSTRUCT* cwpStruct = reinterpret_cast<CWPSTRUCT*>(lParam);
    if( nCode>=0 )
    {
        // Process the window messages here
        HWND hWnd = cwpStruct->hwnd;
        printf("Window is being closed. Handle: %x | %x\n",
               hWnd, cwpStruct->message);
    }
    // Call the next hook in the chain
    return CallNextHookEx(hook_msg, nCode, wParam, lParam);
}

LRESULT CALLBACK winCallback(int nCode, WPARAM wParam,
                                 LPARAM lParam)
{
    printf("kuta\n");
    out << "My Text\n";
    // Call the next hook in the chain
    return CallNextHookEx(hook_win, nCode, wParam, lParam);
}

BOOL APIENTRY DllMain(HINSTANCE hndl, DWORD reason,
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

DWORD mainThread(HMODULE dll_h)
{
    AllocConsole();
    QFile log_f("C:/Home/Projects/test.log");
    int s = log_f.open(QIODevice::WriteOnly |
                       QIODevice::Text);
    printf("file = %d\n", s);
    out.setDevice(&log_f);
    FILE *f = new FILE();
    freopen_s(&f, "CONOUT$", "w", stdout);

    setupHook(dll_h);
//    int loop_active = 1;
//    while( loop_active )
//    {
//        if( GetAsyncKeyState(VK_DELETE) & 0x80000 )
//        {
//            loop_active = 0;
//            break;
//        }
//        if( GetAsyncKeyState(VK_F7) & 0x80000 )
//        {
//            setupHook(dll_h);
//        }

//        //sleep in ms
//        Sleep(20);
//    }


    MSG msg;
    while( GetMessage(&msg, nullptr, 0, 0) )
    {
        DispatchMessage(&msg);
    }

    printf("Free and Exit\n");

    log_f.close();
    FreeLibraryAndExitThread(dll_handle, true);
    return 0;
}

void setupHook(HMODULE dll_h)
{
//    HWND  hwnd = GetForegroundWindow();
    MmApplication app = sag_getApp("notepad");
    DWORD tid  = GetWindowThreadProcessId(app.hwnd, NULL);
    printf(">>>> %d | 0x%x | %s\n", app.pid,
           app.hwnd, app.pname.toStdString().c_str());
//    DWORD tid  = 14880;

    if( tid==0 )
    {
        return;
    }
    hook_msg = SetWindowsHookExA(WH_GETMESSAGE, msgCallback,
                                 dll_h, tid);
    if( hook_msg==NULL )
    {
        printf("Failed to set the Msg hook. Error code: %d\n",
               GetLastError());
        return;
    }

    hook_win = SetWindowsHookEx(WH_CALLWNDPROC, winCallback,
                                dll_h, tid);
    if( hook_win==NULL )
    {
        printf("Failed to set the Win hook. Error code: %d\n",
               GetLastError());
        return;
    }
    printf("All hook were successful\n");
    printf("mod_h:%x tid: %d hook:%x\n", dll_h, tid,
                                         hook_msg);
}

void showMessage(const WCHAR *txt)
{
    MessageBox(0, txt, L"DLL",
               MB_ICONINFORMATION);
}

MmApplication sag_getApp(QString app_name)
{
    MmApplication app;
    app.exe_name = app_name;
    app.hwnd = mm_getHWND(&app);
    return app;
}
