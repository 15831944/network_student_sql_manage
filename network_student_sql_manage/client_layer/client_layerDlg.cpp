sql_insert
refresh
findall
find
update
del
insert
insert
optdone
optdone
refresh
refresh
update
insert

// client_layerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "client_layer.h"
#include "client_layerDlg.h"
#include "afxdialogex.h"
#include "enum.h"
#include "MySocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientLayerDlg �Ի���



CClientLayerDlg::CClientLayerDlg(CWnd* pParent /*=NULL*/)
  : CDialogEx(CClientLayerDlg::IDD , pParent)
  , m_strName(_T(""))
  , m_nSex(0)
  , m_strPhone(_T(""))
  , m_strHobby(_T(""))
  , m_sServer(INVALID_SOCKET)
  , m_nSqlVer(-1)
  , m_nPreOpt(-1)
  , m_nCurOpt(-1)
  , m_pInfo(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientLayerDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX , IDC_LIST_MAIN , m_ListMain);
  DDX_Control(pDX , IDOK , m_btnOK);
  DDX_Control(pDX , IDC_BUTTON_REFRESH , m_btnRefresh);
  DDX_Control(pDX , IDC_BUTTON_LOOKALL , m_btnLookAll);
  DDX_Control(pDX , IDC_BUTTON_UPDATE , m_btnUpdate);
  DDX_Control(pDX , IDC_BUTTON_INSERT , m_btnInsert);
  DDX_Control(pDX , IDC_BUTTON_DEL , m_btnDelete);
  DDX_Control(pDX , IDC_BUTTON_PAGEUP , m_btnPageUp);
  DDX_Control(pDX , IDC_BUTTON_PAGEDOWN , m_btnPageDown);
  DDX_Control(pDX , IDC_EDIT_ID , m_EditID);
  DDX_Control(pDX , IDC_EDIT_NAME , m_EditName);
  DDX_Control(pDX , IDC_COMBO_SEX , m_ComboSex);
  DDX_Control(pDX , IDC_COMBO_PAGE_INFO , m_ComboPageInfo);
  DDX_Control(pDX , IDC_EDIT_PHONE , m_EditPhone);
  DDX_Control(pDX , IDC_EDIT_HOBBY , m_ListHobby);
  DDX_Control(pDX , IDC_STATIC_INFO , m_StaticTextInfo);
  DDX_Control(pDX , IDC_PROGRESS , m_Progress);
  //  DDX_Text(pDX , IDC_EDIT_ID , m_nId);
  DDX_Text(pDX , IDC_EDIT_NAME , m_strName);
  DDX_CBIndex(pDX , IDC_COMBO_SEX , m_nSex);
  DDX_Text(pDX , IDC_EDIT_PHONE , m_strPhone);
  DDX_Text(pDX , IDC_EDIT_HOBBY , m_strHobby);
}

BEGIN_MESSAGE_MAP(CClientLayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDOK , &CClientLayerDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL , &CClientLayerDlg::OnBnClickedCancel)
  ON_WM_TIMER()
  ON_MESSAGE(WM_MYMESSAGE, &CClientLayerDlg::OnMyMessage)
  ON_BN_CLICKED(IDC_BUTTON_INSERT , &CClientLayerDlg::OnBnClickedButtonInsert)
  ON_BN_CLICKED(IDC_BUTTON_LOOKALL , &CClientLayerDlg::OnBnClickedButtonLookall)
  ON_BN_CLICKED(IDC_BUTTON_UPDATE , &CClientLayerDlg::OnBnClickedButtonUpdate)
  ON_BN_CLICKED(IDC_BUTTON_REFRESH , &CClientLayerDlg::OnBnClickedButtonRefresh)
  ON_BN_CLICKED(IDC_BUTTON_DEL , &CClientLayerDlg::OnBnClickedButtonDel)
