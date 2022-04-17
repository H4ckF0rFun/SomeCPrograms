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
		//Ѱ��δ������sender
		for (N = 0; N < MAXCONNECT; N++)
		{
			if (sender[N].working == false)
				break;
		}
		//���޿��е�sender
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
		//working״̬����ַ���䣬�ѵ�ַ����recvfile
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
	char buff[BUFFSIZE];				//����
	char FileName[FILE_NAME_LEN];		//���ܵ��ļ���
	char From[IPLEN];					//������IP
	char SaveFilePath[FILE_PATH_LEN];	//���ܵ��ļ����浽

	unsigned long size = 0;				//�������ݴ�С
	int n_len;							//һ�ν��ܴ�С

	//��������IP���浽From��
	unsigned char *p = (unsigned char*)&Sender->addr.sin_addr.S_un.S_addr;
	sprintf(From, "%d.%d.%d.%d", (int)(*p), (int)*(p + 1), (int)*(p + 2), (int)*(p + 3));
	
	//�����ļ���
	recv(Sender->s, FileName, BUFFSIZE, 0);
	//���ñ���·��
	CreateDirectory("RecvFiles",0);
	sprintf(SaveFilePath, "RecvFiles\\%s", From);
	CreateDirectory(SaveFilePath,0);
	sprintf(SaveFilePath, "RecvFiles\\%s\\%s", From, FileName);
	char MSG[BUFFSIZE];
	sprintf(MSG, "\nReceive a file from %s\nFileName:%s\nReceiving...\n\n", From, FileName);
	printf(MSG);
	//д���ļ�
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
		//���ܳɹ�
		printf("\n");
		printf("%s has been received(%d B).\n", FileName, size);
	}
	closesocket(Sender->s);
	memset(Sender, 0, sizeof(SENDER));
	connectn--;
	updatetitle();
	return;
}