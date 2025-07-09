#include "Seat.h"

extern MAP *hMap;
extern SEAT **hSeat;

/*--------------------------------------------------------
 Create_MAP(): ���� ����ü �ʱ�ȭ
--------------------------------------------------------*/
MAP* Create_MAP() {
	MAP* N;
	
	N = (MAP*)malloc(sizeof(MAP));

	N->Client_Sock = NULL;			// ������ �ִ� ���ϰ� ��������
	N->Client_Info = NULL;			// �����ϱ� ������ �Ҵ���ص� �˴ϴ�.
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 Add_MAP(SOCKET, TCHAR*): ���� ����ü �߰�(����)
--------------------------------------------------------*/
void Add_MAP(SOCKET Client_Sock, TCHAR* ID) {
	MAP* N, *P;

	N = Create_MAP();
	
	N->Client_Sock = Client_Sock;
	Find_Customer_Info(N->Client_Info,ID);
	
	P = hMap;

	while (P->link != NULL) {
		P = P->link;
	}

	P->link = N;
}

/*--------------------------------------------------------
 Del_MAP(TCHAR*): ���� ����ü ����(���� ����)
--------------------------------------------------------*/
void Del_MAP(TCHAR* ID) {
	if (hMap->link != NULL) {
		MAP* E, * P = hMap;
		
		while (P->link != NULL) {
			E = P;
			P = P->link;
			if (lstrcmp(P->Client_Info->ID, ID) == 0) {
				E->link = P->link;
				free(P);
				break;
			}
		}
	}
}

/*--------------------------------------------------------
 SOCKET* Find_Customer_Sock(TCHAR*): �� ���̵� �´�
 SOCKET �ּҸ� ��ȯ �մϴ�.
--------------------------------------------------------*/
void Find_Customer_Sock(SOCKET S,TCHAR* ID) {
	if (hMap->link != NULL) {
		MAP* P = hMap;
		
		while (P->link != NULL) {
			P = P->link;
			if (lstrcmp(P->Client_Info->ID, ID) == 0) {
				S = P->Client_Sock;
				break;
			}
		}
	}

}

/*--------------------------------------------------------
 Find_Seat_Num(TCHAR*): �¼� ��ȣ ��ȯ�Լ�
--------------------------------------------------------*/
int Find_Seat_Num(TCHAR* ID) {
	int i;

	// �¼��� �ɾ��ִ� ���� ã�� ���� ��� �¼���ȣ ��ȯ
	for (i = 0; i < MAX_SEAT; i++) {
		if (hSeat[i]->Client_S_I != NULL) {
			if (hSeat[i]->Client_S_I->Client_Info->ID == ID) {
				return hSeat[i]->S_num;
			}
		}
	}

	// ���ܷ� -1 ����
	return -1;


}

/*--------------------------------------------------------
 Create_SEAT(): �¼� �ʱ�ȭ �Լ�
--------------------------------------------------------*/
SEAT* Create_SEAT() {
	SEAT* N;
	static int num = 1;

	N = (SEAT*)malloc(sizeof(SEAT));

	N->S_num = num++;
	N->S_State = 0;
	N->P_State = 0;
	N->Client_S_I = NULL;
	N->Thread_ID = 0;

	return N;
}