//  ON_NOTIFY(LVN_ITEMCHANGED , IDC_LIST_MAIN , &CClientLayerDlg::OnLvnItemchangedListMain)
ON_NOTIFY(NM_CLICK , IDC_LIST_MAIN , &CClientLayerDlg::OnNMClickListMain)
END_MESSAGE_MAP()


// CClientLayerDlg ��Ϣ�������

BOOL CClientLayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
  // ��ʼ������
  InitInterface();
  // �����м��
  ConnectControl();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CClientLayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientLayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientLayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// ��ʼ������
void CClientLayerDlg::InitInterface()
{
  InitList();
  DisabledAllBtn();

  // �Ա�ؼ�
  m_ComboSex.ResetContent();
  m_ComboSex.InsertString(emComboSex::unkonw, TEXT("δ֪"));
  m_ComboSex.InsertString(emComboSex::man, TEXT("��"));
  m_ComboSex.InsertString(emComboSex::woman, TEXT("Ů"));
  m_ComboSex.SetCurSel(emComboSex::unkonw);

  // ���ý�������ʱ��
  StartProgress();
  
}

void CClientLayerDlg::InitList()
{
  m_ListMain.ModifyStyle(0, LVS_REPORT);
  DWORD dwStye = m_ListMain.GetExtendedStyle();
  dwStye = dwStye | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
  m_ListMain.SetExtendedStyle(dwStye);
  m_ListMain.InsertColumn(emListMain::id , _T("ID"), 0, 60);
  m_ListMain.InsertColumn(emListMain::name , _T("����"), 0, 100);
  m_ListMain.InsertColumn(emListMain::sex , _T("�Ա�"), 0, 60);
  m_ListMain.InsertColumn(emListMain::phone , _T("�绰"), 0, 195);
  m_ListMain.InsertColumn(emListMain::hobby , _T("����"), 0, 200);
}


void CClientLayerDlg::OnBnClickedCancel()
{
  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  CDialogEx::OnCancel();
}



void CClientLayerDlg::DisabledAllBtn(BOOL isDis)
{
  if(isDis)
  {
    m_btnOK.EnableWindow(FALSE);
    m_btnRefresh.EnableWindow(FALSE);
    m_btnLookAll.EnableWindow(FALSE);
    m_btnUpdate.EnableWindow(FALSE);
    m_btnInsert.EnableWindow(FALSE);
    m_btnDelete.EnableWindow(FALSE);
    m_btnPageUp.EnableWindow(FALSE);
    m_btnPageDown.EnableWindow(FALSE);
    m_ComboPageInfo.EnableWindow(FALSE);
  }
  else
  {
    m_btnOK.EnableWindow(TRUE);
    m_btnRefresh.EnableWindow(TRUE);
    m_btnLookAll.EnableWindow(TRUE);
    m_btnUpdate.EnableWindow(TRUE);
    m_btnInsert.EnableWindow(TRUE);
    m_btnDelete.EnableWindow(TRUE);
    m_btnPageUp.EnableWindow(TRUE);
    m_btnPageDown.EnableWindow(TRUE);
    m_ComboPageInfo.EnableWindow(TRUE);
  }
}



void CClientLayerDlg::ManageInputInfo(InputInfo& input , bool isInput)
{
  CString strId;
  if(isInput)
  {
    UpdateData(TRUE);
    input.nId = 0;
    input.nSex = 0;
    input.strName.Empty();
    input.strPhone.Empty();
    input.strHobby.Empty();

    m_EditID.GetWindowTextW(strId);
    if(strId.IsEmpty())
    {
      input.nId = 0;
    }
    else
    {
      input.nId = _wtoi(strId.GetBuffer());
    }
    input.nSex = m_nSex;
    input.strName = m_strName;
    input.strPhone = m_strPhone;
    input.strHobby = m_strHobby;
  }
  else
  {
    strId.Format(TEXT("%d") , input.nId);
    m_EditID.SetWindowTextW(strId);
    m_nSex = input.nSex;
    m_strName = input.strName;
    m_strPhone = input.strPhone;
    m_strHobby = input.strHobby;
    UpdateData(FALSE);
  }
}


