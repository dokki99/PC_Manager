#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__
#include "DB.h"

// 현재 로그인 고객 정보 구조체
typedef struct Client_Information {
	TCHAR ID[21];				// ID
	int RemainTime;				// 남은시간(최대 999:59:59) 3,599,999초
	BOOL State;					// 좌석에 앉아있는지 상태 (TRUE/FALSE)
	Client_Information* link;	// 링크
}CI;

LRESULT CALLBACK Customer_Info_Proc(HWND, UINT, WPARAM, LPARAM);

CI* Create_CI();								// 로그인 손님 구조체 초기화
void Add_Customer(TCHAR*, int);					// 로그인 손님 정보 추가
void Del_Customer(TCHAR*);						// 로그인 손님 정보 삭제
BOOL User_State(const TCHAR* );					// 현재 로그인 되있는 아이디인지 확인	
int Get_Remain_hTime(TCHAR*);					// 고객 정보 구조체의 남은 시간을 리턴
void Update_Remain_hTime(const TCHAR*, int);	// 고객 정보 구조체의 남은 시간을 업데이트
void Find_Customer_Info(CI* ,TCHAR*);					// ID에 맞는 고객 정보 구조체의 주소를 리턴

#endif
