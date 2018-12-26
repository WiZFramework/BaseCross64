/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};


	//--------------------------------------------------------------------------------------
	///	PC��
	//--------------------------------------------------------------------------------------
	class PcSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//�\�z�Ɣj��
		PcSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PcSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	///	PN��
	//--------------------------------------------------------------------------------------
	class PnSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//�\�z�Ɣj��
		PnSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PnSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;

	};


	//--------------------------------------------------------------------------------------
	///	PT��
	//--------------------------------------------------------------------------------------
	class PtSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//�\�z�Ɣj��
		PtSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PtSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	PCT��
	//--------------------------------------------------------------------------------------
	class PctSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//�\�z�Ɣj��
		PctSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PctSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
	};




	//--------------------------------------------------------------------------------------
	///	Pnt��
	//--------------------------------------------------------------------------------------
	class PntSphere : public GameObject {
		Vec3 m_StartPos;
		bool m_TextureUse;
	public:
		//�\�z�Ɣj��
		PntSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TextureUse);
		virtual ~PntSphere();
		//������
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Pnt�X�y�L�����[��
	//--------------------------------------------------------------------------------------
	class PntSpSphere : public GameObject {
		Vec3 m_StartPos;
		bool m_TextureUse;
	public:
		//�\�z�Ɣj��
		PntSpSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TextureUse);
		virtual ~PntSpSphere();
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	Static�L����
	//--------------------------------------------------------------------------------------
	class StaticChara : public GameObject {
		Vec3 m_StartPos;
		bool m_TamgentUse;
	public:
		//�\�z�Ɣj��
		StaticChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse);
		virtual ~StaticChara();
		//������
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Static�L����(�}���`���b�V����)
	//--------------------------------------------------------------------------------------
	class StaticMultiMeshChara : public GameObject {
		Vec3 m_StartPos;
		bool m_TamgentUse;
	public:
		//�\�z�Ɣj��
		StaticMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse);
		virtual ~StaticMultiMeshChara();
		//������
		virtual void OnCreate() override;
	};



	//--------------------------------------------------------------------------------------
	///	Bone�L����
	//--------------------------------------------------------------------------------------
	class BoneChara : public GameObject {
		Vec3 m_StartPos;
		bool m_TamgentUse;
	public:
		//�\�z�Ɣj��
		BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse);
		virtual ~BoneChara();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Bone�L����(�}���`���b�V����)
	//--------------------------------------------------------------------------------------
	class BoneMultiMeshChara : public GameObject {
		Vec3 m_StartPos;
		bool m_TamgentUse;
		//���̓n���h���[
		InputHandler<BoneMultiMeshChara> m_InputHandler;
		//�}���`���b�V���̕\���t���O
		bool m_DrawFlg;
		//�`��X�C�b�`
		void DrawSwitch();
	public:
		//�\�z�Ɣj��
		BoneMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse);
		virtual ~BoneMultiMeshChara();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//A�{�^��
		void OnPushA();
	};




}
//end basecross
