#include <stdio.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <time.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

struct CLIENT
{
	SOCKET s;
	SOCKADDR_IN addr;
	BOOL IsWorking;
	time_t time;
};

void RecvMsg(CLIENT*client);

char WorkDirectory[] = ".\\work";
char IPAddress[] = "127.0.0.1";
int Port = 2020;



CLIENT Client[1024];
int main()
{
	::memset(&Client, 0, sizeof(Client));
	///////
	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 0), &wsadata))
	{
		return -1;
	}
	printf("WSAStartup Successful!\n");
	//////
	SOCKADDR_IN Server = { 0 };
	Server.sin_addr.S_un.S_addr = inet_addr(IPAddress);
	//Server.sin_addr.S_un.S_addr = INADDR_ANY;
	Server.sin_port = htons(Port);
	Server.sin_family = AF_INET;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	if (0 != bind(sock, (SOCKADDR*)&Server, sizeof(Server)))
	{
		return -1;
	}
	printf("Bind Successful!\n");
	if (0 != listen(sock, 1024))
	{
		return -1;
	}
	printf("Listen Successful!\n");
	while (1)
	{
		int i;
		for (i = 0; i < 1024; i++)
		{
			if (Client[i].IsWorking == FALSE)
				break;
		}
		if (i >= 1024)
			continue;

		int addrlen = sizeof(SOCKADDR);
		Client[i].s = accept(sock, (SOCKADDR*)&Client[i].addr, &addrlen);
		Client[i].IsWorking = TRUE;
		Client[i].time = time(NULL);
		DWORD id;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RecvMsg, (LPVOID)&Client[i], NULL, &id);
	}


	return 0;
}


void RecvMsg(CLIENT*client)
{
	char ClientIP[128]; 
	char _szRequest[4097] = { 0 };
	//解析连接者IP地址
	strcpy(ClientIP, inet_ntoa(client->addr.sin_addr));
	
	string str = "Accept a connection from:";
	str += ClientIP;
	str += "\n";
	printf(str.c_str());
	
	int n;
	if ((n=recv(client->s, _szRequest, 4096, 0)) > 0)
	{
		char savepath[4096];
		char filepath[4096];
		char fname[1024] = { 0 };
		memcpy(fname, ctime(&client->time), strlen(ctime(&client->time)) - 1);
		char *p = fname;
		while (*p)
		{
			if (*p == ':')
				*p = '.';
			p++;
		}
		sprintf(savepath, ".\\Requests\\%s", ClientIP);
		sprintf(filepath, ".\\Requests\\%s\\%s.txt", ClientIP, fname);
		CreateDirectory("Requests", 0);
		CreateDirectory(savepath, 0);
		printf(filepath);
		printf("\n");
		FILE*fp = fopen(filepath, "wb");
		if (fp != NULL)
		{
			printf("receive: %d written: %d\n", n, fwrite(_szRequest, 1, n, fp));
			fclose(fp);
		}
	}
	
	//显示消息
	
	string Response;
	//响应请求
	//如果是get请求
	if (!memcmp(_szRequest, "GET ", 4))
	{
		char *p = (char*)(_szRequest + 4);
		//解析资源请求路径
		while (*p != ' '&&p < (_szRequest + sizeof(_szRequest)))
		{
			p++;
		}
		if (p >=(_szRequest + sizeof(_szRequest)))
		{
			Response = "HTTP/1.1 400 Bad Request\r\n\r\n";
			send(client->s, Response.c_str(), Response.length(), 0);
		}
		else if (0 == memcmp(p + 1, "HTTP", 4))
		{
			char *Resource = new char[p - (_szRequest + 4) + 1];
			memcpy(Resource, _szRequest + 4, p - (_szRequest + 4));
			Resource[p - (_szRequest + 4)] = 0;
			//得到资源路径
			string filepath;
			filepath = WorkDirectory;
			if (0 == strcmp(Resource, "/"))
			{
				filepath += "/index.html";
			}
			else
			{
				filepath += Resource;
			}

			//得到文件在本地路径
			delete Resource;
			FILE*fp = fopen(filepath.c_str(), "rb");
			if (fp == NULL)
			{
				//找不到文件
				Response = "HTTP/1.1 404 Not Found\r\n\r\n";
				send(client->s, Response.c_str(), Response.length(), 0);
			}
			else
			{
				//请求成功
				Response = "HTTP/1.1 200 OK\r\n\r\n";
				send(client->s, Response.c_str(), Response.length(), 0);
				int n;
				char buff[1024];

				//发送资源数据
				while ((n = fread(buff, 1, sizeof(buff), fp)) > 0)
				{
					send(client->s, buff, n, 0);
				}
			}

		}
	}
	//否则
	else
	{
		Response = "HTTP/1.1 400 Bad Request\r\nSet-Cookie: name=aasdf\r\n\r\n";
		send(client->s, Response.c_str(), Response.length(), 0);
	}
	////////
	shutdown(client->s,SD_SEND);
	closesocket(client->s);
	client->IsWorking = FALSE;
	return;
}

