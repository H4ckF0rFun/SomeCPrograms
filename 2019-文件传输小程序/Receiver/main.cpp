#include "r_s.h"
struct SENDER
{
	bool working;
	SOCKET s;
	SOCKADDR_IN addr;

}sender[MAXCONNECT] = { 0 };

int N, connectn = 0;
void updatetitle();
void RecvFile(void*n);

int main()
{
	WORD version = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (0 != WSAStartup(version, &wsadata))
	{
		printf("WSAStartup Error!\n");
		return 0;
	}
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = FILE_TRANSFER_PORT;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	SOCKET s = socket(AF_INET,SOCK_STREAM,0);
	if (0 != bind(s, (struct sockaddr*)&addr, sizeof(addr)))
	{
		printf("Bind Error!\n");
		return 0;
	}
	if (0 != listen(s, MAXCONNECT))
	{
		printf("Listen Error!\n");
		return 0;
	}
	updatetitle();
	while (1)
	{
		//寻找未工作的sender
		for (N = 0; N < MAXCONNECT; N++)
		{
			if (sender[N].working == false)
				break;
		}
		//若无空闲的sender
		if (N >= MAXCONNECT)
			continue;

		int n = sizeof (sender[N].addr);
		sender[N].s = accept(s,(struct sockaddr*)&sender[N].addr,&n);
		if (sender[N].s < 0)
		{
			printf("Accept Error!\n");
			continue;
		}

		
		sender[N].working = true;
		connectn++;
		updatetitle();
		DWORD threadid;
		//working状态，地址不变，把地址传给recvfile
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RecvFile, (LPVOID)&sender[N], NULL, &threadid);
	}
	return 0;
}

void updatetitle()
{
	char title[64];
	sprintf(title, "title FileTransferTool-Receiver____Connection Num:  %d", connectn);
	system(title);
}

void RecvFile(void*n)
{
	struct SENDER * Sender=(struct SENDER*)n;
	char buff[BUFFSIZE];				//缓存
	char FileName[FILE_NAME_LEN];		//接受的文件名
	char From[IPLEN];					//发送者IP
	char SaveFilePath[FILE_PATH_LEN];	//接受的文件保存到

	unsigned long size = 0;				//接收数据大小
	int n_len;							//一次接受大小

	//将发送者IP保存到From中
	unsigned char *p = (unsigned char*)&Sender->addr.sin_addr.S_un.S_addr;
	sprintf(From, "%d.%d.%d.%d", (int)(*p), (int)*(p + 1), (int)*(p + 2), (int)*(p + 3));
	
	//接受文件名
	recv(Sender->s, FileName, BUFFSIZE, 0);
	//设置保存路径
	CreateDirectory("RecvFiles",0);
	sprintf(SaveFilePath, "RecvFiles\\%s", From);
	CreateDirectory(SaveFilePath,0);
	sprintf(SaveFilePath, "RecvFiles\\%s\\%s", From, FileName);
	char MSG[BUFFSIZE];
	sprintf(MSG, "\nReceive a file from %s\nFileName:%s\nReceiving...\n\n", From, FileName);
	printf(MSG);
	//写入文件
	FILE*fp = fopen(SaveFilePath,"wb");
	if (fp == NULL)
	{
		printf("ERROR-Reception failure!\n");
	}
	else
	{
		
		while ((n_len = recv(Sender->s, buff, BUFFSIZE, 0))>0)
		{
			size += n_len;
			fwrite(buff, n_len, 1, fp);
			memset(buff, 0, BUFFSIZE);
		}
		if (0 != fclose(fp))
		{
			printf("ERROR-Reception failure!\n");
		}
		//接受成功
		printf("\n");
		printf("%s has been received(%d B).\n", FileName, size);
	}
	closesocket(Sender->s);
	memset(Sender, 0, sizeof(SENDER));
	connectn--;
	updatetitle();
	return;
}