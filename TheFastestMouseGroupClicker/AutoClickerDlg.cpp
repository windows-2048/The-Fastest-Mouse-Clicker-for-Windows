#include "stdafx.h"
#include "AutoClicker.h"
#include "AutoClickerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_KEY_ESCAPE  (WM_USER+1)
#define WM_KEY_SPACE   (WM_USER+3)
#define WM_KEY_F3      (WM_USER+5)
#define WM_KEY_DELETE  (WM_USER+7)
#define WM_KEY_RETURN_DOWN  (WM_USER+9)
#define WM_KEY_RETURN_UP  (WM_USER+11)
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
HHOOK	hHook=NULL;
CString strProgName="The Fastest Mouse Group Clicker v2.5.4.0";
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////

HWND g_hwnd = NULL;

void mouse_event_SendInput(
	DWORD     dwFlags
	, DWORD     dx
	, DWORD     dy
	, DWORD     dwData
	, ULONG_PTR dwExtraInfo
	)
{
	INPUT input[1];
	memset(input, 0, 1 * sizeof(INPUT));

	input[0].type = INPUT_MOUSE;

	input[0].mi.dx = dx;
	input[0].mi.dy = dy;
	input[0].mi.dwFlags = dwFlags;

	input[0].mi.mouseData = dwData;
	input[0].mi.time = 0;
	input[0].mi.dwExtraInfo = dwExtraInfo;

	UINT ret = SendInput(1, input, sizeof(INPUT));

	if (ret != 1)
	{
		if (g_hwnd == NULL)
			g_hwnd = ::FindWindow(NULL, strProgName);

		MessageBoxA(g_hwnd, "Error in SendInput()", "AutoClickerX", MB_OK | MB_ICONERROR);
	}
}

///////////////////////////////////////////////////////////////////////

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAutoClickerDlg dialog

