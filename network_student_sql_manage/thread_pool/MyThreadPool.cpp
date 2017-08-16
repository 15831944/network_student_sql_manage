// ThreadPool1.cpp: implementation of the CThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyThreadPool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadPool::CThreadPool()
{
  m_nCount = 0;
  m_hSemaphore = NULL;
  m_hThreadAry = NULL;
}

CThreadPool::~CThreadPool()
{

}



BOOL CThreadPool::Create(int nCount)
{
  //�����źŵƶ���
  m_nCount = nCount;

  if (m_nCount <= 0)
  {
    return FALSE;
  }

  m_hExitEvent = CreateEvent(NULL,
    TRUE,
    FALSE,
    NULL);
  
  if (m_hExitEvent == NULL)
  {
    return FALSE;
  }

  m_hSemaphore = CreateSemaphore(NULL,
                  0,
                  0x7fffffff,
                  NULL);

  if (m_hSemaphore == NULL)
  {
    return FALSE;
  }

  m_hThreadAry = new HANDLE[m_nCount];
  if (m_hThreadAry == NULL)
  {
    return FALSE;
  }

  for(int i = 0; i < m_nCount; i++)
  {
    m_hThreadAry[i] = (HANDLE)_beginthreadex(NULL,
                                   0,
                                   (unsigned int (__stdcall *)(void *))ThreadFunc,
                                   this,
                                   0,
                                   NULL);
    
	}

  return TRUE;
}

BOOL CThreadPool::Destroy()
{

  //֪ͨ�̳߳��е��߳��˳�
  SetEvent(m_hExitEvent);

  //�ȴ��̳߳������е��߳��˳�
  DWORD dwRet = WaitForMultipleObjects(m_nCount,
                         m_hThreadAry,
                         TRUE,
                         INFINITE);

  if (dwRet != WAIT_FAILED)
  {
    for (int i = 0; i < m_nCount; i++)
    {
      CloseHandle(m_hThreadAry[i]);
    }
    delete[] m_hThreadAry;
  }

   return TRUE;
}

BOOL CThreadPool::Handle(IMyCommand *pCmd)
{
   //���������������
   m_mgr.Insert(pCmd);

   //�ͷ�һ���źŵ���Դ,��ʼ�ɻ�
   ReleaseSemaphore(m_hSemaphore,
                    1,
	                  NULL);

   return TRUE;
}

DWORD CThreadPool::ThreadFunc(LPVOID lpParam)
{
  CThreadPool* pThis = (CThreadPool*)lpParam;
  HANDLE hAry[2];

  hAry[0] = pThis->m_hExitEvent;
  hAry[1] = pThis->m_hSemaphore;

  while (true)
  {
    
    DWORD dwRet = WaitForMultipleObjects(2,
                           hAry,
                           FALSE,
                           INFINITE);
    if (dwRet == WAIT_FAILED)
    {
      //������
    }
    else if (dwRet == WAIT_TIMEOUT)
    {
      //��ʱ����
    }
    else 
    {
      int nIndex = dwRet - WAIT_OBJECT_0;
      switch (nIndex)
      {
      case 0:
        {
          //��ζ�Ÿ��˳��߳���
          return 0;
        }
        break;
      case 1:
        {
          //������,ȡ������,����ʼִ��
          //pCmd->Exec();
          IMyCommand* pCmd = NULL;
          BOOL bRet = pThis->m_mgr.GetCommand(pCmd);
          if (bRet && pCmd != NULL)
          {
            pCmd->Exec();
            if (pCmd != NULL)
            {
              delete pCmd;
            }
          }
          continue;
        }
        break;
      default:
        {

        }
        break;
      }
    }

	}

  return 0;
}
