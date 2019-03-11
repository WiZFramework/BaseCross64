#pragma once


// CFbxOpenDialog �_�C�A���O

class CFbxOpenDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFbxOpenDialog)

public:
	CFbxOpenDialog(CWnd* pParent = nullptr);   // �W���R���X�g���N�^�[
	virtual ~CFbxOpenDialog();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE_SELECT };
#endif

	CString m_FullFileDir;
	CString m_FileName;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFiledialogOpen();
	afx_msg void OnBnClickedButtonFiledialogNormalmapOpen();
	CString m_FullFileName;
	CString m_NormalmapFullFileName;
	CString m_Scale;
	CString m_PosX;
	CString m_PosY;
	CString m_PosZ;
	BOOL m_TextureWrap;
	BOOL m_IsReadStatic;
	BOOL m_WithTangent;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
