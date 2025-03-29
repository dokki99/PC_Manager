#include "Socket.h"
#include "Process.h"

// 소켓관련 변수/////////////////////////////////////////////

extern SOCKET listensock;
extern SOCKET clientsock;
extern SOCKET hSocket;
extern sockaddr_in addr_server;
extern sockaddr_in addr_client;
extern int addrlen_clt;
extern int nReturn;
extern WSADATA wsadata;
extern CS* C_S;
/////////////////////////////////////////////////////////////

// 서버관련 변수/////////////////////////////////////////////

extern BOOL CONN_ST;						// 서버 오픈 상태
extern const unsigned short g_uPort;		// 포트 넘버
extern const int buflen;					// 버퍼 크기
extern TCHAR buf[];

/////////////////////////////////////////////////////////////

// 서버관련 변수/////////////////////////////////////////////
extern SEAT* hSeat[MAX_SEAT];
extern HANDLE Seat_Thread[MAX_SEAT];
extern HWND hWndMain;						// 메인 화면 핸들
/////////////////////////////////////////////////////////////

// 운영 및 동기화 관련 변수//////////////////////////////////

extern SEAT* hSeat[MAX_SEAT];				// 좌석 구조체
extern TCHAR Seat_Code[MAX_SEAT + 1];
extern HANDLE Seat_Mutex[MAX_SEAT];			// 좌석 뮤택스

/////////////////////////////////////////////////////////////

/*--------------------------------------------------------
 Create_CS(): 소켓 정보 추가
--------------------------------------------------------*/
CS* Create_CS() {
	CS* N;

	N = (CS*)malloc(sizeof(CS));

	N->Sock = 0;
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 addsock(SOCKET): 소켓 정보 추가
--------------------------------------------------------*/
void addsock(SOCKET S) {
	CS* N, * P;

	N = Create_CS();
	N->Sock = S;

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
			if (P->Sock == S) {
				E->link = P->link;
				free(P);
				break;
			}
		}
	}
}

/*--------------------------------------------------------
 SVR_Open(): 서버 오픈
--------------------------------------------------------*/
void SVR_Open() {
	int i;
	HANDLE hThread;
	DWORD ThreadID;

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
		nReturn = listen(listensock, MAX_BACKLOG);

		// Relay 스레드 생성 (일단 대기 상태로 생성)
		for (i = 0; i < MAX_SEAT; i++) {
			Seat_Thread[i] = CreateThread(NULL, 0, Relay_Thread, &(hSeat[i]->S_num), CREATE_SUSPENDED, &(hSeat[i]->Thread_ID));
		}

		// accept 스레드 생성
		CloseHandle(hThread = CreateThread(NULL, 0, Connect_Thread, &listensock, 0, &ThreadID));
		//WSAAsyncSelect(listensock, hWndMain, WM_USER + 1, FD_ACCEPT | FD_READ | FD_CLOSE);

		CONN_ST = TRUE;
	}

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

	for (;;) {
		clientsock = accept(*P, (sockaddr*)&addr_client, &addrlen_clt);

		if (clientsock != INVALID_SOCKET) {
			MessageBox(hWndMain, "클라이언트가 접속하였습니다.", "server", MB_OK);

			addsock(clientsock);

			S = C_S;
			while (S->link != NULL) {
				S = S->link;
			}

			CloseHandle(hThread = CreateThread(NULL, 0, Recv_Thread, &(S->Sock), 0, &ThreadID));
		}

	}
	return 0;
}

/*--------------------------------------------------------
 Recv_Thread(LPVOID): 수신 스레드
--------------------------------------------------------*/
DWORD WINAPI Recv_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	TCHAR CODE[4], TEXT[256], Send_TEXT[300];
	int num = 0;
	
	while (1) {
		// 데이터 수신 (소캣 객체, 받을 문자열, 문자열 크기, 옵션);
		nReturn = recv(*P, buf, buflen, 0);

		if (nReturn != 0 && nReturn != SOCKET_ERROR) {
			Split_C_T(CODE, TEXT);
			
			if (lstrcmp(CODE,"C00") == 0) {				
				// 접속한 클라이언트에게 현재 좌석 상태를 보내주기
				Send_Text("S00", "", P);
			}
			else if (lstrcmp(CODE, "C01") == 0) {
				num = atoi(TEXT);
				WaitForSingleObject(Seat_Mutex[num - 1], INFINITE);

				if (hSeat[num - 1]->State == 0) {
					hSeat[num - 1]->State = 1;
					lstrcpy(TEXT, "SUCCESS");
				}
				else {
					lstrcpy(TEXT, "FAIL");
				}

				Send_Text("S01", TEXT, P);

				ReleaseMutex(Seat_Mutex[num - 1]);
			}
		}
		else {
			MessageBox(hWndMain, "수신에러", "server", MB_OK);
			return 0;
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Send_Text(TCHAR*, TCHAR*, SOCKET*): 클라이언트로 텍스트
 송신하는 함수
--------------------------------------------------------*/
void Send_Text(const char* code, const char* Text, SOCKET* P) {
	TCHAR CODE[4];
	TCHAR TEXT[300];

	wsprintf(CODE, "%s", code);

	if (lstrcmp(CODE, "S00") == 0) {
		wsprintf(TEXT, "%s-%s", CODE, Seat_Code);
		nReturn = send(*P, TEXT, sizeof(TEXT), 0);
	}
	else if (lstrcmp(CODE, "S01") == 0) {
		CS* S;

		// 좌석정보 업데이트 
		Update_Seat_Code();
		
		if (lstrcmp(Text, "SUCCESS") == 0) {
			// 여기서 모든 접속 소켓에 좌석에대한 소켓을보내야됌
			S = C_S;

			while (S->link != NULL) {
				wsprintf(TEXT, "%s-%s", "S00", Seat_Code);
				S = S->link;
				nReturn = send(S->Sock, TEXT, sizeof(TEXT), 0);
			}
		}

		// 해당 소켓에 [SUCCESS / FAIL] 메세지 보내기
		wsprintf(TEXT, "%s-%s", CODE, Text);
		nReturn = send(*P, TEXT, sizeof(TEXT), 0);
	}

}


/*--------------------------------------------------------
 SVR_Close(): 서버 종료
--------------------------------------------------------*/
void SVR_Close() {
	int i;
	CS* S;

	// 좌석 스레드 핸들 반환
	for (i = 0; i < MAX_SEAT; i++) {
		CloseHandle(Seat_Thread[i]);
	}

	// 소켓정보 삭제
	while (C_S->link != NULL) {
		S = C_S->link;
		delsock(S->Sock);
	}

	// 뮤택스핸들 반환
	for (i = 0; i < MAX_SEAT; i++) {
		CloseHandle(Seat_Mutex[i]);
	}
}