// control_layer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MySocket.h"
#include "MyThreadPool.h"
#include "IMyCommand.h"


SOCKET g_Sck = INVALID_SOCKET;
SOCKET g_sClient = INVALID_SOCKET; 
SOCKET g_sSqlServer = INVALID_SOCKET;
sockaddr_in g_addr = { 0 };


long StartServer()
{
  // 1. ����һ���׽��� socket
  g_Sck = socket(AF_INET , // ����Э���
                 SOCK_STREAM , // "��ʽ" -- TCP, SOCK_DGRAM "���ݱ�" -- UDP
                 IPPROTO_TCP);
  if(g_Sck == INVALID_SOCKET)
  {
    return S_FALSE;
  }

  // 2. ��IP �� �˿� bind
  // sockaddr_in addr; // һ��14�ֽڴ�Ľṹ��
  // �����Ը��ߵĽṹ��
  g_addr.sin_family = AF_INET;
  g_addr.sin_addr.S_un.S_addr = 0; // IP��ַ
  g_addr.sin_port = htons(CONTROL_PORT); // htons() �������ֽ���תΪ������s����sort
                               // ������Ϊ�����


  int nRet = bind(g_Sck , (sockaddr*)&g_addr , sizeof(sockaddr));
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
  g_sClient = accept(g_Sck , (sockaddr*)&g_addr , &nLength);
  if(g_sClient == INVALID_SOCKET)
  {
    printf("���Ӵ���\n");
    return S_FALSE;
  }

  printf("���ӳɹ�\n");
  return S_OK;
}


long ConnSQLServer()
{
  // 1. ����һ���׽��� socket
  g_sSqlServer = socket(AF_INET , // ����Э���
             SOCK_STREAM , // "��ʽ" -- TCP, SOCK_DGRAM "���ݱ�" -- UDP
             IPPROTO_TCP);
  if(g_sSqlServer == INVALID_SOCKET)
  {
    printf("����SOCKETʧ��\n");
    return S_FALSE;
  }

  // 2. ���÷���� IP �� �˿� // �ͻ��˲���Ҫ��
  sockaddr_in addr; // һ��14�ֽڴ�Ľṹ��
  addr.sin_family = AF_INET;
  addr.sin_addr.S_un.S_addr = inet_addr(SQLSERVER_IP); // IP��ַ
  addr.sin_port = htons(SQLSERVER_PORT); // htons() �������ֽ���תΪ������s����sort

  // 3. ���ӷ���� connect
  printf("����SQL��\n");
  while(1)
  {
    int nRet = connect(g_sSqlServer , (sockaddr*)&addr , sizeof(sockaddr));
    if(nRet == SOCKET_ERROR)
    {
      continue;
    }

    printf("���ӳɹ�\n");
    break;
  }

  return S_OK;
}


int _tmain(int argc, _TCHAR* argv[])
{
  if(!MyWSAStartup())
  {
    printf("SOCKET���󣬰�������˳�\n");
    getchar();
    return S_FALSE;
  }

  if(ConnSQLServer())
  {
    printf("����ʧ�ܣ���������˳�\n");
    getchar();
    return S_FALSE;
  }

  if(StartServer())
  {
    printf("SOCKET���󣬰�������˳�\n");
    getchar();
    return S_FALSE;
  }
  


  //�����̳߳�
  CThreadPool pool;
  BOOL bRet = pool.Create(5);

  if (!bRet)
  {
    return 0;
  }


  // pool.Handle(xxx);

  Sleep(100);

  pool.Destroy();


  getchar();

	return 0;
}