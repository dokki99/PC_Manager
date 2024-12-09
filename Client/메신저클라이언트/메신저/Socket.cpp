#include <Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Common.h"
#include "Socket.h"

extern HWND hWndMain;
////IP와 포트번호 설정
extern const char g_szlpAddress[17] = "127.0.0.1";
extern const unsigned short g_uPort = 7878;
extern TCHAR tgIp[17];
extern TCHAR tgPort[10];
extern TCHAR tgId[10], tgName[10], tgPw[10], tgPn[10], tgAddr[10], tgBirth[10];																//아이디,이름,비번,전화번호,주소,생일 문자열 변수들
extern int nip, nport;

extern DWORD ThreadID;																																					//스레드아이디
extern HANDLE hThread;

extern TCHAR tgcmdserver[200];																																		//서버와 통신하는 커멘드 문자열
////
//리시브스레드 기본 변수
extern HDC hdc;
extern SOCKET* P;
extern TCHAR CODE[3], TEXT[256];
extern int j;
//
///소켓 관련 변수
extern int nReturn;																																								//통신 결과 
extern WSADATA wsadata;																																				//소켓 연결
extern SOCKET clientsock;																																					//소켓번호 담기
extern sockaddr_in addr_client;
extern int addrlen_clt;
///서버로부터 받는 메세지
extern char buf[1024];																																						//메시지 문자열																						
extern char strTemp[1024];																																				//메세지를 채팅창에 수신한 메세지로 변환
///

extern int infotrue;																																							//각 정보 담기 제어
////상황별 주요변수
//커멘드에 여러 정보가 있을때
extern int turn;																																								//유저의 정보를 순서대로 가져오게 하는 순서 제어
extern int ini;																																										//각 정보마다 버퍼(메세지)의 인덱스
extern TCHAR info[200];																																					//요소를 뽑는 문자열
extern int infoi;																																								//요소를 뽑는 문자열의 인덱스
//
//로그인상황의  유저의 변수들
extern TCHAR userid[10];																																					//아이디
extern TCHAR usersn[10];																																					//자리번호
extern TCHAR userpw[10];																																					//비번
extern int pctimei;																																								//시간 인덱스
extern int pctime;																																								//로그인된 유저의 시간
//
///주문상황의 변수들
//주문시간
extern int orderpctime;																																						//주문한  시간으로 기존에 병합
//아이템주문갯수
extern TCHAR tgselitemi[10];																																				//인덱스를 문자열로 변환
extern int selitemi;																																								//리스트 박스의 인덱스
extern TCHAR tgitemcount[10];																																	//개수 문자열
//
///
//상태 체크 변수
extern BOOL chkseat;																																							//자리선택
extern BOOL chkjoin;																																							//가입
extern BOOL chklogin;																																						//로그인
extern BOOL chktimer;																																							//타이머 제어(1이면 실행/0이면 종료)	
extern BOOL chkendbtn;																																					//로그아웃/탈퇴 

extern BOOL chkorder;																																						//주문
extern BOOL chkupdate;																																					//수정
extern BOOL chkpctime;																																					//남은시간0인지
//
extern int i;
//자리번호 선택 변수들
extern TCHAR tgnum[10];																																					//신청하는 자리번호 문자열

//커맨드가 주문인지 판별
extern TCHAR orderbuf[10];																																				//주문요소정보(미리 메세지가 주문인지 판별)
extern int orderi;																																							//주문요소정보 인덱스
//

//유저시간
extern TCHAR pctimebuf[10];																																				//커맨드에서 담아오기
extern TCHAR tpctime[10];																																					//출력하기 위한 문자열
//

//좌석반환변수
extern int returnsntime;																																//자리반납타이머시간
extern BOOL returnsn;																																						//자리반납수행

//자리선택으로 보내는 변수
extern BOOL gohome;

