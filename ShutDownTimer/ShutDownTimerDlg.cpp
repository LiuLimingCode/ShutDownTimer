
// ShutDownTimerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ShutDownTimer.h"
#include "ShutDownTimerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CShutDownTimerDlg 对话框

CShutDownTimerDlg::CShutDownTimerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHUTDOWNTIMER_DIALOG, pParent)
	, m_Timer(_T(""))
	, m_settimer(_T(""))
	, m_setday(0)
	, m_sethour(0)
	, m_setmin(0)
	, m_setsec(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShutDownTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITTIMER, m_Timer);
	DDX_Text(pDX, IDC_EDITSETTIMER, m_settimer);
	DDX_Text(pDX, IDC_EDITSETDAY, m_setday);
	DDX_Text(pDX, IDC_EDITSETHOUR, m_sethour);
	DDX_Text(pDX, IDC_EDITSETMIN, m_setmin);
	DDX_Text(pDX, IDC_EDITSETSEC, m_setsec);
}

BEGIN_MESSAGE_MAP(CShutDownTimerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIOTIMER, &CShutDownTimerDlg::OnBnClickedRadiotimer)
	ON_BN_CLICKED(IDC_RADIODELAY, &CShutDownTimerDlg::OnBnClickedRadiodelay)
	ON_BN_CLICKED(IDC_BUTTONSHUT, &CShutDownTimerDlg::OnBnClickedButtonshut)
	ON_BN_CLICKED(IDC_BUTTONCANCEL, &CShutDownTimerDlg::OnBnClickedButtoncancel)
	ON_BN_CLICKED(IDC_RADIOSETSHUT, &CShutDownTimerDlg::OnBnClickedRadiosetshut)
	ON_BN_CLICKED(IDC_RADIOSETRESHUT, &CShutDownTimerDlg::OnBnClickedRadiosetreshut)
	ON_EN_KILLFOCUS(IDC_EDITSETTIMER, &CShutDownTimerDlg::OnEnKillfocusEditsettimer)
END_MESSAGE_MAP()


// CShutDownTimerDlg 消息处理程序

BOOL CShutDownTimerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetTimer(1, 1000, NULL);

	((CButton *)GetDlgItem(IDC_RADIOSETSHUT))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIOTIMER))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIODELAY))->SetCheck(FALSE);
	GetDlgItem(IDC_EDITSETTIMER)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITSETDAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITSETHOUR)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITSETMIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITSETSEC)->EnableWindow(FALSE);

	CString strDate, strTime;

	GetLocalTime(&st);
	strDate.Format(_T("%04d-%02d-%02d  "), st.wYear, st.wMonth, st.wDay);
	strTime.Format(_T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
	_timer = st;
	_timer.wSecond = 0;
	m_Timer = strDate + strTime;
	m_settimer.Format(_T("%04d-%02d-%02d  %02d:%02d:00"), _timer.wYear, _timer.wMonth, _timer.wDay, _timer.wHour, _timer.wMinute);
	m_setday = 0;
	m_sethour = 0;
	m_setmin = 0;
	m_setsec = 0;
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CShutDownTimerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CShutDownTimerDlg::OnPaint()
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
HCURSOR CShutDownTimerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CShutDownTimerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strDate, strTime;

	switch (nIDEvent)
	{
	case 1:
		GetLocalTime(&st);
		strDate.Format(_T("%04d-%02d-%02d  "), st.wYear, st.wMonth, st.wDay);
		strTime.Format(_T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
		GetDlgItem(IDC_EDITTIMER)->SetWindowTextW(strDate + strTime);
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CShutDownTimerDlg::OnBnClickedRadiotimer()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton *)GetDlgItem(IDC_RADIODELAY))->SetCheck(FALSE);
	GetDlgItem(IDC_EDITSETTIMER)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITSETDAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITSETHOUR)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITSETMIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITSETSEC)->EnableWindow(FALSE);
	
	UpdateData(FALSE);
}