CAutoClickerDlg::CAutoClickerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoClickerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoClickerDlg)
	m_bAlwaysTop = FALSE;
	m_nX = 0;
	m_nY = 0;
	m_nInterval = 0;
	m_nClicks = 0;
	m_nMinutes = 0;
	m_nMouseBtn = -1;
	m_nCount = 0;
	m_nCurrentClicks = 0;
	m_strCurrentSpan = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CAutoClickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoClickerDlg)
	//DDX_Control(pDX, IDC_HOMEPAGE, m_Homepage);
	DDX_Control(pDX, IDC_XY_LIST, m_XYList);
	DDX_Check(pDX, IDC_WIN_ALWAYS_TOP_CHK, m_bAlwaysTop);
	DDX_Text(pDX, IDC_X_EDT, m_nX);
	DDX_Text(pDX, IDC_Y_EDT, m_nY);
	DDX_Text(pDX, IDC_CLICK_INTERVAL_EDT, m_nInterval);
	DDX_Text(pDX, IDC_STOP_AFTER_CLICKS_EDT, m_nClicks);
	DDX_Text(pDX, IDC_STOP_AFTER_MINS_EDT, m_nMinutes);
	DDX_Radio(pDX, IDC_MOUSE_BUTTON_RDO, m_nMouseBtn);
	DDX_Text(pDX, IDC_COUNT_EDT, m_nCount);
	DDX_Text(pDX, IDC_CURRENT_CLICKS_EDT, m_nCurrentClicks);
	DDX_Text(pDX, IDC_CURRENT_MINS_EDT, m_strCurrentSpan);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoClickerDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoClickerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDOK, OnStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SAVE_BTN, OnFileSave)
	ON_BN_CLICKED(IDC_LOAD_BTN, OnFileLoad)
	ON_BN_CLICKED(IDC_RUNSINGLEAPP_BTN, OnRunSingleApp)
	ON_BN_CLICKED(IDC_RANDOMIZE_BTN, OnRandomizeList)
	ON_BN_CLICKED(IDC_BOUNDRECT_BTN, OnBoundingRect)
	ON_BN_CLICKED(IDC_WIN_ALWAYS_TOP_CHK, OnWinAlwaysTopChk)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KEY_ESCAPE,OnKeyEscape)
	ON_MESSAGE(WM_KEY_SPACE, OnKeySpace)
	ON_MESSAGE(WM_KEY_F3, OnKeyF3)
	ON_MESSAGE(WM_KEY_DELETE, OnKeyDelete)
	ON_MESSAGE(WM_KEY_RETURN_DOWN, OnKeyReturnDown)
	ON_MESSAGE(WM_KEY_RETURN_UP, OnKeyReturnUp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAutoClickerDlg message handlers

LRESULT CALLBACK HookMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (g_hwnd == NULL)
		g_hwnd = ::FindWindow(NULL, strProgName);

	LRESULT lResult = 0;

	if(nCode == HC_ACTION) 
    {
		if (wParam == WM_KEYDOWN)
		{
			if (((KBDLLHOOKSTRUCT*)lParam)->vkCode == VK_ESCAPE)
			{
				::SendMessage(g_hwnd, WM_KEY_ESCAPE, 1, 1);
				return 1;
			}
			else if (((KBDLLHOOKSTRUCT*)lParam)->vkCode == VK_SPACE)
			{
				::SendMessage(g_hwnd, WM_KEY_SPACE, 1, 1);
				return 1;
			}
			else if (((KBDLLHOOKSTRUCT*)lParam)->vkCode == VK_F3)
			{
				::SendMessage(g_hwnd, WM_KEY_F3, 1, 1);
				return 1;
			}
			else if (((KBDLLHOOKSTRUCT*)lParam)->vkCode == VK_DELETE)
			{
				::SendMessage(g_hwnd, WM_KEY_DELETE, 1, 1);
				return 1;
			}
			else if (((KBDLLHOOKSTRUCT*)lParam)->vkCode == VK_RETURN)
			{
				::SendMessage(g_hwnd, WM_KEY_RETURN_DOWN, 1, 1);
				return 1;
			}
		}
		else if (wParam == WM_KEYUP)
		{
			if (((KBDLLHOOKSTRUCT*)lParam)->vkCode == VK_RETURN)
			{
				::SendMessage(g_hwnd, WM_KEY_RETURN_UP, 1, 1);
				return 1;
			}
		}
    }

	return CallNextHookEx( hHook, nCode, wParam, lParam);
}


BOOL CAutoClickerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_nMouseBtn		= 0;	//Left Button on Mouse.
	m_nClicks		= 0;	//0 click
	m_nInterval		= 500;	//500ms
	m_bAlwaysTop	= TRUE;	//Always Top
	m_nListIdx		= 0;
	m_nCount		= 0;
 
	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);	//Always Top...
	SetWindowText(strProgName);	
	memset(xyList,0,sizeof(xyList));
	m_bStart		=FALSE;
	m_bPause		= FALSE;

	UpdateData(FALSE);

	//Starting Hooking...
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)::HookMouseProc, GetModuleHandle(NULL), 0);

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_SAVE_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_RANDOMIZE_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BOUNDRECT_BTN)->EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CAutoClickerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	//if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	//{
	//	CAboutDlg dlgAbout;
	//	dlgAbout.DoModal();
	//}
	//else
	//{
		if ((nID & 0x0000FFF0) == SC_MINIMIZE)
		{
			// Minimizing, post to main dialogue also.
			AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
		}
		else if ((nID & 0x0000FFF0) == SC_RESTORE && IsIconic())
		{
			// Restoring, post to main dialogue also.
			AfxGetMainWnd()->ShowWindow(SW_RESTORE);
		}

		CDialog::OnSysCommand(nID, lParam);
	//}
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutoClickerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutoClickerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CAutoClickerDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}


void CAutoClickerDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
	if (m_bStart)
	{
		OnPause();
		return;
	}

	if (m_nListIdx <= 0)
	{
		//MessageBox("No Point.","Check List",MB_OK|MB_ICONERROR);
		return;
	}

	UpdateData(TRUE);

	m_bStart	= TRUE;

	if (m_bPause) 
	{
		m_bPause = FALSE;
	}
	else		  
	{
		m_nCurrentClicks	= 0;
		m_tStart			= CTime::GetCurrentTime();
		m_nCurPos			= m_nListIdx;
	}

	//Move once.
	SetCursorPos(xyList[m_nListIdx-m_nCurPos].x,xyList[m_nListIdx-m_nCurPos].y);	//Move point
	if(m_nMouseBtn==0)
	{
		mouse_event_SendInput(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo()); // Left Key Down
		mouse_event_SendInput(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo()); // Left Key UP
	}
	if(m_nMouseBtn==1)
	{
		mouse_event_SendInput(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, GetMessageExtraInfo()); // Right Key Down
		mouse_event_SendInput(MOUSEEVENTF_RIGHTUP, 0, 0, 0, GetMessageExtraInfo()); // Right Key Up
	}
	SetTimer(1,m_nInterval,NULL);

	m_nCurrentClicks++;

	UpdateData(FALSE);

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_SAVE_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOAD_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_RANDOMIZE_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BOUNDRECT_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLICK_INTERVAL_EDT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_AFTER_CLICKS_EDT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_AFTER_MINS_EDT)->EnableWindow(FALSE);	
}


void CAutoClickerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	//Starting
	m_tEnd				= CTime::GetCurrentTime();
	m_tsSpan			= m_tEnd - m_tStart;

	if(nIDEvent==1)
	{
		UpdateData(TRUE);
		m_nCurPos--;
		if((m_nListIdx-m_nCurPos)==m_nListIdx) m_nCurPos = m_nListIdx;

		SetCursorPos(xyList[m_nListIdx-m_nCurPos].x,xyList[m_nListIdx-m_nCurPos].y);	//Move Position
		if(m_nMouseBtn==0)
		{
			mouse_event_SendInput(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo()); // Left Key Down
			mouse_event_SendInput(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo()); // Left Key Up
		}
		if(m_nMouseBtn==1)
		{
			mouse_event_SendInput(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, GetMessageExtraInfo()); // Right Key Down
			mouse_event_SendInput(MOUSEEVENTF_RIGHTUP, 0, 0, 0, GetMessageExtraInfo()); // Right Key Up
		}
		m_nCurrentClicks++;
		m_strCurrentSpan	= m_tsSpan.Format("%H:%M:%S");

		if ((m_nClicks != 0) && (m_nCurrentClicks > m_nClicks ))
			OnKeyEscape(0,0);
		if ((m_nMinutes != 0) && (m_tsSpan.GetMinutes() >= (int)m_nMinutes) && (m_tsSpan.GetSeconds() >= 1))
			OnKeyEscape(0,0);

		UpdateData(FALSE);
	}
	else if(nIDEvent==2)
	{
		//Keeping clicking automatically
		UpdateData(TRUE);
		if(m_nMouseBtn==0)
		{
			mouse_event_SendInput(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo()); // Left Key Down
			mouse_event_SendInput(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo()); // Left Key Up
		}
		if(m_nMouseBtn==1)
		{
			mouse_event_SendInput(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, GetMessageExtraInfo()); // Right Key Down
			mouse_event_SendInput(MOUSEEVENTF_RIGHTUP, 0, 0, 0, GetMessageExtraInfo()); // Right Key Up
		}
		m_nCurrentClicks++;
		m_strCurrentSpan	= m_tsSpan.Format("%H:%M:%S");

		if ((m_nClicks != 0) && (m_nCurrentClicks > m_nClicks))
			OnKeyEscape(0, 0);
		if ((m_nMinutes != 0) && (m_tsSpan.GetMinutes() >= (int)m_nMinutes) && (m_tsSpan.GetSeconds() >= 1))
			OnKeyEscape(0, 0);

		UpdateData(FALSE);
	}
	

	CDialog::OnTimer(nIDEvent);
}


