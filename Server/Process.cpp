#include "Process.h"

extern HWND hWndMain;
extern SEAT *hSeat[MAX_SEAT];
extern TCHAR Seat_Code[MAX_SEAT + 1];
extern TCHAR buf[];

extern HANDLE Charge_Mutex;
extern HANDLE Send_Mutex;

extern CQ *Charge_Front, *Charge_Rear;			// �ֹ� �켱���� ť
extern SQ *Send_Front, *Send_Rear;				// �۽� �켱���� ť
extern MAP *hMap;								// ���� ����ü

extern int Local_Time;							// ���α׷� ���� �ð�

/*--------------------------------------------------------
 Relay_Thread(LPVOID) : �¼� ������
-------------------------------------------------------- */
DWORD WINAPI Relay_Thread(LPVOID Param) {
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
 Charge_Process(LPVOID NULL) : ť�� ��� �ֹ��� ����������
 ó���ϴ� ������
-------------------------------------------------------- */
DWORD WINAPI Charge_Process(LPVOID Param) {
	CQ* C;
	int C_Time, Total_Second;
	while (1) {
		if (!IsEmpty_CQ()) {
			C_Time = 0;
			//���⼭ ��ť���ָ鼭 ó���մϴ�
			WaitForSingleObject(Charge_Mutex,INFINITE);
			C = Deque_CQ();
			ReleaseMutex(Charge_Mutex);
			
			// DB�� �ݿ��ϰ� Client���� ������ �ð� �����ֱ�
			C_Time = Get_Seconds_Btn(C->Charge_BTN_N);

			// DB �ݿ��� �ð� = ������ �ð� + ���� ���� �մ� �ð�
			Total_Second = C_Time + C->Customer_Info->RemainTime;

			// ���� �¼��� �ɾ� �ִٸ� �����ð� ���ֱ�
			if (C->Customer_Info->State == TRUE) {
				Total_Second -= Local_Time;
			}

			// DB�� �����ð� �ݿ�
			if (Charge_Time(C->Customer_Info->ID, Total_Second) == TRUE) {
				// Client_info ����ü �����ð� ������Ʈ �� Client�� �ݿ�(Send)
				Update_Remain_hTime(C->Customer_Info->ID, Total_Second);
				// �¼��ȿ� �ð��� ������Ʈ ������մϴ�
				// ������ Cusotmer���� ���� ���� Sendť�� ����

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
 Create_CQ(): ���� �켱���� ť �ʱ�ȭ �Լ�
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
 Enque_CQ(TCHAR* ,TCHAR*): ���� �켱���� ť ��ť
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
 Deque_CQ(): ���� �켱���� ť ��ť
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
 IsEmpty_CQ(): �¼� �켱���� ť ������� Ȯ���ϴ� �Լ�
--------------------------------------------------------*/
BOOL IsEmpty_CQ() {
	return (((Charge_Front->link == NULL) && (Charge_Rear->link == NULL)) ? TRUE : FALSE);
}


/*--------------------------------------------------------
 Create_SQ(): �۽� �켱���� ť �ʱ�ȭ �Լ�
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
 Enque_SQ(TCHAR* ,SOCKET): �۽� �켱���� ť ��ť
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
 Deque_SQ(): �۽� �켱���� ť ��ť
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
 IsEmpty_SQ(): �۽� �켱���� ť ������� Ȯ���ϴ� �Լ�
--------------------------------------------------------*/
BOOL IsEmpty_SQ() {
	return (((Send_Front->link == NULL) && (Send_Rear->link == NULL)) ? TRUE : FALSE);
}

/*--------------------------------------------------------
 Transform_Text(cosnt TCHAR*, const TCHAR*, SOCKET*):
 Client�� ���� �ؽ�Ʈ ����� �۽� �켱���� ť�� ��� �Լ�
--------------------------------------------------------*/
void Transform_Text(const TCHAR* CODE, const TCHAR* TEXT, SOCKET* P) {
	TCHAR Send_TEXT[300];

	if (lstrcmp(CODE, "S00") == 0) {
		// �ش� ���Ͽ� [SUCCESS / FAIL] �޼��� ������(�α���)
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