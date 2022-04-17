#include <Windows.h>
#include <WinSock.h>
#include <iostream>
#include <string>

#pragma comment(lib,"ws2_32.lib")
using namespace std;

BOOL Init()
{
	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 0), &wsadata))
		return FALSE;
	return TRUE;
}

void Clear()
{
	WSACleanup();
}


#define BUFFSIZE 4097

#define ERROR_ANALYSEURL_SUCCESE 0
#define ERROR_NOTHTTPPROTOCOL 	-1
#define ERROR_INVALIDURL		-2

#define ERROR_DOWNLOAD_SUCCESS 	 0
#define ERROR_INVALID_RESPONSE 	-1
#define ERROR_REQUEST_ERROR		-2
#define ERROR_CREATEFILE_ERROR	-3
#define ERROR_WRITEFILE_ERROR 	-4
#define ERROR_CLOSEFILE_ERROR 	-5
#define ERROR_ANALYSEURL_ERROR  -6
#define ERROR_CONNECTSERVER_ERROR -7
#define ERROR_FILEEXIST_ERROR	-8
//解析URL
int AnalyseUrl(const char *in_Url, string&out_Host, int&out_Port, string&out_Resource, string&out_FileName)
{
	string url = in_Url;
	//检测协议
	if (memcmp(url.c_str(), "http://", strlen("http://")))
		return ERROR_NOTHTTPPROTOCOL;

	if (!(*(url.c_str() + 7))) return ERROR_INVALIDURL;

	//查找host resource分割符
	char *p = (char*)url.c_str() + 7;
	if (0 == (p = (char*)strstr(p, "/")))
	{
		url += "/";
		p = (char*)strstr(url.c_str(), "/");
	}

	out_Resource = p;
	//url中是否包含参数
	const char *temp;
	if((temp = strstr(p,"?")))
	{
		*(char*)temp = 0;
	}
	const char *pFileName = out_Resource.c_str() + out_Resource.length() - 1;

	if (*pFileName == '/')							//最后一个字符是/
		out_FileName = "index.html";
	else											//最后一个字符不是/
	{
		while (pFileName >= out_Resource.c_str())
		{
			if (*pFileName == '/')
			{
				out_FileName = pFileName + 1;
				break;
			}
			pFileName--;
		}
	}
	p[0] = '\0';

	//端口号
	out_Port = 80;
	char *pPort = (char*)strstr(url.c_str()+7, ":");
	if (pPort)
	{
		sscanf(pPort + 1, "%d", &out_Port);
		*pPort = 0;
	}

	out_Host = url.c_str() + 7;
	return ERROR_ANALYSEURL_SUCCESE;
}

//解析Response
BOOL AnalyseResponse(const char *in_Response, unsigned long& out_FileSize, bool & out_bFileSizeDefinite)
{
	if (!strstr(in_Response, "200 OK\r\n"))
		return FALSE;

	const char *pContentLength = 0;
	out_FileSize = 0;
	if ((pContentLength = strstr(in_Response, "Content-Length")) && (pContentLength = strstr(pContentLength, ":")))
	{
		pContentLength += 1;
		sscanf(pContentLength, "%d", &out_FileSize);
		out_bFileSizeDefinite = true;
	}
	else
		out_bFileSizeDefinite = false;
	return TRUE;
}


