#include "Work.h"

extern HINSTANCE g_hInst;
extern HWND hWndMain;		// 메인 화면 핸들
extern HWND Work_I_List;		// 근무 리스트뷰

/*--------------------------------------------------------
 Work_Info_Proc: 근무정보 프로시저
--------------------------------------------------------*/
LRESULT CALLBACK Work_Info_Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN COL;
	switch (iMessage) {
	case WM_CREATE:
		Work_I_List = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 20, 20, 800, 400, hWnd, NULL, g_hInst, NULL);
		ListView_SetExtendedListViewStyle(Work_I_List, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_LEFT;
		COL.cx = 130;

		COL.pszText = (LPSTR)"No.";				 //첫 번째 헤더
		COL.iSubItem = 0;
		SendMessage(Work_I_List, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

		COL.pszText = (LPSTR)"직원 아이디";		 //두 번째 헤더
		COL.iSubItem = 1;
		SendMessage(Work_I_List, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = (LPSTR)"출근";			//세 번째 헤더
		COL.iSubItem = 2;
		SendMessage(Work_I_List, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		COL.pszText = (LPSTR)"퇴근";			//네 번째 헤더
		COL.iSubItem = 3;
		SendMessage(Work_I_List, LVM_INSERTCOLUMN, 3, (LPARAM)&COL);

		Load_Work_Data();

		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}