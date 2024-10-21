#include "Employee.h"

extern HINSTANCE g_hInst;
extern HWND hWndMain;		// ���� ȭ�� �ڵ�
extern HWND Employee_I_List;	// ���� ����Ʈ��

/*--------------------------------------------------------
 Employee_Info_Proc: �������� ���ν���
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

		COL.pszText = (LPSTR)"���̵�";    //ù ��° ���
		COL.iSubItem = 0;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

		COL.pszText = (LPSTR)"�̸�";    //�� ��° ���
		COL.iSubItem = 1;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

		COL.pszText = (LPSTR)"��й�ȣ";    //�� ��° ���
		COL.iSubItem = 2;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

		COL.pszText = (LPSTR)"����";    //�� ��° ���
		COL.iSubItem = 3;
		SendMessage(Employee_I_List, LVM_INSERTCOLUMN, 3, (LPARAM)&COL);

		Load_Employee_Data();

		return 0;
	case WM_DESTROY:
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}