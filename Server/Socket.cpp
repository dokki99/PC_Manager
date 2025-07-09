#include "Socket.h"

// 소켓관련 변수////////////////////////////////////////////////////
extern BOOL CONN_ST;
////////////////////////////////////////////////////////////////////

// 소켓관련 변수////////////////////////////////////////////////////

extern SOCKET clientsock;
extern sockaddr_in addr_client;
extern int addrlen_clt;
extern int nReturn;
extern CCI *C_CI;

extern HWND hWndMain;							// 메인 화면 핸들

////////////////////////////////////////////////////////////////////

// 송수신 관련 변수/////////////////////////////////////////////////



extern MQ *Message_Front, *Message_Rear;		// 메시지 처리 큐
extern SQ *Send_Front, *Send_Rear;				// 송신 큐

////////////////////////////////////////////////////////////////////

// 동기화 관련 변수/////////////////////////////////////////////////
extern HANDLE Message_Mutex;					// 메시지 처리 뮤택스
extern HANDLE Send_Mutex;						// 송신 뮤택스

////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------
 Create_CCI(): 소켓 정보 초기화
--------------------------------------------------------*/
CCI* Create_CCI() {
	CCI* N;

	N = (CCI*)malloc(sizeof(CCI));

	N->Sock = INVALID_SOCKET;
	N->Thread_Info = NULL;
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 Add_CCI_Sock(SOCKET): 소켓 정보 추가
--------------------------------------------------------*/
void Add_CCI_Sock(SOCKET S) {
	CCI* N, * P;
	N = Create_CCI();
	
	N->Sock = S;

	P = C_CI;
	while (P->link != NULL) {
		P = P->link;
	}

	P->link = N;
}

/*--------------------------------------------------------
 Add_CCI_Thread(SOCKET ,HANDLE ,DWORD) : 소켓 정보에 맞는
 스레드 정보 추가
--------------------------------------------------------*/
void Add_CCI_Thread(SOCKET S,HANDLE Thread_Info, DWORD Thread_ID) {
	CCI* P;
	
	P = C_CI;
	while (P->link != NULL) {
		P = P->link;
		if (P->Sock == S) {
			P->Thread_Info = Thread_Info;
			P->Thread_ID = Thread_ID;
			break;
		}
	}
}

/*--------------------------------------------------------
 Fond_CCI(SOCKET): 구조체에 닮긴 소켓주소 리턴
--------------------------------------------------------*/
SOCKET* Find_CCI(SOCKET S) {
	CCI* P;
	P = C_CI;

	while (P->link != NULL) {
		P = P->link;
		if (P->Sock == S) return &(P->Sock);
	}

	return NULL;
}

/*--------------------------------------------------------
 Del_CCI(SOCKET): 소켓 정보 삭제
--------------------------------------------------------*/
void Del_CCI(SOCKET S) {
	if (C_CI->link != NULL) {
		CCI* E, * P;
		P = C_CI;

		while (P->link != NULL) {
			E = P;
			P = P->link;
			if (P->Sock == S) {
				E->link = P->link;
				closesocket(P->Sock);
				WaitForSingleObject(P->Thread_Info, INFINITE);
				CloseHandle(P->Thread_Info);
				free(P);
				break;
			}
		}
	}
}

/*--------------------------------------------------------
 Connect_Process(LPVOID): 서버 연결 스레드
--------------------------------------------------------*/
DWORD WINAPI Connect_Process(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	HANDLE Thread;
	DWORD Thread_ID;
	SOCKET* Sock;

	while (CONN_ST) {
		clientsock = accept(*P, (sockaddr*)&addr_client, &addrlen_clt);

		if (clientsock != INVALID_SOCKET) {
			MessageBox(hWndMain, "클라이언트가 접속하였습니다.", "server", MB_OK);

			// 클라이언트 연결
			Add_CCI_Sock(clientsock);
			if ((Sock = Find_CCI(clientsock)) != NULL) {
				Thread = CreateThread(NULL, 0, Recv_Thread, Sock, 0, &Thread_ID);
				Add_CCI_Thread(*Sock, Thread, Thread_ID);
			}
			else {
				break;
			}
		}

	}
	return 0;
}

/*--------------------------------------------------------
 Send_Process(LPVOID): 송신 스레드
--------------------------------------------------------*/
DWORD WINAPI Send_Process(LPVOID Param) {
	SQ* S;

	while (CONN_ST) {
		// 여기서 디큐하면서 자동적으로 보내줘야됌
		S = Deque_SQ();

		if (S != NULL) {
			nReturn = send(S->Client_Sock, S->TEXT, sizeof(S->TEXT), 0);
			free(S);
			S = NULL;
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
	TCHAR buf[MAX_BUFLEN];
	
	while (CONN_ST) {
		// 데이터 수신 (소캣 객체, 받을 문자열, 문자열 크기, 옵션);
		nReturn = recv(*P, buf, MAX_BUFLEN, 0);

		if (nReturn != SOCKET_ERROR) {
			if (nReturn == 0) {
				// 정상 종료
				Del_CCI(*P);
				break;
			}
			Split_C_T(buf, CODE, TEXT);
			Enque_MQ(*P, CODE, TEXT);
		}
		else {
			// 비정상 종료
			break;
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Split_C_T(TCHAR* buf,TCHAR* CODE, TCHAR* TEXT) : 수신 받은
 텍스트를 CODE와 TEXT로 분리하는 함수
-------------------------------------------------------- */
void Split_C_T(TCHAR* buf, TCHAR* CODE, TCHAR* TEXT) {
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
 Create_MQ(): 메시지 처리 큐 초기화 함수
--------------------------------------------------------*/
MQ* Create_MQ() {
	MQ* N;
	N = (MQ*)malloc(sizeof(MQ));

	N->Client_Sock = NULL;
	lstrcpy(N->CODE, "");
	lstrcpy(N->TEXT, "");
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 Enque_MQ(SOCKET, const TCHAR*, const TCHAR*): 메시지
 처리 큐 엔큐
--------------------------------------------------------*/
void Enque_MQ(SOCKET Client_Sock, const TCHAR* CODE, const TCHAR* TEXT) {
	WaitForSingleObject(Message_Mutex, INFINITE);
	MQ* N;

	N = Create_MQ();
	N->Client_Sock = Client_Sock;
	lstrcpy(N->CODE, CODE);
	lstrcpy(N->TEXT, TEXT);

	if (Message_Front->link == NULL && Message_Rear->link == NULL) {
		Message_Front->link = N;
		Message_Rear->link = N;
	}
	else {
		N->link = Message_Rear->link;
		Message_Rear->link = N;
	}
	ReleaseMutex(Message_Mutex);
}

/*--------------------------------------------------------
 Deque_MQ(): 메시지 처리 큐 디큐
--------------------------------------------------------*/
MQ* Deque_MQ() {
	WaitForSingleObject(Message_Mutex, INFINITE);
	MQ* E,* N = NULL;

	if (!IsEmpty_MQ()) {
		N = Create_MQ();
		E = Message_Front->link;

		*N = *E;

		Message_Front->link = E->link;
		if (Message_Front->link == NULL) {
			Message_Rear->link = NULL;
		}

		free(E);
	}
	
	ReleaseMutex(Message_Mutex);

	return N;
}

/*--------------------------------------------------------
 IsEmpty_MQ(): 메시지 처리 큐 비었는지 확인하는 함수
--------------------------------------------------------*/
BOOL IsEmpty_MQ() {
	return (((Message_Front->link == NULL) && (Message_Rear->link == NULL)) ? TRUE : FALSE);
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
void Enque_SQ(const TCHAR* TEXT, SOCKET Client_Sock) {
	WaitForSingleObject(Send_Mutex, INFINITE);
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
	ReleaseMutex(Send_Mutex);
}
/*--------------------------------------------------------
 Deque_SQ(): 송신 큐 디큐
--------------------------------------------------------*/
SQ* Deque_SQ() {
	WaitForSingleObject(Send_Mutex, INFINITE);
	SQ* E,* N = NULL;

	if (!IsEmpty_SQ()) {
		N = Create_SQ();
		E = Send_Front->link;

		*N = *E;

		Send_Front->link = E->link;
		if (Send_Front->link == NULL) {
			Send_Rear->link = NULL;
		}

		free(E);
	}
	ReleaseMutex(Send_Mutex);

	return N;
}

/*--------------------------------------------------------
 IsEmpty_SQ(): 송신 큐 비었는지 확인하는 함수
--------------------------------------------------------*/
BOOL IsEmpty_SQ() {
	return (((Send_Front->link == NULL) && (Send_Rear->link == NULL)) ? TRUE : FALSE);
}
