#ifndef __PROCESS_H__
#define __PROCESS_H__
#include "Seat.h"
#include "Socket.h"

// 좌석 메세지 큐
typedef struct Seat_Message_Queue {
	int Command;							// 좌석 스레드에서 할 다음동작 (충전, 주문, 채팅, 자리이동) 
	TCHAR TEXT[256];						// 텍스트
	struct Seat_Message_Queue* link;		// 링크
}SEAT_M_Q;

// 충전 큐
typedef struct Charge_Queue {
	CI* Customer_Info;			// 고객 정보
	int Charge_BTN_N;			// 충전 금액 버튼 번호
	struct Charge_Queue* link;	// 링크
}CQ;

// 주문 큐
typedef struct Order_Queue {
	int S_Num;					// 상품을 주문한 좌석번호
	TCHAR Product_CODE[300];	// 상품 코드-수량
	struct Order_Queue* link;	// 링크
}OQ;

// 주문 서브 큐
typedef struct Order_Sub_Queue {
	int S_Num;						// 상품을 주문한 좌석번호
	int S_Cnt;						// 상품 처리 시간
	struct Order_Sub_Queue* link;	// 링크
}OSQ;


DWORD WINAPI Relay_Thread(LPVOID);			// 좌석 스레드
DWORD WINAPI Message_Process(LPVOID);		// 메세지 처리 스레드
DWORD WINAPI Charge_Process(LPVOID);		// 충전 스레드
DWORD WINAPI Order_Process(LPVOID);			// 주문 처리 스레드
DWORD WINAPI Order_Sub_Process(LPVOID);		// 주문 분할 처리 스레드


void Update_Seat_Code();					// 좌석 현황 최신화
void Split2(TCHAR*, TCHAR*, const TCHAR*);	// 텍스트 한번 더 분할
void Int_To_Time(int, TCHAR*);				// 초단위 시간을 00:00:00 형식으로 바꿔줌
int Time_To_Int(const TCHAR*);				// 00:00:00 시간을 초단위 형식으로 바꿔줌
int Get_Seconds_Btn(int);					// 버튼 번호에 따른 초단위 리턴


// 좌석 메세지 큐 함수 /////////////////////////
SEAT_M_Q* Create_SEAT_M_Q();
void Enque_SEAT_M_Q(int , int , const TCHAR* );
SEAT_M_Q* Deque_SEAT_M_Q(int );
BOOL IsEmpty_SEAT_M_Q(int );
////////////////////////////////////////////////

// 충전 큐 함수 ////////////////////////////////
CQ* Create_CQ();
void Enque_CQ(CI* , const TCHAR* );
CQ* Deque_CQ();
BOOL IsEmpty_CQ();
////////////////////////////////////////////////

// 주문 큐 함수 ////////////////////////////////
OQ* Create_OQ();
void Enque_OQ(int , const TCHAR* );
OQ* Deque_OQ();
BOOL IsEmpty_OQ();
////////////////////////////////////////////////

// 주문 서브 큐 함수 ///////////////////////////
OSQ* Create_OSQ();
void Enque_OSQ(int ,int ,int);
OSQ* Deque_OSQ(int );
BOOL IsEmpty_OSQ(int );
////////////////////////////////////////////////

// 텍스트 변경 함수 ////////////////////////////
void Transform_Text(const TCHAR*, const TCHAR*, SOCKET);	// 송신할 텍스트 변경 함수
////////////////////////////////////////////////

#endif