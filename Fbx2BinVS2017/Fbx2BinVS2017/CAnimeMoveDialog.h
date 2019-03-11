#pragma once


// CAnimeMoveDialog �_�C�A���O

class CAnimeMoveDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimeMoveDialog)

public:
	CAnimeMoveDialog(CWnd* pParent = nullptr);   // �W���R���X�g���N�^�[
	virtual ~CAnimeMoveDialog();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANIME_MOVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_FrameRate;
	CString m_AnimeStartSec;
	CString m_AnimeEndSec;
	BOOL m_IsLooped;
	virtual BOOL OnInitDialog();
};
