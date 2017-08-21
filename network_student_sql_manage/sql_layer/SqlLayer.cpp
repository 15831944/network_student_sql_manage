// sql_layer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MySocket.h"
#include <direct.h>
#include <comutil.h>
#include <comdef.h>

#import "C:/Program Files (x86)/Common Files/System/ado/msado15.dll" no_namespace rename("EOF", "MyEOF")


SOCKET g_Sck = INVALID_SOCKET;
SOCKET g_sClient = INVALID_SOCKET; 
_ConnectionPtr g_conn;


long StartServer()
{
  if(!MyWSAStartup())
  {
    return S_FALSE;
  }

  // 1. ����һ���׽��� socket
  g_Sck = socket(AF_INET , // ����Э���
                 SOCK_STREAM , // "��ʽ" -- TCP, SOCK_DGRAM "���ݱ�" -- UDP
                 IPPROTO_TCP);
  if(g_Sck == INVALID_SOCKET)
  {
    return S_FALSE;
  }

  // 2. ��IP �� �˿� bind
  sockaddr_in addr; // һ��14�ֽڴ�Ľṹ��
   // �����Ը��ߵĽṹ��
  addr.sin_family = AF_INET;
  addr.sin_addr.S_un.S_addr = 0; // IP��ַ
  addr.sin_port = htons(SQLSERVER_PORT); // htons() �������ֽ���תΪ������s����sort
                               // ������Ϊ�����


  int nRet = bind(g_Sck , (sockaddr*)&addr , sizeof(sockaddr));
  if(nRet == SOCKET_ERROR)
  {
    printf("�󶨴���\n");
    return S_FALSE;
  }

  // 3. ���� listen
  nRet = listen(g_Sck , SOMAXCONN);
  if(nRet == SOCKET_ERROR)
  {
    printf("��������\n");
    return S_FALSE;
  }

  printf("�����ɹ����ȴ�����\n");

  // 4. �ȴ�����
  int nLength = sizeof(sockaddr);
  g_sClient = accept(g_Sck , (sockaddr*)&addr , &nLength);
  if(g_sClient == INVALID_SOCKET)
  {
    printf("���Ӵ���\n");
    return S_FALSE;
  }

  printf("���ӳɹ�\n");
  return S_OK;
}


long ConnDataBase()
{
	//"adodb.Connection"
	//_uuidof(Connection)
	HRESULT hr = g_conn.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)){
		return S_FALSE;
	}
	// OutputDebugString(TEXT("���ݿ����ʵ����ʼ���ɹ�\r\n"));

  char szPath[ MAXBYTE ] = "File Name=";
  char *pszPath = NULL;

	try{
    pszPath = _getcwd(NULL , 0);
    if(pszPath == NULL)
    {
      return FALSE;
    }
    strcat_s(szPath , MAXBYTE , pszPath);
    strcat_s(szPath , MAXBYTE , "\\connect.udl");
    // OutputDebugStringA(szPath);
		hr = g_conn->Open( szPath , "", "", adConnectUnspecified);
		if (FAILED(hr))
		{
			printf("conn err");
      return S_FALSE;
		}
	}
	catch (_com_error e){
		//
		_bstr_t bstr = e.Description();
		wprintf(L"%s\n", bstr);
    return S_FALSE;
	}

  printf("���ݿ����ӳɹ�\n");
	return S_OK;
}


int _tmain(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);

  if(ConnDataBase())
  {
    printf("�������ݿ���󣬰�������˳�\n");
    getchar();
    return S_FALSE;
  }

  if(StartServer())
  {
    printf("SOCKET���󣬰�������˳�\n");
    getchar();
    return S_FALSE;
  }

  getchar();

  CoUninitialize();
	return 0;
}

