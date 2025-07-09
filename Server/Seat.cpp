#include "Seat.h"

extern MAP *hMap;
extern SEAT **hSeat;

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
 Add_MAP(SOCKET, TCHAR*): 매핑 구조체 추가(매핑)
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
 Del_MAP(TCHAR*): 매핑 구조체 삭제(매핑 해제)
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
 SOCKET* Find_Customer_Sock(TCHAR*): 고객 아이디에 맞는
 SOCKET 주소를 반환 합니다.
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
 Find_Seat_Num(TCHAR*): 좌석 번호 반환함수
--------------------------------------------------------*/
int Find_Seat_Num(TCHAR* ID) {
	int i;

	// 좌석에 앉아있는 고객이 찾는 고객일 경우 좌석번호 반환
	for (i = 0; i < MAX_SEAT; i++) {
		if (hSeat[i]->Client_S_I != NULL) {
			if (hSeat[i]->Client_S_I->Client_Info->ID == ID) {
				return hSeat[i]->S_num;
			}
		}
	}

	// 예외로 -1 리턴
	return -1;


}

/*--------------------------------------------------------
 Create_SEAT(): 좌석 초기화 함수
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