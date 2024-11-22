#ifndef __COMMON__
#define __COMMON__
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"

//고객 정보 구조체
typedef struct Client_Information {
	TCHAR Name[21];			//이름
	TCHAR ID[21];			//ID
	TCHAR RemainTime[8];	//남은시간(최대 999:59:59)
}CI;

// 좌석 정보 구조체
typedef struct Client_Seat {
	int state;		//좌석 상태
	CI* client;		//비트맵에 띄워질 고객 정보
}SEAT;


#endif
