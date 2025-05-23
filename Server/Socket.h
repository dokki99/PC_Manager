#ifndef __SOKET_H__
#define __SOKET_H__

#include "Common.h"


// 현재 접속 클라이언트 소켓정보
typedef struct Client_Sock {
	SOCKET* Sock;			// 클라이언트 소켓
	Client_Sock* link;		// 링크 정보
}CS;

// 메시지 처리 큐
typedef struct Message_Queue {
	SOCKET* Client_Sock;		// 소켓정보
	TCHAR CODE[4];				// 코드
	TCHAR TEXT[256];			// 텍스트
	Message_Queue* link;		// 링크
}MQ;

// 송신 큐
typedef struct Send_Queue {
	TCHAR TEXT[300];			// 클라이언트로 보낼 텍스트
	SOCKET* Client_Sock;		// 소켓정보
	Send_Queue* link;			// 링크
}SQ;

// 소켓 관련 함수////////////////////////////////////////////////////////////

CS* Create_CS();						// 소켓 구조체 초기화
void addsock(SOCKET*);					// 소켓 정보 추가
void delsock(SOCKET*);					// 소켓 정보 삭제

//////////////////////////////////////////////////////////////////////////////////

// 소켓관련 함수//////////////////////////////////////////////////////////////////

DWORD WINAPI Connect_Thread(LPVOID);						// 서버 연결 스레드
DWORD WINAPI Send_Thread(LPVOID);							// 송신 스레드
DWORD WINAPI Recv_Thread(LPVOID);							// 수신 스레드

void Transform_Text(const TCHAR*, const TCHAR* , SOCKET *);	// 텍스트 송신
void Split_C_T(TCHAR*, TCHAR*);								// 코드-텍스트 분할

////////////////////////////////////////////////////////////////////////
 
// 메시지 처리 큐 함수 /////////////////////////

MQ* Create_MQ();
void Enque_MQ(SOCKET*, const TCHAR*, const TCHAR*);
MQ* Deque_MQ();
BOOL IsEmpty_MQ();

////////////////////////////////////////////////

// 송신 큐 함수 ////////////////////////////////

SQ* Create_SQ();
void Enque_SQ(const TCHAR*, SOCKET*);
SQ* Deque_SQ();
BOOL IsEmpty_SQ();

////////////////////////////////////////////////

/////////////////////////// Format Protocol ///////////////////////////

/*
Format: CODE - TEXT 형식{
 	CODE[0] = (S / C) 서버 / 클라이언트
	CODE[1~2] = (16진수) 00 ~ FF 총 256가지 상태값 표현 가능
}

CODE:
	---------------(Server)---------------
	S00 = 로그인 성공시 남은시간 /실패시 실패 메시지
	S01 = ID 찾아서 메시지
	S02 = PW 초기화 시켜주기
	S03 = 회원가입 성공/실패 메시지
	S04 = 좌석 리스트 메시지
	S05 = 좌석 선점 성공/실패 메시지
	S06 = 좌석 선택에서 요금 충전
	---------------(Client)---------------
	C00 = 로그인 요청 메시지
	C01 = ID 찾기 요청 메시지
	C02 = PW 찾기 요청 메시지
	C03 = 회원가입 요청 메시지
	C04 = 좌석 리스트 요청 메시지
	C05 = 좌석 선점 메시지
	C06 = 좌석 선택에서 요금 충전 메세지
	C07 = 좌석에서 요금충전
	C08 = 주문
*/

#endif