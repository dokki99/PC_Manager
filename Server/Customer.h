#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__
#include "DB.h"

LRESULT CALLBACK Customer_Info_Proc(HWND, UINT, WPARAM, LPARAM);

//�� ���� ����ü
typedef struct Client_Information {
	TCHAR Name[21];			//�̸�
	TCHAR ID[21];			//ID
	TCHAR RemainTime[8];	//�����ð�(�ִ� 999:59:59)
}CI;
#endif