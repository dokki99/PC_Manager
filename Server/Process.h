#ifndef __PROCESS_H__
#define __PROCESS_H__
#include "Seat.h"

typedef struct Priority_Queue {
	int num;				// 앉을 좌석
	Priority_Queue* link;	// 링크
}PQ;

DWORD WINAPI Relay_Thread(LPVOID);		// 좌석 스레드
void Update_Seat_Code();				// 좌석 현황 최신화
void Split_C_T(TCHAR*, TCHAR*);			// 코드-텍스트 분할

PQ* Create_PQ();
void Enque_PQ(int );
int Deque_PQ();
BOOL IsEmpty_PQ();
#endif