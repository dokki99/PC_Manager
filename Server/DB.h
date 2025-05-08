#ifndef __DB_H__
#define __DB_H__

#include "Common.h"
#include <sql.h>
#include <sqlext.h>

// DB���� �Լ�///////////////////////////////////////////////

BOOL DBConnect();
void DBDisconnect();
BOOL DBExcuteSQL();
BOOL Load_Stock_Data();
BOOL Load_Revenue_Data();
BOOL Load_Employee_Data();
BOOL Load_Customer_Data();
BOOL Load_Work_Data();

BOOL Login_Info_Check(const TCHAR* , const TCHAR *, int);
int Get_Time(TCHAR*);
BOOL Charge_Time(TCHAR*, int);
BOOL Find_ID(TCHAR*, TCHAR*);
BOOL PWD_Reset(TCHAR*, TCHAR*);
BOOL Regist_Customer(TCHAR*, TCHAR*, TCHAR*);
/////////////////////////////////////////////////////////////


// DB�����Լ� �ظ��ϸ� ���� ���ν����� �ٲܼ� �ִ°� ���� ���ν����� ����ô�!

#endif