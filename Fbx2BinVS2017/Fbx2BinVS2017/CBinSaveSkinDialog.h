#pragma once


// CBinSaveSkinDialog �_�C�A���O

class CBinSaveSkinDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBinSaveSkinDialog)

public:
	CBinSaveSkinDialog(CWnd* pParent = nullptr);   // �W���R���X�g���N�^�[
	virtual ~CBinSaveSkinDialog();
	CString m_FileDir;
	CString m_FileNameNoExt;
	CString m_FileExt;

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SAVE_BINFILE_SKIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonUpdateExtSkin();
	afx_msg void OnBnClickedOk();
	CString m_FullBinFileName;
	CString m_Scale;
	BOOL m_IsNowScale;
	CString m_FrameRate;
	CString m_AnimeStartSec;
	CString m_AnimeEndSec;
};
