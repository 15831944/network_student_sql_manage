// sql_layer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MySocket.h"


int _tmain(int argc, _TCHAR* argv[])
{
  if(MyWSAStartup())
  {
    return -1;
  }

	return 0;
}

