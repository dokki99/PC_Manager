#include "Process.h"

extern BOOL CONN_ST;									// ���� ���� ����
extern HWND hWndMain;
extern MAP *hMap;												// ���� ����ü
extern SEAT **hSeat;											
extern TCHAR Seat_Code[MAX_SEAT + 1];
extern TCHAR buf[];

extern HANDLE Seat_Thread[MAX_SEAT];							// �¼� ������

extern HANDLE Message_Mutex;				// �޽��� ó�� ���ý�
extern HANDLE Charge_Mutex;										// ���� ���ý�
extern HANDLE Send_Mutex;										// �۽� ���ý�
extern HANDLE Seat_Mutex[MAX_SEAT];								// �¼� ���ؽ�


extern HANDLE Order_Thread;										// �ֹ� ������
extern HANDLE Order_Mutex;										// �ֹ� ���ý�

extern HANDLE* Order_Sub_Thread;								// �ֹ� ó�� ������
extern HANDLE* Order_Sub_Mutex;									// �ֹ� ó�� ���ý�

extern CCI *C_CI;												// ���� ���� Ŭ���̾�Ʈ ���� ����
extern MQ *Message_Front, *Message_Rear;						// �޽��� ó�� ť
extern CQ *Charge_Front, *Charge_Rear;							// �ֹ� ť
extern SQ *Send_Front, *Send_Rear;								// �۽� ť
extern SEAT_M_Q **Seat_Message_Front, **Seat_Message_Rear;		// �¼� �޽��� ť
extern OQ* Order_Front, * Order_Rear;							// �ֹ� ť
extern OSQ **Order_Sub_Front, **Order_Sub_Rear;					// �ֹ� ���� ť
extern int* Order_Sub_Num;										// �ֹ� ���� ��ȣ

extern int Local_Time;											// ���α׷� ���� �ð�

