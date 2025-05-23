#include "Customer.h"

extern HINSTANCE g_hInst;
extern HWND hWndMain;		// 메인 화면 핸들
extern HWND Customer_I_List;	// 회원 리스트뷰
extern CI* C_I;

/*--------------------------------------------------------
 Customer_Info_Proc: 회원정보 프로시저
--------------------------------------------------------*/
LRESULT CALLBACK Customer_Info_Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN COL;
	switch (iMessage) {
	case WM_CREATE:
		Customer_I_List = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 20, 20, 800, 400, hWnd, NULL, g_hInst, NULL);
		ListView_SetExtendedListViewStyle(Customer_I_List, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_LEFT;
		COL.cx = 130;

		COL.pszText = (LPSTR)"아이디";		  //첫 번째 헤더
		COL.iSubItem = 0;
		SendMessage(Customer_I_List, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

		COL.pszText = (LPSTR)"성함";		  //두 번째 헤더
		COL.iSubItem = 1;
		SendMessage(Customer_I_List, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = (LPSTR)"비밀번호";	 //세 번째 헤더
		COL.iSubItem = 2;
		SendMessage(Customer_I_List, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		COL.pszText = (LPSTR)"핸드폰";		 //네 번째 헤더
		COL.iSubItem = 3;
		SendMessage(Customer_I_List, LVM_INSERTCOLUMN, 3, (LPARAM)&COL);

		COL.pszText = (LPSTR)"주소";		 //다섯 번째 헤더
		COL.iSubItem = 4;
		SendMessage(Customer_I_List, LVM_INSERTCOLUMN, 4, (LPARAM)&COL);

		COL.pszText = (LPSTR)"생년월일";    //여섯 번째 헤더
		COL.iSubItem = 5;
		SendMessage(Customer_I_List, LVM_INSERTCOLUMN, 5, (LPARAM)&COL);

		COL.pszText = (LPSTR)"잔여시간";    //일곱 번째 헤더
		COL.iSubItem = 6;
		SendMessage(Customer_I_List, LVM_INSERTCOLUMN, 6, (LPARAM)&COL);

		Load_Customer_Data();

		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

/*--------------------------------------------------------
 Create_CI(): 현재 로그인한 손님 구조체 초기화
--------------------------------------------------------*/
CI* Create_CI() {
	CI* N;

	N = (CI*)malloc(sizeof(CI));

	lstrcpy(N->ID, "");
	N->RemainTime = 0;
	N->State = FALSE;
	
	return N;
}

/*--------------------------------------------------------
 Add_Customer(TCHAR*, int): 로그인 손님 정보 추가
--------------------------------------------------------*/
void Add_Customer(TCHAR* ID, int RemainTime) {
	CI* N, *P;

	N = Create_CI();
	lstrcpy(N->ID, ID);
	N->RemainTime = RemainTime;

	P = C_I;

	while (P->link != NULL) {
		P = P->link;
	}

	P->link = N;
}

/*--------------------------------------------------------
 Del_Customer(TCHAR*): 로그인 손님 정보 삭제
--------------------------------------------------------*/
void Del_Customer(TCHAR* ID) {
	if (C_I->link != NULL) {
		CI* E, * P;
		P = C_I;

		while (P->link != NULL) {
			E = P;
			P = P->link;
			if (lstrcpy(P->ID, ID) == 0) {
				E->link = P->link;
				free(P);
				break;
			}
		}
	}
}


/*--------------------------------------------------------
 User_State(const TCHAR*): 현재 로그인 되있는 아이디 인지
 확인
--------------------------------------------------------*/
BOOL User_State(const TCHAR* ID) {
	if (C_I->link != NULL) {
		CI* E, * P;
		P = C_I;

		while (P->link != NULL) {
			E = P;
			P = P->link;
			if (lstrcpy(P->ID, ID) == 0) {
				E->link = P->link;
				return TRUE;
			}
		}
	}
	return FALSE;
}

/*--------------------------------------------------------
 Get_Remain_hTime(TCHAR*): 현재 고객의 남은시간
 가져오기 <<서버 기준>>
--------------------------------------------------------*/
int Get_Remain_hTime(TCHAR* ID) {
	if (C_I->link != NULL) {
		CI *P = C_I;

		while (P->link != NULL) {
			P = P->link;
			if (lstrcpy(P->ID, ID) == 0) {
				return P->RemainTime;
			}
		}
	}

	return -1;
}
/*--------------------------------------------------------
 Update_Remain_hTime(const TCHAR*, int): 고객 정보 구조체의
 남은 시간을 업데이트
--------------------------------------------------------*/
void Update_Remain_hTime(const TCHAR* ID, int Time) {
	if (C_I->link != NULL) {
		CI* P = C_I;

		while (P->link != NULL) {
			P = P->link;
			if (lstrcpy(P->ID, ID) == 0) {
				P->RemainTime = Time;
			}
		}
	}
}

/*--------------------------------------------------------
 Find_Customer_Info(TCHAR*): ID에 맞는 고객정보 구조체의
 주소를 찾는다
--------------------------------------------------------*/
void Find_Customer_Info(CI* F_CI,TCHAR* ID) {
	if (C_I->link != NULL) {
		CI* P = C_I;

		while (P->link != NULL) {
			P = P->link;
			if (lstrcpy(P->ID, ID) == 0) {
				F_CI = P;
				break;
			}
		}
	}
}