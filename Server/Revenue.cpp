#include "Revenue.h"

extern HINSTANCE g_hInst;
extern HWND hWndMain;		// 메인 화면 핸들
extern HWND Revenue_I_List;		// 매출 리스트뷰

/*--------------------------------------------------------
 Revenue_Info_Proc: 매출정보 프로시저
--------------------------------------------------------*/
LRESULT CALLBACK Revenue_Info_Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN COL;
	switch (iMessage) {
	case WM_CREATE:
		Revenue_I_List = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 20, 20, 800, 400, hWnd, NULL, g_hInst, NULL);
		ListView_SetExtendedListViewStyle(Revenue_I_List, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_LEFT;
		COL.cx = 130;

		COL.pszText = (LPSTR)"No";    //첫 번째 헤더
		COL.iSubItem = 0;
		SendMessage(Revenue_I_List, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

		COL.pszText = (LPSTR)"상품코드";    //두 번째 헤더
		COL.iSubItem = 1;
		SendMessage(Revenue_I_List, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = (LPSTR)"판매직원";    //세 번째 헤더
		COL.iSubItem = 2;
		SendMessage(Revenue_I_List, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		COL.pszText = (LPSTR)"판매날짜";    //네 번째 헤더
		COL.iSubItem = 3;
		SendMessage(Revenue_I_List, LVM_INSERTCOLUMN, 3, (LPARAM)&COL);

		COL.pszText = (LPSTR)"판매수량";    //다섯 번째 헤더
		COL.iSubItem = 4;
		SendMessage(Revenue_I_List, LVM_INSERTCOLUMN, 4, (LPARAM)&COL);

		Load_Revenue_Data();

		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}