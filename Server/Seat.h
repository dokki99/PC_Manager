#ifndef __SEAT_H__
#define __SEAT_H__
#include "Customer.h"


/*----------------------------------------------------------
 & 주의 & : 좌석번호는 관리상 가지고만 있되 클라이언트를
 찾거나 다른 스레드를 찾을때 스레드ID로 찾기
-----------------------------------------------------------*/

// 매핑 구조체
typedef struct Map{
	SOCKET Client_Sock;				// 소켓 참조(CS에서 가져오기)
	CI* Client_Info;				// 고객 정보
	struct Map* link;				// 링크
}MAP;

// 좌석 정보 구조체
typedef struct {
	int S_num;				// 좌석 번호
	int S_State;			// 좌석 상태(0:사용안함, 1:예약중, 2:사용중, 3:준비중)
	int P_State;			// 주문 상태(0:주문안함, 1:주문함)
	MAP* Client_S_I;		// 클라이언트 / 소켓 정보
	DWORD Thread_ID;		// 좌석 스레드 ID
}SEAT;

MAP* Create_MAP();					// 매핑 구조체 초기화
void Add_MAP(SOCKET ,TCHAR*);		// 매핑 구조체 추가(매핑)
void Del_MAP(TCHAR*);				// 매핑 구조체 삭제(매핑 해제)

void Find_Customer_Sock(SOCKET ,TCHAR*);	// 고객 아이디에 맞는 SOCKET 주소 찾기
int Find_Seat_Num(TCHAR*);					// 고객 아이디로 현재 고객이 앉아있는 자리를 찾고 반환
SEAT* Create_SEAT();				// 좌석 초기화
#endif