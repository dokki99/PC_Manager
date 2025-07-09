#ifndef __PROCESS_H__
#define __PROCESS_H__
#include "Seat.h"
#include "Socket.h"

// �¼� �޼��� ť
typedef struct Seat_Message_Queue {
	int Command;							// �¼� �����忡�� �� �������� (����, �ֹ�, ä��, �ڸ��̵�) 
	TCHAR TEXT[256];						// �ؽ�Ʈ
	struct Seat_Message_Queue* link;		// ��ũ
}SEAT_M_Q;

// ���� ť
typedef struct Charge_Queue {
	CI* Customer_Info;			// �� ����
	int Charge_BTN_N;			// ���� �ݾ� ��ư ��ȣ
	struct Charge_Queue* link;	// ��ũ
}CQ;

// �ֹ� ť
typedef struct Order_Queue {
	int S_Num;					// ��ǰ�� �ֹ��� �¼���ȣ
	TCHAR Product_CODE[300];	// ��ǰ �ڵ�-����
	struct Order_Queue* link;	// ��ũ
}OQ;

// �ֹ� ���� ť
typedef struct Order_Sub_Queue {
	int S_Num;						// ��ǰ�� �ֹ��� �¼���ȣ
	int S_Cnt;						// ��ǰ ó�� �ð�
	struct Order_Sub_Queue* link;	// ��ũ
}OSQ;


DWORD WINAPI Relay_Thread(LPVOID);			// �¼� ������
DWORD WINAPI Message_Process(LPVOID);		// �޼��� ó�� ������
DWORD WINAPI Charge_Process(LPVOID);		// ���� ������
DWORD WINAPI Order_Process(LPVOID);			// �ֹ� ó�� ������
DWORD WINAPI Order_Sub_Process(LPVOID);		// �ֹ� ���� ó�� ������


void Update_Seat_Code();					// �¼� ��Ȳ �ֽ�ȭ
void Split2(TCHAR*, TCHAR*, const TCHAR*);	// �ؽ�Ʈ �ѹ� �� ����
void Int_To_Time(int, TCHAR*);				// �ʴ��� �ð��� 00:00:00 �������� �ٲ���
int Time_To_Int(const TCHAR*);				// 00:00:00 �ð��� �ʴ��� �������� �ٲ���
int Get_Seconds_Btn(int);					// ��ư ��ȣ�� ���� �ʴ��� ����


// �¼� �޼��� ť �Լ� /////////////////////////
SEAT_M_Q* Create_SEAT_M_Q();
void Enque_SEAT_M_Q(int , int , const TCHAR* );
SEAT_M_Q* Deque_SEAT_M_Q(int );
BOOL IsEmpty_SEAT_M_Q(int );
////////////////////////////////////////////////

// ���� ť �Լ� ////////////////////////////////
CQ* Create_CQ();
void Enque_CQ(CI* , const TCHAR* );
CQ* Deque_CQ();
BOOL IsEmpty_CQ();
////////////////////////////////////////////////

// �ֹ� ť �Լ� ////////////////////////////////
OQ* Create_OQ();
void Enque_OQ(int , const TCHAR* );
OQ* Deque_OQ();
BOOL IsEmpty_OQ();
////////////////////////////////////////////////

// �ֹ� ���� ť �Լ� ///////////////////////////
OSQ* Create_OSQ();
void Enque_OSQ(int ,int ,int);
OSQ* Deque_OSQ(int );
BOOL IsEmpty_OSQ(int );
////////////////////////////////////////////////

// �ؽ�Ʈ ���� �Լ� ////////////////////////////
void Transform_Text(const TCHAR*, const TCHAR*, SOCKET);	// �۽��� �ؽ�Ʈ ���� �Լ�
////////////////////////////////////////////////

#endif