int HTTPDownload(char * url, char *savepath, char *filename,bool bCoverFile)
{
	//接受缓存buff
	char buff[BUFFSIZE] = { 0 };
	//解析url
	string Host, Resource, FileName;
	int Port;
	if (ERROR_ANALYSEURL_SUCCESE != AnalyseUrl(url, Host, Port, Resource, FileName))
	{
		cout << "Not http protocol or invalid url!" << endl;
		return ERROR_ANALYSEURL_ERROR;
	}

	//传输socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	//服务器信息
	HOSTENT * pHost = gethostbyname(Host.c_str());
	SOCKADDR_IN server = { 0 };

	memcpy(&server.sin_addr, pHost->h_addr_list[0], 4);
	server.sin_family = AF_INET;
	server.sin_port = htons(Port);
	//连接服务器
	if (0 != connect(sock, (SOCKADDR*)&server, sizeof(server)))
		return ERROR_CONNECTSERVER_ERROR;

	//发送Request

	string Request;
	Request += "GET ";
	Request += Resource;
	Request += " HTTP/1.1\r\n";
	Request += "Host: ";
	Request += Host;
	Request += "\r\n";
	Request += "Connection: keep-alive\r\n\r\n";
	send(sock, Request.c_str(), Request.length(), 0);


	bool bStart = false, bContinue = true;
	bool bFileSizeDefinite = false;
	unsigned long FileSize = 0, LeftSize = 0, totalRecv = 0;
	int DataLen;
	char *pDataBegin = 0;
	int Error = ERROR_DOWNLOAD_SUCCESS;


	FILE*fp = 0;
	string tmpResponse;
	while (bContinue && ((!bFileSizeDefinite) || (LeftSize != 0)) && (DataLen = recv(sock, buff, BUFFSIZE-1, 0)) > 0 )
	{
		totalRecv += DataLen;
		//Response还没有接收完
		if (!bStart)
		{
			buff[BUFFSIZE-1] = 0;
			tmpResponse += buff;
			//错误的Response
			if (tmpResponse.length() >= strlen("HTTP") && memcmp("HTTP", tmpResponse.c_str(), strlen("HTTP")))
			{
				bContinue = false;
				Error = ERROR_INVALID_RESPONSE;
				continue;
			}
			else if (tmpResponse.length() >= strlen("HTTP") && strstr(tmpResponse.c_str(), "\r\n\r\n"))
			{
				//Response接受完成
				bStart = true;
				char *Response = new char[strstr(tmpResponse.c_str(), "\r\n\r\n") - tmpResponse.c_str() + 5];
				Response[strstr(tmpResponse.c_str(), "\r\n\r\n") - tmpResponse.c_str() + 4] = 0;
				memcpy(Response, tmpResponse.c_str(), strstr(tmpResponse.c_str(), "\r\n\r\n") - tmpResponse.c_str() + 4);
				//RequestError,非200OK
				if (FALSE == AnalyseResponse(Response, FileSize, bFileSizeDefinite))
				{
					bContinue = false;
					Error = ERROR_REQUEST_ERROR;
					continue;
				}

				

				//创建文件
				string SaveFileName;
				if (savepath)									//填写了保存路径
				{
					SaveFileName = savepath;
					SaveFileName += "\\";
				}
				if (filename)									//填写了文件名
					FileName = filename;

				SaveFileName += FileName;

				if((fp = fopen(SaveFileName.c_str(), "rb")))
				{
					fclose(fp);
					//文件已经存在
					if(!bCoverFile)
					{
						return ERROR_FILEEXIST_ERROR;
					}
				}
				
				fp = fopen(SaveFileName.c_str(), "wb");
				if (!fp)	return ERROR_CREATEFILE_ERROR;							//文件创建失败
				/***********************************************************/
				cout << "FileName:" << FileName << endl;

				char* unit[4] = { "Byte", "KB", "MB", "GB" };
				unsigned size = FileSize;
				double d_size = size;
				int i;
				for (i = 0; (size / 1024) && i < 4; i++)
				{
					size /= 1024;
					d_size /= 1024;
				}
				if (bFileSizeDefinite)
					cout << "FileSize:" << d_size << unit[i] << endl;
				else
					cout << "FileSize: Unknown"<< endl;

				cout << "SaveAs:" << SaveFileName << endl;
				/***********************************************************/
				LeftSize = FileSize;
				pDataBegin = (char*)buff + strlen(Response) % (BUFFSIZE-1);
				DataLen = totalRecv - strlen(Response);
				bStart = true;

			}
		}
		else
		{
			pDataBegin = buff;
		}

		if (bStart)
		{
			int nWrite = 0;
			//文件大小确定
			
				if (bFileSizeDefinite)
					nWrite = DataLen > LeftSize ? LeftSize : DataLen;
				else
					nWrite = DataLen;
				if (nWrite != fwrite(pDataBegin, 1, nWrite, fp))
				{
					bContinue = false;
					Error = ERROR_WRITEFILE_ERROR;
					continue;
				}
				//-----------------------------------------------------
				if (bFileSizeDefinite)
					LeftSize -= nWrite;				//计算剩余的大小
		}

	}
	if (fp && fclose(fp))
		return ERROR_CLOSEFILE_ERROR;
	return ERROR_DOWNLOAD_SUCCESS;
}

int main(int argc, char* argv[])
{

	if (argc == 1)
	{
		cout << argv[0] << ": Not input url!" << endl;
		return -1;
	}
	char * url = 0, *savepath = 0, *filename = 0;
	bool bCoverFile = false;
	if (argc >1)
		url = argv[1];
	if (argc >2)
		savepath = argv[2];
	if (argc >3)
		filename = argv[3];
	if (argc >4)
		bCoverFile = true;
		
	if (FALSE == Init())
		return FALSE;
	
	//url = "http://192.168.0.101/vc6_cn_full.exe";
	switch(HTTPDownload(url, savepath, filename,bCoverFile))
	{
	case ERROR_DOWNLOAD_SUCCESS:
		cout << argv[0] << ": Download file successfully!" << endl;
		break;
	case ERROR_INVALID_RESPONSE:
		cout << argv[0] << ": Download file error!(ERROR_INVALID_RESPONSE)" << endl;
		break;
	case ERROR_REQUEST_ERROR:
		cout << argv[0] << ": Download file error!(ERROR_REQUEST_ERROR)" << endl;
		break;
	case ERROR_CREATEFILE_ERROR:
		cout << argv[0] << ": Download file error!(ERROR_CREATEFILE_ERROR)" << endl;
		break;
	case ERROR_WRITEFILE_ERROR:
		cout << argv[0] << ": Download file error!(ERROR_WRITEFILE_ERROR)" << endl;
		break;
	case ERROR_CLOSEFILE_ERROR:
		cout << argv[0] << ": Download file error!(ERROR_CLOSEFILE_ERROR)" << endl;
		break;
	case ERROR_ANALYSEURL_ERROR:
		cout << argv[0] << ": Download file error!(ERROR_ANALYSEURL_ERROR)" << endl;
		break;
	case ERROR_CONNECTSERVER_ERROR:
		cout << argv[0] << ": Download file error!(ERROR_CONNECTSERVER_ERROR)" << endl;
		break;
	case ERROR_FILEEXIST_ERROR:
		cout << argv[0] << ": Download file error!(ERROR_FILEEXIST_ERROR)" << endl;
		break;
	}
	Clear();
	return 0;
}