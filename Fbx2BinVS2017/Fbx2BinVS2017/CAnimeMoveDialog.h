#pragma once


// CAnimeMoveDialog ダイアログ

class CAnimeMoveDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimeMoveDialog)

public:
	CAnimeMoveDialog(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CAnimeMoveDialog();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANIME_MOVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_FrameRate;
	CString m_AnimeStartSec;
	CString m_AnimeEndSec;
	BOOL m_IsLooped;
	virtual BOOL OnInitDialog();
};
