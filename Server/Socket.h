#ifndef __SOCKET__
#define __SOCKET__
#include "Common.h"
// ���� ���� ����ü
typedef struct Client_Socket {
	TCHAR ID[128];
	SOCKET Client_Sock;
	struct Client_Socket* link;
}CS;




// ���ϰ��� �Լ�/////////////////////////////////////////////

CS* create();							// ���� ����ü �Ҵ�
void addsock(SOCKET);					// ���� ���� �߰�
void delsock(SOCKET);					// ���� ���� ����

/////////////////////////////////////////////////////////////

// �������� �Լ�/////////////////////////////////////////////

void SVR_Open();						// ���� ����
DWORD WINAPI Connect_Thread(LPVOID);	// ���� ���� ������
DWORD WINAPI Recv_Thread(LPVOID);		// ���� ������

/////////////////////////////////////////////////////////////


#endif 
