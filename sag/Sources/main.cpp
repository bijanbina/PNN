#include "hook.h"

static HHOOK hook_msg;
HMODULE dll_handle = NULL;
int msg_counter = 0;

#define CALL_NEXT CallNextHookEx(hook_msg, nCode, wParam, lParam)

LRESULT CALLBACK msgCallback(int nCode, WPARAM wParam,
                                 LPARAM lParam)
{
    PMSG pMsg = (PMSG)lParam;
    char buffer[250] = {0};
    if( pMsg->message==WM_MOUSEMOVE ||
        pMsg->message==WM_SYSTIMER ||
        pMsg->message==WM_TIMER ||
        pMsg->message==WM_SCRN_DRAW ||
        pMsg->message==WM_QUIT ||
        pMsg->message==WM_NCMOUSEMOVE ||
        pMsg->message==WM_NCLBUTTONDOWN ||
        pMsg->message==WM_KEYDOWN ||
        pMsg->message==WM_KEYUP ||
        pMsg->message==WM_MOUSELEFT ||
        pMsg->message==WM_LBUTTONUP ||
        pMsg->message==WM_RBUTTONDOWN ||
        pMsg->message==WM_RBUTTONUP ||
        pMsg->message==WM_NCMOUSELEAVE )
    {
        return CALL_NEXT;
    }
    const char *msg = msgToStr(pMsg->message);
    msg_counter++;

    HWND hwnd = pMsg->hwnd;
    char win_title[256];
    GetWindowTextA(hwnd, win_title, 256);

    if( strlen(win_title) )
    {
        sprintf_s(buffer,
                  "%05d %08X %s, data:%08X, pt:%08X title:%.12s\n",
                  msg_counter, pMsg->hwnd, msg,
                  (int)pMsg->wParam, (int)pMsg->lParam,
                  win_title);
    }
    else
    {
        sprintf_s(buffer,
                  "%05d %08X %s, data:%08X, pt:%08X\n",
                  msg_counter, pMsg->hwnd, msg,
                  (int)pMsg->wParam, (int)pMsg->lParam);
    }
    writeMessage(buffer);

    return CALL_NEXT;
}

BOOL APIENTRY DllMain(HINSTANCE hndl, DWORD reason,
                       LPVOID reserved)
{
    (void) reserved; // to avoid not used warning

    if( reason==DLL_PROCESS_ATTACH )
    {
        dll_handle = hndl;
        initLog();
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

void initLog()
{
    AllocConsole();
    FILE *f = new FILE();
    freopen_s(&f, "CONOUT$", "w", stdout);
}

void WINAPI setupHook(DWORD tid)
{
    printf(">>>> tid=%d c_tid=%x\n", tid);
    if( tid==0 )
    {
        return;
    }
    hook_msg = SetWindowsHookExA(WH_GETMESSAGE, msgCallback,
                                 dll_handle, tid);
    if( hook_msg==NULL )
    {
        printf("Failed to set the Msg hook. Error code: %d\n",
               GetLastError());
        return;
    }

    printf("mod_h:%x tid: %d hook:%x\n", dll_handle, tid,
                                         hook_msg);
}

void writeMessage(const char *msg)
{
    printf(msg);
    // in case of window
//    SendMessageA(caller_hwnd, WM_USER,
//                 0, (LPARAM)msg);

    // in case of no window
//    PostThreadMessageA(caller_th, WM_USER,
//                       0, (LPARAM)msg);
}

const char* msgToStr(int message)
{
    switch( message )
    {
        case WM_SCRN_DRAW: return "WM_SCRN_DRAW";
        case WM_QUIT: return "WM_QUIT";
        case WM_NCMOUSEMOVE: return "WM_NCMOUSEMOVE";
        case WM_NCLBUTTONDOWN: return "WM_NCLBUTTONDOWN";
        case WM_KEYDOWN: return "WM_KEYDOWN";
        case WM_KEYUP: return "WM_KEYUP";
        case WM_CHAR: return "WM_CHAR";
        case WM_TIMER: return "WM_TIMER";
        case WM_MOUSEMOVE: return "WM_MOUSEMOVE";
        case WM_MOUSELEFT: return "WM_MOUSELEFT";
        case WM_LBUTTONUP: return "WM_LBUTTONUP";
        case WM_RBUTTONDOWN: return "WM_RBUTTONDOWN";
        case WM_RBUTTONUP: return "WM_RBUTTONUP";
        case WM_NCMOUSELEAVE: return "WM_NCMOUSELEAVE";
        case WM_DWMNCRENDERINGCHANGED:
            return "WM_DWMNCRENDERINGCHANGED";
    }
    char *msg = (char *)malloc(20);
    sprintf(msg, "0x%06X", message);
    return msg;
}
