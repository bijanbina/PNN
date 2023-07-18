#ifndef SAG_HOOK_H
#define SAG_HOOK_H

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <TlHelp32.h>
#include <Psapi.h>

__declspec(dllexport) void WINAPI setupHook(HWND caller,
                                            UINT umsg);
void  writeMessage(const char *msg);
void  showMessage (const WCHAR *txt);
void  initLog();
DWORD findNotepadTid();

#endif //SAG_HOOK_H
