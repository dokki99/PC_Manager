#include "Seat.h"

extern MAP* hMap;

/*--------------------------------------------------------
 Create_MAP(): 매핑 구조체 초기화
--------------------------------------------------------*/
MAP* Create_MAP() {
	MAP* N;
	
	N = (MAP*)malloc(sizeof(MAP));

	N->Client_Sock = NULL;			// 기존에 있는 소켓과 고객정보를
	N->Client_Info = NULL;			// 매핑하기 때문에 할당안해도 됩니다.
	N->link = NULL;

	return N;
}

/*--------------------------------------------------------
 Add_MAP(TCHAR* ,SOCKET*): 매핑 구조체 추가(매핑)
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
 Del_MAP(TCHAR*): 매핑 구조체 삭제(매핑 해제)
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
 Create_SEAT(): 좌석 초기화 함수
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