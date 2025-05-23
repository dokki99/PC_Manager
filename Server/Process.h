#ifndef __PROCESS_H__
#define __PROCESS_H__
#include "Seat.h"
#include "Socket.h"

// 충전 큐
typedef struct Charge_Queue {
	CI* Customer_Info;			// 고객 정보
	int Charge_BTN_N;			// 충전 금액 버튼 번호
	Charge_Queue* link;			// 링크
}CQ;


DWORD WINAPI Relay_Thread(LPVOID);			// 좌석 스레드
DWORD WINAPI Message_Process(LPVOID);		// 메세지 처리 스레드
DWORD WINAPI Charge_Process(LPVOID);		// 충전 스레드

void Update_Seat_Code();					// 좌석 현황 최신화
void Split2(TCHAR*, TCHAR*, const TCHAR*);	// 텍스트 한번 더 분할
void Int_To_Time(int, TCHAR*);				// 초단위 시간을 00:00:00 형식으로 바꿔줌
int Time_To_Int(const TCHAR*);				// 00:00:00 시간을 초단위 형식으로 바꿔줌
int Get_Seconds_Btn(int);					// 버튼 번호에 따른 초단위 리턴

// 충전 큐 함수 /////////////////////////
CQ* Create_CQ();
void Enque_CQ(CI* , const TCHAR* );
CQ* Deque_CQ();
BOOL IsEmpty_CQ();
/////////////////////////////////////////

// 송신 큐 함수 ////////////////
void Transform_Text(const TCHAR*, const TCHAR*, SOCKET*);	// 송신할 텍스트 변경 함수
///////////////////////////////////////// 

#endif