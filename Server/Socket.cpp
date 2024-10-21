#include "Socket.h"

// ���ϰ��� ����/////////////////////////////////////////////

extern SOCKET listensock;
extern SOCKET clientsock;
extern SOCKET hSocket;
extern CS* C_S;									// ���� Linked		
extern sockaddr_in addr_server;
extern sockaddr_in addr_client;
extern int addrlen_clt;
extern int nReturn;
extern WSADATA wsadata;

/////////////////////////////////////////////////////////////

// �������� ����/////////////////////////////////////////////

extern BOOL CONN_ST;						// ���� ���� ����
extern const unsigned short g_uPort;		// ��Ʈ �ѹ�
extern const int buflen;					// ���� ũ��
extern TCHAR buf[];

/////////////////////////////////////////////////////////////
extern SEAT* hSeat[30];
extern HWND hWndMain;		// ���� ȭ�� �ڵ�

/*--------------------------------------------------------
 create(): ���� ����ü �Ҵ�
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
 SVR_Open(): ���� ����
--------------------------------------------------------*/
void SVR_Open() {
	HANDLE hThread;
	DWORD ThreadID;
	u_long mode = 1; // ����ŷ ���
	CS* S;

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
		nReturn = listen(listensock, 1);

		// accept ������ ����
		CloseHandle(hThread = CreateThread(NULL, 0, Connect_Thread, &listensock, 0, &ThreadID));
		//WSAAsyncSelect(listensock, hWndMain, WM_USER + 1, FD_ACCEPT | FD_READ | FD_CLOSE);

		CONN_ST = TRUE;
	}
	/*
	for (;;) {
		clientsock = accept(listensock, (sockaddr*)&addr_client, &addrlen_clt);

		if (clientsock != INVALID_SOCKET) {
			MessageBox(hWndMain, "Ŭ���̾�Ʈ�� �����Ͽ����ϴ�.", "server", MB_OK);

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

	// ���� ����ϱ� (���� ��ü, Ŭ���̾�Ʈ �ּ� ����, �ּ� ���� ���� ũ��)
	for (;;) {
		clientsock = accept(*P, (sockaddr*)&addr_client, &addrlen_clt);

		if (clientsock != INVALID_SOCKET) {
			MessageBox(hWndMain, "Ŭ���̾�Ʈ�� �����Ͽ����ϴ�.", "server", MB_OK);

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
 Recv_Thread(LPVOID): ���� ������
--------------------------------------------------------*/
DWORD WINAPI Recv_Thread(LPVOID Param) {
	SOCKET* P = (SOCKET*)Param;
	TCHAR CODE[3], TEXT[256];
	int j;

	while (1) {
		// ������ ���� (��Ĺ ��ü, ���� ���ڿ�, ���ڿ� ũ��, �ɼ�);
		nReturn = recv(*P, buf, buflen, 0);

		if (nReturn == 0 || nReturn == SOCKET_ERROR) {
			MessageBox(hWndMain, "���ſ���", "server", MB_OK);
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
 addsock(SOCKET): ���� ���� �߰�
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
 delsock(SOCKET): ���� ���� ����
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