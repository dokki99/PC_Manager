#ifndef __SEAT_H__
#define __SEAT_H__
#include "Customer.h"


/*----------------------------------------------------------
 & ���� & : �¼���ȣ�� ������ ������ �ֵ� Ŭ���̾�Ʈ��
 ã�ų� �ٸ� �����带 ã���� ������ID�� ã��
-----------------------------------------------------------*/

// ���� ����ü
typedef struct Map{
	SOCKET* Client_Sock;	// ����(CS���� ��������)
	CI* Client_Info;		// �� ����
	Map* link;				// ��ũ
}MAP;

// �¼� ���� ����ü
typedef struct {
	int S_num;				// �¼� ��ȣ
	int State;				// �¼� ����(0:������, 1:������, 2:�����, 3:�غ���)
	MAP* Client_S_I;		// Ŭ���̾�Ʈ / ���� ����
	DWORD Thread_ID;		// �¼� ������ ID
}SEAT;

MAP* Create_MAP();					// ���� ����ü �ʱ�ȭ
void Add_MAP(SOCKET* ,TCHAR*);		// ���� ����ü �߰�(����)
void Del_MAP(TCHAR*);				// ���� ����ü ����(���� ����)
SOCKET* Find_Customer_Sock(TCHAR*);	// �� ���̵� �´� SOCKET �ּ� ��ȯ



SEAT* Create_SEAT();				// �¼� �ʱ�ȭ
#endif