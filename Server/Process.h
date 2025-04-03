#ifndef __PROCESS_H__
#define __PROCESS_H__
#include "Seat.h"

typedef struct Charge_Queue {
	CI* Client_Info;			// 충전 신청한 고객
	TCHAR Charge_Amount[10];	// 충전 금액
	Charge_Queue* link;			// 링크
}CQ;

DWORD WINAPI Relay_Thread(LPVOID);		// 좌석 스레드
void Update_Seat_Code();				// 좌석 현황 최신화
void Split_C_T(TCHAR*, TCHAR*);			// 코드-텍스트 분할
void Split_I_P(TCHAR*, TCHAR*, TCHAR*);	// 아이디-패스워드 분할

CQ* Create_CQ();
void Enque_CQ(CQ* );
CQ* Deque_CQ();
BOOL IsEmpty_CQ();
#endif