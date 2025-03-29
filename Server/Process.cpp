#include "Process.h"

extern HWND hWndMain;
extern SEAT *hSeat[MAX_SEAT];
extern TCHAR Seat_Code[MAX_SEAT + 1];
extern TCHAR buf[];

extern PQ *Seat_Front, *Seat_Rear;			// 좌석 우선순위 큐

/*--------------------------------------------------------
 Relay_Thread(LPVOID) : 좌석 스레드
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
 Update_Seat_Code() : 좌석 현황 최신화
-------------------------------------------------------- */
void Update_Seat_Code() {
	int i;
		
	for (i = 0; i < 30; i++) {
		Seat_Code[i] = ((hSeat[i]->State != 0) ? '1' : '0');
	}

	Seat_Code[MAX_SEAT] = '\0';
}

/*--------------------------------------------------------
 Split_C_T(TCHAR* CODE, TCHAR* TEXT) : 수신 받은 텍스트를
 CODE와 TEXT로 분리하는 함수
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
 Create_PQ(): 좌석 우선순위 큐 초기화 함수
--------------------------------------------------------*/
PQ* Create_PQ() {
	PQ* N;

	N = (PQ*)malloc(sizeof(PQ));

	N->num = 0;
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 Enque_PQ(int num): 좌석 우선순위 큐 엔큐
--------------------------------------------------------*/
void Enque_PQ(int num) {
	PQ* N;

	N = Create_PQ();
	N->num = num;

	if (Seat_Front->link == NULL && Seat_Rear->link == NULL) {
		Seat_Front->link = N;
		Seat_Rear->link = N;
	}
	else {
		N->link = Seat_Rear->link;
		Seat_Rear->link = N;
	}
}
/*--------------------------------------------------------
 Deque_PQ(): 좌석 우선순위 큐 디큐
--------------------------------------------------------*/
int Deque_PQ() {
	PQ* E;
	int num;

	E = Seat_Front->link;
	num = E->num;

	Seat_Front = Seat_Front->link;
	if (Seat_Front->link == NULL) {
		Seat_Rear->link = NULL;
	}

	free(E);
	return num;
}

/*--------------------------------------------------------
 IsEmpty_PQ(): 좌석 우선순위 큐 비었는지 확인하는 함수
--------------------------------------------------------*/
BOOL IsEmpty_PQ() {
	return (((Seat_Front->link == NULL) && (Seat_Rear->link == NULL)) ? TRUE : FALSE);
}