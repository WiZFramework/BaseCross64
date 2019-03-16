/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//�p�r: �����̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiSpark::MultiSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiSpark::~MultiSpark() {}

	//������
	void MultiSpark::OnCreate() {
	}


	void MultiSpark::InsertSpark(const Vec3& Pos) {
		auto ParticlePtr = InsertParticle(4);
		ParticlePtr->SetEmitterPos(Pos);
		ParticlePtr->SetTextureResource(L"SPARK_TX");
		ParticlePtr->SetMaxTime(0.1f);
		vector<ParticleSprite>& pSpriteVec = ParticlePtr->GetParticleSpriteVec();
		for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//�F�̎w��
			rParticleSprite.m_Color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	//--------------------------------------------------------------------------------------
	//�@�^�C�����O����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	TilingFixedBox::TilingFixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position),
		m_UPic(UPic),
		m_VPic(VPic)
	{}
	TilingFixedBox::~TilingFixedBox() {}
	//������
	void TilingFixedBox::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rotation);
		PtrTrans->SetPosition(m_Position);
		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.z / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				float FrontBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, 1, 0));
				float BackBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, -1, 0));
				if (FrontBetween < 0.01f || BackBetween < 0.01f) {
					vertices[i].textureCoordinate.y = VCount;
				}
			}
		}
		//�`��R���|�[�l���g�̒ǉ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		PtrDraw->SetOwnShadowActive(true);
		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		PtrDraw->SetTextureResource(L"SKY_TX");
		//�^�C�����O�ݒ�
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);

	}


	//--------------------------------------------------------------------------------------
	///	���˂��鋅��
	//--------------------------------------------------------------------------------------
	FireSphere::FireSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity
	) :
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.25f),
		m_ActiveMaxY(-5.0f)
	{}
	FireSphere::~FireSphere() {}

	void FireSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetScale(Vec3(m_Scale));
		ptrTrans->SetQuaternion(Quat());
		ptrTrans->SetPosition(m_Emitter);
		//�R���W������t����
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetSleepActive(true);
		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();
		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetTextureResource(L"BROWN_TX");

		auto group = GetStage()->GetSharedObjectGroup(L"FireSphereGroup");
		group->IntoGroup(GetThis<GameObject>());

	}

	void FireSphere::OnUpdate() {
		auto ptrTrans = GetComponent<Transform>();
		if (ptrTrans->GetPosition().y > m_ActiveMaxY) {
			float elapsedTime = App::GetApp()->GetElapsedTime();
			auto BeforePos = ptrTrans->GetBeforePosition();
			auto Pos = ptrTrans->GetPosition();
			Pos += m_Velocity * elapsedTime;
			ptrTrans->SetPosition(Pos);
			auto ptrColl = GetComponent<CollisionSphere>();
			auto ptrDraw = GetComponent<BcPNTStaticDraw>();
			if (ptrColl->IsSleep()) {
				ptrDraw->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, 1.0f));
			}
			else {
				ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
				auto EnemyPtr = GetStage()->GetSharedGameObject<EnemyBox>(L"EnemyBox");
				auto BonePtr = GetStage()->GetSharedGameObject<BoneChara>(L"BoneChara");
				auto SpherePtr = GetStage()->GetSharedGameObject<EnemySphere>(L"EnemySphere");
				Vec3 HitPoint;
				TRIANGLE tri;
				bool isModelHit = false;
				if (EnemyPtr->IsHitSegmentTriangles(BeforePos, Pos, tri, HitPoint)) {
					//�X�p�[�N�̕��o
					auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark");
					PtrSpark->InsertSpark(HitPoint);
					isModelHit = true;
				}
				else if (BonePtr->IsHitSegmentTriangles(BeforePos, Pos, tri, HitPoint)) {
					//�X�p�[�N�̕��o
					auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark");
					PtrSpark->InsertSpark(HitPoint);
					isModelHit = true;
				}
				else if (SpherePtr->IsHitSegmentTriangles(BeforePos, Pos, tri, HitPoint)) {
					//�X�p�[�N�̕��o
					auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark");
					PtrSpark->InsertSpark(HitPoint);
					isModelHit = true;
				}
				if (isModelHit) {
					m_Velocity.reflect(tri.GetNormal());
					if (m_Velocity.length() > 20.0f) {
						m_Velocity.normalize();
						m_Velocity *= 20.0f;
					}
				}
			}
		}
		else {
			SetUpdateActive(false);
			SetDrawActive(false);
			return;
		}
	}

	void FireSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->ResetPosition(Emitter);
		m_Velocity = Velocity;
		auto ptrGrav = GetComponent<Gravity>();
		ptrGrav->SetGravityVerocityZero();
		SetUpdateActive(true);
		SetDrawActive(true);
	}

	void FireSphere::OnCollisionEnter(const CollisionPair& Pair) {
		auto ptrTrans = GetComponent<Transform>();
		auto shDest = Pair.m_Dest.lock();
		m_Velocity -= shDest->GetVelocity();
		m_Velocity.reflect(Pair.m_SrcHitNormal);
		if (m_Velocity.length() > 20.0f) {
			m_Velocity.normalize();
			m_Velocity *= 20.0f;
		}
	}

	void FireSphere::OnCollisionExcute(const CollisionPair& Pair) {
		auto shDest = Pair.m_Dest.lock();
		if (shDest->IsFixed()) {
			//����
			m_Velocity *= 0.95f;
			if (m_Velocity.length() < 0.05f) {
				m_Velocity = Vec3(0);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	�G�̋�
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	EnemySphere::EnemySphere(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Position(Position)
	{}
	EnemySphere::~EnemySphere(){}
	//������
	void EnemySphere::OnCreate() {
		GetStage()->SetSharedGameObject(L"EnemySphere", GetThis<EnemySphere>());
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(m_Scale));
		ptrTrans->SetRotation(Vec3(0));
		ptrTrans->SetPosition(m_Position);
		//CollisionSphere�Փ˔����t����(Sphere���o���̂���)
		auto ptrColl = AddComponent<CollisionSphere>();
		//�����ɂ��Ă���
		ptrColl->SetUpdateActive(false);
		//�`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		m_MeshRes = MeshResource::CreateMeshResource(vertices, indices,true);
		ptrDraw->SetMeshResource(m_MeshRes);
		//�`�悷��e�N�X�`����ݒ�
		ptrDraw->SetTextureResource(L"WALL_TX");
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto shadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMeshResource(m_MeshRes);

	}

	bool EnemySphere::IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, TRIANGLE& tri, Vec3& HitPoint) {
		SPHERE StartSp;
		StartSp.m_Center = StartPos;
		StartSp.m_Radius = 0.25f;
		SPHERE EndSp;
		EndSp.m_Center = EndPos;
		EndSp.m_Radius = 0.25f;
		auto ptrColl = GetComponent<CollisionSphere>();
		SPHERE sp1 = ptrColl->GetSphere();
		SPHERE sp2 = HitTest::SphereEnclosingSphere(StartSp, EndSp);
		Vec3 ret;
		if (!HitTest::SPHERE_SPHERE(sp1, sp2)) {
			//����O����
			//����ɂ�薳�ʂȔ�����������
			return false;
		}

		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		size_t hitIndex;
		return PtrDraw->HitTestStaticMeshSegmentTriangles(StartPos, EndPos, HitPoint, tri, hitIndex);
	}

	bool EnemySphere::IsHitSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp, TRIANGLE& tri, Vec3& HitPoint) {
		auto ptrColl = GetComponent<CollisionSphere>();
		SPHERE sp1 = ptrColl->GetSphere();
		SPHERE sp2 = HitTest::SphereEnclosingSphere(StartSp, EndSp);
		Vec3 ret;
		if (!HitTest::SPHERE_SPHERE(sp1, sp2)) {
			//����O����
			//����ɂ�薳�ʂȔ�����������
			return false;
		}
		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		size_t hitIndex;
		if (PtrDraw->HitTestStaticMeshSphereTriangles(StartSp, EndSp, HitPoint, tri, hitIndex)) {
			//hitindex�𒸓_�̃C���f�b�N�X�Ɋ��Z
			hitIndex *= 3;
			auto& verVec = m_MeshRes->GetBackupVerteces<VertexPositionNormalTexture>();
			vector<uint16_t>& indexVec = m_MeshRes->GetBackupIndices<VertexPositionNormalTexture>();
			verVec[indexVec[hitIndex]].position *= 0.95;
			verVec[indexVec[hitIndex + 1]].position *= 0.95;
			verVec[indexVec[hitIndex + 2]].position *= 0.95;
			m_MeshRes->UpdateVirtexBuffer(verVec);
			return true;
		}
		return false;
	}



	//--------------------------------------------------------------------------------------
	//	�G�̔�
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	EnemyBox::EnemyBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{}
	EnemyBox::~EnemyBox() {}
	//������
	void EnemyBox::OnCreate() {
		GetStage()->SetSharedGameObject(L"EnemyBox", GetThis<EnemyBox>());
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetRotation(m_Rotation);
		ptrTrans->SetPosition(m_Position);

		//CollisionObb�Փ˔����t����(OBB���o���̂���)
		auto ptrColl = AddComponent<CollisionObb>();
		//�����ɂ��Ă���
		ptrColl->SetUpdateActive(false);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto shadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		//�`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetOwnShadowActive(true);
		//�`�悷�郁�b�V����ݒ�
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�`�悷��e�N�X�`����ݒ�
		ptrDraw->SetTextureResource(L"CHECKER_TX");
	}

	bool EnemyBox::IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, TRIANGLE& tri, Vec3& HitPoint) {
		auto ptrColl = GetComponent<CollisionObb>();
		OBB obb = ptrColl->GetObb();
		SPHERE StartSp;
		StartSp.m_Center = StartPos;
		StartSp.m_Radius = 0.25f;
		SPHERE EndSp;
		EndSp.m_Center = EndPos;
		EndSp.m_Radius = 0.25f;
		SPHERE sp = HitTest::SphereEnclosingSphere(StartSp, EndSp);
		Vec3 ret;
		if (!HitTest::SPHERE_OBB(sp, obb, ret)) {
			//����O����
			//����ɂ�薳�ʂȔ�����������
			return false;
		}
		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		size_t hitIndex;
		return PtrDraw->HitTestStaticMeshSegmentTriangles(StartPos, EndPos, HitPoint, tri, hitIndex);
	}

	bool EnemyBox::IsHitSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp, TRIANGLE& tri, Vec3& HitPoint) {
		auto ptrColl = GetComponent<CollisionObb>();
		OBB obb = ptrColl->GetObb();
		SPHERE sp = HitTest::SphereEnclosingSphere(StartSp, EndSp);
		Vec3 ret;
		if (!HitTest::SPHERE_OBB(sp, obb,ret)) {
			//����O����
			//����ɂ�薳�ʂȔ�����������
			return false;
		}
		auto PtrDraw = GetComponent<BcPNTStaticDraw>();
		size_t hitIndex;
		return PtrDraw->HitTestStaticMeshSphereTriangles(StartSp, EndSp, HitPoint, tri, hitIndex);
	}


	//--------------------------------------------------------------------------------------
	///	Bone�L����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BoneChara::BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	BoneChara::~BoneChara() {}

	//������
	void BoneChara::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(0.5f, 0.5f, 0.5f);
		ptrTrans->SetRotation(0.0f, XM_PI, 0.0f);
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
		ptrShadow->SetMeshResource(L"BONE_MESH_WITH_TAN");
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		//�`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTnTBoneModelDraw>();
		ptrDraw->SetFogEnabled(true);
		//�`�悷�郁�b�V����ݒ�
		ptrDraw->SetMeshResource(L"BONE_MESH_WITH_TAN");
		ptrDraw->SetNormalMapTextureResource(L"BONE_MESH_NORMAL_TX");
		ptrDraw->SetMeshToTransformMatrix(spanMat);
		ptrDraw->SetOwnShadowActive(true);
		ptrDraw->AddAnimation(L"Default", 0, 50, true, 10.0f);
		ptrDraw->ChangeCurrentAnimation(L"Default");
	}

	bool BoneChara::IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, TRIANGLE& tri, Vec3& HitPoint) {
		SPHERE StartSp;
		StartSp.m_Center = StartPos;
		StartSp.m_Radius = 0.25f;
		SPHERE EndSp;
		EndSp.m_Center = EndPos;
		EndSp.m_Radius = 0.25f;
		SPHERE sp = HitTest::SphereEnclosingSphere(StartSp, EndSp);
		if (length(sp.m_Center - GetComponent<Transform>()->GetPosition()) >= 5.0f) {
			//������5.0�ȏ㗣��Ă�����false
			return false;
		}
		auto PtrDraw = GetComponent<BcPNTnTBoneModelDraw>();
		size_t hitIndex;
		return PtrDraw->HitTestSkinedMeshSegmentTriangles(StartPos, EndPos, HitPoint, tri, hitIndex);

	}

	bool BoneChara::IsHitSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp, TRIANGLE& tri, Vec3& HitPoint) {
		SPHERE sp = HitTest::SphereEnclosingSphere(StartSp, EndSp);
		if (length(sp.m_Center - GetComponent<Transform>()->GetPosition()) >= 5.0f) {
			//������5.0�ȏ㗣��Ă�����false
			return false;
		}
		auto PtrDraw = GetComponent<BcPNTnTBoneModelDraw>();
		size_t hitIndex;
		return PtrDraw->HitTestSkinedMeshSphereTriangles(StartSp, EndSp, HitPoint, tri, hitIndex);
	}

	//�X�V
	void BoneChara::OnUpdate() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		//�A�j���[�V�������X�V����
		auto ptrDraw = GetComponent<BcPNTnTBoneModelDraw>();
		ptrDraw->UpdateAnimation(elapsedTime);
	}


	//--------------------------------------------------------------------------------------
	///	Ball�L����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BallChara::BallChara(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	BallChara::~BallChara() {}

	//������
	void BallChara::OnCreate() {
		auto bonePtr = GetStage()->GetSharedGameObject<BoneChara>(L"BoneChara");
		auto boneTrans = bonePtr->GetComponent<Transform>();
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(1.0f, 1.0f, 1.0f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetPosition(boneTrans->GetPosition());

		//�`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		//�`�悷�郁�b�V����ݒ�
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//�`�悷��e�N�X�`����ݒ�
		ptrDraw->SetTextureResource(L"WALL_TX");
	}

	//�X�V
	void BallChara::OnUpdate() {
		auto bonePtr = GetStage()->GetSharedGameObject<BoneChara>(L"BoneChara");
		auto boneTrans = bonePtr->GetComponent<Transform>();
		auto boneDraw = bonePtr->GetComponent<BcPNTnTBoneModelDraw>();


		auto& bones = boneDraw->GetVecLocalBones();
		Mat4x4 mat = bones[2];
		auto mesh = boneDraw->GetMeshResource();
		vector<Vec3> positions;
		mesh->GetLocalPositions(positions);
		Mat4x4 localMat;
		localMat.translation(positions[1]);
		localMat *= mat;
		localMat *= boneTrans->GetWorldMatrix();
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetPosition(localMat.transInMatrix());
		ptrTrans->SetQuaternion(localMat.quatInMatrix());
	}

}
//end basecross
