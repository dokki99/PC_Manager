#include "DB.h"

// DB���� ����///////////////////////////////////////////////

extern TCHAR DB_ID[];
extern TCHAR DB_PWD[];
extern SQLHENV hEnv;
extern SQLHDBC hDbc;
extern SQLHSTMT hStmt;

/////////////////////////////////////////////////////////////

extern HWND Stock_I_List;		// ��� ����Ʈ��
extern HWND Revenue_I_List;		// ���� ����Ʈ��
extern HWND Employee_I_List;	// ���� ����Ʈ��
extern HWND Customer_I_List;	// ȸ�� ����Ʈ��
extern HWND Work_I_List;		// �ٹ� ����Ʈ��


/*--------------------------------------------------------
 DBConnect(): DB ����
--------------------------------------------------------*/
BOOL DBConnect() {
	// ���� ������ ���� ����
	SQLRETURN Ret;

	// ȯ�� �ڵ��� �Ҵ��ϰ� ���� �Ӽ��� �����Ѵ�.
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) return FALSE;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS) return FALSE;

	// ���� �ڵ��� �Ҵ��ϰ� �����Ѵ�
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) return FALSE;
	Ret = SQLConnect(hDbc, (SQLCHAR*)"PCDB", SQL_NTS, (SQLCHAR*)DB_ID, SQL_NTS, (SQLCHAR*)DB_PWD, SQL_NTS);
	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO)) return FALSE;

	// ��� �ڵ��� �Ҵ��Ѵ�.
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) return FALSE;

	return TRUE;
}


