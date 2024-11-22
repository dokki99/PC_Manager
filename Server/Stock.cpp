#include "Stock.h"

extern HINSTANCE g_hInst;
extern HWND hWndMain;		// ���� ȭ�� �ڵ�
extern HWND Stock_I_List;		// ��� ����Ʈ��

/*--------------------------------------------------------
 Stock_Info_Proc: ������� ���ν���
--------------------------------------------------------*/
LRESULT CALLBACK Stock_Info_Proc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN COL;
	switch (iMessage) {
	case WM_CREATE:
		Stock_I_List = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, 20, 20, 800, 400, hWnd, NULL, g_hInst, NULL);
		ListView_SetExtendedListViewStyle(Stock_I_List, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_LEFT;
		COL.cx = 130;

		COL.pszText = (LPSTR)"��ǰ�ڵ�";    //ù ��° ���
		COL.iSubItem = 0;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

		COL.pszText = (LPSTR)"��ǰ��";    //�� ��° ���
		COL.iSubItem = 1;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = (LPSTR)"����";    //�� ��° ���
		COL.iSubItem = 2;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		COL.pszText = (LPSTR)"����";    //�� ��° ���
		COL.iSubItem = 3;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 3, (LPARAM)&COL);

		COL.pszText = (LPSTR)"ǰ��";    //�ټ� ��° ���
		COL.iSubItem = 4;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 4, (LPARAM)&COL);

		COL.pszText = (LPSTR)"������";    //���� ��° ���
		COL.iSubItem = 5;
		SendMessage(Stock_I_List, LVM_INSERTCOLUMN, 5, (LPARAM)&COL);

		Load_Stock_Data();

		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}