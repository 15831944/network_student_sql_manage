
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
  , m_nId(0)
  , m_strName(_T(""))
  , m_nSex(0)
  , m_strPhone(_T(""))
  , m_strHobby(_T(""))
  , m_sServer(INVALID_SOCKET)
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
  DDX_Text(pDX , IDC_EDIT_ID , m_nId);
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
  m_ListMain.InsertColumn(emListMain::phone , _T("�绰"), 0, 200);
  m_ListMain.InsertColumn(emListMain::hobby , _T("����"), 0, 200);
}


void CClientLayerDlg::OnBnClickedOk()
{
  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  // CDialogEx::OnOK();
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
  if(isInput)
  {
    UpdateData(TRUE);
    input.nId = 0;
    input.nSex = 0;
    input.strName.Empty();
    input.strPhone.Empty();
    input.strHobby.Empty();
    input.nId = m_nId;
    input.nSex = m_nSex;
    input.strName = m_strName;
    input.strPhone = m_strPhone;
    input.strHobby = m_strHobby;
  }
  else
  {
    m_nId = input.nId;
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
