#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__
#include "DB.h"

LRESULT CALLBACK Customer_Info_Proc(HWND, UINT, WPARAM, LPARAM);

//고객 정보 구조체
typedef struct Client_Information {
	TCHAR Name[21];			//이름
	TCHAR ID[21];			//ID
	TCHAR RemainTime[8];	//남은시간(최대 999:59:59)
}CI;
#endif