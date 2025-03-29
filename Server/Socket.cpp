#include "Socket.h"
#include "Process.h"

// ���ϰ��� ����/////////////////////////////////////////////

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

// �������� ����/////////////////////////////////////////////

extern BOOL CONN_ST;						// ���� ���� ����
extern const unsigned short g_uPort;		// ��Ʈ �ѹ�
extern const int buflen;					// ���� ũ��
extern TCHAR buf[];

/////////////////////////////////////////////////////////////

// �������� ����/////////////////////////////////////////////
extern SEAT* hSeat[MAX_SEAT];
extern HANDLE Seat_Thread[MAX_SEAT];
extern HWND hWndMain;						// ���� ȭ�� �ڵ�
/////////////////////////////////////////////////////////////

// � �� ����ȭ ���� ����//////////////////////////////////

extern SEAT* hSeat[MAX_SEAT];				// �¼� ����ü
extern TCHAR Seat_Code[MAX_SEAT + 1];
extern HANDLE Seat_Mutex[MAX_SEAT];			// �¼� ���ý�

/////////////////////////////////////////////////////////////

/*--------------------------------------------------------
 Create_CS(): ���� ���� �߰�
--------------------------------------------------------*/
CS* Create_CS() {
	CS* N;

	N = (CS*)malloc(sizeof(CS));

	N->Sock = 0;
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 addsock(SOCKET): ���� ���� �߰�
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
 delsock(SOCKET): ���� ���� ����
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
 SVR_Open(): ���� ����
--------------------------------------------------------*/
void SVR_Open() {
	int i;
	HANDLE hThread;
	DWORD ThreadID;

	if (!CONN_ST) {
		// ���� �ʱ�ȭ (���� ���̺귯�� ����, ���� �ý��� ���� ����)
		nReturn = WSAStartup(WORD(2.0), &wsadata);

		// ���� ���� (IPv4: AF_INET | IPv6: AF_INET6 , ���� ��� Ÿ��, �������� ����)
		listensock = socket(AF_INET, SOCK_STREAM, 0);

		// ���� �ּ� ����
		addr_server.sin_family = AF_INET;
		addr_server.sin_addr.s_addr = htons(INADDR_ANY);
		addr_server.sin_port = htons(g_uPort);

		// ���� ���ε� (���� ��ü, �Ұ� ��ü�� �ο��� �ּ� ���� ����ü, ����ü ����)
		nReturn = bind(listensock, (sockaddr*)&addr_server, sizeof(sockaddr));

		// ���� ��� (���� ��ü, ���� ��⿭ ũ��)
		nReturn = listen(listensock, MAX_BACKLOG);

		// Relay ������ ���� (�ϴ� ��� ���·� ����)
		for (i = 0; i < MAX_SEAT; i++) {
			Seat_Thread[i] = CreateThread(NULL, 0, Relay_Thread, &(hSeat[i]->S_num), CREATE_SUSPENDED, &(hSeat[i]->Thread_ID));
		}

		// accept ������ ����
		CloseHandle(hThread = CreateThread(NULL, 0, Connect_Thread, &listensock, 0, &ThreadID));
		//WSAAsyncSelect(listensock, hWndMain, WM_USER + 1, FD_ACCEPT | FD_READ | FD_CLOSE);

		CONN_ST = TRUE;
	}

	InvalidateRect(hWndMain, NULL, FALSE);
	MessageBox(hWndMain, "�������¿Ϸ�", "�˸�", MB_OK);
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
 Recv_Thread(LPVOID): ���� ������
--------------------------------------------------------*/
DWORD WINAPI Recv_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	TCHAR CODE[4], TEXT[256], Send_TEXT[300];
	int num = 0;
	
	while (1) {
		// ������ ���� (��Ĺ ��ü, ���� ���ڿ�, ���ڿ� ũ��, �ɼ�);
		nReturn = recv(*P, buf, buflen, 0);

		if (nReturn != 0 && nReturn != SOCKET_ERROR) {
			Split_C_T(CODE, TEXT);
			
			if (lstrcmp(CODE,"C00") == 0) {				
				// ������ Ŭ���̾�Ʈ���� ���� �¼� ���¸� �����ֱ�
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
			MessageBox(hWndMain, "���ſ���", "server", MB_OK);
			return 0;
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Send_Text(TCHAR*, TCHAR*, SOCKET*): Ŭ���̾�Ʈ�� �ؽ�Ʈ
 �۽��ϴ� �Լ�
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

		// �¼����� ������Ʈ 
		Update_Seat_Code();
		
		if (lstrcmp(Text, "SUCCESS") == 0) {
			// ���⼭ ��� ���� ���Ͽ� �¼������� �����������߉�
			S = C_S;

			while (S->link != NULL) {
				wsprintf(TEXT, "%s-%s", "S00", Seat_Code);
				S = S->link;
				nReturn = send(S->Sock, TEXT, sizeof(TEXT), 0);
			}
		}

		// �ش� ���Ͽ� [SUCCESS / FAIL] �޼��� ������
		wsprintf(TEXT, "%s-%s", CODE, Text);
		nReturn = send(*P, TEXT, sizeof(TEXT), 0);
	}

}


/*--------------------------------------------------------
 SVR_Close(): ���� ����
--------------------------------------------------------*/
void SVR_Close() {
	int i;
	CS* S;

	// �¼� ������ �ڵ� ��ȯ
	for (i = 0; i < MAX_SEAT; i++) {
		CloseHandle(Seat_Thread[i]);
	}

	// �������� ����
	while (C_S->link != NULL) {
		S = C_S->link;
		delsock(S->Sock);
	}

	// ���ý��ڵ� ��ȯ
	for (i = 0; i < MAX_SEAT; i++) {
		CloseHandle(Seat_Mutex[i]);
	}
}