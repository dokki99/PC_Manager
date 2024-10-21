#include "Employee.h"

extern HINSTANCE g_hInst;
extern HWND hWndMain;		// 메인 화면 핸들
extern HWND Employee_I_List;	// 직원 리스트뷰

/*--------------------------------------------------------
 Employee_Info_Proc: 직원정보 프로시저
--------------------------------------------------------*/
LRESULT CALLBACK Employee_Info_Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN COL;
	switch (iMessage) {
	case WM_CREATE:
		Employee_I_List = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 20, 20, 800, 400, hWnd, NULL, g_hInst, NULL);
		ListView_SetExtendedListViewStyle(Employee_I_List, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_LEFT;
		COL.cx = 130;

		COL.pszText = (LPSTR)"아이디";    //첫 번째 헤더
		COL.iSubItem = 0;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

		COL.pszText = (LPSTR)"이름";    //두 번째 헤더
		COL.iSubItem = 1;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = (LPSTR)"비밀번호";    //세 번째 헤더
		COL.iSubItem = 2;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		COL.pszText = (LPSTR)"권한";    //네 번째 헤더
		COL.iSubItem = 3;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 3, (LPARAM)&COL);

		Load_Employee_Data();

		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}