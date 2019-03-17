/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�ʒu�w��̃��C��
	//--------------------------------------------------------------------------------------
	PositionLine::PositionLine(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Vec3& EndPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_EndPos(EndPos)
	{}




	//������
	void PositionLine::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat Qt;
		Qt.identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(Vec3(0.0f, 0.0f, 0.0f));

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<PCStaticDraw>();

		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(m_StartPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) },
			{ VertexPositionColor(m_EndPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) }

		};
		vector<uint16_t> indices = {
			0, 1
		};

		auto MeshRes = MeshResource::CreateMeshResource(vertices, indices, true);
		MeshRes->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		PtrDraw->SetOriginalMeshResource(MeshRes);
		PtrDraw->SetOriginalMeshUse(true);

	}

	void PositionLine::OnUpdate() {

		auto PtrDraw = GetComponent<PCStaticDraw>();
		const vector<VertexPositionColor>& BackupVec = PtrDraw->GetOriginalMeshResource()->GetBackupVerteces<VertexPositionColor>();
		vector<VertexPositionColor> new_vec;
		VertexPositionColor new_v;
		new_v = BackupVec[0];
		new_v.position = m_StartPos;
		new_vec.push_back(new_v);

		new_v = BackupVec[1];
		new_v.position = m_EndPos;
		new_vec.push_back(new_v);

		PtrDraw->UpdateVertices(new_vec);

	}


	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedPsBox::FixedPsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	FixedPsBox::~FixedPsBox() {}
	//������
	void FixedPsBox::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z�{�b�N�X
		PsBoxParam param;
		//DEFAULT_CUBE�̃X�P�[�����O�͊e�ӊ�Ȃ̂ŁA�n�[�t�T�C�Y�ɂ���
		param.m_HalfSize = m_Scale * 0.5f;
		//�Œ�Ȃ̂Ŏ��ʂ͂���Ȃ�
		param.m_Mass = 0.0f;
		//�����e���\�����f�t�H���g�ŗǂ�
		param.m_MotionType = PsMotionType::MotionTypeFixed;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		PsPtr->SetDrawActive(true);

	}



	//--------------------------------------------------------------------------------------
	///	���O�h�[��
	//--------------------------------------------------------------------------------------
	Ragdoll::Ragdoll(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos,
		const Quat& StartQt) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_StartQt(StartQt)
	{
	}
	Ragdoll::~Ragdoll() {}

	void Ragdoll::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		float Scale(1.0f);
		Ptr->SetScale(Vec3(Scale));
		Ptr->SetQuaternion(m_StartQt);
		Ptr->SetPosition(m_StartPos);

		//�}���`�����I�u�W�F�N�g
		auto PsPtr = AddComponent<RigidbodyMulti>();
		PsPtr->SetDrawActive(true);
		PsPtr->SetAutoTransform(false);

		uint32_t contactFilterSelfA = 0x01;
		uint32_t contactFilterSelfB = 0x02;
		uint32_t contactFilterSelfC = 0x04;
		uint32_t contactFilterSelfD = 0x08;
		uint32_t contactFilterTargetA = 0x0d;
		uint32_t contactFilterTargetB = 0x0a;
		uint32_t contactFilterTargetC = 0x05;
		uint32_t contactFilterTargetD = 0x0b;

		size_t HeadNum, TorsoNum, BodyNum,
			UpperLegLNum, LowerLegLNum, UpperArmLNum, LowerArmLNum,
			UpperLegRNum, LowerLegRNum, UpperArmRNum, LowerArmRNum;


		float inertiaScale = 3.0f;
		{
			//�����̍쐬
			PsSphereParam param;
			param.m_Radius = 0.3f;
			param.m_Mass = 3.0f;
			//�����e���\���̌v�Z
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.0f, 3.38433f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfB;
			param.m_ContactFilterTarget = contactFilterTargetB;
			//�����̒ǉ�
			HeadNum = PsPtr->AddSphere(param);


		}
		{
			//���̍쐬
			PsSphereParam param;
			param.m_Radius = 0.35f;
			param.m_Mass = 10.0f;
			//�����e���\���̌v�Z
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.0f, 1.96820f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfB;
			param.m_ContactFilterTarget = contactFilterTargetB;
			//���̒ǉ�
			TorsoNum = PsPtr->AddSphere(param);

		}
		{
			//���̂̍쐬
			PsCapsuleParam param;
			param.m_HalfLen = 0.1f;
			param.m_Radius = 0.38f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 8.0f;
			//�����e���\���̌v�Z
			//��������
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.0f, 2.66926f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfA;
			param.m_ContactFilterTarget = contactFilterTargetA;
			//���̂̒ǉ�
			BodyNum = PsPtr->AddCapsule(param);
		}
		{
			//���������̍쐬
			PsCapsuleParam param;
			param.m_HalfLen = 0.35f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 8.0f;
			//�����e���\���̌v�Z
			//��������
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.21f, 1.34871f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfC;
			param.m_ContactFilterTarget = contactFilterTargetC;
			//���������̒ǉ�
			UpperLegLNum = PsPtr->AddCapsule(param);

		}
		{
			//�����ڕ��̍쐬
			PsCapsuleParam param;
			param.m_HalfLen = 0.3f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 4.0f;
			//�����e���\���̌v�Z
			//��������
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.21f, 0.59253f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfD;
			param.m_ContactFilterTarget = contactFilterTargetD;
			//�����ڕ��̒ǉ�
			LowerLegLNum = PsPtr->AddCapsule(param);
		}

		{
			//���㕔�r�̍쐬
			PsCapsuleParam param;
			param.m_HalfLen = 0.25f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 5.0f;
			//�����e���\���̌v�Z
			//��������
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = Quat(0.0f, 0.0f, 0.70711f, 0.70711f) * m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.72813f, 2.87483f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfB;
			param.m_ContactFilterTarget = contactFilterTargetB;
			//���㕔�r�̒ǉ�
			UpperArmLNum = PsPtr->AddCapsule(param);
		}
		{
			//�������r�̍쐬
			PsCapsuleParam param;
			param.m_HalfLen = 0.225f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 3.0f;
			//�����e���\���̌v�Z
			//��������
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = Quat(0.0f, 0.0f, 0.70711f, 0.70711f) * m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(1.42931f, 2.87483f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfC;
			param.m_ContactFilterTarget = contactFilterTargetC;
			//�������r�̒ǉ�
			LowerArmLNum = PsPtr->AddCapsule(param);
		}
		{
			//�E�������̍쐬
			PsCapsuleParam param;
			param.m_HalfLen = 0.35f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 8.0f;
			//�����e���\���̌v�Z
			//��������
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(-0.21f, 1.34871f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfC;
			param.m_ContactFilterTarget = contactFilterTargetC;
			//�E�������̒ǉ�
			UpperLegRNum = PsPtr->AddCapsule(param);

		}

		{
			//�E���ڕ��̍쐬
			PsCapsuleParam param;
			param.m_HalfLen = 0.3f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 4.0f;
			//�����e���\���̌v�Z
			//��������
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(-0.21f, 0.59253f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfD;
			param.m_ContactFilterTarget = contactFilterTargetD;
			//�E���ڕ��̒ǉ�
			LowerLegRNum = PsPtr->AddCapsule(param);
		}

		{
			//�E�㕔�r�̍쐬
			PsCapsuleParam param;
			param.m_HalfLen = 0.25f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 5.0f;
			//�����e���\���̌v�Z
			//��������
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = Quat(0.0f, 0.0f, -0.70711f, 0.70711f) * m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(-0.72813f, 2.87483f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfB;
			param.m_ContactFilterTarget = contactFilterTargetB;
			//�E�㕔�r�̒ǉ�
			UpperArmRNum = PsPtr->AddCapsule(param);
		}
		{
			//�E�����r�̍쐬
			PsCapsuleParam param;
			param.m_HalfLen = 0.225f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 3.0f;
			//�����e���\���̌v�Z
			//��������
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = Quat(0.0f, 0.0f, -0.70711f, 0.70711f) * m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(-1.42931f, 2.87483f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfC;
			param.m_ContactFilterTarget = contactFilterTargetC;
			//�E�����r�̒ǉ�
			LowerArmRNum = PsPtr->AddCapsule(param);
		}
		{
			//���Ɠ��̂̃W���C���g
			PsSwingTwistJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(0.0f, 2.26425f, 0.0f));
			jparam.m_TwistAxis = rotate(m_StartQt, Vec3(0.0f, 1.0f, 0.0f));
			jparam.m_IndexA = PsPtr->GetPsObject(TorsoNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(BodyNum)->GetIndex();
			PsPtr->AddSwingTwistJoint(jparam);
		}
		{
			//���̂Ɠ����̃W���C���g
			PsSwingTwistJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(0.0f, 3.13575f, 0.0f));
			jparam.m_TwistAxis = rotate(m_StartQt, Vec3(0.0f, 1.0f, 0.0f));
			jparam.m_IndexA = PsPtr->GetPsObject(BodyNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(HeadNum)->GetIndex();
			PsPtr->AddSwingTwistJoint(jparam);
		}
		{
			//���̂ƍ��㕔�r�̃W���C���g
			PsSwingTwistJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(0.40038f, 2.87192f, 0.0f));
			jparam.m_TwistAxis = rotate(m_StartQt, Vec3(1.0f, 0.0f, 0.0f));
			jparam.m_IndexA = PsPtr->GetPsObject(BodyNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(UpperArmLNum)->GetIndex();
			PsPtr->AddSwingTwistJoint(jparam);
		}
		{
			//���㕔�r�ƍ������r�̃W���C���g
			PsHingeJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(1.08651f, 2.87483f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(0.0f, -1.0f, 0.0f));
			jparam.m_LowerAngle = 0.0f;
			jparam.m_UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(UpperArmLNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(LowerArmLNum)->GetIndex();
			PsPtr->AddHingeJoint(jparam);
		}
		{
			//���̂ƉE�㕔�r�̃W���C���g
			PsSwingTwistJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(-0.40360f, 2.87499f, 0.00000f));
			jparam.m_TwistAxis = rotate(m_StartQt, Vec3(-1.0f, 0.0f, 0.0f));
			jparam.m_IndexA = PsPtr->GetPsObject(BodyNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(UpperArmRNum)->GetIndex();
			PsPtr->AddSwingTwistJoint(jparam);
		}
		{
			//�E�㕔�r�ƉE�����r�̃W���C���g
			PsHingeJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(-1.09407f, 2.87483f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(0.0f, -1.0f, 0.0f));
			jparam.m_LowerAngle = 0.0f;
			jparam.m_UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(UpperArmRNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(LowerArmRNum)->GetIndex();
			PsPtr->AddHingeJoint(jparam);
		}
		{
			//���ƍ��������̃W���C���g
			PsUniversalJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(0.20993f, 1.69661f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(0.0f, -1.0f, 0.0f));
			jparam.m_Swing1LowerAngle = 0.0f;
			jparam.m_Swing1UpperAngle = 0.52f;
			jparam.m_Swing2LowerAngle = 0.0f;
			jparam.m_Swing2UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(TorsoNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(UpperLegLNum)->GetIndex();
			PsPtr->AddUniversalJoint(jparam);
		}
		{
			//���ƉE�������̃W���C���g
			PsUniversalJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(-0.21311f, 1.69661f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(0.0f, -1.0f, 0.0f));
			jparam.m_Swing1LowerAngle = 0.0f;
			jparam.m_Swing1UpperAngle = 0.52f;
			jparam.m_Swing2LowerAngle = 0.0f;
			jparam.m_Swing2UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(TorsoNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(UpperLegRNum)->GetIndex();
			PsPtr->AddUniversalJoint(jparam);

		}
		{
			//���������ƍ����ڕ��̃W���C���g
			PsHingeJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(0.21000f, 0.97062f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(1.0f, 0.0f, 0.0f));
			jparam.m_LowerAngle = 0.0f;
			jparam.m_UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(UpperLegLNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(LowerLegLNum)->GetIndex();
			PsPtr->AddHingeJoint(jparam);

		}
		{
			//�E�������ƉE���ڕ��̃W���C���g
			PsHingeJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(-0.21000f, 0.97062f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(1.0f, 0.0f, 0.0f));
			jparam.m_LowerAngle = 0.0f;
			jparam.m_UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(UpperLegRNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(LowerLegRNum)->GetIndex();
			PsPtr->AddHingeJoint(jparam);

		}
		//�W���C���g�̃p�����[�^����
		for (auto& j : PsPtr->GetPsJointVec()) {
			j->getPfxJoint().m_constraints[0].m_warmStarting = 1;
			j->getPfxJoint().m_constraints[1].m_warmStarting = 1;
			j->getPfxJoint().m_constraints[2].m_warmStarting = 1;

			j->getPfxJoint().m_constraints[3].m_damping = 0.05f;
			j->getPfxJoint().m_constraints[4].m_damping = 0.05f;
			j->getPfxJoint().m_constraints[5].m_damping = 0.05f;
		}



	}

	void Ragdoll::OnUpdate() {
		auto Ptr = GetComponent<Transform>();
		auto PsPtr = GetComponent<RigidbodyMulti>(false);
		if (PsPtr) {
			auto BasePos = PsPtr->GetPosition(0);
			Ptr->SetPosition(BasePos);
		}

	}

	//--------------------------------------------------------------------------------------
	///	���O�h�[���}�l�[�W���i�`����Ǘ�����j
	//--------------------------------------------------------------------------------------
	RagdollManager::RagdollManager(const shared_ptr<Stage>& StagePtr, const shared_ptr<Ragdoll>& ragdoll) :
		GameObject(StagePtr),
		m_Ragdoll(ragdoll),
		m_HoldIndex(0),
		m_Holded(false)
	{}
	RagdollManager::~RagdollManager() {}

	void RagdollManager::OnCreate() {
		auto RagSh = m_Ragdoll.lock();
		if (RagSh) {
			auto PtrPsBody = RagSh->GetComponent<RigidbodyMulti>();
			{
				//��
				auto Ptr = GetStage()->AddGameObject<GameObject>();
				auto SpherePtr = dynamic_pointer_cast<PsSphere>(PtrPsBody->GetPsObject(0));
				if (SpherePtr) {
					Vec3 Scale(SpherePtr->GetParam().m_Radius * 2.0f);
					Ptr->GetComponent<Transform>()->SetScale(Scale);
					Ptr->GetComponent<Transform>()->SetQuaternion(PtrPsBody->GetOrientation(0));
					Ptr->GetComponent<Transform>()->SetPosition(PtrPsBody->GetPosition(0));
					auto DrawPtr = Ptr->AddComponent<BcPNTStaticDraw>();
					DrawPtr->SetMeshResource(L"DEFAULT_SPHERE");
					m_RagdollObjVec.push_back(Ptr);
				}
			}
			{
				//��
				auto Ptr = GetStage()->AddGameObject<GameObject>();
				auto SpherePtr = dynamic_pointer_cast<PsSphere>(PtrPsBody->GetPsObject(1));
				if (SpherePtr) {
					Vec3 Scale(SpherePtr->GetParam().m_Radius * 2.0f);
					Ptr->GetComponent<Transform>()->SetScale(Scale);
					Ptr->GetComponent<Transform>()->SetQuaternion(PtrPsBody->GetOrientation(1));
					Ptr->GetComponent<Transform>()->SetPosition(PtrPsBody->GetPosition(1));
					auto DrawPtr = Ptr->AddComponent<BcPNTStaticDraw>();
					DrawPtr->SetMeshResource(L"DEFAULT_SPHERE");
					m_RagdollObjVec.push_back(Ptr);
				}
			}
			{
				//����
				auto Ptr = GetStage()->AddGameObject<GameObject>();
				auto CapsulePtr = dynamic_pointer_cast<PsCapsule>(PtrPsBody->GetPsObject(2));
				if (CapsulePtr) {
					Vec3 Scale(
						CapsulePtr->GetParam().m_Radius * 2.0f,
						0.5f,//���̂̕`��͍���0.5f�ŕ`�悷��
						CapsulePtr->GetParam().m_Radius * 2.0f
					);
					Ptr->GetComponent<Transform>()->SetScale(Scale);
					Ptr->GetComponent<Transform>()->SetQuaternion(PtrPsBody->GetOrientation(2));
					Ptr->GetComponent<Transform>()->SetPosition(PtrPsBody->GetPosition(2));
					auto DrawPtr = Ptr->AddComponent<BcPNTStaticDraw>();
					DrawPtr->SetMeshResource(L"DEFAULT_CAPSULE");
					m_RagdollObjVec.push_back(Ptr);
				}
			}
			{
				//�������������̓��[�v�ō쐬
				for (size_t i = 3; i <= 10; i++) {
					auto Ptr = GetStage()->AddGameObject<GameObject>();
					auto CapsulePtr = dynamic_pointer_cast<PsCapsule>(PtrPsBody->GetPsObject(i));
					if (CapsulePtr) {
						Vec3 Scale(
							CapsulePtr->GetParam().m_Radius * 2.0f,
							CapsulePtr->GetParam().m_HalfLen * 1.5f,
							CapsulePtr->GetParam().m_Radius * 2.0f
						);
						Ptr->GetComponent<Transform>()->SetScale(Scale);
						Ptr->GetComponent<Transform>()->SetQuaternion(PtrPsBody->GetOrientation(i));
						Ptr->GetComponent<Transform>()->SetPosition(PtrPsBody->GetPosition(i));
						auto DrawPtr = Ptr->AddComponent<BcPNTStaticDraw>();
						DrawPtr->SetMeshResource(L"DEFAULT_CAPSULE");
						m_RagdollObjVec.push_back(Ptr);
					}

				}
			}
		}
	}

	void RagdollManager::OnUpdate() {
		auto RagSh = m_Ragdoll.lock();
		if (RagSh) {
			auto PtrPsBody = RagSh->GetComponent<RigidbodyMulti>();
			for (size_t i = 0; i < m_RagdollObjVec.size(); i++) {
				auto ShPtr = m_RagdollObjVec[i].lock();
				if (ShPtr) {
					ShPtr->GetComponent<Transform>()->SetQuaternion(PtrPsBody->GetOrientation(i));
					ShPtr->GetComponent<Transform>()->SetPosition(PtrPsBody->GetPosition(i));
				}
				if (m_Holded && i == m_HoldIndex) {
					auto DrawPtr = ShPtr->GetComponent<BcPNTStaticDraw>();
					DrawPtr->SetEmissive(Col4(1.0f, 0.0f, 0, 1.0f));
				}
				else {
					auto DrawPtr = ShPtr->GetComponent<BcPNTStaticDraw>();
					DrawPtr->SetEmissive(Col4(0.0f, 0.0f, 0, 0.0f));
				}
			}
		}
	}




}
//end basecross
