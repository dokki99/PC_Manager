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
extern DWORD Send_TID;
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
extern HANDLE Charge_Mutex;					// ���� ���ý�

extern HANDLE Send_Mutex;					// �۽� ���ý�

extern HANDLE Transform_Mutex;				// ���� ���ý�

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

		// �¼� Relay ������ ���� (�ϴ� ��� ���·� ����)
		for (i = 0; i < MAX_SEAT; i++) {
			Seat_Thread[i] = CreateThread(NULL, 0, Relay_Thread, &(hSeat[i]->S_num), CREATE_SUSPENDED, &(hSeat[i]->Thread_ID));
		}

		// Send ������ (3��) ���� = �ѹ��� ���� ��û�� ���� �۽� �켱���� ť�� ������ �߻��Ұ��� ���
		for (i = 0; i < MAX_SEND_THREAD; i++) {
			CloseHandle(hThread = CreateThread(NULL, 0, Send_Thread, NULL, 0, &Send_TID));
		}
		
		// accept ������ ����(recv������ ���߻���)
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

			addsock(&clientsock);

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
	CI* F_CI;
	CS* S;
	TCHAR CODE[4], TEXT[256], ID[20], PWD[20], pNum[14], Send_TEXT[300], B_num[2];
	int num = 0, RTime;
	
	while (1) {
		// ������ ���� (��Ĺ ��ü, ���� ���ڿ�, ���ڿ� ũ��, �ɼ�);
		nReturn = recv(*P, buf, buflen, 0);

		if (nReturn != 0 && nReturn != SOCKET_ERROR) {
			Split_C_T(CODE, TEXT);
			
			if (lstrcmp(CODE, "C00") == 0) {
				// �α��� �������� Ȯ��
				Split2(ID, PWD, TEXT);
				// �� �ܿ� �ð� DB���� ã�� �����ֱ�
				if (Login_Info_Check(ID, PWD, 1) == TRUE && (RTime = Get_Time(ID)) != -1) {
					Add_Customer(ID, RTime);
					Add_MAP(P, ID);
					wsprintf(TEXT, "%d", RTime);
					lstrcpy(Send_TEXT, TEXT);
				}
				else {
					lstrcpy(Send_TEXT, "FAIL");
				}
					
				WaitForSingleObject(Transform_Mutex, INFINITE);
				Transform_Text("S00", Send_TEXT, P);
				ReleaseMutex(Transform_Mutex);
			}
			else if (lstrcmp(CODE, "C01") == 0) {
				// �ڵ��� ��ȣ�� ���̵� ã�Ƽ� �����ֱ�
				Find_ID(ID, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, TEXT);
				
				WaitForSingleObject(Transform_Mutex, INFINITE);
				Transform_Text("S01", Send_TEXT, P);
				ReleaseMutex(Transform_Mutex);
			}
			else if (lstrcmp(CODE, "C02") == 0) {
				// ID�� ��ȭ��ȣ�� �ް� PW�ʱ�ȭ �����ֱ�
				Login_Info_Check(ID, pNum, 2) != TRUE || PWD_Reset(ID,pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, "����� �ʱ�ȭ �Ǿ����ϴ� 123456789a");

				WaitForSingleObject(Transform_Mutex, INFINITE);
				Transform_Text("S02", Send_TEXT, P);
				ReleaseMutex(Transform_Mutex);
			}
			else if (lstrcmp(CODE, "C03") == 0) {
				// ȸ������ �������� Ȯ�� �� ����
				Login_Info_Check(ID, pNum, 3) == TRUE || Regist_Customer(ID, PWD,pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, "ȸ�������� �Ϸ�Ǿ����ϴ�!!");
				
				WaitForSingleObject(Transform_Mutex, INFINITE);
				Transform_Text("S03", Send_TEXT, P);
				ReleaseMutex(Transform_Mutex);

				// �� �������� ȸ�������� �ٷ� �α����� �����ϰ� ���� ����غ���
			}
			else if (lstrcmp(CODE,"C04") == 0) {				
				// ������ Ŭ���̾�Ʈ���� ���� �¼� ���¸� �����ֱ�
				WaitForSingleObject(Transform_Mutex, INFINITE);
				Transform_Text("S04", "", P);
				ReleaseMutex(Transform_Mutex);
			}
			else if (lstrcmp(CODE, "C05") == 0) {
				// �¼������� Ŭ���̾�Ʈ���� ���� ���п��� �۽�
				num = atoi(TEXT);
				WaitForSingleObject(Seat_Mutex[num - 1], INFINITE);
				
				if (hSeat[num - 1]->State == 0) {
					// �¼� ���� �Ϸ�
					// �¼� ����ü ������ �� �������ֱ�
					hSeat[num - 1]->State = 1;
					// hSeat[num - 1]->Client_Sock = P;
					// hSeat[num - 1]->Client_Info �� ���� ��Ƽ� ���� �־��ݽô�
					// hSeat[num - 1]->Thread_ID �� CreateThread ���� �̹� ������ֽ��ϴ�.
					ResumeThread(Seat_Thread[num - 1]);	// ��Ī�� �����带 ������ ���� �����ݴϴ�.
					// ���Ŀ� �ֹ�/�ڸ��̵�/�ð��߰��� ���� �۾��� �¼������忡�� �ذ�˴ϴ�.
					lstrcpy(TEXT, "SUCCESS");
				}
				else {
					// �¼� ���� ����
					lstrcpy(TEXT, "FAIL");
				}
				
				Update_Seat_Code();			// �¼� �ֽ�ȭ

				// Ŭ���̾�Ʈ ��ü�� �ֽ� �¼� ���� ����
				if (lstrcmp(TEXT, "SUCCESS") == 0) {
					S = C_S;
					while (S->link != NULL) {
						S = S->link;
						Transform_Text("S04", TEXT, S->Sock);
					}
				}

				// �¼� ���� ����/���� �޽��� ����
				Transform_Text("S05", TEXT, P);

				ReleaseMutex(Seat_Mutex[num - 1]);
			}
			else if (lstrcmp(CODE, "C06") == 0) {
				// ������ ������� (���� �¼��� �������� �� ���)
				// DB�� ���̵� �ִ��� Ȯ���ϰ� ���� ������ �ִ��� Ȯ��
				// ���� �켱����ť�� ��ġ�Ѵ��� ó���� �ٽ� �뺸
				Split2(ID, B_num, TEXT);
				if(Login_Info_Check(ID,"",3) == TRUE && User_State(ID) == TRUE) {
					F_CI = Find_Customer_Info(ID);
					WaitForSingleObject(Charge_Mutex, INFINITE);
					Enque_CQ(F_CI, B_num);
					ReleaseMutex(Charge_Mutex);
				}
				else {
					// fail ó��
					WaitForSingleObject(Send_Mutex, INFINITE);
					Enque_SQ("FAIL", P);
					ReleaseMutex(Send_Mutex);
				}
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
 SVR_Close(): ���� ����
--------------------------------------------------------*/
void SVR_Close() {
	int i;
	CS* S;

	// �¼� ������/���ý� �ڵ� ��ȯ
	for (i = 0; i < MAX_SEAT; i++) {
		CloseHandle(Seat_Thread[i]);
		CloseHandle(Seat_Mutex[i]);
	}

	// �۽� ���ý� �ڵ� ��ȯ
	CloseHandle(Send_Mutex);
	
	// ���� ���ý� �ڵ� ��ȯ
	CloseHandle(Charge_Mutex);
	
	// �������� ����
	while (C_S->link != NULL) {
		S = C_S->link;
		delsock(S->Sock);
	}
}