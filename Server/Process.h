#ifndef __PROCESS_H__
#define __PROCESS_H__
#include "Seat.h"
#include "Socket.h"

// ���� ť
typedef struct Charge_Queue {
	CI* Customer_Info;			// �� ����
	int Charge_BTN_N;			// ���� �ݾ� ��ư ��ȣ
	Charge_Queue* link;			// ��ũ
}CQ;


DWORD WINAPI Relay_Thread(LPVOID);			// �¼� ������
DWORD WINAPI Message_Process(LPVOID);		// �޼��� ó�� ������
DWORD WINAPI Charge_Process(LPVOID);		// ���� ������

void Update_Seat_Code();					// �¼� ��Ȳ �ֽ�ȭ
void Split2(TCHAR*, TCHAR*, const TCHAR*);	// �ؽ�Ʈ �ѹ� �� ����
void Int_To_Time(int, TCHAR*);				// �ʴ��� �ð��� 00:00:00 �������� �ٲ���
int Time_To_Int(const TCHAR*);				// 00:00:00 �ð��� �ʴ��� �������� �ٲ���
int Get_Seconds_Btn(int);					// ��ư ��ȣ�� ���� �ʴ��� ����

// ���� ť �Լ� /////////////////////////
CQ* Create_CQ();
void Enque_CQ(CI* , const TCHAR* );
CQ* Deque_CQ();
BOOL IsEmpty_CQ();
/////////////////////////////////////////

// �۽� ť �Լ� ////////////////
void Transform_Text(const TCHAR*, const TCHAR*, SOCKET*);	// �۽��� �ؽ�Ʈ ���� �Լ�
///////////////////////////////////////// 

#endif