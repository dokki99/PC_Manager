#ifndef __SOCKET__
#define __SOCKET__
#include "Common.h"
// 소켓 정보 구조체
typedef struct Client_Socket {
	TCHAR ID[128];
	SOCKET Client_Sock;
	struct Client_Socket* link;
}CS;




// 소켓관련 함수/////////////////////////////////////////////

CS* create();							// 소켓 구조체 할당
void addsock(SOCKET);					// 소켓 정보 추가
void delsock(SOCKET);					// 소켓 정보 삭제

/////////////////////////////////////////////////////////////

// 서버관련 함수/////////////////////////////////////////////

void SVR_Open();						// 서버 오픈
DWORD WINAPI Connect_Thread(LPVOID);	// 서버 연결 스레드
DWORD WINAPI Recv_Thread(LPVOID);		// 수신 스레드

/////////////////////////////////////////////////////////////


#endif 
