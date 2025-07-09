#include "Process.h"

extern BOOL CONN_ST;									// 서버 오픈 상태
extern HWND hWndMain;
extern MAP *hMap;												// 매핑 구조체
extern SEAT **hSeat;											
extern TCHAR Seat_Code[MAX_SEAT + 1];
extern TCHAR buf[];

extern HANDLE Seat_Thread[MAX_SEAT];							// 좌석 스레드

extern HANDLE Message_Mutex;				// 메시지 처리 뮤택스
extern HANDLE Charge_Mutex;										// 충전 뮤택스
extern HANDLE Send_Mutex;										// 송신 뮤택스
extern HANDLE Seat_Mutex[MAX_SEAT];								// 좌석 뮤텍스


extern HANDLE Order_Thread;										// 주문 스레드
extern HANDLE Order_Mutex;										// 주문 뮤택스

extern HANDLE* Order_Sub_Thread;								// 주문 처리 스레드
extern HANDLE* Order_Sub_Mutex;									// 주문 처리 뮤택스

extern CCI *C_CI;												// 현재 접속 클라이언트 연결 정보
extern MQ *Message_Front, *Message_Rear;						// 메시지 처리 큐
extern CQ *Charge_Front, *Charge_Rear;							// 주문 큐
extern SQ *Send_Front, *Send_Rear;								// 송신 큐
extern SEAT_M_Q **Seat_Message_Front, **Seat_Message_Rear;		// 좌석 메시지 큐
extern OQ* Order_Front, * Order_Rear;							// 주문 큐
extern OSQ **Order_Sub_Front, **Order_Sub_Rear;					// 주문 서브 큐
extern int* Order_Sub_Num;										// 주문 서브 번호

extern int Local_Time;											// 프로그램 내부 시간

