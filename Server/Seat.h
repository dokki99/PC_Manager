#ifndef __SEAT_H__
#define __SEAT_H__
#include "Customer.h"


/*----------------------------------------------------------
 & 주의 & : 좌석번호는 관리상 가지고만 있되 클라이언트를
 찾거나 다른 스레드를 찾을때 스레드ID로 찾기
-----------------------------------------------------------*/


// 좌석 정보 구조체
typedef struct {
	int S_num;				// 좌석 번호
	int State;				// 좌석 상태(0:사용안함, 1:예약중, 2:사용중, 3:준비중)
	SOCKET Client_Sock;		// 클라이언트 소켓
	CI* Client_Info;		// 비트맵에 띄워질 고객 정보
	DWORD Thread_ID;		// 좌석 스레드 ID
}SEAT;

SEAT* Create_SEAT();			// 좌석 초기화
#endif