//void CLIENT_Open():소켓 연결 함수(연결스레드 만들기)
void CLIENT_Open() {
	nip = atoi(tgIp);
	nport = atoi(tgPort);
	nReturn = WSAStartup(WORD(2.0), &wsadata);
	clientsock = socket(AF_INET, SOCK_STREAM, 0);
	addr_client.sin_family = AF_INET;
	addr_client.sin_addr.s_addr = inet_addr(tgIp);
	addr_client.sin_port = htons(nport);
	hThread = CreateThread(NULL, 0, ThreadFunc, &clientsock, 0, &ThreadID);																	//연결스레드 만들기
	nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);
	if (nReturn) {
		MessageBox(hWndMain, "연결되었습니다!!", "오류", MB_OK);
	}
}

//DWORD WINAPI ThreadFunc(LPVOID){}:리시브스레드로써 요청의 결과를 다룸
//머릿말인 숫자 다음 숫자가  1이 오면 가능/ 0이 오면 불가
DWORD WINAPI ThreadFunc(LPVOID Param) {
	//HDC hdc;
	//SOCKET* P = (SOCKET*)Param;
	//TCHAR CODE[3] = "", TEXT[256] = "";
	//int j;
	P = (SOCKET*)Param;
	for (;;) {
		lstrcpy(buf, "");
		nReturn = recv(*P, buf, 1024, 0);

		if (nReturn == 0 || nReturn == SOCKET_ERROR) {
			continue;
		}
		else {

			j = 0;
			for (int i = 0; i < lstrlen(buf); i++) {
				if (i > 2) {
					CODE[i] = buf[i];
				}
				else if (i != 3) {
					TEXT[j++] = buf[i];
				}
			}
			CODE[lstrlen(CODE)] = '\0';
			TEXT[lstrlen(TEXT)] = '\0';

			//자리선택 결과(가능:11/불가:10)
			if (buf[0] == '1') {
				if (lstrcmp(buf, TEXT("11")) == 0) {
					returnsntime = 100000;																													//자리반납타이머시간 초기화
					returnsn = 1;																																			//자리반납동작
					chkseat = 1;																																			//자리선택 체크
					//SetTimer(hWndMain, 0, 100, NULL);																									//자동으로 화면모드 설정 타이머
					//SetTimer(hWndMain, 3, 1000000, NULL);																							//자리반납타이머
					//SendMessage(hWndMain, WM_TIMER, 0, 0);
					//SendMessage(hWndMain, WM_TIMER, 3, 0);
					lstrcpy(usersn, tgnum);																															//자리번호 담기		
					lstrcpy(tgnum, "");																																//자리번호 초기화
					wsprintf(buf, "%s", "자리사용가능");
				}
				else wsprintf(buf, "%s", "자리사용불가");
			}
			//회원가입 결과(가능:31/불가:30)
			else if (buf[0] == '3') {
				if (lstrcmp(buf, TEXT("31")) == 0) {
					chkjoin = 1;
					wsprintf(buf, "%s", "회원가입성공");
				}
				else wsprintf(buf, "%s", "회원가입실패");
			}
			//로그인 결과(가능:41NAME,PN,ADDR,BIRTH/불가:4100 또는 40)
			else if (buf[0] == '4') {
				if (buf[1] == '1') {
					infoi = 0;
					ini = 2;
					while (buf[ini] != ':' && buf[ini] != '\0') {																		///":"전까지 담기
						info[infoi++] = buf[ini++];
					}
					info[infoi] = '\0';
					lstrcpy(pctimebuf, info);
					//MessageBox(hWndMain, pctimebuf, "pctimebuf", MB_OK);
					pctime = atoi(pctimebuf);
					//
					//가능할때 입력한 아이디 비번을 저장하고 이름,전화번호,주소,생일들을 담기
					if (pctime >= 0) {
						lstrcpy(userid, tgId);																											//계정 아이디,비번 담기
						lstrcpy(userpw, tgPw);
						i = ini+1;																																		//로그인 커맨드의 가져올 정보의  시작점
						turn = -1;																																//가져올 정보를 순서대로 가져오게 제어
						///로그인 커멘드에서 나머지 정보를 담는 프로세스(NAME-PN-ADDR-BIRTH순)
						while (buf[i] != '\0') {
							infoi = 0;
							if (buf[i] == ':') {																												///각 정보의 시작점으로 이동
								ini = i + 1;
								turn++;
								infotrue = 0;
							}
							if (turn == 0&&infotrue==0) {
								while (buf[ini] != ':' && buf[ini] != '\0') {																		///":"전까지 담기
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 2) infoi -= 2;																								///다음 정보 커맨드(PW,NAME..) 길이 만큼 빼기
								info[infoi] = '\0';
								lstrcpy(tgName, info);
								infotrue = 1;
							}
							else if (turn == 1&&infotrue==0) {
								while (buf[ini] != ':' && buf[ini] != '\0') {
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 4) infoi -= 4;
								info[infoi] = '\0';
								lstrcpy(tgPn, info);
								infotrue = 1;
							}
							else if (turn == 2&&infotrue==0) {
								while (buf[ini] != ':' && buf[ini] != '\0') {
									info[infoi++] = buf[ini++];
								}
								if (infoi >= 5) infoi -= 5;
								info[infoi] = '\0';
								lstrcpy(tgAddr, info);
								infotrue = 1;
							}
							else if (turn == 3&&infotrue==0) {
								while (buf[ini] != '\0') {																									///마지막까지 담기
									info[infoi++] = buf[ini++];
								}
								info[infoi] = '\0';
								lstrcpy(tgBirth, info);
								turn = -1;
								break;
							}
							i++;
						}
						i = 0;
						wsprintf(tpctime, "%d", pctime);																								//남은시간을 출력하기위해 문자열로

						if (pctime > 0) {
							chklogin = 1;
							chktimer = 1;																														//로그인되면 타이머 동작
							wsprintf(buf, "%s", "로그인성공");
						}
						else {
							wsprintf(buf, "%s", "로그인실패(잔여시간없음)");
							chkpctime = 1;
						}
					}
				}
				else wsprintf(buf, "%s", "로그인실패");
			}
			////주문결과(가능:2상품번호(1,2,3)1/불가:2상품번호(1,2,3)0)
			else if (buf[0] == '2') {
				//음료
				if (buf[1] == '1') {
					if (lstrcmp(buf, TEXT("211")) == 0) {
						chkorder = 1;
						wsprintf(buf, "%s", "DRINK구입성공");
					}
					else wsprintf(buf, "%s", "DRINK구입실패");
				}
				//식품
				else if (buf[1] == '2') {
					if (lstrcmp(buf, TEXT("221")) == 0) {
						chkorder = 1;
						wsprintf(buf, "%s", "FOOD구입성공");
					}
					else wsprintf(buf, "%s", "FOOD구입실패");
				}
				//PC
				else if (buf[1] == '3') {
					if (lstrcmp(buf, TEXT("231")) == 0) {
						chkorder = 1;
						orderpctime = atoi(tgitemcount);
						pctime += orderpctime;
						wsprintf(tpctime, "%d", pctime);																								//남은시간을 출력하기위해 문자열로
						lstrcpy(tgitemcount, "");																											//주문수량 문자열 초기화
						wsprintf(buf, "%s", "PC구입성공");
					}
					else wsprintf(buf, "%s", "PC구입실패");
				}
			}
			//수정 결과(가능:61/불가:60)
			else if (buf[0] == '6') {
				if (lstrcmp(buf, TEXT("61")) == 0) {
					wsprintf(buf, "%s", "수정성공");
					chkupdate = 1;
				}
				else wsprintf(buf, "%s", "수정실패");
			}
			//탈퇴 결과(가능:71/불가:70)
			else if (buf[0] == '7') {
				if (lstrcmp(buf, TEXT("71")) == 0) {
					wsprintf(buf, "%s", "탈퇴성공");
					gohome = 1;																																		//자리 선택가기
				}
				else wsprintf(buf, "%s", "탈퇴실패");
			}
			//서버로부터 받은 메세지를 메세지박스로 띄우기
			if (lstrlen(buf) != 0) {
				MessageBox(hWndMain, buf, "수신한 메시지", MB_OK);
			}
		}
	}
	return 0;
}