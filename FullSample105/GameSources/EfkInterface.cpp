/*!
@file EfkInterface.cpp
@brief Effekseer�G�t�F�N�g�̃C���^�[�t�F�C�X����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	Effekseer�G�t�F�N�g�̃G�t�F�N�g
	//--------------------------------------------------------------------------------------
	EfkEffect::EfkEffect(const shared_ptr<EfkInterface>& iface, const wstring& filename):
		m_FileName(filename),
		m_EfkInterface(iface),
		m_Effect(nullptr)
	{
		try {
			if (m_FileName == L"") {
				throw BaseException(
					L"�G�t�F�N�g�t�@�C�������󔒂ł��B",
					L"if (m_FileName == L\"\")",
					L"EfkEffect::EfkEffect()"
				);
			}
			// �G�t�F�N�g�̓Ǎ�
			m_Effect = Effekseer::Effect::Create(iface->m_Manager, (const EFK_CHAR*)m_FileName.c_str());
		}
		catch (...) {
			throw;
		}
	}
	EfkEffect::~EfkEffect() {
		ES_SAFE_RELEASE(m_Effect);
	}

	void EfkEffect::OnCreate() {
	}

	//--------------------------------------------------------------------------------------
	///	Effekseer�G�t�F�N�g��Play�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	EfkPlay::EfkPlay(const shared_ptr<EfkEffect>& effect, const bsm::Vec3& Emitter):
		m_Handle(-1)
	{
		try {
			auto iface = effect->m_EfkInterface.lock();
			if (iface) {
				m_Handle = iface->m_Manager->Play(effect->m_Effect, Emitter.x, Emitter.y, Emitter.z);
				m_EfkInterface = iface;
			}
		}
		catch (...) {
			throw;
		}
	}

	EfkPlay::~EfkPlay() {
		StopEffect();
	}

	void EfkPlay::AddLocation(const bsm::Vec3& Location) {
		auto shptr = m_EfkInterface.lock();
		if (shptr && m_Handle != -1) {
			shptr->m_Manager->AddLocation(m_Handle, ::Effekseer::Vector3D(Location.x, Location.y, Location.z));
		}
	}

	void EfkPlay::StopEffect() {
		auto shptr = m_EfkInterface.lock();
		if (shptr && m_Handle != -1) {
			shptr->m_Manager->StopEffect(m_Handle);
		}
	}

	//--------------------------------------------------------------------------------------
	///	Effekseer�G�t�F�N�g�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	EfkInterface::EfkInterface():
		ObjectInterface(),
		m_Manager(nullptr),
		m_Renderer(nullptr)
	{}
	EfkInterface::~EfkInterface() {
		// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
		m_Manager->Destroy();
		// ���ɕ`��p�C���X�^���X��j��
		m_Renderer->Destroy();
	}

	void EfkInterface::OnCreate() {
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		// �`��p�C���X�^���X�̐���
		m_Renderer = ::EffekseerRendererDX11::Renderer::Create(pDx11Device, pID3D11DeviceContext, 2000);
		// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
		m_Manager = ::Effekseer::Manager::Create(2000);

		// �`��p�C���X�^���X����`��@�\��ݒ�
		m_Manager->SetSpriteRenderer(m_Renderer->CreateSpriteRenderer());
		m_Manager->SetRibbonRenderer(m_Renderer->CreateRibbonRenderer());
		m_Manager->SetRingRenderer(m_Renderer->CreateRingRenderer());
		m_Manager->SetTrackRenderer(m_Renderer->CreateTrackRenderer());
		m_Manager->SetModelRenderer(m_Renderer->CreateModelRenderer());

		// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
		m_Manager->SetTextureLoader(m_Renderer->CreateTextureLoader());
		m_Manager->SetModelLoader(m_Renderer->CreateModelLoader());
	}

	void  EfkInterface::OnUpdate() {
		// �G�t�F�N�g�̍X�V�������s��
		m_Manager->Update();
	}

	void EfkInterface::OnDraw() {
		// �G�t�F�N�g�̕`��J�n�������s���B
		m_Renderer->BeginRendering();

		// �G�t�F�N�g�̕`����s���B
		m_Manager->Draw();

		// �G�t�F�N�g�̕`��I���������s���B
		m_Renderer->EndRendering();

	}

	void Mat4x4ToMatrix44(const bsm::Mat4x4& src, Effekseer::Matrix44& dest) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				dest.Values[i][j] = src(i, j);
			}
		}
	}

	void  EfkInterface::SetViewProj(const bsm::Mat4x4& view, const bsm::Mat4x4& proj) {
		Effekseer::Matrix44 v,p;
		Mat4x4ToMatrix44(view, v);
		Mat4x4ToMatrix44(proj, p);
		m_Renderer->SetCameraMatrix(v);
		m_Renderer->SetProjectionMatrix(p);
	}







}
// end basecross
