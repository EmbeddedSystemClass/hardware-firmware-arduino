
// LoggerDlg.h : header file
//

#pragma once

#include "Serial.h"


// CLoggerDlg dialog
class CLoggerDlg : public CDialog
{
// Construction
public:
	CLoggerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LOGGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetdatetimeButton();

	CSerial serial;
};