void CClientLayerDlg::OnTimer(UINT_PTR nIDEvent)
{
  // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
  switch(nIDEvent)
  {
    case emTimer::progress:
    {
      int nNum = m_Progress.GetPos();
      m_Progress.SetPos((++nNum) % 101);
    }
      break;
    default:
      break;
  }

  CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT CClientLayerDlg::OnMyMessage(WPARAM wParam , LPARAM lParam)
{
  switch(wParam)
  {
    case emMessage::connected:
      StopProgress();
      m_StaticTextInfo.SetWindowTextW(L"���ӳɹ���");
      m_EditID.Clear();
      m_sServer = lParam;
      DisabledAllBtn(FALSE);
      break;
    case emMessage::connect_fail:
      StopProgress();
      m_StaticTextInfo.SetWindowTextW(L"����ʧ�ܣ���ʼ���������");
      m_EditID.Clear();
      break;
    case emMessage::optdone:
      StopProgress();
      DisabledAllBtn(FALSE);
      m_StaticTextInfo.SetWindowTextW((TCHAR*)lParam);
      break;
    case emMessage::recvinfo:
      DisposeRecv((char*)lParam);
      delete (char*)lParam;
      break;
    default:
      break;
  }

  return S_OK;
}


UINT AFX_CDECL CClientLayerDlg::ConnectThreadProc(LPVOID lpParameter)
{
  CClientLayerDlg* obj = (CClientLayerDlg*)lpParameter;
  SOCKET s = INVALID_SOCKET;

  // 1. ����һ���׽��� socket
  s = socket(AF_INET , // ����Э���
             SOCK_STREAM , // "��ʽ" -- TCP, SOCK_DGRAM "���ݱ�" -- UDP
             IPPROTO_TCP);
  if(s == INVALID_SOCKET)
  {
    obj->SendMessage(WM_MYMESSAGE , emMessage::connect_fail);
    return S_FALSE;
  }

  // 2. ���÷���� IP �� �˿� // �ͻ��˲���Ҫ��
  sockaddr_in addr; // һ��14�ֽڴ�Ľṹ��
  addr.sin_family = AF_INET;
  addr.sin_addr.S_un.S_addr = inet_addr(CONTROL_IP); // IP��ַ
  addr.sin_port = htons(CONTROL_PORT); // htons() �������ֽ���תΪ������s����sort

  // 3. ���ӷ���� connect
  while(1)
  {
    int nRet = connect(s , (sockaddr*)&addr , sizeof(sockaddr));
    if(nRet == SOCKET_ERROR)
    {
      continue;
    }

    obj->SendMessage(WM_MYMESSAGE , emMessage::connected, s);
    break;
  }

  return S_OK;
}

void CClientLayerDlg::ConnectControl()
{
  AfxBeginThread(ConnectThreadProc ,
                 (LPVOID)this);
}


void CClientLayerDlg::StartProgress()
{
  m_Progress.ShowWindow(SW_SHOW);
  m_Progress.SetPos(0);
  SetTimer(emTimer::progress , 10, NULL);
}


void CClientLayerDlg::StopProgress()
{
  m_Progress.ShowWindow(SW_HIDE);
  m_Progress.SetPos(0);
  KillTimer(emTimer::progress);
}


void CClientLayerDlg::OnBnClickedButtonInsert()
{
  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  InputInfo* pInfo = new InputInfo;
  if(pInfo == NULL)
  {
    return;
  }

  ManageInputInfo(*pInfo);

  do{
    if(pInfo->strName.IsEmpty())
    {
      MessageBox(TEXT("������������"));
    }
    else if(pInfo->nSex == 0)
    {
      MessageBox(TEXT("��ѡ���Ա�"));
    }
    else if(pInfo->strPhone.IsEmpty())
    {
      MessageBox(TEXT("������绰��"));
    }
    else if(pInfo->strHobby.IsEmpty())
    {
      MessageBox(TEXT("�����밮�ã�"));
    }
    else
    {
      break;
    }
    
    m_StaticTextInfo.SetWindowTextW(L"��������");
    delete pInfo;
    return ;
  } while(0);

  m_pInfo = pInfo;
  m_nCurOpt = emOpt::insert;

  AfxBeginThread(OptThreadProc , (LPVOID)this);

  DisabledAllBtn();
  StartProgress();
}


void CClientLayerDlg::OnBnClickedOk()
{
  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  // CDialogEx::OnOK();
  InputInfo* pInfo = new InputInfo;
  if(pInfo == NULL)
  {
    return;
  }

  ManageInputInfo(*pInfo);

  if(pInfo->nId == 0
     && pInfo->strName.IsEmpty()
     && pInfo->nSex == 0
     && pInfo->strPhone.IsEmpty()
     && pInfo->strHobby.IsEmpty()
     )
  {
    m_StaticTextInfo.SetWindowTextW(L"��������");
    MessageBox(TEXT("�������ѯ������ID ����Ϊ 0��"));

    delete pInfo;
    return;
  }

  m_pInfo = pInfo;
  m_nCurOpt = emOpt::find;

  AfxBeginThread(OptThreadProc , (LPVOID)this);

  DisabledAllBtn();
  StartProgress();
}


void CClientLayerDlg::OnBnClickedButtonLookall()
{
  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  m_nCurOpt = emOpt::findall;
  AfxBeginThread(OptThreadProc , (LPVOID)this);
}


void CClientLayerDlg::OnBnClickedButtonUpdate()
{
  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  InputInfo* pInfo = new InputInfo;
  if(pInfo == NULL)
  {
    return;
  }

  ManageInputInfo(*pInfo);

  do{
    if(pInfo->nId == 0)
    {
      MessageBox(TEXT("������ID��ID ����Ϊ 0��"));
    }
    else if(pInfo->strName.IsEmpty())
    {
      MessageBox(TEXT("������������"));
    }
    else if(pInfo->nSex == 0)
    {
      MessageBox(TEXT("��ѡ���Ա�"));
    }
    else if(pInfo->strPhone.IsEmpty())
    {
      MessageBox(TEXT("������绰��"));
    }
    else if(pInfo->strHobby.IsEmpty())
    {
      MessageBox(TEXT("�����밮�ã�"));
    }
    else
    {
      break;
    }
    
    m_StaticTextInfo.SetWindowTextW(L"��������");
    delete pInfo;
    return ;
  } while(0);

  m_pInfo = pInfo;
  m_nCurOpt = emOpt::update;

  AfxBeginThread(OptThreadProc , (LPVOID)this);

  DisabledAllBtn();
  StartProgress();
}


void CClientLayerDlg::OnBnClickedButtonRefresh()
{
  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  m_nCurOpt = emOpt::refresh;
  AfxBeginThread(OptThreadProc , (LPVOID)this);
}


void CClientLayerDlg::OnBnClickedButtonDel()
{
  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  InputInfo* pInfo = new InputInfo;
  if(pInfo == NULL)
  {
    return;
  }

  ManageInputInfo(*pInfo);

  if(pInfo->nId == 0)
  {
    m_StaticTextInfo.SetWindowTextW(L"��������");
    MessageBox(TEXT("ID ����Ϊ 0��"));
    delete pInfo;
    return ;
  }

  m_pInfo = pInfo;
  m_nCurOpt = emOpt::del;

  AfxBeginThread(OptThreadProc , (LPVOID)this);

  DisabledAllBtn();
  StartProgress();
}


UINT AFX_CDECL CClientLayerDlg::OptThreadProc(LPVOID lpParameter)
{
  CClientLayerDlg* obj = (CClientLayerDlg*)lpParameter;
  TCHAR* pszBuff = NULL;

  int nRet = FALSE;

  switch (obj->m_nCurOpt)
  {
    case emOpt::insert:
      nRet = OptInsert(obj);
      break;
    case emOpt::del:
      nRet = OptDel(obj);
      break;
    case emOpt::update:
      nRet = OptUpdate(obj);
      break;
    case emOpt::find:
      nRet = OptFind(obj);
      break;
    case emOpt::findall:
      nRet = OptFindall(obj);
      break;
    case emOpt::refresh:
      break;
    default:
      break;
  }

  if(nRet == 2)
  {
    pszBuff = TEXT("Զ�����ݿ�δ�仯���ӱ��ػ����������");
  }
  else if(nRet)
  {
    pszBuff = TEXT("������ɣ�");
  }
  else
  {
    pszBuff = TEXT("����ʧ�ܣ�");
  }

  obj->m_nPreOpt = obj->m_nCurOpt;
  obj->m_nCurOpt = -1;
  if(obj->m_pInfo != NULL)
  {
    delete obj->m_pInfo;
    obj->m_pInfo = NULL;
  }

  obj->SendMessage(WM_MYMESSAGE , emMessage::optdone, (LPARAM)pszBuff );

  return 0;
}


BOOL CClientLayerDlg::OptInsert(CClientLayerDlg* obj)
{
  if(obj == NULL || obj->m_pInfo == NULL)
  {
    return FALSE;
  }

  InputInfo* pInfo = obj->m_pInfo;

  CString strSql;
  strSql.Format(TEXT("INSERT INTO tab_student (name, sex, phone, hobby) VALUES ('%s', %d, '%s', '%s')"),
                pInfo->strName,
                pInfo->nSex,
                pInfo->strPhone,
                pInfo->strHobby);
  
  SendPacket(obj->m_sServer ,
             emSocketMsg::sql_insert ,
             (char*)strSql.GetBuffer() ,
             ((strSql.GetLength() + 1) * sizeof(USHORT)));

  char* pSqlMess = RecivPacket(obj->m_sServer);
  
  if(pSqlMess == NULL)
  {
    return 0;
  }

  DWORD nLen = *(DWORD*)pSqlMess;
  BYTE nType = *(BYTE*)(pSqlMess + sizeof(DWORD));

  if(nType != emSocketMsg::sql_insert_succ)
  {
    return FALSE;
  }

  delete pSqlMess;

  return TRUE;
}


BOOL CClientLayerDlg::OptDel(CClientLayerDlg* obj)
{
  if(obj == NULL || obj->m_pInfo == NULL)
  {
    return FALSE;
  }

  InputInfo* pInfo = obj->m_pInfo;

  CString strSql;
  strSql.Format(TEXT("DELETE FROM tab_student WHERE id = %d"),
                pInfo->nId);
  
  SendPacket(obj->m_sServer ,
             emSocketMsg::sql_del ,
             (char*)strSql.GetBuffer() ,
             ((strSql.GetLength() + 1) * sizeof(USHORT)));

  char* pSqlMess = RecivPacket(obj->m_sServer);
  
  if(pSqlMess == NULL)
  {
    return 0;
  }

  DWORD nLen = *(DWORD*)pSqlMess;
  BYTE nType = *(BYTE*)(pSqlMess + sizeof(DWORD));

  if(nType != emSocketMsg::sql_del_succ)
  {
    return FALSE;
  }

  delete pSqlMess;

  return TRUE;
}


BOOL CClientLayerDlg::OptUpdate(CClientLayerDlg* obj)
{
  if(obj == NULL || obj->m_pInfo == NULL)
  {
    return FALSE;
  }

  InputInfo* pInfo = obj->m_pInfo;

  CString strSql;
  strSql.Format(TEXT("UPDATE tab_student SET name='%s', sex=%d, phone='%s', hobby='%s' WHERE id = %d"),
                pInfo->strName,
                pInfo->nSex,
                pInfo->strPhone,
                pInfo->strHobby,
                pInfo->nId);
  
  SendPacket(obj->m_sServer ,
             emSocketMsg::sql_update ,
             (char*)strSql.GetBuffer() ,
             ((strSql.GetLength() + 1) * sizeof(USHORT)));

  char* pSqlMess = RecivPacket(obj->m_sServer);
  
  if(pSqlMess == NULL)
  {
    return 0;
  }

  DWORD nLen = *(DWORD*)pSqlMess;
  BYTE nType = *(BYTE*)(pSqlMess + sizeof(DWORD));

  if(nType != emSocketMsg::sql_update_succ)
  {
    return FALSE;
  }

  delete pSqlMess;

  return TRUE;
}


BOOL CClientLayerDlg::OptFindall(CClientLayerDlg* obj)
{
  if(obj == NULL)
  {
    return FALSE;
  }

  if(obj->m_nPreOpt == obj->m_nCurOpt)
  {

    // return 2;
  }

  InputInfo* pInfo = obj->m_pInfo;

  CString strSql;
  strSql.Format(TEXT("SELECT ID, name, sex, phone, hobby FROM tab_student"));
  
  SendPacket(obj->m_sServer ,
             emSocketMsg::sql_findall ,
             (char*)strSql.GetBuffer() ,
             ((strSql.GetLength() + 1) * sizeof(USHORT)));

  char* pSqlMess = RecivPacket(obj->m_sServer);
  
  if(pSqlMess == NULL)
  {
    return 0;
  }

  DWORD nLen = *(DWORD*)pSqlMess;
  BYTE nType = *(BYTE*)(pSqlMess + sizeof(DWORD));

  if(nType != emSocketMsg::sql_findall_succ)
  {
    return FALSE;
  }

  obj->SendMessage(WM_MYMESSAGE , emMessage::recvinfo , (LPARAM)pSqlMess);
  // delete pSqlMess;

  return TRUE;
}


BOOL CClientLayerDlg::OptFind(CClientLayerDlg* obj)
{
  if(obj == NULL)
  {
    return FALSE;
  }

  InputInfo* pInfo = obj->m_pInfo;

  CString strID;
  strID.Format(TEXT("%d") , pInfo->nId);

  CString strSql;
  strSql.Format(TEXT("SELECT ID, name, sex, phone, hobby FROM tab_student WHERE \
                      ID LIKE '%%%s%%' AND\
                      name LIKE '%%%s%%' AND \
                      sex  LIKE '%%%s%%' AND \
                      phone LIKE '%%%s%%' AND \
                      hobby LIKE '%%%s%%'"),
                      pInfo->nId == 0 ? L"" : strID.GetBuffer(),
                      pInfo->strName.IsEmpty() ? L"" : pInfo->strName.GetBuffer(),
                      pInfo->nSex == 0 ? L"" : pInfo->nSex == 1 ? L"1" : L"2",
                      pInfo->strPhone.IsEmpty() ? L"" : pInfo->strPhone.GetBuffer(),
                      pInfo->strHobby.IsEmpty() ? L"" : pInfo->strHobby.GetBuffer()
                      );
  
  SendPacket(obj->m_sServer ,
             emSocketMsg::sql_find ,
             (char*)strSql.GetBuffer() ,
             ((strSql.GetLength() + 1) * sizeof(USHORT)));

  char* pSqlMess = RecivPacket(obj->m_sServer);
  
  if(pSqlMess == NULL)
  {
    return 0;
  }

  DWORD nLen = *(DWORD*)pSqlMess;
  BYTE nType = *(BYTE*)(pSqlMess + sizeof(DWORD));

  if(nType != emSocketMsg::sql_find_succ)
  {
    return FALSE;
  }

  obj->SendMessage(WM_MYMESSAGE , emMessage::recvinfo , (LPARAM)pSqlMess);
  // delete pSqlMess;

  return TRUE;
}



void CClientLayerDlg::DisposeRecv(char* pMess)
{
  m_ListMain.DeleteAllItems();

  DWORD nLen = *(DWORD*)pMess;
  UINT nItem = *(UINT*)(pMess + sizeof(DWORD) + sizeof(BYTE));

  char* pwMess = (pMess + sizeof(DWORD) + sizeof(BYTE) + sizeof(UINT));
  DWORD nRul = 0;

  UINT nValLen = 0;
  TCHAR szBuff[ MAXBYTE ] = { 0 };
  for(int i = 0; (int)nRul < (int)nLen - (int)sizeof(DWORD) - (int)sizeof(BYTE) - (int)sizeof(UINT); ++i)
  {
    memcpy_s(&nValLen , MAXBYTE , pwMess + nRul , sizeof(UINT));
    nRul += sizeof(UINT);
    memcpy_s(szBuff , MAXBYTE , pwMess + nRul , nValLen * sizeof(USHORT));
    nRul += nValLen * sizeof(USHORT);
    m_ListMain.InsertItem(i , szBuff);

    memcpy_s(&nValLen , MAXBYTE , pwMess + nRul , sizeof(UINT));
    nRul += sizeof(UINT);
    memcpy_s(szBuff , MAXBYTE , pwMess + nRul , nValLen * sizeof(USHORT));
    nRul += nValLen * sizeof(USHORT);
    m_ListMain.SetItemText(i , 1, szBuff);

    memcpy_s(&nValLen , MAXBYTE , pwMess + nRul , sizeof(UINT));
    nRul += sizeof(UINT);
    memcpy_s(szBuff , MAXBYTE , pwMess + nRul , nValLen * sizeof(USHORT));
    nRul += nValLen * sizeof(USHORT);
    m_ListMain.SetItemData(i , *(UINT*)szBuff - '0');
    if(*(UINT*)szBuff - '0' == 1)
    {
      m_ListMain.SetItemText(i , 2, TEXT("��"));
    }
    else
    {
      m_ListMain.SetItemText(i , 2, TEXT("Ů"));
    }

    memcpy_s(&nValLen , MAXBYTE , pwMess + nRul , sizeof(UINT));
    nRul += sizeof(UINT);
    memcpy_s(szBuff , MAXBYTE , pwMess + nRul , nValLen * sizeof(USHORT));
    nRul += nValLen * sizeof(USHORT);
    m_ListMain.SetItemText(i , 3, szBuff);

    memcpy_s(&nValLen , MAXBYTE , pwMess + nRul , sizeof(UINT));
    nRul += sizeof(UINT);
    memcpy_s(szBuff , MAXBYTE , pwMess + nRul , nValLen * sizeof(USHORT));
    nRul += nValLen * sizeof(USHORT);
    m_ListMain.SetItemText(i , 4, szBuff);

  }
}


//void CClientLayerDlg::OnLvnItemchangedListMain(NMHDR *pNMHDR , LRESULT *pResult)
//{
//  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//  // TODO:  �ڴ���ӿؼ�֪ͨ����������
//  *pResult = 0;
//}


void CClientLayerDlg::OnNMClickListMain(NMHDR *pNMHDR , LRESULT *pResult)
{
  LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  *pResult = 0;

  int nSel = m_ListMain.GetNextItem(-1 , LVNI_ALL | LVNI_SELECTED);
  if(nSel < 0)
  {
    return;
  }

  InputInfo* pInfo = new InputInfo;

  pInfo->nId = _wtoi(m_ListMain.GetItemText(nSel , 0));
  pInfo->nSex = m_ListMain.GetItemData(nSel);
  pInfo->strName = m_ListMain.GetItemText(nSel , 1);
  pInfo->strPhone = m_ListMain.GetItemText(nSel , 3);
  pInfo->strHobby = m_ListMain.GetItemText(nSel , 4);

  ManageInputInfo(*pInfo , FALSE);
  delete pInfo;
}
