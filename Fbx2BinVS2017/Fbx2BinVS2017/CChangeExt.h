#pragma once


// CChangeExt �_�C�A���O

class CChangeExt : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeExt)

public:
	CChangeExt(CWnd* pParent = nullptr);   // �W���R���X�g���N�^�[
	virtual ~CChangeExt();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHANGE_EXT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CString m_NewExt;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
