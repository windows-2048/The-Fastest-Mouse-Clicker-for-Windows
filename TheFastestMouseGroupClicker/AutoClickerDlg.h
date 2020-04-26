// AutoClickerDlg.h : header file
//

#if !defined(AFX_AUTOCLICKERDLG_H__500A9D39_8C14_4FA9_871F_97859F2D6659__INCLUDED_)
#define AFX_AUTOCLICKERDLG_H__500A9D39_8C14_4FA9_871F_97859F2D6659__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CAutoClickerDlg dialog

class CAutoClickerDlg : public CDialog
{
// Construction
public:
	CAutoClickerDlg(CWnd* pParent = NULL);	// standard constructor
	long OnKeyEscape(WPARAM wParam, LPARAM lParam);
	void OnPause();
	long OnKeySpace(WPARAM wParam, LPARAM lParam);
	long OnKeyF3(WPARAM wParam, LPARAM lParam);
	long OnKeyDelete(WPARAM wParam, LPARAM lParam);
	long OnKeyReturnDown(WPARAM wParam, LPARAM lParam);
	long OnKeyReturnUp(WPARAM wParam, LPARAM lParam);

	struct Point
	{
		int x;
		int y;
		Point() : x(), y() {}
		Point(int ix, int iy) : x(ix), y(iy) {}
		bool isEqualTo(const Point& that)
		{
			return (x == that.x) && (y == that.y);
		}
	} xyList[1000];

private:
// Dialog Data
	//{{AFX_DATA(CAutoClickerDlg)
	enum { IDD = IDD_AUTOCLICKER_DIALOG };
	CListBox	m_XYList;
	BOOL	m_bAlwaysTop;
	UINT	m_nX;
	UINT	m_nY;
	UINT	m_nInterval;
	UINT	m_nClicks;
	UINT	m_nMinutes;
	int		m_nMouseBtn;
	UINT	m_nCount;
	UINT	m_nCurrentClicks;
	CString	m_strCurrentSpan;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoClickerDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	HICON	m_hIcon;
	CString m_strProgName;
	int		m_nListIdx;
	int		m_nCurPos;
	BOOL	m_bStart;
	BOOL	m_bPause;
	CTime   m_tStart;
	CTime   m_tEnd;
	CTimeSpan m_tsSpan;

	// Generated message map functions
	//{{AFX_MSG(CAutoClickerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCancelMode();
	afx_msg void OnStart();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFileSave();
	afx_msg void OnFileLoad();
	afx_msg void OnRunSingleApp();
	afx_msg void OnRandomizeList();
	afx_msg void OnBoundingRect();
	afx_msg void OnWinAlwaysTopChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL ListCanBeRandomized();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCLICKERDLG_H__500A9D39_8C14_4FA9_871F_97859F2D6659__INCLUDED_)
