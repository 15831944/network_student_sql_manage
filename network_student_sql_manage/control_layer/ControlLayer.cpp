// control_layer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MySocket.h"
#include "MyThreadPool.h"
#include "IMyCommand.h"

int _tmain(int argc, _TCHAR* argv[])
{
  if(!MyWSAStartup())
  {
    return -1;
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