long CAutoClickerDlg::OnKeyEscape(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);

	if(m_bStart)
	{
		m_bStart = FALSE;
		KillTimer(1);
		KillTimer(2);
		GetDlgItem(IDOK)->EnableWindow(m_nListIdx > 0 ? TRUE : FALSE);
		GetDlgItem(IDC_SAVE_BTN)->EnableWindow(m_nListIdx > 0 ? TRUE : FALSE);
		GetDlgItem(IDC_LOAD_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_RANDOMIZE_BTN)->EnableWindow(ListCanBeRandomized());
		GetDlgItem(IDC_BOUNDRECT_BTN)->EnableWindow(ListCanBeRandomized());
		GetDlgItem(IDC_CLICK_INTERVAL_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP_AFTER_CLICKS_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP_AFTER_MINS_EDT)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);

	return 1;
}


void CAutoClickerDlg::OnPause()
{
	UpdateData(TRUE);

	if(m_bStart)
	{
		m_bStart = FALSE;
		KillTimer(1);
		KillTimer(2);
		//MessageBox("Program has been paused. Click [Start] to restart");
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_SAVE_BTN)->EnableWindow(m_nListIdx > 0 ? TRUE : FALSE);
		GetDlgItem(IDC_LOAD_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_RANDOMIZE_BTN)->EnableWindow(ListCanBeRandomized());
		GetDlgItem(IDC_BOUNDRECT_BTN)->EnableWindow(ListCanBeRandomized());
		GetDlgItem(IDC_CLICK_INTERVAL_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP_AFTER_CLICKS_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP_AFTER_MINS_EDT)->EnableWindow(TRUE);
	}

	m_bPause = TRUE;
	
	UpdateData(FALSE);

	return;
}


long CAutoClickerDlg::OnKeySpace(WPARAM wParam, LPARAM lParam)
{
	OnStart();
	return 1;
}


long CAutoClickerDlg::OnKeyF3(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	CString strList;

	UpdateData(TRUE);

	if (m_nListIdx < 1000)
	{
		GetCursorPos(&point);
		m_nX = point.x;
		m_nY = point.y;
		strList.Format("X=%d, Y=%d", point.x, point.y);
		m_XYList.AddString(strList);
		xyList[m_nListIdx].x = point.x;
		xyList[m_nListIdx].y = point.y;
		m_nListIdx++;
		m_nCount++;
	}

	m_bPause = FALSE;

	UpdateData(FALSE);

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVE_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_RANDOMIZE_BTN)->EnableWindow(ListCanBeRandomized());
	GetDlgItem(IDC_BOUNDRECT_BTN)->EnableWindow(ListCanBeRandomized());

	return 1;
}


long CAutoClickerDlg::OnKeyDelete(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	CPoint point;
	UpdateData(TRUE);

	if (m_nListIdx >= 1)
	{
		--m_nListIdx;
		--m_nCount;
		m_XYList.DeleteString(m_nListIdx);
		point.x = xyList[m_nListIdx - 1].x;
		point.y = xyList[m_nListIdx - 1].y;
		m_nX = point.x;
		m_nY = point.y;
	}

	if (m_nListIdx <= 0)
	{
		m_nListIdx = 0;
		m_nCount = 0;
		m_nX = 0;
		m_nY = 0;
	}

	m_bPause = FALSE;

	UpdateData(FALSE);

	GetDlgItem(IDOK)->EnableWindow(m_nListIdx > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_SAVE_BTN)->EnableWindow(m_nListIdx > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_RANDOMIZE_BTN)->EnableWindow(ListCanBeRandomized());
	GetDlgItem(IDC_BOUNDRECT_BTN)->EnableWindow(ListCanBeRandomized());

	return 1;
}


