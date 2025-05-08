#include "Process.h"

extern HWND hWndMain;
extern SEAT *hSeat[MAX_SEAT];
extern TCHAR Seat_Code[MAX_SEAT + 1];
extern TCHAR buf[];

extern HANDLE Charge_Mutex;
extern HANDLE Send_Mutex;

extern CQ *Charge_Front, *Charge_Rear;			// 주문 우선순위 큐
extern SQ *Send_Front, *Send_Rear;				// 송신 우선순위 큐
extern MAP *hMap;								// 매핑 구조체

extern int Local_Time;							// 프로그램 내부 시간

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
 Charge_Process(LPVOID NULL) : 큐에 담긴 주문을 순차적으로
 처리하는 스레드
-------------------------------------------------------- */
DWORD WINAPI Charge_Process(LPVOID Param) {
	CQ* C;
	int C_Time, Total_Second;
	while (1) {
		if (!IsEmpty_CQ()) {
			C_Time = 0;
			//여기서 디큐해주면서 처리합니다
			WaitForSingleObject(Charge_Mutex,INFINITE);
			C = Deque_CQ();
			ReleaseMutex(Charge_Mutex);
			
			// DB에 반영하고 Client에게 충전된 시간 보내주기
			C_Time = Get_Seconds_Btn(C->Charge_BTN_N);

			// DB 반영할 시간 = 충전할 시간 + 현재 남은 손님 시간
			Total_Second = C_Time + C->Customer_Info->RemainTime;

			// 만약 좌석에 앉아 있다면 지연시간 빼주기
			if (C->Customer_Info->State == TRUE) {
				Total_Second -= Local_Time;
			}

			// DB에 충전시간 반영
			if (Charge_Time(C->Customer_Info->ID, Total_Second) == TRUE) {
				// Client_info 구조체 충전시간 업데이트 후 Client에 반영(Send)
				Update_Remain_hTime(C->Customer_Info->ID, Total_Second);
				// 좌석안에 시간도 없데이트 해줘야합니다
				// 접속한 Cusotmer한테 내용 전달 Send큐에 전달

			}

			free(C);	// 메모리 반환
		}
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
 Split2(TCHAR* , TCHAR* , TCHAR* TEXT) : 텍스트를 
 2문장으로 한번더 분리하는 함수
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
 Int_To_Time(int, TCHAR*) : 초단위 시간을 00:00:00 형식으로
 바꿔줌
-------------------------------------------------------- */
void Int_To_Time(int S, TCHAR* T) {
	int Hour, Minute, Second;

	Hour = S / 3600;
	Minute = (S % 3600) / 60;
	Second = (S % 3600) % 60;

	wsprintf(T, "%d:%d:%d", Hour, Minute, Second);
}

/*--------------------------------------------------------
 Time_To_Int(TCHAR*) : 00:00:00 시간을 초단위 형식으로
 바꿔줌
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
 Get_Seconds_Btn(int): 버튼 번호에 따른 초단위 리턴
--------------------------------------------------------*/
int Get_Seconds_Btn(int num) {
	int Seconds = 0;

	switch (num) {
	case 1:
		// 버튼1: 1,000원(1시간) 충전
		Seconds = 3600;
		break;
	case 2:
		// 버튼2: 2,000원(2시간) 충전
		Seconds = 7200;
		break;	
	case 3:
		// 버튼3: 3,000원(3시간) 충전
		Seconds = 10800;
		break;
	case 4:
		// 버튼4: 5,000원(5시간) 충전
		Seconds = 18000;
		break;
	case 5:
		// 버튼5: 10,000원(10시간) 충전
		Seconds = 36000;
		break;
	case 6:
		// 버튼6: 15,000원(17시간) 충전
		Seconds = 61200;
		break;
	case 7:
		// 버튼7: 20,000원(25시간) 충전
		Seconds = 90000;
		break;
	case 8:
		// 버튼8: 30,000원(35시간) 충전
		Seconds = 126000;
		break;
	case 9:
		// 버튼9: 40,000원(50시간) 충전
		Seconds = 180000;
		break;
	case 10:
		// 버튼10: 50,000원(60시간) 충전
		Seconds = 216000;
		break;
	default:
		break;
	}

	return Seconds;
}

/*--------------------------------------------------------
 Create_CQ(): 충전 우선순위 큐 초기화 함수
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
 Enque_CQ(TCHAR* ,TCHAR*): 충전 우선순위 큐 엔큐
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
 Deque_CQ(): 충전 우선순위 큐 디큐
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
 IsEmpty_CQ(): 좌석 우선순위 큐 비었는지 확인하는 함수
--------------------------------------------------------*/
BOOL IsEmpty_CQ() {
	return (((Charge_Front->link == NULL) && (Charge_Rear->link == NULL)) ? TRUE : FALSE);
}


/*--------------------------------------------------------
 Create_SQ(): 송신 우선순위 큐 초기화 함수
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
 Enque_SQ(TCHAR* ,SOCKET): 송신 우선순위 큐 엔큐
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
 Deque_SQ(): 송신 우선순위 큐 디큐
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
 IsEmpty_SQ(): 송신 우선순위 큐 비었는지 확인하는 함수
--------------------------------------------------------*/
BOOL IsEmpty_SQ() {
	return (((Send_Front->link == NULL) && (Send_Rear->link == NULL)) ? TRUE : FALSE);
}

/*--------------------------------------------------------
 Transform_Text(cosnt TCHAR*, const TCHAR*, SOCKET*):
 Client로 보낼 텍스트 만들고 송신 우선순위 큐에 담는 함수
--------------------------------------------------------*/
void Transform_Text(const TCHAR* CODE, const TCHAR* TEXT, SOCKET* P) {
	TCHAR Send_TEXT[300];

	if (lstrcmp(CODE, "S00") == 0) {
		// 해당 소켓에 [SUCCESS / FAIL] 메세지 보내기(로그인)
		wsprintf(Send_TEXT, "%s-%s", CODE, TEXT);
		Enque_SQ(Send_TEXT, P);
	}
	else if (lstrcmp(CODE, "S01") == 0) {
		// 비밀번호 초기화 메세지 보내기
		wsprintf(Send_TEXT, "%s-%s", CODE, TEXT);
		Enque_SQ(Send_TEXT, P);
	}
	else if (lstrcmp(CODE, "S02") == 0) {
		// 해당 소켓에 [SUCCESS / FAIL] 메세지 보내기(비번 초기화)
		wsprintf(Send_TEXT, "%s-%s", CODE, TEXT);
		Enque_SQ(Send_TEXT, P);
	}
	else if (lstrcmp(CODE, "S03") == 0) {
		// 해당 소켓에 [SUCCESS / FAIL] 메세지 보내기(회원가입)
		wsprintf(Send_TEXT, "%s-%s", CODE, TEXT);
		Enque_SQ(Send_TEXT, P);
	}
	else if (lstrcmp(CODE, "S04") == 0) {
		wsprintf(Send_TEXT, "%s-%s", CODE, Seat_Code);
		Enque_SQ(Send_TEXT, P);
	}
	else if (lstrcmp(CODE, "S05") == 0) {
		// 해당 소켓에 [SUCCESS / FAIL] 메세지 보내기
		wsprintf(Send_TEXT, "%s-%s", CODE, TEXT);
		Enque_SQ(Send_TEXT, P);
	}

}