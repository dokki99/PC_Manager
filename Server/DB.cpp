#include "DB.h"

// DB관련 변수///////////////////////////////////////////////

extern TCHAR ID[];
extern TCHAR PWD[];
extern SQLHENV hEnv;
extern SQLHDBC hDbc;
extern SQLHSTMT hStmt;

/////////////////////////////////////////////////////////////

extern HWND Stock_I_List;		// 재고 리스트뷰
extern HWND Revenue_I_List;		// 매출 리스트뷰
extern HWND Employee_I_List;	// 직원 리스트뷰
extern HWND Customer_I_List;	// 회원 리스트뷰
extern HWND Work_I_List;		// 근무 리스트뷰

/*--------------------------------------------------------
 DBConnect(): DB 연결
--------------------------------------------------------*/
BOOL DBConnect() {
	// 연결 설정을 위한 변수
	SQLRETURN Ret;

	// 환경 핸들을 할당하고 버전 속성을 설정한다.
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) return FALSE;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS) return FALSE;

	// 연결 핸들을 할당하고 연결한다
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) return FALSE;
	Ret = SQLConnect(hDbc, (SQLCHAR*)"PCDB", SQL_NTS, (SQLCHAR*)ID, SQL_NTS, (SQLCHAR*)PWD, SQL_NTS);
	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO)) return FALSE;

	// 명령 핸들을 할당한다.
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) return FALSE;

	return TRUE;
}


/*--------------------------------------------------------
 DBDisConnect(): DB 해제
--------------------------------------------------------*/
void DBDisconnect() {
	// 뒷정리
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

/*--------------------------------------------------------
 DBExcuteSQL(): 데이터 가져오기
--------------------------------------------------------*/
BOOL DBExcuteSQL() {
	// 결과값을 돌려받기 위한 변수들
	SQLCHAR S_Code[30], S_Name[30], S_Category[30];
	int S_Price, S_Amount;
	double S_Margin;
	SQLLEN I_S_Code, I_S_Name, I_S_Category, I_S_Price, I_S_Amount, I_S_Margin;

	// 화면 출력을 위한 변수들
	TCHAR str[255];

	// 결과를 돌려받기 위해 바인딩한다.
	SQLBindCol(hStmt, 1, SQL_C_CHAR, S_Code, sizeof(S_Code), &I_S_Code);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, S_Name, sizeof(S_Name), &I_S_Name);
	SQLBindCol(hStmt, 3, SQL_C_ULONG, &S_Price, 0, &I_S_Price);
	SQLBindCol(hStmt, 4, SQL_C_ULONG, &S_Amount, 0, &I_S_Amount);
	SQLBindCol(hStmt, 5, SQL_C_CHAR, S_Category, sizeof(S_Category), &I_S_Category);
	SQLBindCol(hStmt, 6, SQL_C_ULONG, &S_Margin, 0, &I_S_Margin);

	// SQL문을 실행한다
	if (SQLExecDirect(hStmt, (SQLCHAR*)"select S_Code, S_Name, S_Price, S_Amount,S_Category, S_Margin from dbo.Stock_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	// 읽어온 데이터 출력
	while (SQLFetch(hStmt) != SQL_NO_DATA) {
		wsprintf(str, "이름:%s\t가격:%d", S_Name, S_Price);

	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Load_Stock_Data(): 재고정보 가져오기
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
		// 아이템
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
 Load_Revenue_Data(): 매출정보 가져오기
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
		// 아이템
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
 Load_Employee_Data(): 직원정보 가져오기
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
		// 아이템
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
 Load_Customer_Data(): 회원정보 가져오기
--------------------------------------------------------*/
BOOL Load_Customer_Data() {
	LVITEM LI;
	SQLCHAR C_ID[30], C_Name[30], C_PWD[30], C_Phone[50], C_Adress[50], C_Birth[15];
	TIMESTAMP_STRUCT C_Time;
	SQLLEN I_C_ID, I_C_Name, I_C_PWD, I_C_Phone, I_C_Adress, I_C_Birth, I_C_Time;
	TCHAR Date[100];

	SQLBindCol(hStmt, 1, SQL_C_CHAR, C_ID, sizeof(C_ID), &I_C_ID);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, C_Name, sizeof(C_Name), &I_C_Name);
	SQLBindCol(hStmt, 3, SQL_C_CHAR, C_PWD, sizeof(C_PWD), &I_C_PWD);
	SQLBindCol(hStmt, 4, SQL_C_CHAR, C_Phone, sizeof(C_Phone), &I_C_Phone);
	SQLBindCol(hStmt, 5, SQL_C_CHAR, C_Adress, sizeof(C_Adress), &I_C_Adress);
	SQLBindCol(hStmt, 6, SQL_C_CHAR, C_Birth, sizeof(C_Birth), &I_C_Birth);
	SQLBindCol(hStmt, 7, SQL_C_TYPE_TIMESTAMP, &C_Time, sizeof(C_Time), &I_C_Time);

	if (SQLExecDirect(hStmt, (SQLCHAR*)"select C_ID, C_Name, C_PWD, C_Phone, C_Adress, C_Birth, C_Time from dbo.Customer_Info", SQL_NTS) != SQL_SUCCESS) return FALSE;

	LI.mask = LVIF_TEXT;

	while (SQLFetch(hStmt) != SQL_NO_DATA) {

		// 아이템
		LI.iItem = 0;
		LI.iSubItem = 0;
		LI.pszText = (LPSTR)C_ID;
		SendMessage(Customer_I_List, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 1;
		LI.pszText = (LPSTR)C_Name;
		SendMessage(Customer_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 2;
		LI.pszText = (LPSTR)C_PWD;
		SendMessage(Customer_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 3;
		LI.pszText = (LPSTR)C_Phone;
		SendMessage(Customer_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 4;
		LI.pszText = (LPSTR)C_Adress;
		SendMessage(Customer_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 5;
		LI.pszText = (LPSTR)C_Birth;
		SendMessage(Customer_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem = 6;
		wsprintf(Date, "%d-%d-%d %d:%d:%d", C_Time.year, C_Time.month, C_Time.day, C_Time.hour, C_Time.minute, C_Time.second);
		LI.pszText = Date;
		SendMessage(Customer_I_List, LVM_SETITEM, 0, (LPARAM)&LI);

	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

/*--------------------------------------------------------
 Load_Work_Data(): 근무정보 가져오기
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

		// 아이템
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