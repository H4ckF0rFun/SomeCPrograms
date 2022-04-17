#include "r_s.h"

struct SFILE
{
	char FilePath[FILE_PATH_LEN];
	char FileName[FILE_NAME_LEN];
	struct SFILE*next;
};
struct SFILE* head,*p_last;

char RecvIP[IPLEN];
char FileName[FILE_NAME_LEN];
char FilePath[FILE_PATH_LEN];
int total, finish, error;

void updatetitle();
void InputClear();
void AddToSendList(char *fname,char *fpath);
void GetFileName(char *fname, char *fpath);
void SendFiles();
bool SendFile(char*fname,char*fpath);


int main()
{
	
	//加载套接字库
	WORD version = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (0 != WSAStartup(version, &wsadata))
	{
		printf("WSAStartupError!\n");
		return 0;
	}

	head = (struct SFILE*)malloc(sizeof(struct SFILE));
	head->next = NULL;

	while (1)
	{
		p_last = head;
		finish = 0;
		total = 0;
		error = 0;

		system("title FileTransferTool-Sender");
		//输入IP
		printf("Receiver IP Address:");
		scanf("%s",RecvIP);
		InputClear();
		//输入文件路径
		while (1)
		{
			printf("\n");
			printf("#File %d Path(Input \"END\" to end):\n",total+1);
			scanf("%s",FilePath);
			InputClear();
			if (!strcmp(FilePath, "END"))
			{
				break;
			}
			FILE*fp = fopen(FilePath,"rb");
			
			if (0 == fp)
			{
				printf("-Error! Invalid File Path.\n");
				continue;
			}
			printf("-OK\n");
			fclose(fp);
			GetFileName(FileName,FilePath);
			AddToSendList(FileName,FilePath);
			total++;
		}
		printf("\n");
		//printf("%d\n",total);
		//struct SFILE*p=head;
		//while (p->next != NULL)
		//{
		//	p = p->next;
		//	printf("FileName:%s  FilePath:%s\n", p->FileName, p->FilePath);
		//}
		SendFiles();
		printf("\nSend: %d\t\tFault:%d\n",total,error);
		system("pause");
	}
	return 0;
}
void InputClear()
{
	char ch;
	while ((ch = getchar()) != '\n');
}
void AddToSendList(char *fname, char *fpath)
{
	p_last->next = (struct SFILE*)malloc(sizeof(struct SFILE));
	strcpy(p_last->next->FileName,fname);
	strcpy(p_last->next->FilePath, fpath);
	p_last = p_last->next;
	p_last->next = NULL;
}
void GetFileName(char *fname, char *fpath)
{
	int i;
	for (i = strlen(fpath); i > 0; i--)
	{
		if (fpath[i - 1] == '\\' || fpath[i - 1 ]== '/')
		{
			strcpy(fname,&fpath[i]);
			return;
		}
	}
	strcpy(fname,fpath);
	return;
}
void updatetitle()
{
	char str[50];
	sprintf(str,"title Total:%d_____Finish:%d",total,finish);
	system(str);
}
void SendFiles()
{
	struct SFILE*p;
	updatetitle();
	while (head->next != NULL)
	{
		p = head->next;
		if (false == SendFile(p->FileName, p->FilePath))
		{
			error++;
		}
		finish++;
		head->next = p->next;
		free(p);
		updatetitle();
	}
}
bool SendFile(char*fname, char*fpath)
{
	printf("\n");
	printf("Sending \"%s\" to %s\n",fname,RecvIP);
	struct sockaddr_in receiver = { 0 };
	receiver.sin_family = AF_INET;
	receiver.sin_port = FILE_TRANSFER_PORT;
	receiver.sin_addr.S_un.S_addr = inet_addr(RecvIP);

	SOCKET s = socket(AF_INET,SOCK_STREAM,0);
	if (s < 0)
	{
		printf("-Error.Socket error.\n");
		return false;
	}
	if (connect(s, (struct sockaddr*)&receiver, sizeof(receiver))!=0)
	{
		printf("-Error.Couldn't connect to %s.\n",RecvIP);
		closesocket(s);
		return false;
	}
	
	
	if (BUFFSIZE != send(s, fname, BUFFSIZE, 0))
	{
		printf("-Couldn't Send %s.\n", fname);
		closesocket(s);
		return false;
	}
	FILE*fp = fopen(fpath,"rb");
	if (0 == fp)
	{
		printf("-Couldn't open %s.\n",fname);
		closesocket(s);
		return false;
	}
	unsigned long size = 0;
	//
	char *buff = (char*)malloc(BUFFSIZE);
	int n_len;
	while ((n_len = fread(buff, 1, BUFFSIZE, fp))>0)
	{
		size += n_len;
		
		if (n_len != send(s, buff, n_len, 0))
		{
			printf("-Couldn't Send %s.\n", fname);
			closesocket(s);
			return false;
		}
		memset(buff,0,BUFFSIZE);
	}

	//
	if (0 != fclose(fp))
	{
		printf("-Couldn't close %s.\n",fname);
		free(buff);
		closesocket(s);
		return false;
	}
	free(buff);
	closesocket(s);
	printf("%s has been sent(%d B).\n",fname,size);
	return true;
}