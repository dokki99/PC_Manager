#ifndef __SOKET_H__
#define __SOKET_H__
#include "Common.h"

// ���� ���� Ŭ���̾�Ʈ ��������
typedef struct Client_Connect_Info {
	SOCKET Sock;						// Ŭ���̾�Ʈ ����
	HANDLE Thread_Info;					// Ŭ���̾�Ʈ ������
	DWORD Thread_ID;					// Ŭ���̾�Ʈ ������ ���̵�
	struct Client_Connect_Info* link;	// ��ũ ����
}CCI;

// �޽��� ó�� ť
typedef struct Message_Queue {
	SOCKET Client_Sock;				// ��������
	TCHAR CODE[4];					// �ڵ�
	TCHAR TEXT[256];				// �ؽ�Ʈ
	struct Message_Queue* link;		// ��ũ
}MQ;

// �۽� ť
typedef struct Send_Queue {
	TCHAR TEXT[300];				// Ŭ���̾�Ʈ�� ���� �ؽ�Ʈ
	SOCKET Client_Sock;				// ��������
	struct Send_Queue* link;		// ��ũ
}SQ;

// ���� ���� �Լ�////////////////////////////////////////////////////////////

CCI* Create_CCI();							// ���� ����ü �ʱ�ȭ
void Add_CCI_Sock(SOCKET);					// ���� ���� �߰�
void Add_CCI_Thread(SOCKET,HANDLE,DWORD);	// ������ ���� �߰�
SOCKET* Find_CCI(SOCKET);					// ����ü�� ��� �����ּ� ����

void Del_CCI(SOCKET);					// ���� ���� ����

//////////////////////////////////////////////////////////////////////////////////

// ���ϰ��� �Լ�//////////////////////////////////////////////////////////////////

DWORD WINAPI Connect_Process(LPVOID);						// ���� ���� ������
DWORD WINAPI Send_Process(LPVOID);							// �۽� ������
DWORD WINAPI Recv_Thread(LPVOID);							// ���� ������

void Transform_Text(const TCHAR*, const TCHAR* , SOCKET );	// �ؽ�Ʈ �۽�
void Split_C_T(TCHAR*, TCHAR*, TCHAR*);								// �ڵ�-�ؽ�Ʈ ����

////////////////////////////////////////////////////////////////////////
 
// �޽��� ó�� ť �Լ� /////////////////////////

MQ* Create_MQ();
void Enque_MQ(SOCKET, const TCHAR*, const TCHAR*);
MQ* Deque_MQ();
BOOL IsEmpty_MQ();

////////////////////////////////////////////////

// �۽� ť �Լ� ////////////////////////////////

SQ* Create_SQ();
void Enque_SQ(const TCHAR*, SOCKET);
SQ* Deque_SQ();
BOOL IsEmpty_SQ();

////////////////////////////////////////////////

/////////////////////////// Format Protocol ///////////////////////////

/*
Format: CODE - TEXT ����{
 	CODE[0] = (S / C) ���� / Ŭ���̾�Ʈ
	CODE[1~2] = (16����) 00 ~ FF �� 256���� ���°� ǥ�� ����
}

CODE:
	---------------(Server)---------------
	S00 = �α��� ������ �����ð� /���н� ���� �޽���
	S01 = ID ã�Ƽ� �޽���
	S02 = PW �ʱ�ȭ �����ֱ�
	S03 = ȸ������ ����/���� �޽���
	S04 = �¼� ����Ʈ �޽���
	S05 = �¼� ���� ����/���� �޽���
	S06 = �¼� ���ÿ��� ��� ����
	---------------(Client)---------------
	C00 = �α��� ��û �޽���
	C01 = ID ã�� ��û �޽���
	C02 = PW ã�� ��û �޽���
	C03 = ȸ������ ��û �޽���
	C04 = �¼� ����Ʈ ��û �޽���
	C05 = �¼� ���� �޽���
	C06 = �¼� ���ÿ��� ��� ���� �޼���
	C07 = �¼����� �������
	C08 = �ֹ�
*/

#endif