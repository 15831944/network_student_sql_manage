// control_layer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MySocket.h"
#include "MyThreadPool.h"
#include "IMyCommand.h"
#include "afxwin.h"
#include "MyCommand.h"


SOCKET g_Sck = INVALID_SOCKET;
// SOCKET g_sClient = INVALID_SOCKET; 
SOCKET g_sSqlServer = INVALID_SOCKET;
sockaddr_in g_addr = { 0 };
CList<SOCKET> g_ListSck;
CThreadPool pool;


void DisPose(SOCKET sClient, char* pMess)
{
  CMyCommand* cmd = new CMyCommand;
  cmd->m_sClient = sClient;
  cmd->m_pMess = pMess;
  cmd->m_sServer = g_sSqlServer;
  pool.Handle(cmd);
}

UINT AFX_CDECL DisSqlPoseThreadProc(LPVOID lpParameter)
{
  while(1)
  {
    char* pSqlMess = RecivPacket(g_sSqlServer);
    
    if(pSqlMess == NULL)
    {
      return 0;
    }

    DWORD nLen = *(DWORD*)pSqlMess;
    BYTE nType = *(BYTE*)(pSqlMess + sizeof(DWORD));

    printf("�յ�����˳���Ϊ %3u �� %3d ��Ϣ\n", nLen , (int)nType);

    SOCKET s = *(SOCKET*)(pSqlMess + sizeof(DWORD) + sizeof(BYTE));

    SendPacket(s , nType ,
               pSqlMess + sizeof(DWORD) + sizeof(BYTE) + sizeof(SOCKET) ,
               nLen - sizeof(BYTE) - sizeof(SOCKET));

    delete pSqlMess;
  }

  return 0;
}


UINT AFX_CDECL RecvThreadProc(LPVOID lpParameter)
{
  SOCKET sClient = INVALID_SOCKET;
  int nLength = sizeof(sockaddr);
  char szBuff[ MAXBYTE ] = { 0 };
  int nRecv = 0;
  char* pMess = NULL;
  u_long UnBlockMode = 1;
  u_long BlockMode = 0;

  while(1)
  {
    POSITION pos = g_ListSck.GetHeadPosition();

    SOCKET sClient = INVALID_SOCKET;
    while(pos != NULL)
    {
      sClient = g_ListSck.GetNext(pos);
      ioctlsocket(sClient ,FIONBIO,&UnBlockMode);

      nRecv = recv(sClient , szBuff , 1 , MSG_PEEK);

      if(nRecv > 0)
      {
        ioctlsocket(sClient,FIONBIO,&BlockMode);
        pMess = RecivPacket(sClient);
        if(pMess == NULL)
        {
          POSITION pos = g_ListSck.Find(sClient , g_ListSck.GetHeadPosition());
          if(pos != NULL)
          {
            g_ListSck.RemoveAt(pos);
            printf("%u ���ӶϿ�\n", sClient);
          }
          continue;
        }

        // ������Ϣ
        DisPose(sClient, pMess);
      }

      nRecv = 0;
      pMess = NULL;
      sClient = INVALID_SOCKET;
    }
  }
}


UINT AFX_CDECL ConnectThreadProc(LPVOID lpParameter)
{
  SOCKET sClient = INVALID_SOCKET;
  int nLength = sizeof(sockaddr);
  while(1)
  {
    sClient = accept(g_Sck , (sockaddr*)&g_addr , &nLength);
    if(sClient == INVALID_SOCKET || sClient == NULL)
    {
      printf("���Ӵ���\n");
      return S_FALSE;
    }
    g_ListSck.AddTail(sClient);

    printf("%u ���ӳɹ�\n", sClient);
  }
}


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
  printf("����'D'�˳��м��\n");

  // 4. �ȴ�����
  AfxBeginThread(ConnectThreadProc , NULL);

  // 5. �����Ϣ
  AfxBeginThread(RecvThreadProc , NULL);

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
    
    AfxBeginThread(DisSqlPoseThreadProc , NULL);

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
  BOOL bRet = pool.Create(4);

  if (!bRet)
  {
    return 0;
  }


  // pool.Handle(xxx);


  while(char c = getchar())
  {
    if(c == 'D')
    {
      break;
    }
  }

  pool.Destroy();

  POSITION pos = g_ListSck.GetHeadPosition();
  SOCKET sClient = INVALID_SOCKET;
  while(pos != NULL)
  {
    sClient = g_ListSck.GetNext(pos);
    closesocket(sClient);
    sClient = INVALID_SOCKET;
  }
  g_ListSck.RemoveAll();

  closesocket(g_sSqlServer);
  closesocket(g_Sck);
	return 0;
}