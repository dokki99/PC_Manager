#ifndef __DB__
#define __DB__

#include "Common.h"
#include <sql.h>
#include <sqlext.h>

// DB관련 함수///////////////////////////////////////////////

BOOL DBConnect();
void DBDisconnect();
BOOL DBExcuteSQL();
BOOL Load_Stock_Data();
BOOL Load_Revenue_Data();
BOOL Load_Employee_Data();
BOOL Load_Customer_Data();
BOOL Load_Work_Data();
/////////////////////////////////////////////////////////////


#endif