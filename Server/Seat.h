#ifndef __SEAT_H__
#define __SEAT_H__
#include "Customer.h"


/*----------------------------------------------------------
 & ���� & : �¼���ȣ�� ������ ������ �ֵ� Ŭ���̾�Ʈ��
 ã�ų� �ٸ� �����带 ã���� ������ID�� ã��
-----------------------------------------------------------*/

// ���� ����ü
typedef struct Map{
	SOCKET Client_Sock;				// ���� ����(CS���� ��������)
	CI* Client_Info;				// �� ����
	struct Map* link;				// ��ũ
}MAP;

// �¼� ���� ����ü
typedef struct {
	int S_num;				// �¼� ��ȣ
	int S_State;			// �¼� ����(0:������, 1:������, 2:�����, 3:�غ���)
	int P_State;			// �ֹ� ����(0:�ֹ�����, 1:�ֹ���)
	MAP* Client_S_I;		// Ŭ���̾�Ʈ / ���� ����
	DWORD Thread_ID;		// �¼� ������ ID
}SEAT;

MAP* Create_MAP();					// ���� ����ü �ʱ�ȭ
void Add_MAP(SOCKET ,TCHAR*);		// ���� ����ü �߰�(����)
void Del_MAP(TCHAR*);				// ���� ����ü ����(���� ����)

void Find_Customer_Sock(SOCKET ,TCHAR*);	// �� ���̵� �´� SOCKET �ּ� ã��
int Find_Seat_Num(TCHAR*);					// �� ���̵�� ���� ���� �ɾ��ִ� �ڸ��� ã�� ��ȯ
SEAT* Create_SEAT();				// �¼� �ʱ�ȭ
#endif