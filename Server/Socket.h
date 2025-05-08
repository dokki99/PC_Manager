#ifndef __SOKET_H__
#define __SOKET_H__

#include "Common.h"
#include "Seat.h"

// 현재 접속 클라이언트 소켓정보
typedef struct Client_Sock {
	SOCKET* Sock;			// 클라이언트 소켓
	Client_Sock* link;		// 링크 정보
}CS;

// 소켓 관련 함수////////////////////////////////////////////////////////////

CS* Create_CS();						// 소켓 구조체 초기화
void addsock(SOCKET*);					// 소켓 정보 추가
void delsock(SOCKET*);					// 소켓 정보 삭제


//////////////////////////////////////////////////////////////////////////////////


// 서버관련 함수//////////////////////////////////////////////////////////////////

void SVR_Open();											// 서버 오픈
DWORD WINAPI Connect_Thread(LPVOID);						// 서버 연결 스레드
DWORD WINAPI Send_Thread(LPVOID);							// 송신 스레드
DWORD WINAPI Recv_Thread(LPVOID);							// 수신 스레드
void Transform_Text(const TCHAR*, const TCHAR* , SOCKET *);	// 텍스트 송신

void SVR_Close();											// 서버 클로즈

////////////////////////////////////////////////////////////////////////


/////////////////////////// Format Protocol ///////////////////////////

/*
Format: CODE - TEXT 형식{
 	CODE[0] = (S / C) 서버 / 클라이언트
	CODE[1~2] = (16진수) 00 ~ FF 총 256가지 상태값 표현 가능
}

CODE:
	---------------(Server)---------------
	S00 = 로그인 성공/실패 메시지
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
*/

#endif