#include "Process.h"

extern HWND hWndMain;
extern SEAT *hSeat[MAX_SEAT];
extern TCHAR Seat_Code[MAX_SEAT + 1];
extern TCHAR buf[];

extern HANDLE Seat_Thread[MAX_SEAT];		// �¼� ������

extern HANDLE Charge_Mutex;					// ���� ���ý�
extern HANDLE Send_Mutex;					// �۽� ���ý�
extern HANDLE Seat_Mutex[MAX_SEAT];			// �¼� ���ؽ�

extern CS *C_S;									// ���� ���� Ŭ���̾�Ʈ ����
extern MQ *Message_Front, *Message_Rear;		// �޽��� ó�� ť
extern SQ *Send_Front, *Send_Rear;				// �۽� ť
extern CQ *Charge_Front, *Charge_Rear;			// �ֹ� ť
extern MAP *hMap;								// ���� ����ü

extern int Local_Time;							// ���α׷� ���� �ð�

/*--------------------------------------------------------
 Relay_Thread(LPVOID) : �¼� ������
-------------------------------------------------------- */
DWORD WINAPI Relay_Thread(LPVOID Param) {
	// �ֹ�, ���, ä���� �����ؾ߉� (����ť)
	int* num_adress = (int*)Param;
	int num = *num_adress;
	TCHAR as[200];

	wsprintf(as, "%d", num);

	MessageBox(hWndMain, as, "!", MB_OK);

	while (1) {

	}
	return 0;
}

