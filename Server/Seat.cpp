#include "Seat.h"

extern MAP* hMap;

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
 Add_MAP(TCHAR* ,SOCKET*): ���� ����ü �߰�(����)
--------------------------------------------------------*/
void Add_MAP(SOCKET* Client_Sock, TCHAR* ID) {
	MAP* N, *P;

	N = Create_MAP();
	
	N->Client_Sock = Client_Sock;
	N->Client_Info = Find_Customer_Info(ID);
	
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
		MAP* E, * P;
		P = hMap;

		while (P->link != NULL) {
			E = P;
			P = P->link;
			if (lstrcpy(P->Client_Info->ID, ID) == 0) {
				E->link = P->link;
				free(P);
				break;
			}
		}
	}
}


/*--------------------------------------------------------
 Create_SEAT(): �¼� �ʱ�ȭ �Լ�
--------------------------------------------------------*/
SEAT* Create_SEAT() {
	SEAT* N;
	static int num = 1;

	N = (SEAT*)malloc(sizeof(SEAT));

	N->S_num = num++;
	N->State = 0;
	N->Client_S_I = Create_MAP();
	N->Thread_ID = 0;

	return N;
}