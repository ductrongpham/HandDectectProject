//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//#include <winsock2.h>
//#include <process.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <iostream>
//#include <math.h>
//#define _WIN32_WINNT 0x0501 
//#include <winuser.h>
//#pragma comment(lib,"ws2_32.lib" )
//unsigned int __stdcall ServClient(void *data);
//using namespace std;
//
//void MouseMove(int dx, int dy)
//{
//	INPUT  Input = { 0 };
//	Input.type = INPUT_MOUSE;
//	Input.mi.dwFlags = MOUSEEVENTF_MOVE; //MOUSEEVENTF_ABSOLUTE;
//	Input.mi.dx = dx;
//	Input.mi.dy = dy;
//	::SendInput(1, &Input, sizeof(INPUT));
//}
//
//void MouseLeftBtnDown()
//{
//	INPUT    Input = { 0 };
//	Input.type = INPUT_MOUSE;
//	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
//	::SendInput(1, &Input, sizeof(INPUT));
//}
//
//void MouseLeftBtnUp()
//{
//	INPUT    Input = { 0 };
//	//::ZeroMemory(&Input,sizeof(INPUT));
//	Input.type = INPUT_MOUSE;
//	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
//	::SendInput(1, &Input, sizeof(INPUT));
//}
//
//void MouseRightBtnDown()
//{
//	INPUT    Input = { 0 };
//	Input.type = INPUT_MOUSE;
//	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
//	::SendInput(1, &Input, sizeof(INPUT));
//}
//
//void MouseRightBtnUp()
//{
//	INPUT Input = { 0 };
//	//::ZeroMemory(&Input,sizeof(INPUT));
//	Input.type = INPUT_MOUSE;
//	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
//	::SendInput(1, &Input, sizeof(INPUT));
//}
//
//struct PointCenter{
//	int x;
//	int y;
//	int fingerNum;
//};
//struct PointCenter pointC;
//int main(int argc, char* argv[])
//{
//	WSADATA wsaData;
//	int iResult;
//	sockaddr_in addr;
//	SOCKET sock, client;
//	addr.sin_family = AF_INET;
//	addr.sin_port = htons(2222);
//	addr.sin_addr.S_un.S_addr = inet_addr("192.168.1.159");
//	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);//2.2
//	if (iResult)
//	{
//		printf("WSA startup failed");
//		return 0;
//	}
//	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (sock == INVALID_SOCKET)
//	{
//		printf("Invalid socket");
//		return 0;
//	}
//	iResult = bind(sock, (sockaddr*)&addr, sizeof(sockaddr_in));
//	if (iResult)
//	{
//		printf("bind failed %u", GetLastError());
//		return 0;
//	}
//	cout << "wait for client connection " << endl;
//	iResult = listen(sock, SOMAXCONN);
//	if (iResult)
//	{
//		printf("iResult failed %u", GetLastError());
//		return 0;
//	}
//	while (client = accept(sock, 0, 0))
//	{
//		if (client == INVALID_SOCKET)
//		{
//			printf("invalid client socket", GetLastError());
//			continue;
//		}
//		_beginthreadex(0, 0, ServClient, (void*)&client, 0, 0);
//	}
//	return 0;
//}
//
//
//unsigned int __stdcall ServClient(void *data)
//{
//	SOCKET* client = (SOCKET*)data;
//	SOCKET Client = *client;
//	printf("Client connected\n");
//	int n = 0;
//
//	//các cờ và tọa độ điều khiển chuột
//	int xcu, ycu, xx, yy;
//	xcu = ycu = xx = yy = 0;
//	int flagM = 1;
//	int flagC = 1;
//	char str[256];
//	while (1){
//		//n = recv(Client, (char*)&pointC, sizeof(pointC), NULL);
//		n = recv(Client, (char*) str, sizeof(pointC), NULL);
//		cout << "str: " << str << endl;
//		////điều khiển chuột 
//		////dieu khien con chuot
//		//xx = pointC.x;
//		//yy = pointC.y;
//
//		//if (xcu != xx && ycu != yy && pointC.fingerNum == 1){
//		//	if (flagM == 0){
//		//		flagM = 1;
//		//	}
//		//	else{
//		//		int dx = xx - xcu;
//		//		int dy = (yy - ycu);
//		//		if (!(abs(dx) <= 2 && abs(dy) <= 2))
//		//		{
//		//			MouseMove(4 * dx, 4 * dy);
//		//		}
//		//			
//		//	}
//		//	flagC = 0;
//		//}
//		//else if (pointC.fingerNum >= 4 && flagC == 0){
//		//	flagC = 1;
//		//	MouseLeftBtnDown();
//		//	cout << "click left mouse " << endl;
//		//	Sleep(100);
//		//	MouseLeftBtnUp();
//		//}
//		//else if (pointC.fingerNum == 3 && flagC == 0){
//		//	flagC = 1;
//		//	MouseRightBtnDown();
//		//	cout << "click right mouse " << endl;
//		//	Sleep(100);
//		//	MouseRightBtnUp();
//		//}
//		//else{
//		//	flagM = 0;
//		//}
//		//
//		////else{
//		////	flagM = 0;
//		////}
//
//		////if (pointC.fingerNum >= 4 && flagC == 0){
//		////	flagC = 1;
//		////	MouseLeftBtnDown();
//		////	cout << "click left mouse " << endl;
//		////	Sleep(100);
//		////	MouseLeftBtnUp();
//		////}
//		////else if (pointC.fingerNum == 3 && flagC == 0){
//		////	flagC = 1;
//		////	MouseRightBtnDown();
//		////	cout << "click right mouse " << endl;
//		////	Sleep(100);
//		////	MouseRightBtnUp();
//		////}
//
//		//xcu = xx;
//		//ycu = yy;
//
//		//memset(&pointC, 0, sizeof(pointC));
//		if (n < 0) break;
//	}
//	cout << "ket thuc phien" << endl;
//	return 0;
//}



