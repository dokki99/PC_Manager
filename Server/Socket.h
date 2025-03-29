#ifndef __SOKET_H__
#define __SOKET_H__

#include "Common.h"
#include "Seat.h"

// ���� ���� Ŭ���̾�Ʈ ��������
typedef struct Client_Sock {
	SOCKET Sock;			// Ŭ���̾�Ʈ ����
	Client_Sock* link;		// ��ũ ����
}CS;

// ���� ���� �Լ�////////////////////////////////////////////////////////////

CS* Create_CS();						// ���� ����ü �ʱ�ȭ
void addsock(SOCKET);					// ���� ���� �߰�
void delsock(SOCKET);					// ���� ���� ����

//////////////////////////////////////////////////////////////////////////////////

// �������� �Լ�//////////////////////////////////////////////////////////////////

void SVR_Open();										// ���� ����
DWORD WINAPI Connect_Thread(LPVOID);					// ���� ���� ������
DWORD WINAPI Recv_Thread(LPVOID);						// ���� ������
void Send_Text(const char*, const char* , SOCKET *);	// �ؽ�Ʈ �۽�
void SVR_Close();										// ���� Ŭ����

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////// Format Protocol ///////////////////////////

/*
Format: CODE - TEXT ����{
 	CODE[0] = (S / C) ���� / Ŭ���̾�Ʈ
	CODE[1~2] = (16����) 00 ~ FF �� 256���� ���°� ǥ�� ����
}

CODE:
	S00 = �¼�����
*/

#endif