#include "Process.h"

extern HWND hWndMain;
extern SEAT *hSeat[MAX_SEAT];
extern TCHAR Seat_Code[MAX_SEAT + 1];
extern TCHAR buf[];

extern CQ *Charge_Front, *Charge_Rear;			// �ֹ� �켱���� ť

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
 Split_I_P(TCHAR* ID, TCHAR* PWD, TCHAR* TEXT) : �ؽ�Ʈ�� 
 ID�� PWD�� �и��ϴ� �Լ�
-------------------------------------------------------- */
void Split_I_P(TCHAR* ID, TCHAR* PWD, TCHAR* TEXT) {
	int i,j;

	lstrcpy(ID, "");
	lstrcpy(PWD, "");

	for (i = 0; i < lstrlen(TEXT); i++) {
		if (TEXT[i] != '-') {
			ID[i] = TEXT[i];
		}
		else {
			break;
		}
	}
	ID[i] = '\0';

	for (j = i + 1; j < lstrlen(TEXT); j++) {
		PWD[j - (i + 1)] = TEXT[j];
	}

	PWD[j - (i + 1)] = '\0';

}

/*--------------------------------------------------------
 Create_CQ(): �¼� �켱���� ť �ʱ�ȭ �Լ�
--------------------------------------------------------*/
CQ* Create_CQ() {
	CQ* N;

	N = (CQ*)malloc(sizeof(CQ));

	N->Client_Info = NULL;
	lstrcpy(N->Charge_Amount, "");
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 Enque_CQ(CQ*): �¼� �켱���� ť ��ť
--------------------------------------------------------*/
void Enque_CQ(CQ* Charge) {
	CQ* N;

	N = Create_CQ();
	N = Charge;
	
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
 Deque_CQ(): �¼� �켱���� ť ��ť
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