/*!
@file App.h
@brief �A�v���P�[�V�����N���X�B���͋@�퓙
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	class DeviceResources;
	class TextureResource;
	class AudioResource;

	//--------------------------------------------------------------------------------------
	///	�}���`�T�E���h�Ŏg�p����A�C�e��
	//--------------------------------------------------------------------------------------
	struct SoundItem {
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_SourceVoice;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		SoundItem() :
			m_SourceVoice(nullptr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		~SoundItem() {
		}
	};

	//--------------------------------------------------------------------------------------
	/// XAudio2�}�l�[�W���N���X
	//--------------------------------------------------------------------------------------
	class XAudio2Manager : public ObjectInterface ,public ShapeInterface {
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		XAudio2Manager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~XAudio2Manager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �I�[�f�B�I���L�����ǂ���
		@return	�L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsAudioActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief XAudio2�C���^�[�t�F�C�X�𓾂�
		@return	XAudio2�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IXAudio2> GetXAudio2() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �}�X�^�����O�{�C�X�𓾂�
		@return	�}�X�^�����O�{�C�X
		*/
		//--------------------------------------------------------------------------------------
		IXAudio2MasteringVoice* GetMasteringVoice() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �T�E���h�A�C�e���̔z��𓾂�
		@return	�T�E���h�A�C�e���̔z��̎Q��
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<SoundItem>>& GetSoundItemVec();
		const vector<shared_ptr<SoundItem>>& GetSoundItemVec() const;
		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Đ��J�n�B
		@param[in]	ResKey	���\�[�X�L�[
		@param[in]	LoopCount = 0	���[�v�񐔁iXAUDIO2_LOOP_INFINITE�Ŗ������[�v�j
		@param[in]	Volume = 1.0f	�{�����[��
		@return	SoundItem�̃|�C���^�B���s��nullptr
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<SoundItem> Start(const wstring& ResKey, size_t LoopCount = 0, float Volume = 1.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�T�E���h�̋����I��
		@param[in]	Item	�A�C�e��
		@return	�Ȃ��i�w��̃T�E���h���܂��Đ����Ȃ�I��������j
		*/
		//--------------------------------------------------------------------------------------
		void Stop(const shared_ptr<SoundItem>& Item);
		//--------------------------------------------------------------------------------------
		/*!
		@brief OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate();
		//--------------------------------------------------------------------------------------
		/*!
		@brief OnUpdate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw() {}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	�I�[�f�B�I���\�[�X�iwav�j�N���X
	//--------------------------------------------------------------------------------------
	class AudioResource : public BaseResource {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^<br />
		�E�F�u�t�@�C������n��
		@param[in]	FileName	�t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		AudioResource(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AudioResource();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�T�E���h�f�[�^�̎擾
		@return	���̃E�F�u���\�[�X�̃T�E���h�f�[�^
		*/
		//--------------------------------------------------------------------------------------
		const vector<byte>& GetSoundData()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�E�F�u�t�H�[�}�b�g�̎擾
		@return	���̃E�F�u���\�[�X��WAVEFORMATEX�E�F�u�t�H�[�}�b�g�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		WAVEFORMATEX*  GetOutputWaveFormatEx() const;

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	�o�b�N�A�b�v�f�[�^�i�e�j
	//--------------------------------------------------------------------------------------
	struct BackupDataBase {
		BackupDataBase() {}
		virtual ~BackupDataBase() {}

	};
	//--------------------------------------------------------------------------------------
	///	�o�b�N�A�b�v�f�[�^
	//--------------------------------------------------------------------------------------
	template<typename T>
	struct BackupData : public BackupDataBase {
		vector<T> m_Vertices;
		vector<uint16_t> m_Indices;
	};

	//--------------------------------------------------------------------------------------
	///	VertexPositionNormalTexture�ǂݍ��ݗp�\����
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTexturePOD {
		float position[3];	///< �ʒu���
		float normal[3];	///< �@��
		float textureCoordinate[2];	///< �e�N�X�`��UV
	};

	//--------------------------------------------------------------------------------------
	///	VertexPositionNormalTangentTexture�ǂݍ��ݗp�\����
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTangentTexturePOD {
		float position[3];	///< �ʒu���
		float normal[3];	///< �@��
		float tangent[4];	///< �^���W�F���g
		float textureCoordinate[2];	///< �e�N�X�`��UV
	};

	//--------------------------------------------------------------------------------------
	///	VertexPositionNormalTextureSkinning�ǂݍ��ݗp�\����
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTextureSkinningPOD {
		float position[3];	///< �ʒu���
		float normal[3];	///< �@��
		float textureCoordinate[2];	///< �e�N�X�`��UV
		uint32_t indices[4];	///< �{�[���̃C���f�b�N�X
		float weights[4];	///< �e�{�[���̃E�G�C�g
	};

	//--------------------------------------------------------------------------------------
	///	VertexPositionNormalTangentTextureSkinning�ǂݍ��ݗp�\����
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTangentTextureSkinningPOD {
		float position[3];	///< �ʒu���
		float normal[3];	///< �@��
		float tangent[4];	///< �^���W�F���g
		float textureCoordinate[2];	///< �e�N�X�`��UV
		uint32_t indices[4];	///< �{�[���̃C���f�b�N�X
		float weights[4];	///< �e�{�[���̃E�G�C�g
	};


	//--------------------------------------------------------------------------------------
	///	�}�e���A���\����
	//--------------------------------------------------------------------------------------
	struct MaterialEx {
		///�J�n�C���f�b�N�X
		UINT m_StartIndex;
		///�`��C���f�b�N�X�J�E���g
		UINT m_IndexCount;
		/// �f�t�B�[�Y�i���̂̐F�j
		bsm::Col4 m_Diffuse;
		/// �X�y�L�����[�i���ˌ��j
		bsm::Col4 m_Specular;
		/// �A���r�G���g�i���F�j
		bsm::Col4 m_Ambient;
		/// �G�~�b�V�u�i���ˌ��j
		bsm::Col4 m_Emissive;
		/// �V�F�[�_���\�[�X�r���[�i�e�N�X�`�����\�[�X�j
		shared_ptr<TextureResource> m_TextureResource;
	};

	//--------------------------------------------------------------------------------------
	///	�}�e���A���ǂݍ��ݗp�\����
	//--------------------------------------------------------------------------------------
	struct MaterialExPOD {
		///�J�n�C���f�b�N�X
		UINT m_StartIndex;
		///�`��C���f�b�N�X�J�E���g
		UINT m_IndexCount;
		/// �f�t�B�[�Y�i���̂̐F�j
		float m_Diffuse[4];
		/// �X�y�L�����[�i���ˌ��j
		float m_Specular[4];
		/// �A���r�G���g�i���F�j
		float m_Ambient[4];
		/// �G�~�b�V�u�i���ˌ��j
		float m_Emissive[4];
	};

	//--------------------------------------------------------------------------------------
	///	�s��ǂݍ��ݗp�\����
	//--------------------------------------------------------------------------------------
	struct	MatrixPOD
	{
		float	m_Mat[4][4];	///>�s��
	};



	//--------------------------------------------------------------------------------------
	///	�u���b�N�^�C�v��`�i���f�����b�V���ǂݍ��ݗp�j
	//--------------------------------------------------------------------------------------
	enum class BlockType {
		Vertex,	///< ���_
		Index,	///< �C���f�b�N�X
		Material,	///< �}�e���A��
		MaterialCount,	///< �}�e���A����
		SkinedVertex,	///< �X�L�����_
		BoneCount,	///< �{�[����
		AnimeMatrix,	///< �A�j���[�V�����s��
		VertexWithTangent,	///< �^���W�F���g�t�����_
		SkinedVertexWithTangent,	///< �X�L���^���W�F���g�t�����_
		MashCount,				///< ���b�V�����i�}���`���b�V���p�j
		End = 100	///< �I��
	};

	//--------------------------------------------------------------------------------------
	///	�u���b�N�w�b�_�\����
	//--------------------------------------------------------------------------------------
	struct BlockHeader {
		BlockType m_Type;	///< �u���b�N�^�C�v
		UINT m_Size;	///< �o�C�g��
	};


	//--------------------------------------------------------------------------------------
	/// XBox�R���g���[���[�̃X�e�[�^�X�BXINPUT_STATE�h��
	//--------------------------------------------------------------------------------------
	struct CONTROLER_STATE : public XINPUT_STATE
	{

		WORD wButtons;		///< �{�^�����
		BYTE bLeftTrigger;	///< ���g���K
		BYTE bRightTrigger;	///< �E�g���K

		float fThumbRX;	///< �E�X�e�B�b�NX��flort�ϊ�
		float fThumbRY;	///< �E�X�e�B�b�NY��flort�ϊ�
		float fThumbLX;	///< ���X�e�B�b�NX��flort�ϊ�
		float fThumbLY;	///< ���X�e�B�b�NY��flort�ϊ�
		WORD wNowUpdateButtons;	///< ��Ԃ��ς�����Ƃ��Ƀt���O�����{�^��
		WORD wPressedButtons;	///< �����ꂽ�u�Ԃ���������{�^��
		WORD wReleasedButtons;	///< �����ꂽ�u�Ԃ���������{�^��
		WORD wLastButtons;	///< ��O�̃{�^��
		bool bConnected;	///< �ڑ����Ă��邩�ǂ���
	};
	//--------------------------------------------------------------------------------------
	///�L�[�{�[�h�̃X�e�[�^�X�B�}�E�X�̃X�e�[�^�X���擾����
	//--------------------------------------------------------------------------------------
	struct KEYBOARD_STATE {
		static const DWORD MAX_KEYVCODE = 256; ///< �L�[�����
		bool m_bPushKeyTbl[MAX_KEYVCODE];	///< ���݂̃L�[�{�[�h�̏��
		bool m_bLastKeyTbl[MAX_KEYVCODE];	///< ��O�̃L�[�{�[�h�̏��
		bool m_bPressedKeyTbl[MAX_KEYVCODE];	///< �����ꂽ�u�Ԃ̃L�[�{�[�h
		bool m_bUpKeyTbl[MAX_KEYVCODE];		///< �����ꂽ�u�Ԃ̃L�[�{�[�h
		bool m_KeyMessageActive;	///<�����̃L�[�C�x���g������
		Point2D<int> m_MouseClientPoint; ///< �N���C�A���g��̃}�E�X�|�C���g
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		KEYBOARD_STATE() :
			m_KeyMessageActive{ false },
			m_MouseClientPoint{ 0, 0 }
		{
			//�L�[�{�[�h�e�[�u���̏�����
			::ZeroMemory(&m_bLastKeyTbl, sizeof(m_bLastKeyTbl));
			::ZeroMemory(&m_bPressedKeyTbl, sizeof(m_bPressedKeyTbl));
			::ZeroMemory(&m_bPushKeyTbl, sizeof(m_bPushKeyTbl));
			::ZeroMemory(&m_bUpKeyTbl, sizeof(m_bUpKeyTbl));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}�E�X�g�p�Ń}�E�X���N���C�A���g�̈�ɂ��邩�ǂ����̃`�F�b�N
		@param[in]	hWnd	�E�C���h�E�̃n���h���B
		@param[in]	UseKeyVec	�g�p����L�[�R�[�h�z��
		@return �}�E�X�g�p�Ń}�E�X���N���C�A���g�̈�ɂ����true
		*/
		//--------------------------------------------------------------------------------------
		bool IsMouseEnabled(HWND hWnd, vector<DWORD>& UseKeyVec) {
			//�}�E�X�|�C���g��0,0�ɏ�����
			m_MouseClientPoint = { 0,0 };
			//�}�E�X���p�ɂ�����炸�A�}�E�X�|�C���^���W�͐ݒ�
			POINT p;
			::GetCursorPos(&p);
			if (::ScreenToClient(hWnd, &p)) {
				//�N���C�A���g���W�ɕϊ��ł����Ƃ��̂݁A�}�E�X���W��ݒ�
				m_MouseClientPoint = { p.x, p.y };
			}
			else {
				return false;
			}

			//UseKeyVec�Ƀ}�E�X���������ꍇ
			vector<DWORD> MouseTemp = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON };
			bool MouseChk = false;
			for (auto chk : MouseTemp) {
				auto it = find(UseKeyVec.begin(), UseKeyVec.end(), chk);
				if (it != UseKeyVec.end()) {
					MouseChk = true;
					break;
				}
			}
			//�}�E�X�g�p�ō��W���N���C�A���g�̈���Ȃ�true
			if (MouseChk) {
				RECT rc;
				::GetClientRect(hWnd, &rc);
				if (::PtInRect(&rc, p)) {
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�L�[�X�e�[�g�𓾂�
		@param[in]	hWnd	�E�C���h�E�̃n���h���BEsc�L�[���͎��Ƀ��b�Z�[�W�𑗂邽��
		@param[in]	UseKeyVec	�g�p����L�[�R�[�h�z��
		@return �L�[���͂������true
		*/
		//--------------------------------------------------------------------------------------
		bool GetKeyState(HWND hWnd, vector<DWORD>& UseKeyVec) {
			m_KeyMessageActive = false;
			//��O�ɃR�s�[
			CopyMemory(m_bLastKeyTbl, m_bPushKeyTbl, sizeof(m_bLastKeyTbl));
			//�L�[�{�[�h�̏�Ԃ�������
			::ZeroMemory(&m_bPushKeyTbl, sizeof(m_bPushKeyTbl));
			::ZeroMemory(&m_bPressedKeyTbl, sizeof(m_bPressedKeyTbl));
			::ZeroMemory(&m_bUpKeyTbl, sizeof(m_bUpKeyTbl));
			//�}�E�X�̃`�F�b�N
			bool MouseEnabled = IsMouseEnabled(hWnd, UseKeyVec);
			//�������g�Ƀt�H�[�J�X���Ȃ��ꍇ��false
			if (::GetFocus() != hWnd) {
				return false;
			}
			SHORT ret = GetAsyncKeyState((int)VK_ESCAPE);
			if (ret & 0x8000) {
				//Esc�L�[�͖�������false
				return false;
			}
			size_t sz = UseKeyVec.size();
			for (size_t i = 0; i < sz; i++) {
				ret = GetAsyncKeyState((int)UseKeyVec[i]);
				if (UseKeyVec[i] == VK_LBUTTON || UseKeyVec[i] == VK_RBUTTON || UseKeyVec[i] == VK_MBUTTON) {
					if (!MouseEnabled) {
						//�}�E�X�������Ȃ�Used�ɓ����Ă��Ă�����
						continue;
					}
				}
				if (ret & 0x8000) {
					m_bPushKeyTbl[UseKeyVec[i]] = true;
					m_KeyMessageActive = true;	//���b�Z�[�W���Q�[�����󂯎��
					if (!m_bLastKeyTbl[UseKeyVec[i]]) {
						//�O�񉟂���ĂȂ��č��񉟂��ꂽ
						m_bPressedKeyTbl[UseKeyVec[i]] = true;
					}
				}
				else {
					//�L�[�͉�����ĂȂ�
					if (m_bLastKeyTbl[UseKeyVec[i]]) {
						//�O�񉟂���Ă��č��񉟂���ĂȂ�
						m_bUpKeyTbl[UseKeyVec[i]] = true;
						m_KeyMessageActive = true;	//���b�Z�[�W���Q�[�����󂯎��
					}
				}
			}
			return m_KeyMessageActive;
		}
	};

	//--------------------------------------------------------------------------------------
	///	���͋@��
	//--------------------------------------------------------------------------------------
	class InputDevice {
		static const DWORD MAX_CONTROLLERS = 4; // �R���g���[����
		vector<CONTROLER_STATE> m_State; // �R���g���[���X�e�[�g�̔z��
		KEYBOARD_STATE m_KeyState; // �L�[�{�[�h�X�e�[�g
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		InputDevice() :
			m_State(MAX_CONTROLLERS),
			m_KeyState()
		{
			for (DWORD i = 0; i < MAX_CONTROLLERS; i++) {
				::ZeroMemory(&m_State[i], sizeof(CONTROLER_STATE));
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		~InputDevice() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �}�E�X�ƃL�[�{�[�h�̏�Ԃ𓾂�
		@param[in]	hWnd	�E�C���h�E�̃n���h���BEsc�L�[���͎��Ƀ��b�Z�[�W�𑗂邽��
		@param[in]	UseKeyVec	�g�p����L�[
		@return	�}�E�X���L�[�{�[�h���b�Z�[�W�������true
		*/
		//--------------------------------------------------------------------------------------
		bool ResetInputState(HWND hWnd, vector<DWORD>& UseKeyVec) {
			bool Ret1 = m_KeyState.GetKeyState(hWnd, UseKeyVec);
			return Ret1;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���g���[���[�̏�Ԃ�m_State�Ɏ擾����
		*/
		//--------------------------------------------------------------------------------------
		void ResetControlerState() {
			DWORD dwResult;
			for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
			{
				//��O�̏�Ԃ�ۑ�
				m_State[i].wLastButtons = m_State[i].Gamepad.wButtons;
				// XInput���猻�݂̃X�e�[�^�X�𓾂�
				XINPUT_STATE State;
				dwResult = XInputGetState(i, &State);
				if (dwResult == ERROR_SUCCESS) {
					m_State[i].dwPacketNumber = State.dwPacketNumber;
					m_State[i].Gamepad = State.Gamepad;
					m_State[i].bConnected = true;
					//�{�^���̐ݒ�
					//�R�s�[
					m_State[i].wButtons = m_State[i].Gamepad.wButtons;
					//�X�V�{�^��
					m_State[i].wNowUpdateButtons = m_State[i].Gamepad.wButtons;
					m_State[i].bLeftTrigger = m_State[i].Gamepad.bLeftTrigger;
					m_State[i].bRightTrigger = m_State[i].Gamepad.bRightTrigger;
					//�O����ω����Ă���{�^���̂݃t���O�𗧂Ă�
					//XOR���Z�ɂ�����
					m_State[i].wNowUpdateButtons ^= m_State[i].wLastButtons;
					//�����ꂽ�u�Ԃ��Z�b�g
					m_State[i].wPressedButtons = m_State[i].wNowUpdateButtons & m_State[i].wButtons;
					//�����ꂽ�u�Ԃ��Z�b�g
					m_State[i].wReleasedButtons = m_State[i].wNowUpdateButtons & (m_State[i].wButtons ^ 0xffff);
					//���o�[�̃f�b�h�]�[���̐ݒ�
					if ((m_State[i].Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
						m_State[i].Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
						(m_State[i].Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
							m_State[i].Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
					{
						m_State[i].Gamepad.sThumbLX = 0;
						m_State[i].Gamepad.sThumbLY = 0;
					}
					if (m_State[i].Gamepad.sThumbLX >= 0) {
						m_State[i].fThumbLX = (float)m_State[i].Gamepad.sThumbLX / (float)SHRT_MAX;
					}
					else {
						m_State[i].fThumbLX = (float)m_State[i].Gamepad.sThumbLX / (float)(SHRT_MAX + 1);
					}
					if (m_State[i].Gamepad.sThumbLY >= 0) {
						m_State[i].fThumbLY = (float)m_State[i].Gamepad.sThumbLY / (float)SHRT_MAX;
					}
					else {
						m_State[i].fThumbLY = (float)m_State[i].Gamepad.sThumbLY / (float)(SHRT_MAX + 1);
					}

					if ((m_State[i].Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
						m_State[i].Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
						(m_State[i].Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
							m_State[i].Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
					{
						m_State[i].Gamepad.sThumbRX = 0;
						m_State[i].Gamepad.sThumbRY = 0;
					}
					if (m_State[i].Gamepad.sThumbRX >= 0) {
						m_State[i].fThumbRX = (float)m_State[i].Gamepad.sThumbRX / (float)SHRT_MAX;
					}
					else {
						m_State[i].fThumbRX = (float)m_State[i].Gamepad.sThumbRX / (float)(SHRT_MAX + 1);
					}
					if (m_State[i].Gamepad.sThumbRY >= 0) {
						m_State[i].fThumbRY = (float)m_State[i].Gamepad.sThumbRY / (float)SHRT_MAX;
					}
					else {
						m_State[i].fThumbRY = (float)m_State[i].Gamepad.sThumbRY / (float)(SHRT_MAX + 1);
					}
				}
				else {
					m_State[i].bConnected = false;
				}
			}
		}
		//	�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief �L�[�{�[�h�X�e�[�g�̎擾
		@return	�L�[�{�[�h�X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		const KEYBOARD_STATE& GetKeyState() const { return m_KeyState; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���g���[���X�e�[�g�̎擾
		@return	�R���g���[���X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		const vector<CONTROLER_STATE>& GetControlerVec()const { return m_State; }
	};


	struct Event;


	//--------------------------------------------------------------------------------------
	///	�C�x���g�z���N���X
	//--------------------------------------------------------------------------------------
	class EventDispatcher {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	SceneBasePtr	�V�[���x�[�X�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		explicit EventDispatcher();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EventDispatcher();


		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C�x���g���󂯎��O���[�v�ɒǉ��i�O���[�v���Ȃ���΂��̖��O�ō쐬�j
		@param[in]	GroupKey	�O���[�v��
		@param[in]	Receiver	�󂯎葤�I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddEventReceiverGroup(const wstring& GroupKey, const shared_ptr<ObjectInterface>& Receiver);

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C�x���g��POST�i�L���[�ɓ����j
		@param[in]	Delay	�z���܂ł̎���
		@param[in]	Sender	���葤�I�u�W�F�N�g�inullptr�j
		@param[in]	Receiver	�󂯎葤�I�u�W�F�N�g
		@param[in]	MsgStr	���b�Z�[�W������
		@param[in]	Info	�ǉ����������[�U�[�f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void PostEvent(float Delay, const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
			const wstring& MsgStr, const  shared_ptr<void>& Info = shared_ptr<void>());
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C�x���g��POST�i�L���[�ɓ����j
		@param[in]	DispatchTime	POST���鎞�ԁi0�Ŏ��̃^�[���j
		@param[in]	Sender	�C�x���g���M�ҁinullptr�j
		@param[in]	ReceiverKey	�󂯎葤�I�u�W�F�N�g�𔻕ʂ���L�[
		@param[in]	MsgStr	���b�Z�[�W
		@param[in,out]	Info	�ǉ����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void PostEvent(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
			const wstring& MsgStr, const  shared_ptr<void>& Info = shared_ptr<void>());
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C�x���g��SEND�i�L���[�ɓ��ꂸ�ɂ��̂܂ܑ���j
		@param[in]	Sender	���葤�I�u�W�F�N�g�inullptr�j
		@param[in]	Receiver	�󂯎葤�I�u�W�F�N�g
		@param[in]	MsgStr	���b�Z�[�W������
		@param[in]	Info	�ǉ����������[�U�[�f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SendEvent(const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
			const wstring& MsgStr, const  shared_ptr<void>& Info = shared_ptr<void>());
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C�x���g��SEND�i�L���[�ɓ��ꂸ�ɂ��̂܂ܑ���j
		@param[in]	Sender	�C�x���g���M�ҁinullptr�j
		@param[in]	Receiver	�C�x���g��M�ҁinullptr�s�j
		@param[in]	MsgStr	���b�Z�[�W
		@param[in,out]	Info	�ǉ����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SendEvent(const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
			const wstring& MsgStr, const  shared_ptr<void>& Info = shared_ptr<void>());

		//--------------------------------------------------------------------------------------
		/*!
		@brief	POST�C�x���g�̑��M(���C�����[�v�ŌĂ΂��)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void DispatchDelayedEvwnt();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�L���[�ɂ��܂��Ă��郁�b�Z�[�W���폜����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ClearEventQ();
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		EventDispatcher(const EventDispatcher&) = delete;
		EventDispatcher& operator=(const EventDispatcher&) = delete;
		//���[�u�֎~
		EventDispatcher(const EventDispatcher&&) = delete;
		EventDispatcher& operator=(const EventDispatcher&&) = delete;
	};



	//--------------------------------------------------------------------------------------
	///	�A�v���P�[�V�����N���X
	//--------------------------------------------------------------------------------------
	class App {
		// �f���[�^�[
		struct AppDeleter
		{
			void operator()(App *p) { delete p; }
		};
		static unique_ptr<App, AppDeleter> m_App;		///< Singleton�ŗ��p���鎩�����g�̃|�C���^
		HINSTANCE m_hInstance;							///< ���W���[���̃C���X�^���X
		HWND m_hWnd;									///< ���C���E�C���h�E�̃n���h��
		bool m_FullScreen;								///< �t���X�N���[�����ǂ���
		UINT m_GameWidth;								///< �Q�[���Օ�(�s�N�Z��)
		UINT m_GameHeight;								///< �Q�[���Ս���(�s�N�Z��)
		shared_ptr<DeviceResources> m_DeviceResources;	///< �f�o�C�X
		shared_ptr<SceneInterface> m_SceneInterface;	///< �V�[��
		shared_ptr<EventDispatcher> m_EventDispatcher;	///< �C�x���g���M�I�u�W�F�N�g

		map<wstring, shared_ptr<BaseResource> > m_ResMap;		///< �L�[�ƃ��\�[�X�����ѕt����}�b�v
		StepTimer m_Timer;										///< �^�C�}�[
		InputDevice m_InputDevice;					///< ���͋@��
		shared_ptr<XAudio2Manager> m_XAudio2Manager; ///< XAudio2�I�[�f�B�I�}�l�[�W��
		wstring		m_wstrModulePath;		///< ���W���[�����t���p�X
		wstring		m_wstrDir;				///< ���W���[��������f�B���N�g��
		wstring		m_wstrDataPath;			///< ��΃p�X�̃��f�B�A�f�B���N�g��
		wstring		m_wstrShadersPath;		///< ��΃p�X�̃V�F�[�_�f�B���N�g��
		wstring		m_wstrRelativeDataPath;	///< ���΃p�X�̃��f�B�A�f�B���N�g��
		wstring		m_wstrRelativeShadersPath;	///< ���΃p�X�̃V�F�[�_�f�B���N�g��

		bool m_ScriptsDirActive;				///<�X�N���v�g�f�B���N�g�����L�����ǂ���
		wstring		m_wstrScriptsPath;			///< ��΃p�X�̃X�N���v�g�f�B���N�g��
		wstring		m_wstrRelativeScriptsPath;	///< ���΃p�X�̃X�N���v�g�f�B���N�g��

		wstring		m_wstrRelativeAssetsPath;	///< ���΃p�X�̃A�Z�b�g�f�B���N�g��

		App(HINSTANCE hInstance, HWND hWnd, bool FullScreen, UINT Width, UINT Height);
		virtual ~App() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V���O���g���\�z�ƃA�N�Z�T�i�\�z���͂��ׂẴp�����[�^��ݒ肷��j
		@param[in]	hInstance	�C���X�^���X
		@param[in]	hWnd		�E�C���h�E�̃n���h��
		@param[in]	FullScreen	�t���X�N���[�����ǂ���
		@param[in]	Width	��
		@param[in]	Height	����
		@param[in]	ShadowActive	�e���L�����ǂ���
		@return	App��unique_ptr
		*/
		//--------------------------------------------------------------------------------------
		static unique_ptr<App, AppDeleter>& CreateApp(HINSTANCE hInstance, HWND hWnd,
			bool FullScreen, UINT Width, UINT Height,bool ShadowActive = true);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V���O���g���A�N�Z�T
		@return	App��unique_ptr
		*/
		//--------------------------------------------------------------------------------------
		static unique_ptr<App, AppDeleter>& GetApp();
		//--------------------------------------------------------------------------------------
		/*!
		@brief App���쐬����Ă邩�`�F�b�N
		@return	�쐬����Ă����true
		*/
		//--------------------------------------------------------------------------------------
		static bool AppCheck();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �����j��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void DeleteApp();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ǉ��������i�����_�����O�^�[�Q�b�g�̏����Ȃǁj
		@param[in]	ShadowActive	�e���L�����ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AfterInitContents(bool ShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �I�[�f�B�I�}�l�[�W���̎擾
		@return	�I�[�f�B�I�}�l�[�W��
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<XAudio2Manager>& GetXAudio2Manager();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief �A�v���P�[�V�����̃C���X�^���X�擾
		@return	�A�v���P�[�V�����̃C���X�^���X
		*/
		//--------------------------------------------------------------------------------------
		HINSTANCE GetHInstance() const { return m_hInstance; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �E�C���h�E�̃n���h���̎擾
		@return	�E�C���h�E�̃n���h��
		*/
		//--------------------------------------------------------------------------------------
		HWND GetHWnd() const { return m_hWnd; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �t���X�N���[�����ǂ���
		@return	�t���X�N���[���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsFullScreen() const { return m_FullScreen; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �t���X�N���[�����ǂ���
		@return	�t���X�N���[���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetFullScreen() const { return m_FullScreen; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �Q�[���Ղ̕��̎擾
		@return	�Q�[���Ղ̕�
		*/
		//--------------------------------------------------------------------------------------
		UINT GetGameWidth() const { return m_GameWidth; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �Q�[���Ղ̕��̎擾
		@return	�Q�[���Ղ̕�
		*/
		//--------------------------------------------------------------------------------------
		UINT GetGameHeight() const { return m_GameHeight; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�o�C�X���\�[�X�̎擾
		@return	�f�o�C�X���\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<DeviceResources> GetDeviceResources() const { return m_DeviceResources; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V�[���C���^�[�t�F�C�X�̎擾
		@return	�V�[���C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<SceneInterface> GetSceneInterface() const { return m_SceneInterface; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V�[���C���^�[�t�F�C�X�̐ݒ�
		@param[in]	ptr	�V�[���C���^�[�t�F�C�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetSceneInterface(const shared_ptr<SceneInterface>& ptr) { m_SceneInterface = ptr; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g���M�I�u�W�F�N�g�̎擾
		@return	�C�x���g���M�I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<EventDispatcher> GetEventDispatcher() const { return m_EventDispatcher; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g���M�I�u�W�F�N�g�̐ݒ�
		@param[in]	ptr	�C�x���g���M�I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetEventDispatcher(const shared_ptr<EventDispatcher>& ptr) { m_EventDispatcher = ptr; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �^�C�}�[�̎擾(�ݒ�A�N�Z�T�͂Ȃ�)
		@return	�^�C�}�[
		*/
		//--------------------------------------------------------------------------------------
		StepTimer& GetStepTimer() { return m_Timer; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �O��̃^�[������̌o�ߎ��Ԃ𓾂�
		@return	�O��̃^�[������̌o�ߎ���
		*/
		//--------------------------------------------------------------------------------------
		float GetElapsedTime() {
			double d = m_Timer.GetElapsedSeconds();
			//0���Z����̂���1000����1�����͕Ԃ�Ȃ��悤�ɂ���
			if (d < 0.001) {
				d = 0.001;
			}
			return (float)d;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���͋@��̎擾
		@return	���͋@��
		*/
		//--------------------------------------------------------------------------------------
		const InputDevice& GetInputDevice() const { return m_InputDevice; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �}�E�X�ƃL�[�{�[�h�̏�Ԃ𓾂�
		@param[in]	hWnd	�E�C���h�E�̃n���h���BEsc�L�[���͎��Ƀ��b�Z�[�W�𑗂邽��
		@param[in]	UseKeyVec	�g�p����L�[
		@return	�}�E�X���L�[�{�[�h���b�Z�[�W�������true
		*/
		//--------------------------------------------------------------------------------------
		bool ResetInputState(HWND hWnd, vector<DWORD>& UseKeyVec) {
			if (UseKeyVec.size() == 0) {
				return false;
			}
			return m_InputDevice.ResetInputState(hWnd, UseKeyVec);
		}
		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �V�[���̍쐬
		@tparam	T	�V�[���N���X
		@tparam Ts...	�ϒ��p�����[�^�^
		@return	�V�[��
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> CreateScene(Ts&&... params) {
			try {
				//�V�[���̍쐬
				auto Ptr = ObjectFactory::Create<T>(params...);
				m_SceneInterface = Ptr;
				return Ptr;
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V�[���̎擾
		@tparam	T	�V�[���N���X
		@return	�V�[��
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetScene() const {
			try {
				auto Ptr = dynamic_pointer_cast<T>(m_SceneInterface);
				if (!Ptr) {
					wstring str = L"m_SceneInterface��";
					str += Util::GetWSTypeName<T>();
					str += L"�^�̃V�[���Ɍ^�ϊ��ł��܂���";
					throw BaseException(
						str,
						L"if(!dynamic_pointer_cast<T>(m_SceneInterface))",
						L"App::GetScene<T>()"
					);

				}
				return Ptr;
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V�[���̍X�V�ƕ`��
		@param[in]	SyncInterval	�C���^�[�o���l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateDraw(unsigned int SyncInterval);
		//!�t���[���J�E���g�𓾂�
		//--------------------------------------------------------------------------------------
		/*!
		@brief �t���[���J�E���g�𓾂�
		@return	�t���[���J�E���g
		*/
		//--------------------------------------------------------------------------------------
		unsigned int GetFrameCount() {
			return (unsigned int)m_Timer.GetFrameCount();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���\�[�X�̓o�^�i�d��������Ή������Ȃ����A����������Η�O�j
		@param[in]	Key ���\�[�X�L�[
		@param[in]	ResObj ���\�[�X
		@return	�Ȃ��i����������Η�O�j
		*/
		//--------------------------------------------------------------------------------------
		void RegisterResource(const wstring& Key, const shared_ptr<BaseResource>& ResObj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���̓o�^(�����L�[�̃e�N�X�`�����Ȃ���΃t�@�C�����ō쐬���A�o�^)�B<br />
		�������O�̃e�N�X�`��������΂��̃|�C���^��Ԃ�
		@param[in]	Key	���\�[�X�L�[
		@param[in]	TextureFileName �e�N�X�`���t�@�C����
		@param[in]	TexType = L"WIC" �e�N�X�`���^�C�v�i�f�t�H���gWIC�j
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> RegisterTexture(const wstring& Key,
			const wstring& TextureFileName, const wstring& TexType = L"WIC");
		//--------------------------------------------------------------------------------------
		/*!
		@brief Wav���\�[�X�̓o�^(�����L�[��Wav���\�[�X���Ȃ���΃t�@�C�����ō쐬���A�o�^)�B<br />
		�������O��Wav���\�[�X������΂��̃|�C���^��Ԃ�
		@param[in]	Key ���\�[�X�L�[
		@param[in]	WavFileName �e�N�X�`���t�@�C����
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<AudioResource> RegisterWav(const wstring& Key, const wstring& WavFileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���\�[�X�̊m�F
		@tparam	T �m�F���郊�\�[�X�^
		@param[in]	Key ���\�[�X�L�[
		@return	���̃L�[�̃��\�[�X�������true�A�������false�i�������Ă��w��̌^�łȂ���Η�O�j
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		bool CheckResource(const wstring& Key) const {
			if (Key == L"") {
				throw BaseException(
					L"�L�[���󔒂ł�",
					L"if(Key == L\"\")",
					L"App::CheckResource()"
				);
			}
			map<wstring, shared_ptr<BaseResource> >::const_iterator it;
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end()) {
				//�w��̖��O����������
				shared_ptr<T> pT = dynamic_pointer_cast<T>(it->second);
				if (pT) {
					return true;
				}
				else {
					wstring keyerr = Key;
					wstring str = L"�w��̃L�[��";
					str += Util::GetWSTypeName<T>();
					str += L"*�^�ɕϊ��ł��܂���";
					throw BaseException(
						str,
						keyerr,
						L"App::CheckResource()"
					);
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �L�[�̊m�F
		@param[in]	Key ���\�[�X�L�[
		@return	���̃L�[�����݂����true�B���\�[�X�̃|�C���^��Ԃ��킯�ł͂Ȃ��B�L�[�̏d���̂݃`�F�b�N
		*/
		//--------------------------------------------------------------------------------------
		bool CheckResourceKey(const wstring& Key) const {
			if (Key == L"") {
				throw BaseException(
					L"�L�[���󔒂ł�",
					L"if(Key == L\"\")",
					L"App::CheckResourceKey()"
				);
			}
			map<wstring, shared_ptr<BaseResource> >::const_iterator it;
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end()) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���\�[�X�̎擾
		@tparam	T �擾���郊�\�[�X�^
		@param[in]	Key ���\�[�X�L�[
		@return	���\�[�X�̃|�C���^�i�o�^���Ȃ���Η�O�j
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetResource(const wstring& Key) const {
			if (Key == L"") {
				throw BaseException(
					L"�L�[���󔒂ł�",
					L"if(Key == L\"\")",
					L"App::GetResource()"
				);
			}
			map<wstring, shared_ptr<BaseResource> >::const_iterator it;
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end()) {
				//�w��̖��O����������
				shared_ptr<T> pT = dynamic_pointer_cast<T>(it->second);
				if (pT) {
					return pT;
				}
				else {
					wstring keyerr = Key;
					wstring str = L"�w��̃L�[��";
					str += Util::GetWSTypeName<T>();
					str += L"*�^�ɕϊ��ł��܂���";
					throw BaseException(
						str,
						keyerr,
						L"App::GetResource()"
					);
				}
			}
			else {
				//������Ȃ�
				wstring keyerr = Key;
				throw BaseException(
					L"�w��̃L�[�͑��݂��܂���",
					keyerr,
					L"App::GetResource()"
				);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���\�[�X�L�[�̎擾
		@tparam	T �擾���郊�\�[�X�^
		@param[in]	Ptr ���\�[�X�̃|�C���^
		@return	���\�[�X�L�[�i�o�^���Ȃ���Η�O�j
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		const wstring& GetResourceKey(const shared_ptr<T>& Ptr) const {
			map<wstring, shared_ptr<BaseResource> >::const_iterator it;
			for (it = m_ResMap.begin(); it != m_ResMap.end(); it++) {
				if (it->second == static_pointer_cast<BaseResource>(Ptr)) {
					return it->first;
				}
			}
			//������Ȃ�
			wstring str = Util::GetWSTypeName<T>();
			throw BaseException(
				L"�w��̃��\�[�X�͑��݂��܂���B",
				str,
				L"App::GetResourceKey()"
			);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���\�[�X�̉��
		@tparam	T ������郊�\�[�X�^
		@param[in]	Key ���\�[�X�L�[
		@return	�Ȃ��i�o�^���Ȃ���Η�O�j
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		void UnRegisterResource(const wstring& Key) {
			try {
				//�ȉ��Ɏ��s�������O����������
				shared_ptr<T> p = GetResource<T>(Key);
				m_ResMap.erase(Key);
				p.reset();

			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���W���[���f�B���N�g���̎擾
		@param[out]	Dir �擾���镶����
		@return	�Ȃ��iDir�ɑ��΃p�X������j
		*/
		//--------------------------------------------------------------------------------------
		void GetModuleDirectory(wstring& Dir) {
			Dir = m_wstrDir;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�[�^�f�B���N�g���̎擾
		@param[out]	Dir �擾���镶����
		@return	�Ȃ��iDir�ɑ��΃p�X������j
		*/
		//--------------------------------------------------------------------------------------
		void GetDataDirectory(wstring& Dir) {
			Dir = m_wstrRelativeDataPath;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�[�^�f�B���N�g���̎擾�i������̎Q�Ƃ��擾�j
		@return	�f�[�^�f�B���N�g���̑��΃p�X
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetDataDirWString()const {
			return m_wstrRelativeDataPath;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief �V�F�[�_�f�B���N�g���̎擾
		@param[out]	Dir �擾���镶����
		@return	�Ȃ��iDir�ɑ��΃p�X������j
		*/
		//--------------------------------------------------------------------------------------
		void GetShadersDirectory(wstring& Dir) {
			Dir = m_wstrRelativeShadersPath;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V�F�[�_�f�B���N�g���̎擾�i������̎Q�Ƃ��擾�j
		@return	�V�F�[�_�f�B���N�g���̑��΃p�X
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetShadersPath()const {
			return m_wstrRelativeShadersPath;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�N���v�g�f�B���N�g�����L�����ǂ���
		@return	�L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsScriptsDirActive() const {
			return m_ScriptsDirActive;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�N���v�g�f�B���N�g���̎擾
		@param[out]	Dir �擾���镶����
		@return	�Ȃ��iDir�ɑ��΃p�X������j
		*/
		//--------------------------------------------------------------------------------------
		void GetScriptsDirectory(wstring& Dir) {
			if (!m_ScriptsDirActive) {
				throw BaseException(
					L"�X�N���v�g���L���ł͂���܂���B",
					L"if (!m_ScriptsDirActive)",
					L"App::GetScriptsDirectory()"
				);
			}
			Dir = m_wstrRelativeScriptsPath;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�N���v�g�̐�΃f�B���N�g���̎擾
		@param[out]	Dir �擾���镶����
		@return	�Ȃ��iDir�ɑ��΃p�X������j
		*/
		//--------------------------------------------------------------------------------------
		void GetScriptsFullDirectory(wstring& Dir) {
			if (!m_ScriptsDirActive) {
				throw BaseException(
					L"�X�N���v�g���L���ł͂���܂���B",
					L"if (!m_ScriptsDirActive)",
					L"App::GetScriptsFullDirectory()"
				);
			}
			Dir = m_wstrScriptsPath;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief �A�Z�b�g�f�B���N�g���̎擾
		@param[out]	Dir �擾���镶����
		@return	�Ȃ��iDir�ɑ��΃p�X������j
		*/
		//--------------------------------------------------------------------------------------
		void GetAssetsDirectory(wstring& Dir) {
			Dir = m_wstrRelativeAssetsPath;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �E�C���h�E���b�Z�[�W
		@param[in]	message	���b�Z�[�W
		@param[in]	wParam	��1�p�����[�^
		@param[in]	lParam	��2�p�����[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
		map<wstring, wstring> m_ConfigMap;		///< �ėp�}�b�v�i�e�A�v���P�V�����Ŏg�p�ł���j
	private:
		//�R�s�[�֎~
		App(const App&) = delete;
		App& operator=(const App&) = delete;
		//���[�u�֎~
		App(const App&&) = delete;
		App& operator=(const App&&) = delete;

	};





}
//end basecross

