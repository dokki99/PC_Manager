#ifndef __PROCESS_H__
#define __PROCESS_H__
#include "Seat.h"

typedef struct Charge_Queue {
	CI* Client_Info;			// ���� ��û�� ��
	TCHAR Charge_Amount[10];	// ���� �ݾ�
	Charge_Queue* link;			// ��ũ
}CQ;

DWORD WINAPI Relay_Thread(LPVOID);		// �¼� ������
void Update_Seat_Code();				// �¼� ��Ȳ �ֽ�ȭ
void Split_C_T(TCHAR*, TCHAR*);			// �ڵ�-�ؽ�Ʈ ����
void Split_I_P(TCHAR*, TCHAR*, TCHAR*);	// ���̵�-�н����� ����

CQ* Create_CQ();
void Enque_CQ(CQ* );
CQ* Deque_CQ();
BOOL IsEmpty_CQ();
#endif