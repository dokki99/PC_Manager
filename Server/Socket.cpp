#include "Socket.h"

// ���ϰ��� ����////////////////////////////////////////////////////
extern BOOL CONN_ST;
////////////////////////////////////////////////////////////////////

// ���ϰ��� ����////////////////////////////////////////////////////

extern SOCKET clientsock;
extern sockaddr_in addr_client;
extern int addrlen_clt;
extern int nReturn;
extern CCI *C_CI;

extern HWND hWndMain;							// ���� ȭ�� �ڵ�

////////////////////////////////////////////////////////////////////

// �ۼ��� ���� ����/////////////////////////////////////////////////



extern MQ *Message_Front, *Message_Rear;		// �޽��� ó�� ť
extern SQ *Send_Front, *Send_Rear;				// �۽� ť

////////////////////////////////////////////////////////////////////

// ����ȭ ���� ����/////////////////////////////////////////////////
extern HANDLE Message_Mutex;					// �޽��� ó�� ���ý�
extern HANDLE Send_Mutex;						// �۽� ���ý�

////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------
 Create_CCI(): ���� ���� �ʱ�ȭ
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
 Add_CCI_Sock(SOCKET): ���� ���� �߰�
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
 Add_CCI_Thread(SOCKET ,HANDLE ,DWORD) : ���� ������ �´�
 ������ ���� �߰�
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
 Fond_CCI(SOCKET): ����ü�� ��� �����ּ� ����
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
 Del_CCI(SOCKET): ���� ���� ����
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
 Connect_Process(LPVOID): ���� ���� ������
--------------------------------------------------------*/
DWORD WINAPI Connect_Process(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	HANDLE Thread;
	DWORD Thread_ID;
	SOCKET* Sock;

	while (CONN_ST) {
		clientsock = accept(*P, (sockaddr*)&addr_client, &addrlen_clt);

		if (clientsock != INVALID_SOCKET) {
			MessageBox(hWndMain, "Ŭ���̾�Ʈ�� �����Ͽ����ϴ�.", "server", MB_OK);

			// Ŭ���̾�Ʈ ����
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
 Send_Process(LPVOID): �۽� ������
--------------------------------------------------------*/
DWORD WINAPI Send_Process(LPVOID Param) {
	SQ* S;

	while (CONN_ST) {
		// ���⼭ ��ť�ϸ鼭 �ڵ������� ������߉�
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
 Recv_Thread(LPVOID): ���� ������
--------------------------------------------------------*/
DWORD WINAPI Recv_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	TCHAR CODE[4], TEXT[256];
	TCHAR buf[MAX_BUFLEN];
	
	while (CONN_ST) {
		// ������ ���� (��Ĺ ��ü, ���� ���ڿ�, ���ڿ� ũ��, �ɼ�);
		nReturn = recv(*P, buf, MAX_BUFLEN, 0);

		if (nReturn != SOCKET_ERROR) {
			if (nReturn == 0) {
				// ���� ����
				Del_CCI(*P);
				break;
			}
			Split_C_T(buf, CODE, TEXT);
			Enque_MQ(*P, CODE, TEXT);
		}
		else {
			// ������ ����
			break;
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Split_C_T(TCHAR* buf,TCHAR* CODE, TCHAR* TEXT) : ���� ����
 �ؽ�Ʈ�� CODE�� TEXT�� �и��ϴ� �Լ�
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
 Create_MQ(): �޽��� ó�� ť �ʱ�ȭ �Լ�
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
 Enque_MQ(SOCKET, const TCHAR*, const TCHAR*): �޽���
 ó�� ť ��ť
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
 Deque_MQ(): �޽��� ó�� ť ��ť
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
 IsEmpty_MQ(): �޽��� ó�� ť ������� Ȯ���ϴ� �Լ�
--------------------------------------------------------*/
BOOL IsEmpty_MQ() {
	return (((Message_Front->link == NULL) && (Message_Rear->link == NULL)) ? TRUE : FALSE);
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
 Deque_SQ(): �۽� ť ��ť
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
 IsEmpty_SQ(): �۽� ť ������� Ȯ���ϴ� �Լ�
--------------------------------------------------------*/
BOOL IsEmpty_SQ() {
	return (((Send_Front->link == NULL) && (Send_Rear->link == NULL)) ? TRUE : FALSE);
}
