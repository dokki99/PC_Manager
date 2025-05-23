#include "Socket.h"

// 소켓관련 변수/////////////////////////////////////////////

extern SOCKET clientsock;
extern sockaddr_in addr_client;
extern int addrlen_clt;
extern int nReturn;
extern CS* C_S;

extern HWND hWndMain;		// 메인 화면 핸들
/////////////////////////////////////////////////////////////

// 송수신 관련 변수/////////////////////////////////////////////

extern const int buflen;					// 버퍼 크기
extern TCHAR buf[];

extern MQ* Message_Front, * Message_Rear;		// 메시지 처리 큐
extern SQ* Send_Front, * Send_Rear;				// 송신 큐

/////////////////////////////////////////////////////////////

// 동기화 관련 변수//////////////////////////////////////////

extern HANDLE Send_Mutex;					// 송신 뮤택스

/////////////////////////////////////////////////////////////

/*--------------------------------------------------------
 Create_CS(): 소켓 정보 초기화
--------------------------------------------------------*/
CS* Create_CS() {
	CS* N;

	N = (CS*)malloc(sizeof(CS));

	N->Sock = NULL;
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 addsock(SOCKET): 소켓 정보 추가
--------------------------------------------------------*/
void addsock(SOCKET* S) {
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
void delsock(SOCKET* S) {
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

			addsock(&clientsock);

			S = C_S;
			while (S->link != NULL) {
				S = S->link;
			}

			CloseHandle(hThread = CreateThread(NULL, 0, Recv_Thread, S->Sock, 0, &ThreadID));
		}

	}
	return 0;
}

/*--------------------------------------------------------
 Send_Thread(LPVOID): 송신 스레드
--------------------------------------------------------*/
DWORD WINAPI Send_Thread(LPVOID Param) {
	SQ* S;

	while (1) {
		if (!IsEmpty_SQ()) {
			// 여기서 디큐하면서 자동적으로 보내줘야됌
			
			WaitForSingleObject(Send_Mutex, INFINITE);
			S = Deque_SQ();
			ReleaseMutex(Send_Mutex);

			nReturn = send(*(S->Client_Sock), S->TEXT, sizeof(S->TEXT), 0);

			free(S);
		}
	}
	return 0;
}


/*--------------------------------------------------------
 Recv_Thread(LPVOID): 수신 스레드
--------------------------------------------------------*/
DWORD WINAPI Recv_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	TCHAR CODE[4], TEXT[256];
	
	while (1) {
		// 데이터 수신 (소캣 객체, 받을 문자열, 문자열 크기, 옵션);
		nReturn = recv(*P, buf, buflen, 0);

		if (nReturn != 0 && nReturn != SOCKET_ERROR) {
			Split_C_T(CODE, TEXT);
			Enque_MQ(P, CODE, TEXT);
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Split_C_T(TCHAR* CODE, TCHAR* TEXT) : 수신 받은 텍스트를
 CODE와 TEXT로 분리하는 함수
-------------------------------------------------------- */
void Split_C_T(TCHAR* CODE, TCHAR* TEXT) {
	int i;

	lstrcpy(CODE, "");
	lstrcpy(TEXT, "");

	for (i = 0; i < lstrlen(buf); i++) {
		if (i < 3) {
			CODE[i] = buf[i];
		}
		else if (i == 3) {
			CODE[i] = '\0';
		}
		else if (i > 3) {
			TEXT[i - 4] = buf[i];
		}
	}

	TEXT[i - 4] = '\0';
}

/*--------------------------------------------------------
 Create_SQ(): 송신 큐 초기화 함수
--------------------------------------------------------*/
SQ* Create_SQ() {
	SQ* N;

	N = (SQ*)malloc(sizeof(SQ));

	lstrcpy(N->TEXT, "");
	N->Client_Sock = 0;
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 Enque_SQ(TCHAR* ,SOCKET): 송신 큐 엔큐
--------------------------------------------------------*/
void Enque_SQ(const TCHAR* TEXT, SOCKET* Client_Sock) {
	SQ* N;

	N = Create_SQ();
	lstrcpy(N->TEXT, TEXT);
	N->Client_Sock = Client_Sock;

	if (Send_Front->link == NULL && Send_Rear->link == NULL) {
		Send_Front->link = N;
		Send_Rear->link = N;
	}
	else {
		N->link = Send_Rear->link;
		Send_Rear->link = N;
	}
}
/*--------------------------------------------------------
 Deque_SQ(): 송신 큐 디큐
--------------------------------------------------------*/
SQ* Deque_SQ() {
	SQ* E, * N;

	N = Create_SQ();

	E = Send_Front->link;
	*N = *E;

	Send_Front = Send_Front->link;
	if (Send_Front->link == NULL) {
		Send_Rear->link = NULL;
	}

	free(E);
	return N;
}

/*--------------------------------------------------------
 IsEmpty_SQ(): 송신 큐 비었는지 확인하는 함수
--------------------------------------------------------*/
BOOL IsEmpty_SQ() {
	return (((Send_Front->link == NULL) && (Send_Rear->link == NULL)) ? TRUE : FALSE);
}
