#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__
#include "DB.h"

// ���� �α��� �� ���� ����ü
typedef struct Client_Information {
	TCHAR ID[21];				// ID
	int RemainTime;				// �����ð�(�ִ� 999:59:59) 3,599,999��
	BOOL State;					// �¼��� �ɾ��ִ��� ���� (TRUE/FALSE)
	Client_Information* link;	// ��ũ
}CI;

LRESULT CALLBACK Customer_Info_Proc(HWND, UINT, WPARAM, LPARAM);

CI* Create_CI();								// �α��� �մ� ����ü �ʱ�ȭ
void Add_Customer(TCHAR*, int);					// �α��� �մ� ���� �߰�
void Del_Customer(TCHAR*);						// �α��� �մ� ���� ����
BOOL User_State(const TCHAR* );					// ���� �α��� ���ִ� ���̵����� Ȯ��	
int Get_Remain_hTime(TCHAR*);					// �� ���� ����ü�� ���� �ð��� ����
void Update_Remain_hTime(const TCHAR*, int);	// �� ���� ����ü�� ���� �ð��� ������Ʈ
void Find_Customer_Info(CI* ,TCHAR*);					// ID�� �´� �� ���� ����ü�� �ּҸ� ����

#endif
