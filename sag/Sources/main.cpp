#include "hook.h"

static HHOOK hook_msg;
static HHOOK hook_win;
HMODULE dll_handle = NULL;

#define CALL_NEXT CallNextHookEx(hook_msg, nCode, wParam, lParam)

LRESULT CALLBACK msgCallback(int nCode, WPARAM wParam,
                                 LPARAM lParam)
{
    PMSG pMsg = (PMSG)lParam;		// WH_CALLWNDPROC
    char buffer[250] = {0};
    if( pMsg->message==WM_MOUSEMOVE ||
        pMsg->message==0x118 ||
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
    sprintf_s(buffer,
              "%08X %s, w: %08X, l: %08X\n",
              pMsg->hwnd, msg, (int)pMsg->wParam,
              (int)pMsg->lParam);

//    HWND hwnd = pMsg->hwnd;
//    char win_title[256];
//    GetWindowTextA(hwnd, win_title, 256);
//    printf("msgCallback: %x | %x %s\n", hwnd,
//           cwpStruct->message, win_title);
    writeMessage(buffer);

    return CALL_NEXT;
}

LRESULT CALLBACK winCallback(int nCode, WPARAM wParam,
                                 LPARAM lParam)
{
    CWPSTRUCT* cwpStruct = reinterpret_cast<CWPSTRUCT*>(lParam);
//    if( nCode>=0 )
//    {
        // Process the window messages here
        HWND hwnd = cwpStruct->hwnd;
        char win_title[256];
        GetWindowTextA(hwnd, win_title, 256);
        printf("winCallback: %x | %x %s\n", hwnd,
               cwpStruct->message, win_title);

        writeMessage("win");
//    }
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

    printf("Log is initialized\n");
    writeMessage("log");
}

void WINAPI setupHook(HWND caller, UINT umsg)
{
//    umsg_h = umsg;
//    caller_h = caller;
    initLog();
//    HWND  hwnd = GetForegroundWindow();
    DWORD tid  = findNotepadTid();

    printf(">>>> tid=%d\n", tid);
//    DWORD tid  = 14880;

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

//    hook_win = SetWindowsHookEx(WH_CALLWNDPROC, winCallback,
//                                dll_handle, tid);
//    if( hook_win==NULL )
//    {
//        printf("Failed to set the Win hook. Error code: %d\n",
//               GetLastError());
//        return;
//    }
//    printf("All hook were successful\n");
    printf("mod_h:%x tid: %d hook:%x\n", dll_handle, tid,
                                         hook_msg);
}

void showMessage(const WCHAR *txt)
{
    MessageBox(0, txt, L"DLL",
               MB_ICONINFORMATION);
}

DWORD findNotepadTid()
{
    auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    DWORD tid = 0;
    THREADENTRY32 th32;
    th32.dwSize = sizeof(th32);

    Thread32First(hSnapshot, &th32);
    do {
        auto hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, th32.th32OwnerProcessID);
        if (hProcess) {
            WCHAR name[MAX_PATH];
            if( GetProcessImageFileName(hProcess, name, MAX_PATH) > 0) {
                auto bs = ::wcsrchr(name, L'\\');
                if (bs && ::_wcsicmp(bs, L"\\notepad.exe") == 0) {
                    tid = th32.th32ThreadID;
                }
            }
            CloseHandle(hProcess);
        }
    }
    while (tid == 0 && Thread32Next(hSnapshot, &th32));
    CloseHandle(hSnapshot);

    return tid;
}

void writeMessage(const char *msg)
{
    FILE *pFile;
    pFile=fopen("C:/Home/Projects/test.log", "a");
    fprintf(pFile, "%s", msg);
    fclose(pFile);
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