long CAutoClickerDlg::OnKeyReturnDown(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	UpdateData(FALSE);


	if (m_nMouseBtn == 0)
	{
		mouse_event_SendInput(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo()); // Left Key Down
		mouse_event_SendInput(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo()); // Left Key UP
	}
	if (m_nMouseBtn == 1)
	{
		mouse_event_SendInput(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, GetMessageExtraInfo()); // Right Key Down
		mouse_event_SendInput(MOUSEEVENTF_RIGHTUP, 0, 0, 0, GetMessageExtraInfo()); // Right Key Up
	}

	return 1;
}


long CAutoClickerDlg::OnKeyReturnUp(WPARAM wParam, LPARAM lParam)
{
	return 1;
}


void CAutoClickerDlg::OnFileSave() 
{
	// TODO: Add your control notification handler code here
	CFile	save;
	CFileDialog	pDlg(FALSE,"COL",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"XY File (*.XYF)|*.XYF|");
	pDlg.m_ofn.lpstrTitle = "Save to File";

	if (m_XYList.GetCount() == 0) 
	{
		MessageBox("No List.","Failure",MB_OK|MB_ICONERROR);
		return;
	}

	if(pDlg.DoModal()==IDOK)
	{
		if(save.Open(pDlg.GetPathName(),CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
		{
			save.Write(xyList,sizeof(xyList));
			save.Close();
		}
	}	
}


void CAutoClickerDlg::OnFileLoad() 
{
	// TODO: Add your control notification handler code here
	CFile	saved;
	CString strList;
	CFileDialog	pDlg(TRUE,"COL",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"XY File (*.XYF)|*.XYF|");
	pDlg.m_ofn.lpstrTitle = "Open file";

	UpdateData(TRUE);

	if(pDlg.DoModal()==IDOK)
	{
		if(saved.Open(pDlg.GetPathName(),CFile::modeRead|CFile::modeNoTruncate|CFile::typeBinary))
		{
			//Initialize
			memset(xyList,0,sizeof(xyList));
			saved.Read(xyList,sizeof(xyList));
			saved.Close();

			m_XYList.ResetContent();
			m_nX		= 0;
			m_nY		= 0;
			m_nCount	= 0;
			m_nListIdx	= 0;

			for(int i = 0; i < 1000; i++)
			{
				if (xyList[i].x > 0 && xyList[i].y > 0)
				{
					strList.Format("X=%d, Y=%d", xyList[i].x, xyList[i].y);
					m_XYList.AddString(strList);
					m_nX = xyList[i].x;
					m_nY = xyList[i].y;
					++m_nCount;
					++m_nListIdx;
				}
				else
					break;
			}

			m_bPause = FALSE;

			GetDlgItem(IDOK)->EnableWindow(TRUE);
			GetDlgItem(IDC_SAVE_BTN)->EnableWindow(TRUE);
			GetDlgItem(IDC_RANDOMIZE_BTN)->EnableWindow(ListCanBeRandomized());
			GetDlgItem(IDC_BOUNDRECT_BTN)->EnableWindow(ListCanBeRandomized());
		}
		else MessageBox("Not found.","Failure",MB_OK|MB_ICONERROR);
	}

	UpdateData(FALSE);	
}


void CAutoClickerDlg::OnRunSingleApp()
{
	WCHAR path[MAX_PATH];
	memset(path, 0, MAX_PATH * sizeof(WCHAR));
	DWORD plen = GetModuleFileNameW(NULL, path, MAX_PATH);
	if (plen == 0)
		return;
	size_t i = MAX_PATH;
	--i;
	while (path[i] != L'\\')
		--i;
	path[i] = L'\0';
	BOOL BRes = SetCurrentDirectoryW(path);
	if (!BRes)
		return;

	DWORD dwCreationFlags = 0;
	DWORD waitRes = 0;
	DWORD dwExitCode = 0;

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (CreateProcessW(
		L"TheFastestMouseClicker.exe"   // Module name (TODO: alternate data stream welcome)
		, L""        // Command line TODO: host, port, weak-encrypted-sha256Image
		, NULL           // Process handle not inheritable
		, NULL           // Thread handle not inheritable
		, FALSE          // Set handle inheritance to FALSE
		, dwCreationFlags              // No creation flags
		, NULL           // Use parent's environment block
		, NULL		// Use parent's starting directory 
		, &si            // Pointer to STARTUPINFO structure
		, &pi
		)           // Pointer to PROCESS_INFORMATION structure
		)
		EndDialog(NULL);
	return;
}


#include <random>

void CAutoClickerDlg::OnRandomizeList()
{
	if (!ListCanBeRandomized())
		return;

	Point tclMin(1000000, 1000000);
	Point tclMax(-1, -1);

	for (int i = 0; i < m_nListIdx; ++i)
	{
		const Point& tcl = xyList[i];

		if (tcl.x < tclMin.x)
			tclMin.x = tcl.x;

		if (tcl.x > tclMax.x)
			tclMax.x = tcl.x;

		if (tcl.y < tclMin.y)
			tclMin.y = tcl.y;

		if (tcl.y > tclMax.y)
			tclMax.y = tcl.y;
	}

	UpdateData(TRUE);

	std::random_device rd1;   // non-deterministic generator
	std::mt19937 gen1(rd1());  // to seed mersenne twister
	std::random_device rd2;   // non-deterministic generator
	std::mt19937 gen2(rd2());  // to seed mersenne twister

	m_XYList.ResetContent();
	m_nX = 0;
	m_nY = 0;
	m_nCount = 0;
	m_nListIdx = 0;

	CString strList;

	for (int i = 0; i < 1000; ++i)
	{
		xyList[i].x = tclMin.x + int(gen1() % (tclMax.x + 1 - tclMin.x));
		xyList[i].y = tclMin.y + int(gen2() % (tclMax.y + 1 - tclMin.y));

		strList.Format("X=%d, Y=%d", xyList[i].x, xyList[i].y);
		m_XYList.AddString(strList);
		m_nX = xyList[i].x;
		m_nY = xyList[i].y;

		++m_nListIdx;
		++m_nCount;
	}

	m_bPause = FALSE;

	UpdateData(FALSE);
}


void CAutoClickerDlg::OnBoundingRect()
{
	if (!ListCanBeRandomized())
		return;

	Point tclMin(1000000, 1000000);
	Point tclMax(-1, -1);

	for (int i = 0; i < m_nListIdx; ++i)
	{
		const Point& tcl = xyList[i];

		if (tcl.x < tclMin.x)
			tclMin.x = tcl.x;

		if (tcl.x > tclMax.x)
			tclMax.x = tcl.x;

		if (tcl.y < tclMin.y)
			tclMin.y = tcl.y;

		if (tcl.y > tclMax.y)
			tclMax.y = tcl.y;
	}

	UpdateData(TRUE);

	m_XYList.ResetContent();
	m_nX = 0;
	m_nY = 0;
	m_nCount = 0;
	m_nListIdx = 0;

	CString strList;

	for (int i = 0; i < 2; ++i)
	{
		xyList[i].x = (i == 0) ? tclMin.x : tclMax.x;
		xyList[i].y = (i == 0) ? tclMin.y : tclMax.y;

		strList.Format("X=%d, Y=%d", xyList[i].x, xyList[i].y);
		m_XYList.AddString(strList);
		m_nX = xyList[i].x;
		m_nY = xyList[i].y;

		++m_nListIdx;
		++m_nCount;
	}

	m_bPause = FALSE;

	UpdateData(FALSE);
}


BOOL CAutoClickerDlg::ListCanBeRandomized()
{
	if (m_nListIdx < 2)
		return false;

	for (int i = 0; i < m_nListIdx - 1; ++i)
	{
		if (!xyList[i].isEqualTo(xyList[i + 1]))
			return true;
	}

	return false;
}


void CAutoClickerDlg::OnWinAlwaysTopChk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_bAlwaysTop==TRUE)	
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	else 
		SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	UpdateData(FALSE);
}
