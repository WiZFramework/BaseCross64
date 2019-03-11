
// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "Fbx2BinVS2017.h"
#include "CFbxOpenDialog.h"
#include "CAnimeMoveDialog.h"
#include "CBinSaveDialog.h"
#include "CBinSaveSkinDialog.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Project.h"
using namespace basecross;


#include "MainFrm.h"
#include "Project.h"
using namespace basecross;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_32771, &CMainFrame::OnNewStage)
	ON_COMMAND(ID_32772, &CMainFrame::OnOpenFbxFile)
	ON_COMMAND(ID_32773, &CMainFrame::OnSaveBin)
	ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnMoveFbxFile)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMainFrame::OnUpdateMoveFbxFile)
	ON_UPDATE_COMMAND_UI(ID_32773, &CMainFrame::OnUpdateSaveBin)
END_MESSAGE_MAP()

// CMainFrame �R���X�g���N�V����/�f�X�g���N�V����

CMainFrame::CMainFrame() noexcept
{
	//FBX���b�V���ǂݍ��ݎ��̏����l
	m_MeshScale = L"1.0";
	m_MeshPosX = L"0.0";
	m_MeshPosY = L"0.0";
	m_MeshPosZ = L"0.0";
	//���b�V���̃t���p�X�f�B���N�g���i�Ō��/�͂��Ȃ��j
	m_MeshDir = L"";
	//���b�V���̃t�@�C����
	m_MeshFilename = L"";
	//�X�^�e�B�b�N�Ƃ��ēǂݍ���
	m_IsReadStatic = FALSE;
	//�^���W�F���g�͓ǂݍ��܂Ȃ�
	m_WithTangent = FALSE;
	m_IsNowScale = TRUE;

	m_TextureWrap = FALSE;

	//�ȉ��A�j���[�V�����֌W
	//�t���[�����[�g
	m_FrameRate = 10;
	m_StartSec = 0;
	m_EndSec = 1;
	m_Looped = TRUE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// �t���[���̃N���C�A���g�̈�S�̂��߂�r���[���쐬���܂��B
	if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("�r���[ �E�B���h�E���쐬�ł��܂���ł����B\n");
		return -1;
	}
	//�A�v���P�[�V�����N���X�ƃf�o�C�X�V�[���̍쐬
	try {
		((CFbx2BinVS2017App*)AfxGetApp())->CreateAppScene(m_wndView.GetSafeHwnd());
	}
	catch (...) {
		TRACE0("�V�[���̍쐬�Ɏ��s���܂����B\n");
		return -1;
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame �̐f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ���b�Z�[�W �n���h���[

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// �r���[ �E�B���h�E�Ƀt�H�[�J�X��^���܂��B
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// �r���[�ɍŏ��ɃR�}���h����������@���^���܂��B
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// ����ȊO�̏ꍇ�́A����̏������s���܂��B
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnNewStage()
{
	if (App::GetApp()->GetScene<Scene>()->CheckMesh()) {
		if (AfxMessageBox(L"���݂̃X�e�[�W�͍폜���Ă�낵���ł����H", MB_OKCANCEL) != IDOK) {
			return;
		}
	}
	App::GetApp()->GetScene<Scene>()->ResetNewStage();
}


void CMainFrame::OnOpenFbxFile()
{
	CFbxOpenDialog FbxOpenDialog;
	FbxOpenDialog.m_FullFileDir = m_MeshDir.c_str();
	FbxOpenDialog.m_FileName = m_MeshFilename.c_str();
	if (m_MeshDir != L"" && m_MeshFilename != L"") {
		FbxOpenDialog.m_FullFileName = FbxOpenDialog.m_FullFileDir;
		FbxOpenDialog.m_FullFileName += L"\\";
		FbxOpenDialog.m_FullFileName += FbxOpenDialog.m_FileName;
	}
	else {
		FbxOpenDialog.m_FullFileName = L"";
	}
	FbxOpenDialog.m_Scale = m_MeshScale;
	FbxOpenDialog.m_PosX = m_MeshPosX;
	FbxOpenDialog.m_PosY = m_MeshPosY;
	FbxOpenDialog.m_PosZ = m_MeshPosZ;
	FbxOpenDialog.m_IsReadStatic = m_IsReadStatic;

	if (FbxOpenDialog.DoModal() == IDOK) {
		wchar_t RelativePath[MAX_PATH];
		m_MeshDir = FbxOpenDialog.m_FullFileDir;
		m_MeshFilename = FbxOpenDialog.m_FileName;
		m_MeshScale = FbxOpenDialog.m_Scale;
		m_MeshPosX = FbxOpenDialog.m_PosX;
		m_MeshPosY = FbxOpenDialog.m_PosY;
		m_MeshPosZ = FbxOpenDialog.m_PosZ;
		m_IsReadStatic = FbxOpenDialog.m_IsReadStatic;
		m_WithTangent = FbxOpenDialog.m_WithTangent;
		m_NormalmapFullFileName = FbxOpenDialog.m_NormalmapFullFileName;
		m_TextureWrap = FbxOpenDialog.m_TextureWrap;

		wstring ModDIr;
		App::GetApp()->GetModuleDirectory(ModDIr);

		if (PathRelativePathTo(
			RelativePath,
			ModDIr.c_str(),
			FILE_ATTRIBUTE_DIRECTORY,
			m_MeshDir.c_str(),
			FILE_ATTRIBUTE_DIRECTORY
		)) {
			//�t�@�C���̃I�[�v��
			wstring Dir = RelativePath;
			Dir += L"\\";
			float Scale = (float)_wtof(m_MeshScale);
			Vec3 Pos(
				(float)_wtof(m_MeshPosX),
				(float)_wtof(m_MeshPosY),
				(float)_wtof(m_MeshPosZ)
			);
			try {
				App::GetApp()->GetScene<Scene>()->ReadFbxFile(Dir, m_MeshFilename, 0, Scale, Pos, m_IsReadStatic == TRUE,
					m_WithTangent == TRUE, m_NormalmapFullFileName, m_TextureWrap == TRUE);
			}
			catch (...) {
				AfxMessageBox(L"FBX�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B");
			}
		}
	}
}


void CMainFrame::OnSaveBin()
{
	if (App::GetApp()->GetScene<Scene>()->CheckMesh()) {
		wchar_t Drivebuff[_MAX_DRIVE];
		wchar_t Dirbuff[_MAX_DIR];
		wchar_t FileNamebuff[_MAX_FNAME];
		wchar_t Extbuff[_MAX_EXT];

		::ZeroMemory(Drivebuff, sizeof(Drivebuff));
		::ZeroMemory(Dirbuff, sizeof(Dirbuff));
		::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
		::ZeroMemory(Extbuff, sizeof(Extbuff));

		wstring FullPath = m_MeshDir;
		FullPath += L"\\";
		FullPath += m_MeshFilename;

		//�t���p�X����A�e�u���b�N�ɕ�����
		_wsplitpath_s(FullPath.c_str(),
			Drivebuff, _MAX_DRIVE,
			Dirbuff, _MAX_DIR,
			FileNamebuff, _MAX_FNAME,
			Extbuff, _MAX_EXT);

		if (App::GetApp()->GetScene<Scene>()->CheckSkinMesh()) {
			CBinSaveSkinDialog BinSaveSkinDialog;
			BinSaveSkinDialog.m_FileDir = Drivebuff;
			BinSaveSkinDialog.m_FileDir += Dirbuff;
			BinSaveSkinDialog.m_FileNameNoExt = FileNamebuff;
			BinSaveSkinDialog.m_FileExt = L".bmf";

			BinSaveSkinDialog.m_FullBinFileName = BinSaveSkinDialog.m_FileDir;
			BinSaveSkinDialog.m_FullBinFileName += BinSaveSkinDialog.m_FileNameNoExt;
			BinSaveSkinDialog.m_FullBinFileName += BinSaveSkinDialog.m_FileExt;

			BinSaveSkinDialog.m_FrameRate.Format(L"%d", m_FrameRate);
			BinSaveSkinDialog.m_AnimeStartSec.Format(L"%d", m_StartSec);
			BinSaveSkinDialog.m_AnimeEndSec.Format(L"%d", m_EndSec);


			BinSaveSkinDialog.m_Scale = m_MeshScale;
			BinSaveSkinDialog.m_IsNowScale = m_IsNowScale;

			if (BinSaveSkinDialog.DoModal() == IDOK) {
				wstring SaveFileDir = BinSaveSkinDialog.m_FileDir;
				wstring SaveFileName = BinSaveSkinDialog.m_FileNameNoExt;
				SaveFileName += BinSaveSkinDialog.m_FileExt;
				m_FrameRate = (UINT)_wtoi(BinSaveSkinDialog.m_FrameRate);
				m_StartSec = (UINT)_wtoi(BinSaveSkinDialog.m_AnimeStartSec);
				m_EndSec = (UINT)_wtoi(BinSaveSkinDialog.m_AnimeEndSec);
				float Scale = (float)_wtof(m_MeshScale);
				if (!BinSaveSkinDialog.m_IsNowScale) {
					Scale = 1.0f;
				}
				try {
					App::GetApp()->GetScene<Scene>()->SaveSkinBinFile(SaveFileDir, SaveFileName, 0, Scale,
						m_FrameRate, m_StartSec, m_EndSec);
					AfxMessageBox(L"�X�L���o�C�i���t�@�C����ۑ����܂����B");
				}
				catch (...) {
					AfxMessageBox(L"�X�L���o�C�i���t�@�C���̕ۑ��Ɏ��s���܂����B");
				}
			}


		}
		else {

			CBinSaveDialog BinSaveDialog;
			BinSaveDialog.m_FileDir = Drivebuff;
			BinSaveDialog.m_FileDir += Dirbuff;
			BinSaveDialog.m_FileNameNoExt = FileNamebuff;
			BinSaveDialog.m_FileExt = L".bmf";

			BinSaveDialog.m_FullBinFileName = BinSaveDialog.m_FileDir;
			BinSaveDialog.m_FullBinFileName += BinSaveDialog.m_FileNameNoExt;
			BinSaveDialog.m_FullBinFileName += BinSaveDialog.m_FileExt;

			BinSaveDialog.m_Scale = m_MeshScale;
			BinSaveDialog.m_IsNowScale = m_IsNowScale;

			if (BinSaveDialog.DoModal() == IDOK) {
				wstring SaveFileDir = BinSaveDialog.m_FileDir;
				wstring SaveFileName = BinSaveDialog.m_FileNameNoExt;
				SaveFileName += BinSaveDialog.m_FileExt;
				float Scale = (float)_wtof(m_MeshScale);
				if (!BinSaveDialog.m_IsNowScale) {
					Scale = 1.0f;
				}
				try {
					App::GetApp()->GetScene<Scene>()->SaveStaticBinFile(SaveFileDir, SaveFileName, 0, Scale);
					AfxMessageBox(L"�X�^�e�B�b�N�o�C�i���t�@�C����ۑ����܂����B");
				}
				catch (...) {
					AfxMessageBox(L"�X�^�e�B�b�N�o�C�i���t�@�C���̕ۑ��Ɏ��s���܂����B");
				}
			}
		}
	}
	else {
		AfxMessageBox(L"���b�V�����ǂ܂�Ă���B");
	}

}


void CMainFrame::OnMoveFbxFile()
{
	if (!App::GetApp()->GetScene<Scene>()->CheckSkinMesh()) {
		AfxMessageBox(L"���b�V�����ǂ܂�ĂȂ����A�X�L�����b�V���ł͂���܂���B");
	}
	else {
		CAnimeMoveDialog AnimeMoveDialog;
		AnimeMoveDialog.m_FrameRate.Format(L"%d", m_FrameRate);
		AnimeMoveDialog.m_AnimeStartSec.Format(L"%d", m_StartSec);
		AnimeMoveDialog.m_AnimeEndSec.Format(L"%d", m_EndSec);
		AnimeMoveDialog.m_IsLooped = m_Looped;
		if (AnimeMoveDialog.DoModal() == IDOK) {
			m_FrameRate = (UINT)_wtoi(AnimeMoveDialog.m_FrameRate);
			m_StartSec = (UINT)_wtoi(AnimeMoveDialog.m_AnimeStartSec);
			m_EndSec = (UINT)_wtoi(AnimeMoveDialog.m_AnimeEndSec);
			m_Looped = (AnimeMoveDialog.m_IsLooped == TRUE);
			App::GetApp()->GetScene<Scene>()->MoveFbxFile(m_FrameRate, m_StartSec, m_EndSec, m_Looped == TRUE);
		}
	}
}


void CMainFrame::OnUpdateMoveFbxFile(CCmdUI *pCmdUI)
{
	if (!App::GetApp()->GetScene<Scene>()->CheckSkinMesh()) {
		pCmdUI->Enable(FALSE);
	}
	else {
		pCmdUI->Enable(TRUE);
	}
}


void CMainFrame::OnUpdateSaveBin(CCmdUI *pCmdUI)
{
	if (!App::GetApp()->GetScene<Scene>()->CheckMesh()) {
		pCmdUI->Enable(FALSE);
	}
	else {
		pCmdUI->Enable(TRUE);
	}
}