/*--------------------------------------------------------
 Relay_Thread(LPVOID) : 좌석 스레드
 num은 1부터 시작
-------------------------------------------------------- */
DWORD WINAPI Relay_Thread(LPVOID Param) {
	// 주문, 요금, 채팅이 가능해야됌
	int* num_adress = (int*)Param;
	int num = *num_adress;
	SEAT_M_Q* M;

	while (CONN_ST) {
		M = Deque_SEAT_M_Q(num);

		if (M != NULL) {
			if (M->Command == 1) {
				// 충전
				Enque_CQ(hSeat[num - 1]->Client_S_I->Client_Info, M->TEXT);
			}
			else if (M->Command == 2) {
				// 주문
				Enque_OQ(num, M->TEXT);
				hSeat[num - 1]->P_State = 1;
			}
			else if (M->Command == 3) {
				// 채팅

			}
			else if (M->Command == 4) {
				// 자리이동
			}
			else {
				// 예외
			}
			free(M);
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Message_Process(LPVOID) : 큐에 담긴 메시지를 순차적으로
 처리하는 스레드
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
				// 로그인 가능한지 확인
				Split2(ID, PWD, M->TEXT);

				// 고객 잔여 시간 DB에서 찾아 보내주기
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
				// 핸드폰 번호로 아이디 찾아서 보내주기
				lstrcpy(pNum, M->TEXT);
				Find_ID(ID, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, ID);

				Transform_Text("S01", Send_TEXT, M->Client_Sock);
			}
			else if (lstrcmp(M->CODE, "C02") == 0) {
				// ID와 전화번호를 받고 PW초기화 시켜주기
				Login_Info_Check(ID, pNum, 2) != TRUE || PWD_Reset(ID, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, "비번이 초기화 되었습니다 123456789a");

				Transform_Text("S02", Send_TEXT, M->Client_Sock);
			}
			else if (lstrcmp(M->CODE, "C03") == 0) {
				// 회원가입 가능한지 확인 및 가입
				Login_Info_Check(ID, pNum, 3) == TRUE || Regist_Customer(ID, PWD, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, "회원가입이 완료되었습니다!!");

				Transform_Text("S03", Send_TEXT, M->Client_Sock);
		
				// 이 과정에서 회원가입후 바로 로그인이 가능하게 할지 고민해보기
			}
			else if (lstrcmp(M->CODE, "C04") == 0) {
				// 접속한 클라이언트에게 현재 좌석 상태를 보내주기
				Transform_Text("S04", "", M->Client_Sock);
			}
			else if (lstrcmp(M->CODE, "C05") == 0) {
				// 좌석선점후 클라이언트에게 성공 실패여부 송신
				num = atoi(M->TEXT);
				WaitForSingleObject(Seat_Mutex[num - 1], INFINITE);

				if (hSeat[num - 1]->S_State == 0) {
					// 좌석 선점 완료
					// 좌석 구조체 나머지 값 연결해주기
					hSeat[num - 1]->S_State = 1;
					MP = hMap;
					while (MP->link != NULL) {
						if (MP->Client_Sock == M->Client_Sock) {
							// 현재 좌석에 앉은 고객의 상태값을 변경합니다.
							MP->Client_Info->State = TRUE;

							// 매핑한 값을 SEAT구조체에 넣어줍니다
							hSeat[num - 1]->Client_S_I = MP;
							break;
						}
					}
					ResumeThread(Seat_Thread[num - 1]); // 매칭된 스레드를 대기상태 해제 시켜줍니다.
					// 이후에 주문/자리이동/시간추가와 같은 작업은 좌석스레드에서 해결됩니다.
					lstrcpy(M->TEXT, "SUCCESS");
				}
				else {
					// 좌석 선점 실패
					lstrcpy(M->TEXT, "FAIL");
				}

				Update_Seat_Code();			// 좌석 최신화

				// 클라이언트 전체에 최신 좌석 정보 전송
				if (lstrcmp(M->TEXT, "SUCCESS") == 0) {
					L_CI = C_CI;
					while (L_CI->link != NULL) {
						L_CI = L_CI->link;
						Transform_Text("S04", M->TEXT, L_CI->Sock);
					}
				}

				// 좌석 선점 성공/실패 메시지 전송
				Transform_Text("S05", M->TEXT, M->Client_Sock);

				ReleaseMutex(Seat_Mutex[num - 1]);
			}
			else if (lstrcmp(M->CODE, "C06") == 0) {
				// 계정에 요금충전
				// DB에 아이디 있는지 확인하고 현재 접속해 있는지 확인
				// 충전 큐에 배치한다음 처리후 다시 통보

				Split2(ID, B_num, M->TEXT);
				if (Login_Info_Check(ID, "", 3) == TRUE && User_State(ID) == TRUE) {
					Find_Customer_Info(F_CI, ID);
					// 좌석을 보유하고있는 고객은 좌석 프로세스에서 처리
					if (F_CI->State == TRUE) {
						if ((num = Find_Seat_Num(F_CI->ID)) != -1) {
							Enque_SEAT_M_Q(num, 1, B_num);		// 알맞는 좌석스레드에 메시지 보냄
						}
						else {
							// 에러처리
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
					// fail 처리
					Enque_SQ("FAIL", M->Client_Sock);
				}
			}
			free(M);
		}
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
	SOCKET S = INVALID_SOCKET;
	TCHAR Send_TEXT[256];

	while (CONN_ST) {
		//여기서 디큐해주면서 처리합니다
		C = Deque_CQ();
	
		if (C != NULL) {
			// Client가 충전할 시간 가져오기
			C_Time = Get_Seconds_Btn(C->Charge_BTN_N);

			// DB 반영할 시간 = 충전할 시간 + 현재 남은 손님 시간
			Total_Second = C_Time + C->Customer_Info->RemainTime;

			// 만약 좌석에 앉아 있다면 충전하는 동안 지연시간 빼주기
			if (C->Customer_Info->State == TRUE) {
				Total_Second -= Local_Time;
			}

			// DB에 충전시간 반영
			if (Charge_Time(C->Customer_Info->ID, Total_Second) == TRUE) {
				// Client_info 구조체 충전시간 업데이트
				Update_Remain_hTime(C->Customer_Info->ID, Total_Second);

				// 충전 요청을한 Client에게 현재 남은시간 보내주기(Send Thread를 이용할 것)
				Find_Customer_Sock(S, C->Customer_Info->ID);
				if (S != INVALID_SOCKET) {
					// 고객 구조체 남은시간 업데이트
					C->Customer_Info->RemainTime = Total_Second;
					// DB에 저장된 시간을 Send큐에 담아 Client에 반영
					wsprintf(Send_TEXT, "%d", Total_Second);
					Transform_Text("S00", Send_TEXT, S);
				}
				else {
					// 매핑 오류(데이터 롤백 시키고 클라이언트한테 실패 메시지)
				}
			}
			else {
				// 충전 실패
			}

			free(C);	// 메모리 반환
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Order_Process(LPVOID NULL) : 주문을 받고 주문 서브 큐에
 분할
-------------------------------------------------------- */
DWORD WINAPI Order_Process(LPVOID Param) {
	OQ* O;
	TCHAR ORDER[30], Num[30], Temp[300];
	int i, j, O_Num;
	
	while (CONN_ST) {
		O = Deque_OQ();

		if (O != NULL) {
			// 상품코드 - 수량 형식의 텍스트를 분할하여 각각의 처리 큐에 삽입
			// 분할 형식 상품코드-수량/상품코드 - 수량/ ... 상품코드 - 수량/
			while (1) {
				// 문자열이 비어있으면 반복문 종료
				if (O->Product_CODE[0] == '\0')	break;

				// 상품코드 얻기
				for (i = 0; i < strlen(O->Product_CODE); i++) {
					if (O->Product_CODE[i] == '-')	break;
					ORDER[i] = O->Product_CODE[i];
				}
				ORDER[i] = '\0';
				j = i;

				// 수량 얻기
				for (i = j + 1; i < strlen(O->Product_CODE); i++) {
					if (O->Product_CODE[i] == '/')	break;
					Num[i - (j + 1)] = O->Product_CODE[i];
				}
				Num[i - (j + 1)] = '\0';
				O_Num = atoi(Num);
				j = i;

				// 텍스트가 남아있으면 다시 돌아가서 텍스트를 분할
				if ((j + 1) <= strlen(O->Product_CODE)) {
					for (i = j + 1; i < strlen(O->Product_CODE); i++) {
						Temp[i - (j + 1)] = O->Product_CODE[i];
					}
					Temp[i - (j + 1)] = '\0';
					lstrcpy(O->Product_CODE, Temp);

					// 분할한 상품 - 수량을 각각의 처리 큐로 엔큐
					if (lstrcmp(ORDER, "R00") == 0) {
						// 음료
						Enque_OSQ(0, O->S_Num, O_Num * 1);
					}
					else if (lstrcmp(ORDER, "R01") == 0) {
						// 과자
						Enque_OSQ(1, O->S_Num, O_Num * 3);
					}
					else if (lstrcmp(ORDER, "R02") == 0) {
						// 음식
						Enque_OSQ(2, O->S_Num, O_Num * 10);
					}
					else if (lstrcmp(ORDER, "R03") == 0) {
						// 확장 예정
					}
					else {
						// 예외
					}
				}

			}
			free(O);
		}
	}
	return 0;
}

/*--------------------------------------------------------
 Order_Sub_Process() : 주문을 분할하여 처리하는 스레드
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
 Update_Seat_Code() : 좌석 현황 최신화
-------------------------------------------------------- */
void Update_Seat_Code() {
	int i;
		
	for (i = 0; i < 30; i++) {
		Seat_Code[i] = ((hSeat[i]->S_State != 0) ? '1' : '0');
	}

	Seat_Code[MAX_SEAT] = '\0';
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
 Create_SEAT_M_Q(): 좌석 메시지 큐 초기화 함수
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
 Enque_SEAT_M_Q(int, int, const TCHAR*): 좌석 메시지 큐
 엔큐
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
 Deque_SEAT_M_Q(int ): 좌석 메시지 큐 디큐
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
 IsEmpty_SEAT_M_Q(): 좌석 메시지 큐 비어있는지 확인
--------------------------------------------------------*/
BOOL IsEmpty_SEAT_M_Q(int S_Num) {
	return (((Seat_Message_Front[S_Num - 1]->link == NULL) && (Seat_Message_Rear[S_Num - 1]->link == NULL)) ? TRUE : FALSE);
}


/*--------------------------------------------------------
 Create_CQ(): 충전 큐 초기화 함수
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
 Enque_CQ(TCHAR* ,TCHAR*): 충전 큐 엔큐
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
 Deque_CQ(): 충전 큐 디큐
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
 IsEmpty_CQ(): 좌석 큐 비었는지 확인하는 함수
--------------------------------------------------------*/
BOOL IsEmpty_CQ() {
	return (((Charge_Front->link == NULL) && (Charge_Rear->link == NULL)) ? TRUE : FALSE);
}

/*--------------------------------------------------------
 Create_OQ(): 주문 큐 초기화 및 생성
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
 Enque_OQ(int, const TCHAR*): 주문 큐 엔큐
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
 Deque_OQ(): 주문 큐 디큐
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
 IsEmpty_OQ(): 주문 큐 비어있는지 확인
--------------------------------------------------------*/
BOOL IsEmpty_OQ() {
	return (((Order_Front->link == NULL) && (Order_Rear->link == NULL)) ? TRUE : FALSE);
}

/*--------------------------------------------------------
 Create_OSQ(): 주문 서브 큐 초기화
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
 Enque_OSQ(int ,int ,int ): 주문 서브 큐 엔큐
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
 Deque_OSQ(int ): 주문 서브 큐 디큐
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
 IsEmpty_OSQ(int ): 주문 서브 큐 비어있는지 확인
--------------------------------------------------------*/
BOOL IsEmpty_OSQ(int M_Num) {
	return (((Order_Sub_Front[M_Num]->link == NULL) && (Order_Sub_Rear[M_Num]->link == NULL)) ? TRUE : FALSE);
}

/*--------------------------------------------------------
 Transform_Text(cosnt TCHAR*, const TCHAR*, SOCKET*):
 Client로 보낼 텍스트 만들고 송신 큐에 담는 함수
--------------------------------------------------------*/
void Transform_Text(const TCHAR* CODE, const TCHAR* TEXT, SOCKET P) {
	WaitForSingleObject(Send_Mutex, INFINITE);
	TCHAR Send_TEXT[300];

	if (lstrcmp(CODE, "S00") == 0) {
		// 해당 소켓에 [남은시간 / FAIL] 메세지 보내기
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
	ReleaseMutex(Send_Mutex);
}