/*--------------------------------------------------------
 DBDisConnect(): DB ����
--------------------------------------------------------*/
void DBDisconnect() {
	// ������
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

/*--------------------------------------------------------
 DBExcuteSQL(): ������ ��������
--------------------------------------------------------*/
BOOL DBExcuteSQL() {
	// ������� �����ޱ� ���� ������
	SQLCHAR S_Code[30], S_Name[30], S_Category[30];
	int S_Price, S_Amount;
	double S_Margin;
	SQLLEN I_S_Code, I_S_Name, I_S_Category, I_S_Price, I_S_Amount, I_S_Margin;

	// ȭ�� ����� ���� ������
	TCHAR str[255];

	// ����� �����ޱ� ���� ���ε��Ѵ�.
	SQLBindCol(hStmt, 1, SQL_C_CHAR, S_Code, sizeof(S_Code), &I_S_Code);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, S_Name, sizeof(S_Name), &I_S_Name);
	SQLBindCol(hStmt, 3, SQL_C_ULONG, &S_Price, 0, &I_S_Price);
	SQLBindCol(hStmt, 4, SQL_C_ULONG, &S_Amount, 0, &I_S_Amount);
	SQLBindCol(hStmt, 5, SQL_C_CHAR, S_Category, sizeof(S_Category), &I_S_Category);
	SQLBindCol(hStmt, 6, SQL_C_ULONG, &S_Margin, 0, &I_S_Margin);

	// SQL���� �����Ѵ�
	if (SQLExecDirect(hStmt, (SQLCHAR*)"select S_Code, S_Name, S_Price, S_Amount,S_Category, S_Margin from dbo.Stock_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	// �о�� ������ ���
	while (SQLFetch(hStmt) != SQL_NO_DATA) {
		wsprintf(str, "�̸�:%s\t����:%d", S_Name, S_Price);

	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Load_Stock_Data(): ������� ��������
--------------------------------------------------------*/
BOOL Load_Stock_Data() {
	LVITEM LI;
	SQLCHAR S_Code[30], S_Name[30], S_Category[30];
	int S_Price, S_Amount;
	double S_Margin;
	SQLLEN I_S_Code, I_S_Name, I_S_Category, I_S_Price, I_S_Amount, I_S_Margin;
	TCHAR Price[100], Amount[100], Margin[100];

	SQLBindCol(hStmt, 1, SQL_C_CHAR, S_Code, sizeof(S_Code), &I_S_Code);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, S_Name, sizeof(S_Name), &I_S_Name);
	SQLBindCol(hStmt, 3, SQL_C_ULONG, &S_Price, 0, &I_S_Price);
	SQLBindCol(hStmt, 4, SQL_C_ULONG, &S_Amount, 0, &I_S_Amount);
	SQLBindCol(hStmt, 5, SQL_C_CHAR, S_Category, sizeof(S_Category), &I_S_Category);
	SQLBindCol(hStmt, 6, SQL_C_DOUBLE, &S_Margin, 0, &I_S_Margin);

	if (SQLExecDirect(hStmt, (SQLCHAR*)"select S_Code, S_Name, S_Price, S_Amount, S_Category ,S_Margin from dbo.Stock_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	LI.mask = LVIF_TEXT;

	while (SQLFetch(hStmt) != SQL_NO_DATA) {
		// ������
		LI.iItem = 0;
		LI.iSubItem = 0;
		LI.pszText = (LPSTR)S_Code;
		SendMessage(Stock_I_List, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 1;
		LI.pszText = (LPSTR)S_Name;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 2;
		wsprintf(Price, "%d", S_Price);
		LI.pszText = Price;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 3;
		wsprintf(Amount, "%d", S_Amount);
		LI.pszText = Amount;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 4;
		LI.pszText = (LPSTR)S_Category;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 5;
		sprintf_s(Margin, "%.2f(%%)", S_Margin);
		LI.pszText = Margin;
		SendMessage(Stock_I_List, LVM_SETITEM, 0, (LPARAM)&LI);
	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Load_Revenue_Data(): �������� ��������
--------------------------------------------------------*/
BOOL Load_Revenue_Data() {
	LVITEM LI;
	SQLCHAR S_Code[30], E_ID[30];
	TIMESTAMP_STRUCT R_Date;
	int R_No, R_Amount;
	SQLLEN I_R_No, I_S_Code, I_E_ID, I_R_Date, I_R_Amount;
	TCHAR No[100], Amount[100], Date[100];

	SQLBindCol(hStmt, 1, SQL_C_ULONG, &R_No, 0, &I_R_No);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, S_Code, sizeof(S_Code), &I_S_Code);
	SQLBindCol(hStmt, 3, SQL_C_CHAR, E_ID, sizeof(E_ID), &I_E_ID);
	SQLBindCol(hStmt, 4, SQL_C_TYPE_TIMESTAMP, &R_Date, sizeof(R_Date), &I_R_Date);
	SQLBindCol(hStmt, 5, SQL_C_ULONG, &R_Amount, 0, &I_R_Amount);

	if (SQLExecDirect(hStmt, (SQLCHAR*)"select R_No, S_Code, E_ID, R_Date, R_Amount from dbo.Revenue_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	LI.mask = LVIF_TEXT;

	while (SQLFetch(hStmt) != SQL_NO_DATA) {
		// ������
		LI.iItem = 0;
		LI.iSubItem = 0;
		wsprintf(No, "%d", R_No);
		LI.pszText = No;
		SendMessage(Revenue_I_List, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 1;
		LI.pszText = (LPSTR)S_Code;
		SendMessage(Revenue_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 2;
		LI.pszText = (LPSTR)E_ID;
		SendMessage(Revenue_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 3;
		wsprintf(Date, "%d-%d-%d %d:%d:%d", R_Date.year, R_Date.month, R_Date.day, R_Date.hour, R_Date.minute, R_Date.second);
		LI.pszText = Date;
		SendMessage(Revenue_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 4;
		wsprintf(Amount, "%d", R_Amount);
		LI.pszText = Amount;
		SendMessage(Revenue_I_List, LVM_SETITEM, 0, (LPARAM)&LI);
	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Load_Employee_Data(): �������� ��������
--------------------------------------------------------*/
BOOL Load_Employee_Data() {
	LVITEM LI;
	SQLCHAR E_ID[30], E_Name[30], E_PWD[30], E_Permission;
	SQLLEN I_E_ID, I_E_Name, I_E_PWD, I_E_Permission;
	TCHAR Permission[30];

	SQLBindCol(hStmt, 1, SQL_C_CHAR, E_ID, sizeof(E_ID), &I_E_ID);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, E_Name, sizeof(E_Name), &I_E_Name);
	SQLBindCol(hStmt, 3, SQL_C_CHAR, E_PWD, sizeof(E_PWD), &I_E_PWD);
	SQLBindCol(hStmt, 4, SQL_C_BIT, &E_Permission, sizeof(E_Permission), &I_E_Permission);

	if (SQLExecDirect(hStmt, (SQLCHAR*)"select E_ID, E_Name, E_PWD, E_Permission from dbo.Employee_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	LI.mask = LVIF_TEXT;

	while (SQLFetch(hStmt) != SQL_NO_DATA) {
		// ������
		LI.iItem = 0;
		LI.iSubItem = 0;
		LI.pszText = (LPSTR)E_ID;
		SendMessage(Employee_I_List, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 1;
		LI.pszText = (LPSTR)E_Name;
		SendMessage(Employee_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 2;
		LI.pszText = (LPSTR)E_PWD;
		SendMessage(Employee_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 3;
		wsprintf(Permission, "%d", E_Permission);
		if (lstrcmp(Permission, "1") == 0) {
			LI.pszText = (LPSTR)"True";
		}
		else {
			LI.pszText = (LPSTR)"False";
		}
		SendMessage(Employee_I_List, LVM_SETITEM, 0, (LPARAM)&LI);
	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Load_Customer_Data(): ȸ������ ��������
--------------------------------------------------------*/
BOOL Load_Customer_Data() {
	LVITEM LI;
	SQLCHAR C_ID[30], C_PWD[30], C_Phone[50];
	TIMESTAMP_STRUCT C_Time;
	SQLLEN I_C_ID, I_C_PWD, I_C_Phone, I_C_Time;
	TCHAR Date[100];

	SQLBindCol(hStmt, 1, SQL_C_CHAR, C_ID, sizeof(C_ID), &I_C_ID);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, C_PWD, sizeof(C_PWD), &I_C_PWD);
	SQLBindCol(hStmt, 3, SQL_C_CHAR, C_Phone, sizeof(C_Phone), &I_C_Phone);
	SQLBindCol(hStmt, 4, SQL_C_TYPE_TIMESTAMP, &C_Time, sizeof(C_Time), &I_C_Time);

	if (SQLExecDirect(hStmt, (SQLCHAR*)"select C_ID, C_PWD, C_Phone, C_Time from dbo.Customer_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	LI.mask = LVIF_TEXT;

	while (SQLFetch(hStmt) != SQL_NO_DATA) {

		// ������
		LI.iItem = 0;
		LI.iSubItem = 0;
		LI.pszText = (LPSTR)C_ID;
		SendMessage(Customer_I_List, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 1;
		LI.pszText = (LPSTR)C_PWD;
		SendMessage(Customer_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 2;
		LI.pszText = (LPSTR)C_Phone;
		SendMessage(Customer_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 3;
		wsprintf(Date, "%d-%d-%d %d:%d:%d", C_Time.year, C_Time.month, C_Time.day, C_Time.hour, C_Time.minute, C_Time.second);
		LI.pszText = Date;
		SendMessage(Customer_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Load_Work_Data(): �ٹ����� ��������
--------------------------------------------------------*/
BOOL Load_Work_Data() {
	LVITEM LI;
	int W_No;
	SQLCHAR  E_ID[30];
	TIMESTAMP_STRUCT W_SDate, W_EDate;
	SQLLEN I_W_No, I_E_ID, I_W_SDate, I_W_EDate;
	TCHAR No[100], Date[100];

	SQLBindCol(hStmt, 1, SQL_C_ULONG, &W_No, 0, &I_W_No);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, E_ID, sizeof(E_ID), &I_E_ID);
	SQLBindCol(hStmt, 3, SQL_C_TYPE_TIMESTAMP, &W_SDate, sizeof(W_SDate), &I_W_SDate);
	SQLBindCol(hStmt, 4, SQL_C_TYPE_TIMESTAMP, &W_EDate, sizeof(W_EDate), &I_W_EDate);

	if (SQLExecDirect(hStmt, (SQLCHAR*)"select W_No, E_ID, W_Start, W_End from dbo.Work_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	LI.mask = LVIF_TEXT;

	while (SQLFetch(hStmt) != SQL_NO_DATA) {

		// ������
		LI.iItem = 0;
		LI.iSubItem = 0;
		wsprintf(No, "%d", W_No);
		LI.pszText = No;
		SendMessage(Work_I_List, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 1;
		LI.pszText = (LPSTR)E_ID;
		SendMessage(Work_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 2;
		wsprintf(Date, "%d-%d-%d %d:%d:%d", W_SDate.year, W_SDate.month, W_SDate.day, W_SDate.hour, W_SDate.minute, W_SDate.second);
		LI.pszText = (LPSTR)Date;
		SendMessage(Work_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 3;
		wsprintf(Date, "%d-%d-%d %d:%d:%d", W_EDate.year, W_EDate.month, W_EDate.day, W_EDate.hour, W_EDate.minute, W_EDate.second);
		LI.pszText = (LPSTR)Date;
		SendMessage(Work_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Login_Info_Check(TCHAR* ,TCHAR *, int ): 
 �α��� / ȸ������ ���� ���� Ȯ���ϴ� �Լ�
 
 state = 1 : ID,PWD		$ �α���
 state = 2 : ID,pNum	$ ��й�ȣ �ʱ�ȭ
 state = 3 : ID,pNum	$ ȸ������ �� �������
--------------------------------------------------------*/
BOOL Login_Info_Check(const TCHAR* ID, const TCHAR* Data, int state) {
	int Cnt;
	TCHAR Query[200];
	SQLLEN I_Cnt;

	lstrcpy(Query, "select COUNT(*) from dbo.Customer_Info where C_ID = '");
	lstrcat(Query, ID);
	if (state == 1) {
		lstrcat(Query, "' AND C_PWD = '");
	}
	else if(state == 2) {
		lstrcat(Query, "' AND C_Phone = '");
	}
	else if (state == 3) {
		lstrcat(Query, "' OR C_Phone = '");				// ��������� ��쿡��
	}
	lstrcat(Query, Data);
	lstrcat(Query, "'");

	SQLBindCol(hStmt, 1, SQL_C_ULONG, &Cnt, 0, &I_Cnt);

	if(SQLExecDirect(hStmt, (SQLCHAR*)Query, SQL_NTS) != SQL_SUCCESS) return FALSE;	

	SQLFetch(hStmt);
	
	if (hStmt) SQLCloseCursor(hStmt);

	return Cnt != 0 ? TRUE : FALSE;
}

/*--------------------------------------------------------
 Get_Time(TCHAR* ): �α��� ������ �´� ȸ���� ����
 �ð��� �����ɴϴ�.
--------------------------------------------------------*/
int Get_Time(TCHAR* ID) {
	int Second;
	TCHAR Query[200];
	SQLLEN L_Second;

	lstrcpy(Query, "select C_TIME from dbo.Customer_Info where C_ID = '");
	lstrcat(Query, ID);
	lstrcat(Query, "'");

	SQLBindCol(hStmt, 1, SQL_C_ULONG, &Second, 0, &L_Second);

	if (SQLExecDirect(hStmt, (SQLCHAR*)Query, SQL_NTS) != SQL_SUCCESS) return -1;

	SQLFetch(hStmt);

	if (hStmt) SQLCloseCursor(hStmt);

	return Second;
}

/*--------------------------------------------------------
 Charge_Time(TCHAR* ,int ): �α��� ������ �´�
 ȸ���� �ð��� �����մϴ�.
--------------------------------------------------------*/
BOOL Charge_Time(TCHAR* ID,int Total_Second) {
	TCHAR Query[200],Charge_Time[15];

	wsprintf(Charge_Time, "%d", Total_Second);

	lstrcpy(Query, "UPDATE Customer_Info SET C_TIME = ");
	lstrcat(Query, Charge_Time);
	lstrcat(Query, " WHERE C_ID = '");
	lstrcat(Query, ID);
	lstrcat(Query, "'");

	if (SQLExecDirect(hStmt, (SQLCHAR*)Query, SQL_NTS) != SQL_SUCCESS) return FALSE;

	SQLFetch(hStmt);

	if (hStmt) SQLCloseCursor(hStmt);

	return TRUE;
}


/*--------------------------------------------------------
 Find_ID(TCHAR* ,TCHAR *): �ڵ�����ȣ�� ��Ī�Ǵ� ID ã��
--------------------------------------------------------*/
BOOL Find_ID(TCHAR* ID, TCHAR* pNum) {
	TCHAR C_ID[14];
	TCHAR Query[200];
	SQLLEN I_C_ID;

	lstrcpy(C_ID, "");	// �̺κ��� DB���� �����̱� ������ ���� �ʱ�ȭ�ؼ� �ȵ˴ϴ�.

	lstrcpy(Query, "select C_ID from dbo.Customer_Info where C_Phone = '");
	lstrcat(Query, pNum);
	lstrcat(Query, "'");

	SQLBindCol(hStmt, 1, SQL_C_CHAR, C_ID, sizeof(C_ID), &I_C_ID);

	if (SQLExecDirect(hStmt, (SQLCHAR*)Query, SQL_NTS) != SQL_SUCCESS) return FALSE;

	SQLFetch(hStmt);

	if (hStmt) SQLCloseCursor(hStmt);

	if (lstrcmp(C_ID, "") == 0)	return FALSE;
	
	lstrcpy(ID, C_ID);
	
	return TRUE;
}

/*--------------------------------------------------------
 PWD_Reset(TCHAR* ,TCHAR *): ���̵� �ڵ��� ��ȣ�� �Է��Ͽ�
 ��й�ȣ 123456789a�� �ʱ�ȭ
--------------------------------------------------------*/
BOOL PWD_Reset(TCHAR* ID, TCHAR* pNum) {
	TCHAR Query[200];

	lstrcpy(Query, "update dbo.Customer_Info set C_PWD = '123456789a' where C_ID = '");
	lstrcat(Query, ID);
	lstrcat(Query, "' AND C_Phone = '");
	lstrcat(Query, pNum);
	lstrcat(Query, "'");

	if (SQLExecDirect(hStmt, (SQLCHAR*)Query, SQL_NTS) != SQL_SUCCESS) return FALSE;

	return TRUE;
	
}

/*--------------------------------------------------------
 Regist_Customer(TCHAR* , TCHAR* , TCHAR *): ȸ������ �Լ�
--------------------------------------------------------*/
BOOL Regist_Customer(TCHAR* ID, TCHAR* PWD, TCHAR *pNum) {
	TCHAR Query[200];

	lstrcpy(Query, "INSERT INTO Customer_Info VALUES(");
	lstrcat(Query, ID);
	lstrcat(Query, ", ");
	lstrcat(Query, PWD);
	lstrcat(Query, ", ");
	lstrcat(Query, pNum);
	lstrcat(Query, ", 0)");

	if (SQLExecDirect(hStmt, (SQLCHAR*)Query, SQL_NTS) != SQL_SUCCESS) return FALSE;

	return TRUE;
}