
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//HANDLE hSemaphore;
CRITICAL_SECTION s;

// CMFCApplication1Dlg 对话框
CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, m_1(1000)
	, m_2(0)
	, m_3(0)
	, m_4(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CWinThread* MainCWinThread = NULL;
	CWinThread* CWinThread1 = NULL;
	CWinThread* CWinThread2 = NULL;
	CWinThread* CWinThread3 = NULL;	
}

CMFCApplication1Dlg::~CMFCApplication1Dlg()
{
	//3.关闭信号量
	DeleteCriticalSection(&s);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_1);
	DDX_Text(pDX, IDC_EDIT2, m_2);
	DDX_Text(pDX, IDC_EDIT3, m_3);
	DDX_Text(pDX, IDC_EDIT4, m_4);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_MESSAGE(WM_UPDATE_STATIC, &CMFCApplication1Dlg::OnUpdateStatic)   //增加1个自定义的消息映射函数
END_MESSAGE_MAP()

// CMFCApplication1Dlg 消息处理程序
BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//创建一个新线程，发送消息，更新控件
	m_pThread = AfxBeginThread(ThreadFunction, this);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//点击按钮，开始创建1个主线程，主线程再创建3个线程抢红包
void CMFCApplication1Dlg::OnBnClickedButton1()
{		
	MainCWinThread = AfxBeginThread(MainThread, this);
}

//1.主线程函数
UINT CMFCApplication1Dlg::MainThread(LPVOID pParam)
{
	//1.获得窗口的指针，通过参数传进来
	CMFCApplication1Dlg* pDlg = NULL;
	pDlg = (CMFCApplication1Dlg*)pParam;

	//2.创建信号量
	InitializeCriticalSection(&s);

	//3.创建3个线程	
	pDlg->CWinThread1 = AfxBeginThread(Thread1, pDlg);
	pDlg->CWinThread2 = AfxBeginThread(Thread2, pDlg);
	pDlg->CWinThread3 = AfxBeginThread(Thread3, pDlg);

	return 0;
}

UINT CMFCApplication1Dlg::Thread1(LPVOID pParam)
{
	//1.获得窗口的指针，通过参数传进来
	CMFCApplication1Dlg* pDlg = NULL;
	pDlg = (CMFCApplication1Dlg*)pParam;

	BOOL bFlag = TRUE;
	while (bFlag)
	{
		EnterCriticalSection(&s);
		if (pDlg->m_1 > 0)
		{
			pDlg->m_1--;
			pDlg->m_2++;			
		}
		else
		{
			bFlag = FALSE;
		}

		LeaveCriticalSection(&s);
		Sleep(1);
	}	
	return 0;
}

UINT CMFCApplication1Dlg::Thread2(LPVOID pParam)
{
	//1.获得窗口的指针，通过参数传进来
	CMFCApplication1Dlg* pDlg = NULL;
	pDlg = (CMFCApplication1Dlg*)pParam;

	BOOL bFlag = TRUE;
	while (bFlag)
	{
		EnterCriticalSection(&s);
		if (pDlg->m_1 > 0)
		{
			pDlg->m_1--;
			pDlg->m_3++;		
		}
		else
		{
			bFlag = FALSE;
		}
		LeaveCriticalSection(&s);
		Sleep(1);
	}
	return 0;

}

UINT CMFCApplication1Dlg::Thread3(LPVOID pParam)
{
	//1.获得窗口的指针，通过参数传进来
	CMFCApplication1Dlg* pDlg = NULL;
	pDlg = (CMFCApplication1Dlg*)pParam;

	BOOL bFlag = TRUE;
	while (bFlag)
	{
		EnterCriticalSection(&s);
		if (pDlg->m_1 > 0)
		{			
			pDlg->m_1--;
			pDlg->m_4++;			
		}
		else
		{
			bFlag = FALSE;
		}
		LeaveCriticalSection(&s);
		Sleep(1);
	}
	return 0;
}

//自定义更新控件的函数
LRESULT CMFCApplication1Dlg::OnUpdateStatic(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)
	{
		UpdateData(FALSE);
	}
	return 0;
}

//创建的线程，专门用来发送消息给窗口，以便更新控件
UINT CMFCApplication1Dlg::ThreadFunction(LPVOID pParam)
{
	CMFCApplication1Dlg *pDlg = (CMFCApplication1Dlg *)pParam;

	//不停的循环更新
	while (TRUE) 
	{
		::PostMessage(pDlg->m_hWnd, WM_UPDATE_STATIC, 0, 0);
		Sleep(1);
	}
	return 0;
}