/*--------------------------------------------------------
 Message_Process(LPVOID) : ť�� ��� �޽����� ����������
 ó���ϴ� ������
-------------------------------------------------------- */
DWORD WINAPI Message_Process(LPVOID Param) {
	MQ* M;
	MAP* MP;
	CS* S;
	CI* F_CI = NULL;
	TCHAR ID[20], PWD[20], Send_TEXT[300], pNum[14], B_num[2];
	int num, RTime;

	while (1) {
		if (!IsEmpty_MQ()) {
			M = Deque_MQ();
			
			if (lstrcmp(M->CODE, "C00") == 0) {
				// �α��� �������� Ȯ��
				Split2(ID, PWD, M->TEXT);

				// �� �ܿ� �ð� DB���� ã�� �����ֱ�
				if (Login_Info_Check(ID, PWD, 1) == TRUE && (RTime = Get_Time(ID)) != -1) {
					Add_Customer(ID, RTime);
					Add_MAP(M->Client_Sock, ID);
					wsprintf(M->TEXT, "%d", RTime);
					lstrcpy(Send_TEXT, M->TEXT);
				}
				else {
					lstrcpy(Send_TEXT, "FAIL");
				}

				WaitForSingleObject(Send_Mutex, INFINITE);
				Transform_Text("S00", Send_TEXT, M->Client_Sock);
				ReleaseMutex(Send_Mutex);
			}
			else if (lstrcmp(M->CODE, "C01") == 0) {
				// �ڵ��� ��ȣ�� ���̵� ã�Ƽ� �����ֱ�
				Find_ID(ID, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, M->TEXT);

				WaitForSingleObject(Send_Mutex, INFINITE);
				Transform_Text("S01", Send_TEXT, M->Client_Sock);
				ReleaseMutex(Send_Mutex);
			}
			else if (lstrcmp(M->CODE, "C02") == 0) {
				// ID�� ��ȭ��ȣ�� �ް� PW�ʱ�ȭ �����ֱ�
				Login_Info_Check(ID, pNum, 2) != TRUE || PWD_Reset(ID, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, "����� �ʱ�ȭ �Ǿ����ϴ� 123456789a");

				WaitForSingleObject(Send_Mutex, INFINITE);
				Transform_Text("S02", Send_TEXT, M->Client_Sock);
				ReleaseMutex(Send_Mutex);
			}
			else if (lstrcmp(M->CODE, "C03") == 0) {
				// ȸ������ �������� Ȯ�� �� ����
				Login_Info_Check(ID, pNum, 3) == TRUE || Regist_Customer(ID, PWD, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, "ȸ�������� �Ϸ�Ǿ����ϴ�!!");

				WaitForSingleObject(Send_Mutex, INFINITE);
				Transform_Text("S03", Send_TEXT, M->Client_Sock);
				ReleaseMutex(Send_Mutex);

				// �� �������� ȸ�������� �ٷ� �α����� �����ϰ� ���� ����غ���
			}
			else if (lstrcmp(M->CODE, "C04") == 0) {
				// ������ Ŭ���̾�Ʈ���� ���� �¼� ���¸� �����ֱ�
				WaitForSingleObject(Send_Mutex, INFINITE);
				Transform_Text("S04", "", M->Client_Sock);
				ReleaseMutex(Send_Mutex);
			}
			else if (lstrcmp(M->CODE, "C05") == 0) {
				// �¼������� Ŭ���̾�Ʈ���� ���� ���п��� �۽�
				num = atoi(M->TEXT);
				WaitForSingleObject(Seat_Mutex[num - 1], INFINITE);

				if (hSeat[num - 1]->State == 0) {
					// �¼� ���� �Ϸ�
					// �¼� ����ü ������ �� �������ֱ�
					hSeat[num - 1]->State = 1;
					MP = hMap;
					while (MP->link != NULL) {
						if (MP->Client_Sock == M->Client_Sock) {
							// ���� �¼��� ���� ���� ���°��� �����մϴ�.
							MP->Client_Info->State = TRUE;

							// ������ ���� SEAT����ü�� �־��ݴϴ�
							hSeat[num - 1]->Client_S_I = MP;
							break;
						}
					}
					ResumeThread(Seat_Thread[num - 1]);	// ��Ī�� �����带 ������ ���� �����ݴϴ�.
					// ���Ŀ� �ֹ�/�ڸ��̵�/�ð��߰��� ���� �۾��� �¼������忡�� �ذ�˴ϴ�.
					lstrcpy(M->TEXT, "SUCCESS");
				}
				else {
					// �¼� ���� ����
					lstrcpy(M->TEXT, "FAIL");
				}

				Update_Seat_Code();			// �¼� �ֽ�ȭ

				// Ŭ���̾�Ʈ ��ü�� �ֽ� �¼� ���� ����
				if (lstrcmp(M->TEXT, "SUCCESS") == 0) {
					S = C_S;
					while (S->link != NULL) {
						S = S->link;
						Transform_Text("S04", M->TEXT, S->Sock);
					}
				}

				// �¼� ���� ����/���� �޽��� ����
				Transform_Text("S05", M->TEXT, M->Client_Sock);

				ReleaseMutex(Seat_Mutex[num - 1]);
			}
			else if (lstrcmp(M->CODE, "C06") == 0) {
				// ������ �������
				// DB�� ���̵� �ִ��� Ȯ���ϰ� ���� ������ �ִ��� Ȯ��
				// ���� ť�� ��ġ�Ѵ��� ó���� �ٽ� �뺸

				Split2(ID, B_num, M->TEXT);
				if (Login_Info_Check(ID, "", 3) == TRUE && User_State(ID) == TRUE) {
					Find_Customer_Info(F_CI,ID);
					// �¼��� �����ϰ��ִ� ���� �¼� ���μ������� ó��
					if (F_CI->State == TRUE) {
						if ((num = Find_Seat_Num(F_CI->ID)) != -1) {
							WaitForSingleObject(Seat_Mutex[num - 1], INFINITE);

							ReleaseMutex(Seat_Mutex[num - 1]);
						}
						else {
							// ����ó��
						}
					}
					else {
						WaitForSingleObject(Charge_Mutex, INFINITE);
						Enque_CQ(F_CI, B_num);
						ReleaseMutex(Charge_Mutex);
					}
				}
				else {
					// fail ó��
					WaitForSingleObject(Send_Mutex, INFINITE);
					Enque_SQ("FAIL", M->Client_Sock);
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
 Charge_Process(LPVOID NULL) : ť�� ��� �ֹ��� ����������
 ó���ϴ� ������
-------------------------------------------------------- */
DWORD WINAPI Charge_Process(LPVOID Param) {
	CQ* C;
	int C_Time, Total_Second;
	SOCKET* S = NULL;
	TCHAR Send_TEXT[256];

	while (1) {
		if (!IsEmpty_CQ()) {
			C_Time = 0;
			//���⼭ ��ť���ָ鼭 ó���մϴ�
			WaitForSingleObject(Charge_Mutex,INFINITE);
			C = Deque_CQ();
			ReleaseMutex(Charge_Mutex);
			
			// Client�� ������ �ð� ��������
			C_Time = Get_Seconds_Btn(C->Charge_BTN_N);

			// DB �ݿ��� �ð� = ������ �ð� + ���� ���� �մ� �ð�
			Total_Second = C_Time + C->Customer_Info->RemainTime;

			// ���� �¼��� �ɾ� �ִٸ� �����ϴ� ���� �����ð� ���ֱ�
			if (C->Customer_Info->State == TRUE) {
				Total_Second -= Local_Time;
			}

			// DB�� �����ð� �ݿ�
			if (Charge_Time(C->Customer_Info->ID, Total_Second) == TRUE) {
				// Client_info ����ü �����ð� ������Ʈ
				Update_Remain_hTime(C->Customer_Info->ID, Total_Second);

				// ���� ��û���� Client���� ���� �����ð� �����ֱ�(Send Thread�� �̿��� ��)
				Find_Customer_Sock(S, C->Customer_Info->ID);
				if (S != NULL) {
					// �� ����ü �����ð� ������Ʈ
					C->Customer_Info->RemainTime = Total_Second;
					// DB�� ����� �ð��� Sendť�� ��� Client�� �ݿ�
					wsprintf(Send_TEXT, "%d", Total_Second);
					Transform_Text("S00", Send_TEXT, S);
				}
				else {
					// ���� ����(������ �ѹ� ��Ű�� Ŭ���̾�Ʈ���� ���� �޽���)
				}
			}
			else {
				// ���� ����
			}

			free(C);	// �޸� ��ȯ
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Update_Seat_Code() : �¼� ��Ȳ �ֽ�ȭ
-------------------------------------------------------- */
void Update_Seat_Code() {
	int i;
		
	for (i = 0; i < 30; i++) {
		Seat_Code[i] = ((hSeat[i]->State != 0) ? '1' : '0');
	}

	Seat_Code[MAX_SEAT] = '\0';
}



/*--------------------------------------------------------
 Split2(TCHAR* , TCHAR* , TCHAR* TEXT) : �ؽ�Ʈ�� 
 2�������� �ѹ��� �и��ϴ� �Լ�
-------------------------------------------------------- */
void Split2(TCHAR* TEXT1, TCHAR* TEXT2, const TCHAR* TEXT) {
	int i,j;

	lstrcpy(TEXT1, "");
	lstrcpy(TEXT2, "");

	for (i = 0; i < lstrlen(TEXT); i++) {
		if (TEXT[i] != '-') {
			TEXT1[i] = TEXT[i];
		}
		else {
			break;
		}
	}
	TEXT1[i] = '\0';

	for (j = i + 1; j < lstrlen(TEXT); j++) {
		TEXT2[j - (i + 1)] = TEXT[j];
	}

	TEXT2[j - (i + 1)] = '\0';

}
/*--------------------------------------------------------
 Int_To_Time(int, TCHAR*) : �ʴ��� �ð��� 00:00:00 ��������
 �ٲ���
-------------------------------------------------------- */
void Int_To_Time(int S, TCHAR* T) {
	int Hour, Minute, Second;

	Hour = S / 3600;
	Minute = (S % 3600) / 60;
	Second = (S % 3600) % 60;

	wsprintf(T, "%d:%d:%d", Hour, Minute, Second);
}

/*--------------------------------------------------------
 Time_To_Int(TCHAR*) : 00:00:00 �ð��� �ʴ��� ��������
 �ٲ���
-------------------------------------------------------- */
int Time_To_Int(const TCHAR* T) {
	int i, j, k = 0, total;
	TCHAR Hour[4], Minute[3], Second[3];

	for (i = 0; i < lstrlen(T); i++) {
		if (T[i] == ':') break;
		Hour[k++] = T[i];
	}
	Hour[k] = '\0';

	k = 0;
	for (j = i + 1; j < lstrlen(T); j++) {
		if (T[j] == ':') break;
		Minute[k++] = T[j];
	}
	Minute[k] = '\0';
	
	k = 0;
	for (i = j + 1; i < lstrlen(T); i++) {
		if (T[i] == ':') break;
		Second[k++] = T[i];
	}
	Second[k] = '\0';

	total = 3600 * atoi(Hour) + 60 * atoi(Minute) + atoi(Second);

	return total;
}


/*--------------------------------------------------------
 Get_Seconds_Btn(int): ��ư ��ȣ�� ���� �ʴ��� ����
--------------------------------------------------------*/
int Get_Seconds_Btn(int num) {
	int Seconds = 0;

	switch (num) {
	case 1:
		// ��ư1: 1,000��(1�ð�) ����
		Seconds = 3600;
		break;
	case 2:
		// ��ư2: 2,000��(2�ð�) ����
		Seconds = 7200;
		break;	
	case 3:
		// ��ư3: 3,000��(3�ð�) ����
		Seconds = 10800;
		break;
	case 4:
		// ��ư4: 5,000��(5�ð�) ����
		Seconds = 18000;
		break;
	case 5:
		// ��ư5: 10,000��(10�ð�) ����
		Seconds = 36000;
		break;
	case 6:
		// ��ư6: 15,000��(17�ð�) ����
		Seconds = 61200;
		break;
	case 7:
		// ��ư7: 20,000��(25�ð�) ����
		Seconds = 90000;
		break;
	case 8:
		// ��ư8: 30,000��(35�ð�) ����
		Seconds = 126000;
		break;
	case 9:
		// ��ư9: 40,000��(50�ð�) ����
		Seconds = 180000;
		break;
	case 10:
		// ��ư10: 50,000��(60�ð�) ����
		Seconds = 216000;
		break;
	default:
		break;
	}

	return Seconds;
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
 Enque_MQ(SOCKET*, const TCHAR*, const TCHAR*): �޽��� 
 ó�� ť ��ť
--------------------------------------------------------*/
void Enque_MQ(SOCKET* Client_Sock, const TCHAR* CODE, const TCHAR* TEXT) {
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
}

/*--------------------------------------------------------
 Deque_MQ(): �޽��� ó�� ť ��ť
--------------------------------------------------------*/
MQ* Deque_MQ() {
	MQ* E, * N;

	N = Create_MQ();

	E = Message_Front->link;
	*N = *E;

	Message_Front = Message_Front->link;
	if (Message_Front->link == NULL) {
		Message_Rear->link = NULL;
	}

	free(E);
	return N;
}

/*--------------------------------------------------------
 IsEmpty_MQ(): �޽��� ó�� ť ������� Ȯ���ϴ� �Լ�
--------------------------------------------------------*/
BOOL IsEmpty_MQ() {
	return (((Message_Front->link == NULL) && (Message_Rear->link == NULL)) ? TRUE : FALSE);
}

/*--------------------------------------------------------
 Create_CQ(): ���� ť �ʱ�ȭ �Լ�
--------------------------------------------------------*/
CQ* Create_CQ() {
	CQ* N;

	N = (CQ*)malloc(sizeof(CQ));

	N->Customer_Info = NULL;
	N->Charge_BTN_N = 0;
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 Enque_CQ(TCHAR* ,TCHAR*): ���� ť ��ť
--------------------------------------------------------*/
void Enque_CQ(CI* Customer_Info, const TCHAR* B_num) {
	CQ* N;

	N = Create_CQ();
	N->Customer_Info = Customer_Info;
	N->Charge_BTN_N = atoi(B_num);

	if (Charge_Front->link == NULL && Charge_Rear->link == NULL) {
		Charge_Front->link = N;
		Charge_Rear->link = N;
	}
	else {
		N->link = Charge_Rear->link;
		Charge_Rear->link = N;
	}
}

/*--------------------------------------------------------
 Deque_CQ(): ���� ť ��ť
--------------------------------------------------------*/
CQ* Deque_CQ() {
	CQ* E, *N;

	N = Create_CQ();

	E = Charge_Front->link;
	*N = *E;

	Charge_Front = Charge_Front->link;
	if (Charge_Front->link == NULL) {
		Charge_Rear->link = NULL;
	}

	free(E);
	return N;
}

/*--------------------------------------------------------
 IsEmpty_CQ(): �¼� ť ������� Ȯ���ϴ� �Լ�
--------------------------------------------------------*/
BOOL IsEmpty_CQ() {
	return (((Charge_Front->link == NULL) && (Charge_Rear->link == NULL)) ? TRUE : FALSE);
}



/*--------------------------------------------------------
 Transform_Text(cosnt TCHAR*, const TCHAR*, SOCKET*):
 Client�� ���� �ؽ�Ʈ ����� �۽� ť�� ��� �Լ�
--------------------------------------------------------*/
void Transform_Text(const TCHAR* CODE, const TCHAR* TEXT, SOCKET* P) {
	TCHAR Send_TEXT[300];

	if (lstrcmp(CODE, "S00") == 0) {
		// �ش� ���Ͽ� [�����ð� / FAIL] �޼��� ������
		wsprintf(Send_TEXT, "%s-%s", CODE, TEXT);
		Enque_SQ(Send_TEXT, P);
	}
	else if (lstrcmp(CODE, "S01") == 0) {
		// ��й�ȣ �ʱ�ȭ �޼��� ������
		wsprintf(Send_TEXT, "%s-%s", CODE, TEXT);
		Enque_SQ(Send_TEXT, P);
	}
	else if (lstrcmp(CODE, "S02") == 0) {
		// �ش� ���Ͽ� [SUCCESS / FAIL] �޼��� ������(��� �ʱ�ȭ)
		wsprintf(Send_TEXT, "%s-%s", CODE, TEXT);
		Enque_SQ(Send_TEXT, P);
	}
	else if (lstrcmp(CODE, "S03") == 0) {
		// �ش� ���Ͽ� [SUCCESS / FAIL] �޼��� ������(ȸ������)
		wsprintf(Send_TEXT, "%s-%s", CODE, TEXT);
		Enque_SQ(Send_TEXT, P);
	}
	else if (lstrcmp(CODE, "S04") == 0) {
		wsprintf(Send_TEXT, "%s-%s", CODE, Seat_Code);
		Enque_SQ(Send_TEXT, P);
	}
	else if (lstrcmp(CODE, "S05") == 0) {
		// �ش� ���Ͽ� [SUCCESS / FAIL] �޼��� ������
		wsprintf(Send_TEXT, "%s-%s", CODE, TEXT);
		Enque_SQ(Send_TEXT, P);
	}

}