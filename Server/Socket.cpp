#include "Socket.h"

// ���ϰ��� ����/////////////////////////////////////////////

extern SOCKET clientsock;
extern sockaddr_in addr_client;
extern int addrlen_clt;
extern int nReturn;
extern CS* C_S;

extern HWND hWndMain;		// ���� ȭ�� �ڵ�
/////////////////////////////////////////////////////////////

// �ۼ��� ���� ����/////////////////////////////////////////////

extern const int buflen;					// ���� ũ��
extern TCHAR buf[];

extern MQ* Message_Front, * Message_Rear;		// �޽��� ó�� ť
extern SQ* Send_Front, * Send_Rear;				// �۽� ť

/////////////////////////////////////////////////////////////

// ����ȭ ���� ����//////////////////////////////////////////

extern HANDLE Send_Mutex;					// �۽� ���ý�

/////////////////////////////////////////////////////////////

/*--------------------------------------------------------
 Create_CS(): ���� ���� �ʱ�ȭ
--------------------------------------------------------*/
CS* Create_CS() {
	CS* N;

	N = (CS*)malloc(sizeof(CS));

	N->Sock = NULL;
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 addsock(SOCKET): ���� ���� �߰�
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
 delsock(SOCKET): ���� ���� ����
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
 Connect_Thread(LPVOID): ���� ���� ������
--------------------------------------------------------*/
DWORD WINAPI Connect_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	HANDLE hThread;
	DWORD ThreadID;
	CS* S;

	for (;;) {
		clientsock = accept(*P, (sockaddr*)&addr_client, &addrlen_clt);

		if (clientsock != INVALID_SOCKET) {
			MessageBox(hWndMain, "Ŭ���̾�Ʈ�� �����Ͽ����ϴ�.", "server", MB_OK);

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
 Send_Thread(LPVOID): �۽� ������
--------------------------------------------------------*/
DWORD WINAPI Send_Thread(LPVOID Param) {
	SQ* S;

	while (1) {
		if (!IsEmpty_SQ()) {
			// ���⼭ ��ť�ϸ鼭 �ڵ������� ������߉�
			
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
 Recv_Thread(LPVOID): ���� ������
--------------------------------------------------------*/
DWORD WINAPI Recv_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	TCHAR CODE[4], TEXT[256];
	
	while (1) {
		// ������ ���� (��Ĺ ��ü, ���� ���ڿ�, ���ڿ� ũ��, �ɼ�);
		nReturn = recv(*P, buf, buflen, 0);

		if (nReturn != 0 && nReturn != SOCKET_ERROR) {
			Split_C_T(CODE, TEXT);
			Enque_MQ(P, CODE, TEXT);
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Split_C_T(TCHAR* CODE, TCHAR* TEXT) : ���� ���� �ؽ�Ʈ��
 CODE�� TEXT�� �и��ϴ� �Լ�
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
 Create_SQ(): �۽� ť �ʱ�ȭ �Լ�
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
 Enque_SQ(TCHAR* ,SOCKET): �۽� ť ��ť
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
 Deque_SQ(): �۽� ť ��ť
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
 IsEmpty_SQ(): �۽� ť ������� Ȯ���ϴ� �Լ�
--------------------------------------------------------*/
BOOL IsEmpty_SQ() {
	return (((Send_Front->link == NULL) && (Send_Rear->link == NULL)) ? TRUE : FALSE);
}
