#ifndef __COMMON__
#define __COMMON__
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"

//�� ���� ����ü
typedef struct Client_Information {
	TCHAR Name[21];			//�̸�
	TCHAR ID[21];			//ID
	TCHAR RemainTime[8];	//�����ð�(�ִ� 999:59:59)
}CI;

// �¼� ���� ����ü
typedef struct Client_Seat {
	int state;		//�¼� ����
	CI* client;		//��Ʈ�ʿ� ����� �� ����
}SEAT;


#endif