/*--------------------------------------------------------
 Relay_Thread(LPVOID) : �¼� ������
 num�� 1���� ����
-------------------------------------------------------- */
DWORD WINAPI Relay_Thread(LPVOID Param) {
	// �ֹ�, ���, ä���� �����ؾ߉�
	int* num_adress = (int*)Param;
	int num = *num_adress;
	SEAT_M_Q* M;

	while (CONN_ST) {
		M = Deque_SEAT_M_Q(num);

		if (M != NULL) {
			if (M->Command == 1) {
				// ����
				Enque_CQ(hSeat[num - 1]->Client_S_I->Client_Info, M->TEXT);
			}
			else if (M->Command == 2) {
				// �ֹ�
				Enque_OQ(num, M->TEXT);
				hSeat[num - 1]->P_State = 1;
			}
			else if (M->Command == 3) {
				// ä��

			}
			else if (M->Command == 4) {
				// �ڸ��̵�
			}
			else {
				// ����
			}
			free(M);
		}
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
	CCI* L_CI;
	CI* F_CI = NULL;
	TCHAR ID[20], PWD[20], Send_TEXT[300], pNum[12], B_num[2];
	int num, RTime;

	while (CONN_ST) {
 		M = Deque_MQ();

		if (M != NULL) {
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

				Transform_Text("S00", Send_TEXT, M->Client_Sock);
			}
			else if (lstrcmp(M->CODE, "C01") == 0) {
				// �ڵ��� ��ȣ�� ���̵� ã�Ƽ� �����ֱ�
				lstrcpy(pNum, M->TEXT);
				Find_ID(ID, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, ID);

				Transform_Text("S01", Send_TEXT, M->Client_Sock);
			}
			else if (lstrcmp(M->CODE, "C02") == 0) {
				// ID�� ��ȭ��ȣ�� �ް� PW�ʱ�ȭ �����ֱ�
				Login_Info_Check(ID, pNum, 2) != TRUE || PWD_Reset(ID, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, "����� �ʱ�ȭ �Ǿ����ϴ� 123456789a");

				Transform_Text("S02", Send_TEXT, M->Client_Sock);
			}
			else if (lstrcmp(M->CODE, "C03") == 0) {
				// ȸ������ �������� Ȯ�� �� ����
				Login_Info_Check(ID, pNum, 3) == TRUE || Regist_Customer(ID, PWD, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, "ȸ�������� �Ϸ�Ǿ����ϴ�!!");

				Transform_Text("S03", Send_TEXT, M->Client_Sock);
		
				// �� �������� ȸ�������� �ٷ� �α����� �����ϰ� ���� ����غ���
			}
			else if (lstrcmp(M->CODE, "C04") == 0) {
				// ������ Ŭ���̾�Ʈ���� ���� �¼� ���¸� �����ֱ�
				Transform_Text("S04", "", M->Client_Sock);
			}
			else if (lstrcmp(M->CODE, "C05") == 0) {
				// �¼������� Ŭ���̾�Ʈ���� ���� ���п��� �۽�
				num = atoi(M->TEXT);
				WaitForSingleObject(Seat_Mutex[num - 1], INFINITE);

				if (hSeat[num - 1]->S_State == 0) {
					// �¼� ���� �Ϸ�
					// �¼� ����ü ������ �� �������ֱ�
					hSeat[num - 1]->S_State = 1;
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
					ResumeThread(Seat_Thread[num - 1]); // ��Ī�� �����带 ������ ���� �����ݴϴ�.
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
					L_CI = C_CI;
					while (L_CI->link != NULL) {
						L_CI = L_CI->link;
						Transform_Text("S04", M->TEXT, L_CI->Sock);
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
					Find_Customer_Info(F_CI, ID);
					// �¼��� �����ϰ��ִ� ���� �¼� ���μ������� ó��
					if (F_CI->State == TRUE) {
						if ((num = Find_Seat_Num(F_CI->ID)) != -1) {
							Enque_SEAT_M_Q(num, 1, B_num);		// �˸´� �¼������忡 �޽��� ����
						}
						else {
							// ����ó��
						}
					}
					else {
						Enque_CQ(F_CI, B_num);
					}
				}
				else if (lstrcmp(M->CODE, "C07") == 0) {

				}
				else if (lstrcmp(M->CODE, "C08") == 0) {

				}
				else {
					// fail ó��
					Enque_SQ("FAIL", M->Client_Sock);
				}
			}
			free(M);
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
	SOCKET S = INVALID_SOCKET;
	TCHAR Send_TEXT[256];

	while (CONN_ST) {
		//���⼭ ��ť���ָ鼭 ó���մϴ�
		C = Deque_CQ();
	
		if (C != NULL) {
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
				if (S != INVALID_SOCKET) {
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
 Order_Process(LPVOID NULL) : �ֹ��� �ް� �ֹ� ���� ť��
 ����
-------------------------------------------------------- */
DWORD WINAPI Order_Process(LPVOID Param) {
	OQ* O;
	TCHAR ORDER[30], Num[30], Temp[300];
	int i, j, O_Num;
	
	while (CONN_ST) {
		O = Deque_OQ();

		if (O != NULL) {
			// ��ǰ�ڵ� - ���� ������ �ؽ�Ʈ�� �����Ͽ� ������ ó�� ť�� ����
			// ���� ���� ��ǰ�ڵ�-����/��ǰ�ڵ� - ����/ ... ��ǰ�ڵ� - ����/
			while (1) {
				// ���ڿ��� ��������� �ݺ��� ����
				if (O->Product_CODE[0] == '\0')	break;

				// ��ǰ�ڵ� ���
				for (i = 0; i < strlen(O->Product_CODE); i++) {
					if (O->Product_CODE[i] == '-')	break;
					ORDER[i] = O->Product_CODE[i];
				}
				ORDER[i] = '\0';
				j = i;

				// ���� ���
				for (i = j + 1; i < strlen(O->Product_CODE); i++) {
					if (O->Product_CODE[i] == '/')	break;
					Num[i - (j + 1)] = O->Product_CODE[i];
				}
				Num[i - (j + 1)] = '\0';
				O_Num = atoi(Num);
				j = i;

				// �ؽ�Ʈ�� ���������� �ٽ� ���ư��� �ؽ�Ʈ�� ����
				if ((j + 1) <= strlen(O->Product_CODE)) {
					for (i = j + 1; i < strlen(O->Product_CODE); i++) {
						Temp[i - (j + 1)] = O->Product_CODE[i];
					}
					Temp[i - (j + 1)] = '\0';
					lstrcpy(O->Product_CODE, Temp);

					// ������ ��ǰ - ������ ������ ó�� ť�� ��ť
					if (lstrcmp(ORDER, "R00") == 0) {
						// ����
						Enque_OSQ(0, O->S_Num, O_Num * 1);
					}
					else if (lstrcmp(ORDER, "R01") == 0) {
						// ����
						Enque_OSQ(1, O->S_Num, O_Num * 3);
					}
					else if (lstrcmp(ORDER, "R02") == 0) {
						// ����
						Enque_OSQ(2, O->S_Num, O_Num * 10);
					}
					else if (lstrcmp(ORDER, "R03") == 0) {
						// Ȯ�� ����
					}
					else {
						// ����
					}
				}

			}
			free(O);
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Order_Sub_Process() : �ֹ��� �����Ͽ� ó���ϴ� ������
-------------------------------------------------------- */
DWORD WINAPI Order_Sub_Process(LPVOID Param) {
	OSQ* OS;
	int* Num_Adress = (int*)Param;
	int Num = *Num_Adress;

	while (CONN_ST) {
		OS = Deque_OSQ(Num);

		if (OS != NULL) {
			hSeat[Num]->P_State = 0;
			free(OS);
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
		Seat_Code[i] = ((hSeat[i]->S_State != 0) ? '1' : '0');
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
 Create_SEAT_M_Q(): �¼� �޽��� ť �ʱ�ȭ �Լ�
--------------------------------------------------------*/
SEAT_M_Q* Create_SEAT_M_Q() {
	SEAT_M_Q* N;

	N = (SEAT_M_Q*)malloc(sizeof(SEAT_M_Q));

	N->Command = 0;
	lstrcpy(N->TEXT, "");
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 Enque_SEAT_M_Q(int, int, const TCHAR*): �¼� �޽��� ť
 ��ť
--------------------------------------------------------*/
void Enque_SEAT_M_Q(int S_Num, int Command, const TCHAR* TEXT) {
	WaitForSingleObject(Seat_Mutex[S_Num - 1], INFINITE);
	SEAT_M_Q* N;

	N = Create_SEAT_M_Q();

	N->Command = Command;
	lstrcpy(N->TEXT, TEXT);
	N->link = NULL;

	if (Seat_Message_Front[S_Num - 1]->link == NULL && Seat_Message_Rear[S_Num - 1]->link == NULL) {
		Seat_Message_Front[S_Num - 1]->link = N;
		Seat_Message_Rear[S_Num - 1]->link = N;
	}
	else {
		N->link = Seat_Message_Rear[S_Num - 1]->link;
		Seat_Message_Rear[S_Num - 1]->link = N;
	}
	ReleaseMutex(Seat_Mutex[S_Num - 1]);
}

/*--------------------------------------------------------
 Deque_SEAT_M_Q(int ): �¼� �޽��� ť ��ť
--------------------------------------------------------*/
SEAT_M_Q* Deque_SEAT_M_Q(int S_Num) {
	WaitForSingleObject(Seat_Mutex[S_Num - 1], INFINITE);
	SEAT_M_Q* N = NULL, * E;
	
	if (!IsEmpty_SEAT_M_Q(S_Num)) {

		N = Create_SEAT_M_Q();

		E = Seat_Message_Front[S_Num - 1]->link;
		*N = *E;

		Seat_Message_Front[S_Num - 1] = Seat_Message_Front[S_Num - 1]->link;
		if (Seat_Message_Front[S_Num - 1]->link == NULL) {
			Seat_Message_Rear[S_Num - 1]->link = NULL;
		}

		free(E);
	}
	ReleaseMutex(Seat_Mutex[S_Num - 1]);
	
	return N;
}

/*--------------------------------------------------------
 IsEmpty_SEAT_M_Q(): �¼� �޽��� ť ����ִ��� Ȯ��
--------------------------------------------------------*/
BOOL IsEmpty_SEAT_M_Q(int S_Num) {
	return (((Seat_Message_Front[S_Num - 1]->link == NULL) && (Seat_Message_Rear[S_Num - 1]->link == NULL)) ? TRUE : FALSE);
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
	WaitForSingleObject(Charge_Mutex, INFINITE);
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
	ReleaseMutex(Charge_Mutex);
}

/*--------------------------------------------------------
 Deque_CQ(): ���� ť ��ť
--------------------------------------------------------*/
CQ* Deque_CQ() {
	WaitForSingleObject(Charge_Mutex, INFINITE);
	CQ* E, * N = NULL;

	if (!IsEmpty_CQ()) {

		N = Create_CQ();

		E = Charge_Front->link;
		*N = *E;

		Charge_Front = Charge_Front->link;
		if (Charge_Front->link == NULL) {
			Charge_Rear->link = NULL;
		}

		free(E);
	}
	ReleaseMutex(Charge_Mutex);

	return N;
}

/*--------------------------------------------------------
 IsEmpty_CQ(): �¼� ť ������� Ȯ���ϴ� �Լ�
--------------------------------------------------------*/
BOOL IsEmpty_CQ() {
	return (((Charge_Front->link == NULL) && (Charge_Rear->link == NULL)) ? TRUE : FALSE);
}

/*--------------------------------------------------------
 Create_OQ(): �ֹ� ť �ʱ�ȭ �� ����
--------------------------------------------------------*/
OQ* Create_OQ() {
	OQ* N;

	N = (Order_Queue*)malloc(sizeof(Order_Queue));

	N->S_Num = 0;
	lstrcpy(N->Product_CODE, "");
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 Enque_OQ(int, const TCHAR*): �ֹ� ť ��ť
--------------------------------------------------------*/
void Enque_OQ(int S_Num, const TCHAR* Product_CODE) {
	WaitForSingleObject(Order_Mutex, INFINITE);
	OQ* N;

	N = Create_OQ();
	N->S_Num = S_Num;
	lstrcpy(N->Product_CODE, Product_CODE);
	N->link = NULL;

	if (Order_Front->link == NULL && Order_Rear->link == NULL) {
		Order_Front->link = N;
		Order_Rear->link = N;
	}
	else {
		N->link = Order_Rear->link;
		Order_Rear->link = N;
	}
	ReleaseMutex(Order_Mutex);
}

/*--------------------------------------------------------
 Deque_OQ(): �ֹ� ť ��ť
--------------------------------------------------------*/
OQ* Deque_OQ() {
	WaitForSingleObject(Order_Mutex, INFINITE);
	OQ* E, * N = NULL;

	if (!IsEmpty_OQ()) {
		OQ* E, * N;

		N = Create_OQ();

		E = Order_Front->link;
		*N = *E;

		Order_Front = Order_Front->link;
		if (Order_Front->link == NULL) {
			Order_Rear->link = NULL;
		}

		free(E);
	}
	ReleaseMutex(Order_Mutex);

	return N;
}

/*--------------------------------------------------------
 IsEmpty_OQ(): �ֹ� ť ����ִ��� Ȯ��
--------------------------------------------------------*/
BOOL IsEmpty_OQ() {
	return (((Order_Front->link == NULL) && (Order_Rear->link == NULL)) ? TRUE : FALSE);
}

/*--------------------------------------------------------
 Create_OSQ(): �ֹ� ���� ť �ʱ�ȭ
--------------------------------------------------------*/
OSQ* Create_OSQ() {
	OSQ* N;

	N = (OSQ*)malloc(sizeof(OSQ));

	N->S_Num = 0;
	N->S_Cnt = 0;
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 Enque_OSQ(int ,int ,int ): �ֹ� ���� ť ��ť
--------------------------------------------------------*/
void Enque_OSQ(int M_Num,int S_Num, int S_Cnt) {
	WaitForSingleObject(Order_Sub_Mutex[M_Num], INFINITE);
	OSQ* N;

	N = Create_OSQ();
	N->S_Num = S_Num;
	N->S_Cnt = S_Cnt;
	N->link = NULL;

	if (Order_Sub_Front[M_Num - 1]->link == NULL && Order_Sub_Rear[M_Num - 1]->link == NULL) {
		Order_Sub_Front[M_Num - 1]->link = N;
		Order_Sub_Rear[M_Num - 1]->link = N;
	}
	else {
		N->link = Order_Sub_Rear[M_Num - 1]->link;
		Order_Sub_Rear[M_Num - 1]->link = N;
	}
	ReleaseMutex(Order_Sub_Mutex[M_Num]);
}

/*--------------------------------------------------------
 Deque_OSQ(int ): �ֹ� ���� ť ��ť
--------------------------------------------------------*/
OSQ* Deque_OSQ(int M_Num) {
	WaitForSingleObject(Order_Sub_Mutex[M_Num], INFINITE);
	OSQ* E, * N = NULL;

	if (!IsEmpty_OSQ(M_Num)) {
		

		N = Create_OSQ();

		E = Order_Sub_Front[M_Num]->link;
		*N = *E;

		Order_Sub_Front[M_Num] = Order_Sub_Front[M_Num]->link;
		if (Order_Sub_Front[M_Num]->link == NULL) {
			Order_Sub_Rear[M_Num]->link = NULL;
		}

		free(E);
	}
	ReleaseMutex(Order_Sub_Mutex[M_Num]);
	
	return N;
}


/*--------------------------------------------------------
 IsEmpty_OSQ(int ): �ֹ� ���� ť ����ִ��� Ȯ��
--------------------------------------------------------*/
BOOL IsEmpty_OSQ(int M_Num) {
	return (((Order_Sub_Front[M_Num]->link == NULL) && (Order_Sub_Rear[M_Num]->link == NULL)) ? TRUE : FALSE);
}

/*--------------------------------------------------------
 Transform_Text(cosnt TCHAR*, const TCHAR*, SOCKET*):
 Client�� ���� �ؽ�Ʈ ����� �۽� ť�� ��� �Լ�
--------------------------------------------------------*/
void Transform_Text(const TCHAR* CODE, const TCHAR* TEXT, SOCKET P) {
	WaitForSingleObject(Send_Mutex, INFINITE);
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
	ReleaseMutex(Send_Mutex);
}