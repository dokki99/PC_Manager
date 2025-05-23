#include "Process.h"

extern HWND hWndMain;
extern SEAT *hSeat[MAX_SEAT];
extern TCHAR Seat_Code[MAX_SEAT + 1];
extern TCHAR buf[];

extern HANDLE Seat_Thread[MAX_SEAT];		// 좌석 스레드

extern HANDLE Charge_Mutex;					// 충전 뮤택스
extern HANDLE Send_Mutex;					// 송신 뮤택스
extern HANDLE Seat_Mutex[MAX_SEAT];			// 좌석 뮤텍스

extern CS *C_S;									// 현재 접속 클라이언트 정보
extern MQ *Message_Front, *Message_Rear;		// 메시지 처리 큐
extern SQ *Send_Front, *Send_Rear;				// 송신 큐
extern CQ *Charge_Front, *Charge_Rear;			// 주문 큐
extern MAP *hMap;								// 매핑 구조체

extern int Local_Time;							// 프로그램 내부 시간

/*--------------------------------------------------------
 Relay_Thread(LPVOID) : 좌석 스레드
-------------------------------------------------------- */
DWORD WINAPI Relay_Thread(LPVOID Param) {
	// 주문, 요금, 채팅이 가능해야됌 (원형큐)
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
 Message_Process(LPVOID) : 큐에 담긴 메시지를 순차적으로
 처리하는 스레드
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

				WaitForSingleObject(Send_Mutex, INFINITE);
				Transform_Text("S00", Send_TEXT, M->Client_Sock);
				ReleaseMutex(Send_Mutex);
			}
			else if (lstrcmp(M->CODE, "C01") == 0) {
				// 핸드폰 번호로 아이디 찾아서 보내주기
				Find_ID(ID, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, M->TEXT);

				WaitForSingleObject(Send_Mutex, INFINITE);
				Transform_Text("S01", Send_TEXT, M->Client_Sock);
				ReleaseMutex(Send_Mutex);
			}
			else if (lstrcmp(M->CODE, "C02") == 0) {
				// ID와 전화번호를 받고 PW초기화 시켜주기
				Login_Info_Check(ID, pNum, 2) != TRUE || PWD_Reset(ID, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, "비번이 초기화 되었습니다 123456789a");

				WaitForSingleObject(Send_Mutex, INFINITE);
				Transform_Text("S02", Send_TEXT, M->Client_Sock);
				ReleaseMutex(Send_Mutex);
			}
			else if (lstrcmp(M->CODE, "C03") == 0) {
				// 회원가입 가능한지 확인 및 가입
				Login_Info_Check(ID, pNum, 3) == TRUE || Regist_Customer(ID, PWD, pNum) != TRUE ?
					lstrcpy(Send_TEXT, "FAIL") : lstrcpy(Send_TEXT, "회원가입이 완료되었습니다!!");

				WaitForSingleObject(Send_Mutex, INFINITE);
				Transform_Text("S03", Send_TEXT, M->Client_Sock);
				ReleaseMutex(Send_Mutex);

				// 이 과정에서 회원가입후 바로 로그인이 가능하게 할지 고민해보기
			}
			else if (lstrcmp(M->CODE, "C04") == 0) {
				// 접속한 클라이언트에게 현재 좌석 상태를 보내주기
				WaitForSingleObject(Send_Mutex, INFINITE);
				Transform_Text("S04", "", M->Client_Sock);
				ReleaseMutex(Send_Mutex);
			}
			else if (lstrcmp(M->CODE, "C05") == 0) {
				// 좌석선점후 클라이언트에게 성공 실패여부 송신
				num = atoi(M->TEXT);
				WaitForSingleObject(Seat_Mutex[num - 1], INFINITE);

				if (hSeat[num - 1]->State == 0) {
					// 좌석 선점 완료
					// 좌석 구조체 나머지 값 연결해주기
					hSeat[num - 1]->State = 1;
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
					ResumeThread(Seat_Thread[num - 1]);	// 매칭된 스레드를 대기상태 해제 시켜줍니다.
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
					S = C_S;
					while (S->link != NULL) {
						S = S->link;
						Transform_Text("S04", M->TEXT, S->Sock);
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
					Find_Customer_Info(F_CI,ID);
					// 좌석을 보유하고있는 고객은 좌석 프로세스에서 처리
					if (F_CI->State == TRUE) {
						if ((num = Find_Seat_Num(F_CI->ID)) != -1) {
							WaitForSingleObject(Seat_Mutex[num - 1], INFINITE);

							ReleaseMutex(Seat_Mutex[num - 1]);
						}
						else {
							// 에러처리
						}
					}
					else {
						WaitForSingleObject(Charge_Mutex, INFINITE);
						Enque_CQ(F_CI, B_num);
						ReleaseMutex(Charge_Mutex);
					}
				}
				else {
					// fail 처리
					WaitForSingleObject(Send_Mutex, INFINITE);
					Enque_SQ("FAIL", M->Client_Sock);
					ReleaseMutex(Send_Mutex);
				}
			}

		}
		else {
			MessageBox(hWndMain, "수신에러", "server", MB_OK);
			return 0;
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
	SOCKET* S = NULL;
	TCHAR Send_TEXT[256];

	while (1) {
		if (!IsEmpty_CQ()) {
			C_Time = 0;
			//여기서 디큐해주면서 처리합니다
			WaitForSingleObject(Charge_Mutex,INFINITE);
			C = Deque_CQ();
			ReleaseMutex(Charge_Mutex);
			
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
				if (S != NULL) {
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
 Create_MQ(): 메시지 처리 큐 초기화 함수
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
 Enque_MQ(SOCKET*, const TCHAR*, const TCHAR*): 메시지 
 처리 큐 엔큐
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
 Deque_MQ(): 메시지 처리 큐 디큐
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
 IsEmpty_MQ(): 메시지 처리 큐 비었는지 확인하는 함수
--------------------------------------------------------*/
BOOL IsEmpty_MQ() {
	return (((Message_Front->link == NULL) && (Message_Rear->link == NULL)) ? TRUE : FALSE);
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
 Deque_CQ(): 충전 큐 디큐
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
 IsEmpty_CQ(): 좌석 큐 비었는지 확인하는 함수
--------------------------------------------------------*/
BOOL IsEmpty_CQ() {
	return (((Charge_Front->link == NULL) && (Charge_Rear->link == NULL)) ? TRUE : FALSE);
}



/*--------------------------------------------------------
 Transform_Text(cosnt TCHAR*, const TCHAR*, SOCKET*):
 Client로 보낼 텍스트 만들고 송신 큐에 담는 함수
--------------------------------------------------------*/
void Transform_Text(const TCHAR* CODE, const TCHAR* TEXT, SOCKET* P) {
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

}