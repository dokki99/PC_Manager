#ifndef __PROCESS_H__
#define __PROCESS_H__
#include "Seat.h"

// ���� �켱���� ť
typedef struct Charge_Queue {
	CI* Customer_Info;			// �� ����
	int Charge_BTN_N;			// ���� �ݾ� ��ư ��ȣ
	Charge_Queue* link;			// ��ũ
}CQ;

// �۽� �켱���� ť
typedef struct Send_Queue {
	TCHAR TEXT[300];			// Ŭ���̾�Ʈ�� ���� �ؽ�Ʈ
	SOCKET* Client_Sock;		// ��������
	Send_Queue* link;			// ��ũ
}SQ;


DWORD WINAPI Relay_Thread(LPVOID);			// �¼� ������
DWORD WINAPI Charge_Process(LPVOID);		// ���� ������
void Update_Seat_Code();					// �¼� ��Ȳ �ֽ�ȭ
void Split_C_T(TCHAR*, TCHAR*);				// �ڵ�-�ؽ�Ʈ ����
void Split2(TCHAR*, TCHAR*, const TCHAR*);	// �ؽ�Ʈ �ѹ� �� ����
void Int_To_Time(int, TCHAR*);				// �ʴ��� �ð��� 00:00:00 �������� �ٲ���
int Time_To_Int(const TCHAR*);				// 00:00:00 �ð��� �ʴ��� �������� �ٲ���
int Get_Seconds_Btn(int);					// ��ư ��ȣ�� ���� �ʴ��� ����

// ���� �켱���� ť �Լ� ////////////////
CQ* Create_CQ();
void Enque_CQ(CI* , const TCHAR* );
CQ* Deque_CQ();
BOOL IsEmpty_CQ();
/////////////////////////////////////////

// �۽� �켱���� ť �Լ� ////////////////
SQ* Create_SQ();
void Enque_SQ(const TCHAR*, SOCKET *);
SQ* Deque_SQ();
BOOL IsEmpty_SQ();
/////////////////////////////////////////

// �۽� �켱���� ť �Լ� ////////////////
void Transform_Text(const TCHAR*, const TCHAR*, SOCKET*);	// �۽��� �ؽ�Ʈ ���� �Լ�
///////////////////////////////////////// 

#endif