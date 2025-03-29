#ifndef __PROCESS_H__
#define __PROCESS_H__
#include "Seat.h"

typedef struct Priority_Queue {
	int num;				// ���� �¼�
	Priority_Queue* link;	// ��ũ
}PQ;

DWORD WINAPI Relay_Thread(LPVOID);		// �¼� ������
void Update_Seat_Code();				// �¼� ��Ȳ �ֽ�ȭ
void Split_C_T(TCHAR*, TCHAR*);			// �ڵ�-�ؽ�Ʈ ����

PQ* Create_PQ();
void Enque_PQ(int );
int Deque_PQ();
BOOL IsEmpty_PQ();
#endif