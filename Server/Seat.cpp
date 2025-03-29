#include "Seat.h"

/*--------------------------------------------------------
 Create_SEAT(): �¼� �ʱ�ȭ �Լ�
--------------------------------------------------------*/
SEAT* Create_SEAT() {
	SEAT* N;
	static int num = 1;

	N = (SEAT*)malloc(sizeof(SEAT));

	N->S_num = num++;
	N->State = 0;
	N->Client_Sock = NULL;
	N->Client_Info = NULL;
	N->Thread_ID = 0;

	return N;
}