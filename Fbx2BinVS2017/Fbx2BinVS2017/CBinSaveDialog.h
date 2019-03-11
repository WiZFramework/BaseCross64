#pragma once


// CBinSaveDialog ダイアログ

class CBinSaveDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBinSaveDialog)

public:
	CBinSaveDialog(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CBinSaveDialog();
	CString m_FileDir;
	CString m_FileNameNoExt;
	CString m_FileExt;


// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SAVE_BINFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonUpdateExt();
	virtual BOOL OnInitDialog();
	BOOL m_IsNowScale;
	CString m_Scale;
	CString m_FullBinFileName;
};
