/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox() {}

	//������
	void FixedBox::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetRotation(m_Rotation);
		ptrTrans->SetPosition(m_Position);
		//OBB�Փ�j�����t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//�^�O������
		AddTag(L"FixedBox");
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ptrShadow = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetTextureResource(L"SKY_TX");
		ptrDraw->SetOwnShadowActive(true);

	}


	//--------------------------------------------------------------------------------------
	///	PC��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PcSphere::PcSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PcSphere::~PcSphere() {}

	//������
	void PcSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//�`��R���|�[�l���g
		auto ptrDraw = AddComponent<BcPCStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionColor> newVertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionColor newV;
			newV.position = vertices[i].position;
			newV.color = Col4(
				newV.position.x * 2.0f,
				newV.position.y * 2.0f,
				newV.position.z * 2.0f,
				1.0f);
			newVertices.push_back(newV);

		}
		ptrDraw->CreateOriginalMesh(newVertices, indices);
		ptrDraw->SetOriginalMeshUse(true);
		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(ptrDraw->GetOriginalMeshResource());
	}

	void PcSphere::OnUpdate() {
		auto beh = GetBehavior<VertexBehavior>();
		beh->ExpandAndContract<VertexPositionColor, BcPCStaticDraw>();
	}

	//--------------------------------------------------------------------------------------
	///	PN��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PnSphere::PnSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PnSphere::~PnSphere() {}

	//������
	void PnSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//�`��R���|�[�l���g
		auto ptrDraw = AddComponent<BcPNStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionNormal> newVertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionNormal newV;
			newV.position = vertices[i].position;
			newV.normal = vertices[i].normal;
			newVertices.push_back(newV);

		}
		ptrDraw->CreateOriginalMesh(newVertices, indices);
		ptrDraw->SetOriginalMeshUse(true);
		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(ptrDraw->GetOriginalMeshResource());
	}

	void PnSphere::OnUpdate() {
		auto beh = GetBehavior<VertexBehavior>();
		beh->ExpandAndContract<VertexPositionNormal, BcPNStaticDraw>();
	}


	//--------------------------------------------------------------------------------------
	///	PT��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PtSphere::PtSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PtSphere::~PtSphere() {}

	//������
	void PtSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//�`��R���|�[�l���g
		auto ptrDraw = AddComponent<BcPTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionTexture> newVertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionTexture newV;
			newV.position = vertices[i].position;
			newV.textureCoordinate = vertices[i].textureCoordinate;
			newVertices.push_back(newV);
		}
		ptrDraw->CreateOriginalMesh(newVertices, indices);
		ptrDraw->SetOriginalMeshUse(true);
		ptrDraw->SetTextureResource(L"SKY_TX");
		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(ptrDraw->GetOriginalMeshResource());
	}
	void PtSphere::OnUpdate() {
		auto beh = GetBehavior<VertexBehavior>();
		beh->ExpandAndContract<VertexPositionTexture, BcPTStaticDraw>();
	}


	//--------------------------------------------------------------------------------------
	///	PCT��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PctSphere::PctSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PctSphere::~PctSphere() {}

	//������
	void PctSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//�`��R���|�[�l���g
		auto ptrDraw = AddComponent<BcPCTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionColorTexture> newVertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionColorTexture newV;
			newV.position = vertices[i].position;
			newV.textureCoordinate = vertices[i].textureCoordinate;
			newV.color = Col4(
				newV.position.x * 2.0f,
				newV.position.y * 2.0f,
				newV.position.z * 2.0f,
				1.0f);
			newVertices.push_back(newV);
		}
		ptrDraw->CreateOriginalMesh(newVertices, indices);
		ptrDraw->SetOriginalMeshUse(true);
		ptrDraw->SetTextureResource(L"SKY_TX");
		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(ptrDraw->GetOriginalMeshResource());
	}

	void PctSphere::OnUpdate() {
		auto beh = GetBehavior<VertexBehavior>();
		beh->ExpandAndContract<VertexPositionColorTexture, BcPCTStaticDraw>();
	}



	//--------------------------------------------------------------------------------------
	///	Pnt��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PntSphere::PntSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TextureUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TextureUse(TextureUse)
	{
	}
	PntSphere::~PntSphere() {}

	//������
	void PntSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");
		//�`��R���|�[�l���g
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetOwnShadowActive(true);
		if (m_TextureUse) {
			ptrDraw->SetTextureResource(L"SKY_TX");
		}
	}

	//--------------------------------------------------------------------------------------
	///	Pnt�X�y�L�����[��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	PntSpSphere::PntSpSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TextureUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TextureUse(TextureUse)
	{
	}
	PntSpSphere::~PntSpSphere() {}
	//������
	void PntSpSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		//�`��R���|�[�l���g
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetSpecularColorAndPower(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		if (m_TextureUse) {
			ptrDraw->SetTextureResource(L"SKY_TX");
		}
	}


	//--------------------------------------------------------------------------------------
	///	Static�L����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	StaticChara::StaticChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{
	}
	StaticChara::~StaticChara() {}
	//������
	void StaticChara::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(0.5f, 0.5f, 0.5f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetPosition(m_StartPos);

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ptrShadow = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ptrShadow->SetMeshResource(L"MODEL_MESH");
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		if (m_TamgentUse) {
			auto ptrDraw = AddComponent<BcPNTnTStaticModelDraw>();
			ptrDraw->SetFogEnabled(true);
			ptrDraw->SetMeshResource(L"MODEL_MESH_WITH_TAN");
			ptrDraw->SetNormalMapTextureResource(L"MODEL_NORMAL_TX");
			ptrDraw->SetMeshToTransformMatrix(spanMat);
		}
		else {
			auto ptrDraw = AddComponent<BcPNTStaticModelDraw>();
			ptrDraw->SetFogEnabled(true);
			ptrDraw->SetMeshResource(L"MODEL_MESH");
			ptrDraw->SetMeshToTransformMatrix(spanMat);
		}
	}

	//--------------------------------------------------------------------------------------
	///	Static�L����(�}���`���b�V����)
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	StaticMultiMeshChara::StaticMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{}
	StaticMultiMeshChara::~StaticMultiMeshChara() {}

	//������
	void StaticMultiMeshChara::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(0.5f, 0.5f, 0.5f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetPosition(m_StartPos);

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ptrShadow = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ptrShadow->SetMultiMeshResource(L"ObjectOnly_MESH");
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		if (m_TamgentUse) {
			auto ptrDraw = AddComponent<BcPNTnTStaticModelDraw>();
			ptrDraw->SetFogEnabled(true);
			ptrDraw->SetMultiMeshResource(L"ObjectOnly_MESH_WITH_TAN");
			ptrDraw->SetNormalMapTextureResource(L"OBJECT_NORMAL_TX");
			ptrDraw->SetMeshToTransformMatrix(spanMat);
		}
		else {
			auto ptrDraw = AddComponent<BcPNTStaticModelDraw>();
			ptrDraw->SetFogEnabled(true);
			ptrDraw->SetMultiMeshResource(L"ObjectOnly_MESH");
			ptrDraw->SetMeshToTransformMatrix(spanMat);
		}
	}




	//--------------------------------------------------------------------------------------
	///	Bone�L����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BoneChara::BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{
	}
	BoneChara::~BoneChara() {}

	//������
	void BoneChara::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(0.5f, 0.5f, 0.5f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetPosition(m_StartPos);

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ptrShadow = AddComponent<Shadowmap>();

		//�e�̌`�i���b�V���j��ݒ�
		ptrShadow->SetMeshResource(L"Chara_R_MESH");
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		if (m_TamgentUse) {
			//�`��R���|�[�l���g�̐ݒ�
			auto ptrDraw = AddComponent<BcPNTnTBoneModelDraw>();
			ptrDraw->SetFogEnabled(true);
			//�`�悷�郁�b�V����ݒ�
			ptrDraw->SetMeshResource(L"Chara_R_MESH_WITH_TAN");
			ptrDraw->SetNormalMapTextureResource(L"Chara_R_NORMAL_TX");
			ptrDraw->SetMeshToTransformMatrix(spanMat);
			ptrDraw->AddAnimation(L"Default", 0, 100, true, 20.0f);
			ptrDraw->ChangeCurrentAnimation(L"Default");
		}
		else {
			//�`��R���|�[�l���g�̐ݒ�
			auto ptrDraw = AddComponent<BcPNTBoneModelDraw>();
			ptrDraw->SetFogEnabled(true);
			//�`�悷�郁�b�V����ݒ�
			ptrDraw->SetMeshResource(L"Chara_R_MESH");
			ptrDraw->SetMeshToTransformMatrix(spanMat);

			ptrDraw->AddAnimation(L"Default", 0, 100, true, 20.0f);
			ptrDraw->ChangeCurrentAnimation(L"Default");

		}
		//��������
		SetAlphaActive(true);
	}

	//�X�V
	void BoneChara::OnUpdate() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		//�A�j���[�V�������X�V����
		if (m_TamgentUse) {
			auto ptrDraw = GetComponent<BcPNTnTBoneModelDraw>();
			ptrDraw->UpdateAnimation(elapsedTime);
		}
		else {
			auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();
			ptrDraw->UpdateAnimation(elapsedTime);
		}
	}


	//--------------------------------------------------------------------------------------
	///	Bone�L����(�}���`���b�V����)
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BoneMultiMeshChara::BoneMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{
	}
	BoneMultiMeshChara::~BoneMultiMeshChara() {}

	//������
	void BoneMultiMeshChara::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(0.5f, 0.5f, 0.5f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetPosition(m_StartPos);

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ptrShadow = AddComponent<Shadowmap>();

		//�e�̌`�i���b�V���j��ݒ�
		ptrShadow->SetMultiMeshResource(L"Object_WalkAnimation_MESH");
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		if (m_TamgentUse) {
			//�`��R���|�[�l���g�̐ݒ�
			auto ptrDraw = AddComponent<BcPNTnTBoneModelDraw>();
			ptrDraw->SetFogEnabled(true);
			//�`�悷�郁�b�V����ݒ�
			ptrDraw->SetMultiMeshResource(L"Object_WalkAnimation_MESH_WITH_TAN");
			ptrDraw->SetNormalMapTextureResource(L"OBJECT_NORMAL_TX");
			ptrDraw->SetMeshToTransformMatrix(spanMat);
			ptrDraw->AddAnimation(L"Default", 0, 30, true, 10.0f);
			ptrDraw->ChangeCurrentAnimation(L"Default");
		}
		else {
			//�`��R���|�[�l���g�̐ݒ�
			auto ptrDraw = AddComponent<BcPNTBoneModelDraw>();
			ptrDraw->SetFogEnabled(true);
			//�`�悷�郁�b�V����ݒ�
			ptrDraw->SetMultiMeshResource(L"Object_WalkAnimation_MESH");
			ptrDraw->SetSamplerState(SamplerState::LinearWrap);
			ptrDraw->SetMeshToTransformMatrix(spanMat);

			ptrDraw->AddAnimation(L"Default", 0, 30, true, 10.0f);
			ptrDraw->ChangeCurrentAnimation(L"Default");

		}
	}

	//�X�V
	void BoneMultiMeshChara::OnUpdate() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		//�A�j���[�V�������X�V����
		if (m_TamgentUse) {
			auto ptrDraw = GetComponent<BcPNTnTBoneModelDraw>();
			ptrDraw->UpdateAnimation(elapsedTime);

		}
		else {
			auto ptrDraw = GetComponent<BcPNTBoneModelDraw>();
			ptrDraw->UpdateAnimation(elapsedTime);
		}
	}



}
//end basecross
