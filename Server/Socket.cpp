#include "Socket.h"

// 소켓관련 변수/////////////////////////////////////////////

extern SOCKET listensock;
extern SOCKET clientsock;
extern SOCKET hSocket;
extern CS* C_S;									// 소켓 Linked		
extern sockaddr_in addr_server;
extern sockaddr_in addr_client;
extern int addrlen_clt;
extern int nReturn;
extern WSADATA wsadata;

/////////////////////////////////////////////////////////////

// 서버관련 변수/////////////////////////////////////////////

extern BOOL CONN_ST;						// 서버 오픈 상태
extern const unsigned short g_uPort;		// 포트 넘버
extern const int buflen;					// 버퍼 크기
extern TCHAR buf[];

/////////////////////////////////////////////////////////////
extern SEAT* hSeat[30];
extern HWND hWndMain;		// 메인 화면 핸들

/*--------------------------------------------------------
 create(): 소켓 구조체 할당
--------------------------------------------------------*/
CS* create() {
	CS* N;

	N = (CS*)malloc(sizeof(CS));

	lstrcpy(N->ID, TEXT(""));
	N->Client_Sock = 0;
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 SVR_Open(): 서버 오픈
--------------------------------------------------------*/
void SVR_Open() {
	HANDLE hThread;
	DWORD ThreadID;
	u_long mode = 1; // 논블로킹 모드
	CS* S;

	if (!CONN_ST) {
		// 소켓 초기화 (윈속 라이브러리 버전, 윈속 시스템 관련 정보)
		nReturn = WSAStartup(WORD(2.0), &wsadata);

		// 소켓 생성 (IPv4: AF_INET | IPv6: AF_INET6 , 소켓 통신 타입, 프로토콜 결정)
		listensock = socket(AF_INET, SOCK_STREAM, 0);

		// 서버 주소 설정
		addr_server.sin_family = AF_INET;
		addr_server.sin_addr.s_addr = htons(INADDR_ANY);
		addr_server.sin_port = htons(g_uPort);

		// 소켓 바인드 (소켓 객체, 소객 객체에 부여할 주소 정보 구조체, 구조체 길이)
		nReturn = bind(listensock, (sockaddr*)&addr_server, sizeof(sockaddr));

		// 접속 대기 (소켓 객체, 연결 대기열 크기)
		nReturn = listen(listensock, 1);

		// accept 스레드 생성
		CloseHandle(hThread = CreateThread(NULL, 0, Connect_Thread, &listensock, 0, &ThreadID));
		//WSAAsyncSelect(listensock, hWndMain, WM_USER + 1, FD_ACCEPT | FD_READ | FD_CLOSE);

		CONN_ST = TRUE;
	}
	/*
	for (;;) {
		clientsock = accept(listensock, (sockaddr*)&addr_client, &addrlen_clt);

		if (clientsock != INVALID_SOCKET) {
			MessageBox(hWndMain, "클라이언트가 접속하였습니다.", "server", MB_OK);

			addsock(clientsock);

			S = C_S;
			while (S->link != NULL) {
				S = S->link;
			}

			CloseHandle(hThread = CreateThread(NULL, 0, Recv_Thread, &(S->Client_Sock), 0, &ThreadID));
		}

	}
	*/

	InvalidateRect(hWndMain, NULL, FALSE);
	MessageBox(hWndMain, "서버오픈완료", "알림", MB_OK);
}


/*--------------------------------------------------------
 Connect_Thread(LPVOID): 서버 연결 스레드
--------------------------------------------------------*/
DWORD WINAPI Connect_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	HANDLE hThread;
	DWORD ThreadID;
	CS* S;

	// 접속 허용하기 (소켓 객체, 클라이언트 주소 정보, 주소 정보 구조 크기)
	for (;;) {
		clientsock = accept(*P, (sockaddr*)&addr_client, &addrlen_clt);

		if (clientsock != INVALID_SOCKET) {
			MessageBox(hWndMain, "클라이언트가 접속하였습니다.", "server", MB_OK);

			addsock(clientsock);

			S = C_S;
			while (S->link != NULL) {
				S = S->link;
			}

			CloseHandle(hThread = CreateThread(NULL, 0, Recv_Thread, &(S->Client_Sock), 0, &ThreadID));
		}

	}
	return 0;
}

/*--------------------------------------------------------
 Recv_Thread(LPVOID): 수신 스레드
--------------------------------------------------------*/
DWORD WINAPI Recv_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	TCHAR CODE[3], TEXT[256];
	int j;

	while (1) {
		// 데이터 수신 (소캣 객체, 받을 문자열, 문자열 크기, 옵션);
		nReturn = recv(*P, buf, buflen, 0);

		if (nReturn == 0 || nReturn == SOCKET_ERROR) {
			MessageBox(hWndMain, "수신에러", "server", MB_OK);
			return 0;
		}
		else {

			j = 0;
			for (int i = 0; i < lstrlen(buf); i++) {
				if (i > 2) {
					CODE[i] = buf[i];
				}
				else if (i != 3) {
					TEXT[j++] = buf[i];
				}
			}
			CODE[lstrlen(CODE)] = '\0';
			TEXT[lstrlen(TEXT)] = '\0';

			if (lstrcmp(CODE, "SS") == 0) {
				hSocket = (SOCKET)(*P);
				if (hSeat[atoi(TEXT) - 1]->state != false) {
					nReturn = send(hSocket, "ST", sizeof("ST"), 0);
					hSeat[atoi(TEXT) - 1]->state = false;
				}
				else {
					nReturn = send(hSocket, "SF", sizeof("SF"), 0);
					hSeat[atoi(TEXT) - 1]->state = true;
				}
			}
			else if (lstrcmp(CODE, "SR") == 0) {
				if (hSeat[atoi(TEXT) - 1]->state != true) {
					nReturn = send(hSocket, "sf", sizeof("sf"), 0);
					hSeat[atoi(TEXT) - 1]->state = true;
				}
				else {
					nReturn = send(hSocket, "sf", sizeof("SR"), 0);
				}
			}
			else if (lstrcmp(CODE, "JR") == 0) {
				hSocket = (SOCKET)(*P);
				nReturn = send(hSocket, "JT", sizeof("ST"), 0);
				nReturn = send(hSocket, "JF", sizeof("ST"), 0);
			}
		}
	}
	return 0;
}

/*--------------------------------------------------------
 addsock(SOCKET): 소켓 정보 추가
--------------------------------------------------------*/
void addsock(SOCKET S) {
	CS* N, * P;

	N = create();
	N->Client_Sock = S;

	P = C_S;

	while (P->link != NULL) {
		P = P->link;
	}

	P->link = N;

}

/*--------------------------------------------------------
 delsock(SOCKET): 소켓 정보 삭제
--------------------------------------------------------*/
void delsock(SOCKET S) {
	if (C_S->link != NULL) {
		CS* E, * P;
		P = C_S;

		while (P->link != NULL) {
			E = P;
			P = P->link;
			if (P->Client_Sock == S) {
				E->link = P->link;
				break;
			}
		}
		free(P);
	}
}