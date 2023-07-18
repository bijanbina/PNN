#ifndef SAG_HOOK_H
#define SAG_HOOK_H

#include <windows.h>
#include <stdio.h>
#include <iostream>

#define WM_SCRN_DRAW                0x000F
#define WM_QUIT                     0x0012
#define WM_NCMOUSEMOVE              0x00A0
#define WM_NCLBUTTONDOWN            0x00A1
#define WM_KEYDOWN                  0x0100
#define WM_KEYUP                    0x0101
#define WM_CHAR                     0x0102
#define WM_TIMER                    0x0113
#define WM_SYSTIMER                 0x0118
#define WM_MOUSEMOVE                0x0200
#define WM_MOUSELEFT                0x0201
#define WM_LBUTTONUP                0x0202
#define WM_RBUTTONDOWN              0x0204
#define WM_RBUTTONUP                0x0205
#define WM_NCMOUSELEAVE             0x02A2
#define WM_DWMNCRENDERINGCHANGED    0x031F

__declspec(dllexport) void WINAPI setupHook(DWORD tid);
void  writeMessage(const char *msg);
void  showMessage (const WCHAR *txt);
void  initLog();
const char *msgToStr(int message);

#endif //SAG_HOOK_H