#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include<stdio.h>
#include<winsock2.h>
#include<iostream>
#pragma comment(lib,"ws2_32.lib")
#define PORT 2222
#define _WIN32_WINNT 0x0501 
#include <winuser.h>
using namespace std;

void MouseMove(int dx, int dy)
{
	INPUT  Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE; //MOUSEEVENTF_ABSOLUTE;
	Input.mi.dx = dx;
	Input.mi.dy = dy;
	::SendInput(1, &Input, sizeof(INPUT));
}

void MouseLeftBtnDown()
{
	INPUT    Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));
}

void MouseLeftBtnUp()
{
	INPUT    Input = { 0 };
	//::ZeroMemory(&Input,sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void MouseRightBtnDown()
{
	INPUT    Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));
}

void MouseRightBtnUp()
{
	INPUT Input = { 0 };
	//::ZeroMemory(&Input,sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}


struct PointCenter
{
	int x; //Tọa độ x
	int y; //Tọa độ y
	int fingerNum; //Số ngón tay
};
int main()
{
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	WSADATA wsa;
	slen = sizeof(si_other);
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("192.168.1.159");
	server.sin_port = htons(PORT);
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	struct PointCenter pointC;
	//các cờ và tọa độ điều khiển chuột
	int xcu, ycu, xx, yy;
	xcu = ycu = xx = yy = 0;
	int flagM = 1;
	int flagC = 1;

	while (1)
	{
		fflush(stdout);
		if ((recv_len = recvfrom(s, (char*)&pointC, sizeof(pointC), 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		//điều khiển chuột 
		//dieu khien con chuot
		xx = pointC.x;
		yy = pointC.y;

		if (xcu != xx && ycu != yy && pointC.fingerNum == 1){
			if (flagM == 0){
				flagM = 1;
			}
			else{
				int dx = xx - xcu;
				int dy = (yy - ycu);
				if (!(abs(dx) <= 2 && abs(dy) <= 2))
				{
					MouseMove(4 * dx, 4 * dy);
				}

			}
			flagC = 0;
		}
		else if (pointC.fingerNum >= 4 && flagC == 0){
			flagC = 1;
			MouseLeftBtnDown();
			cout << "click left mouse " << endl;
			Sleep(100);
			MouseLeftBtnUp();
		}
		else if (pointC.fingerNum == 3 && flagC == 0){
			flagC = 1;
			MouseRightBtnDown();
			cout << "click right mouse " << endl;
			Sleep(100);
			MouseRightBtnUp();
		}
		else{
			flagM = 0;
		}
		xcu = xx;
		ycu = yy;
		memset(&pointC, 0, sizeof(pointC));
	}
	closesocket(s);
	WSACleanup();
	return 0;
}

