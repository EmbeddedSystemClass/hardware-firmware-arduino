
// LoggerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Logger.h"
#include "LoggerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoggerDlg dialog


CLoggerDlg::CLoggerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoggerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoggerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLoggerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SETDATETIME_BUTTON, &CLoggerDlg::OnBnClickedSetdatetimeButton)
END_MESSAGE_MAP()


// CLoggerDlg message handlers

BOOL CLoggerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	serial.Open(11, 9600);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoggerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoggerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLoggerDlg::OnBnClickedSetdatetimeButton()
{
				

	char time[] = { /*Signature*/ 0xCC, 0x33, 0x55, 0xAA, /*time*/ 0, /*hour*/ 0, /*minute*/ 0, /*second*/ 0,         0, 0 };
	char date[] = { /*Signature*/ 0xCC, 0x33, 0x55, 0xAA, /*date*/ 1, /*year*/ 19, /*year*/ 70, /*month*/  0, /*day*/ 0, 0 };

	SYSTEMTIME systime;
	GetLocalTime(&systime);

	time[5] = (char)systime.wHour;
	time[6] = (char)systime.wMinute;
	time[7] = (char)systime.wSecond;

	//serial.Open(6, 9600);	
	serial.SendData(time, 10, 1);
	serial.SendData(date, 10, 1);
	//serial.Close();
}
