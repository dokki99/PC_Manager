#ifndef __SOCKET__
#define __SOCKET__
#include "Common.h"
void CLIENT_Open();							//소켓 연결 함수(연결스레드 만들기)
DWORD WINAPI ThreadFunc(LPVOID Param);					//리시브스레드로써 요청의 결과를 다룸
//extern TCHAR tgIp[17];
//extern TCHAR tgPort[10];
#endif