void CShutDownTimerDlg::OnBnClickedRadiodelay()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton *)GetDlgItem(IDC_RADIOTIMER))->SetCheck(FALSE);
	GetDlgItem(IDC_EDITSETTIMER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITSETDAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITSETHOUR)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITSETMIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITSETSEC)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

void CShutDownTimerDlg::OnBnClickedButtonshut()
{
	UpdateData(TRUE);
	long long _sec;
	CString str;
	char cmd[30] = { 0 };
	int temp;
	if (((CButton *)GetDlgItem(IDC_RADIOTIMER))->GetCheck() == 1)
	{
		GetLocalTime(&st);

		_sec = (_timer.wDay - st.wDay) * 3600 * 24 + (_timer.wHour - st.wHour) * 3600 +
			(_timer.wMinute - st.wMinute) * 60 + (_timer.wSecond - st.wSecond);

		if (_sec < 0)
		{
			MessageBox(_T("输入时间存在错误！"), _T("警告"), MB_OK | MB_ICONWARNING);
			return;
		}
	}
	else if (((CButton *)GetDlgItem(IDC_RADIODELAY))->GetCheck() == 1)
	{
		_sec = m_setday * 3600 * 24 + m_sethour * 3600 + m_setmin * 60 + m_setsec;
	}

	if (_sec > 315360000) _sec = 315360000;

	if (((CButton *)GetDlgItem(IDC_RADIOSETSHUT))->GetCheck() == 1)
		str.Format(_T("shutdown -s -t %lld"), _sec);
	else if (((CButton *)GetDlgItem(IDC_RADIOSETRESHUT))->GetCheck() == 1)
		str.Format(_T("shutdown -r -t %lld"), _sec);

	for (temp = 0; temp < str.GetLength() && temp < 29; ++temp)
	{
		cmd[temp] = (char)str[temp];
	}
	cmd[temp] = 0;
	system(cmd);
	// TODO: 在此添加控件通知处理程序代码
}

void CShutDownTimerDlg::OnEnKillfocusEditsettimer()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	CString str;
	int time[6] = { 0 };
	int num__ = 0, num_1 = 0, index__ = 0, index_1 = 0;
	while ((index__ = m_settimer.Find('-', index__ + 1)) != -1)
		num__++;
	while ((index_1 = m_settimer.Find(':', index_1 + 1)) != -1)
		num_1++;

	if (num__ == 2 && num_1 == 2)
	{
		int temp, remain;
		str = m_settimer;
		for (temp = 0; temp < 6; temp++)
		{
			time[temp] = _wtoi(str);
			while (str[0] >= _T('0') && str[0] <= _T('9') && str.GetLength() != 0) str.Delete(0);
			while ((str[0] < _T('0') || str[0] > _T('9')) && str.GetLength() != 0) str.Delete(0);
		}
		_timer.wSecond = time[5] % 60;
		remain = time[5] / 60;

		_timer.wMinute = (time[4] + remain) % 60;
		remain = (time[4] + remain) / 60;

		_timer.wHour = (time[3] + remain) % 24;
		remain = (time[3] + remain) / 24;

		_timer.wDay = time[2] + remain;
	}
	else
	{
		_timer = st;
		_timer.wSecond = 0;
	}

	m_settimer.Format(_T("%04d-%02d-%02d  %02d:%02d:%02d"), _timer.wYear, _timer.wMonth, _timer.wDay, _timer.wHour, _timer.wMinute, _timer.wSecond);
	UpdateData(FALSE);
}

void CShutDownTimerDlg::OnBnClickedButtoncancel()
{
	system("shutdown -a");
	// TODO: 在此添加控件通知处理程序代码
}

void CShutDownTimerDlg::OnBnClickedRadiosetshut()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTONSHUT)->SetWindowTextW(_T("设定关机"));
	((CButton *)GetDlgItem(IDC_RADIOSETRESHUT))->SetCheck(FALSE);
}

void CShutDownTimerDlg::OnBnClickedRadiosetreshut()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTONSHUT)->SetWindowTextW(_T("设定重启"));
	((CButton *)GetDlgItem(IDC_RADIOSETSHUT))->SetCheck(FALSE);
}

