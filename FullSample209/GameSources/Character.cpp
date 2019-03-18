/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

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
	//�@�G
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Enemy::Enemy(const shared_ptr<Stage>& StagePtr, 
		const shared_ptr<StageCellMap>& CellMap, 
		const Vec3& Position):
		GameObject(StagePtr),
		m_CelMap(CellMap),
		m_StartPosition(Position),
		m_Force(0),
		m_Velocity(0)
	{}

	Enemy::~Enemy(){}
	//������
	void Enemy::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptr = GetComponent<Transform>();
		ptr->SetScale(0.5f, 0.5f, 0.5f);
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(m_StartPosition);
		//CollisionSphere�Փ˔����t����
		AddComponent<CollisionSphere>();
		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			AddComponent<PathSearch>(MapPtr);
		}
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto shadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//�`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		//�`�悷�郁�b�V����ݒ�
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetDiffuse(Col4(1.0f, 0, 0, 1.0f));
	}

	void Enemy::OnUpdate() {
		m_Force.setAll(0.0f);
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		PlayerPos.y = GetStartPosition().y;
		if (SeekBehavior(PlayerPos) == CellSearchFlg::Failed) {
			if (SeekBehavior(GetStartPosition()) == CellSearchFlg::Arrived) {
				ArriveBehavior(GetStartPosition());
			}
		}
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_Velocity += m_Force * ElapsedTime;
		auto EnemyPos = GetComponent<Transform>()->GetPosition();
		if (length(EnemyPos - PlayerPos) <= 1.8f) {
			m_Velocity *= 0.95f;
		}
		auto Pos = GetComponent<Transform>()->GetPosition();
		Pos += m_Velocity * ElapsedTime;
		GetComponent<Transform>()->SetPosition(Pos);
	}

	bool Enemy::Search(const Vec3& TargetPos) {
		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			auto PathPtr = GetComponent<PathSearch>();
			m_CellPath.clear();
			//�p�X������������
			if (PathPtr->SearchCell(TargetPos, m_CellPath)) {
				//��������������
				m_CellIndex = 0;
				m_TargetCellIndex = (int)(m_CellPath.size() - 1);
				if (m_CellIndex == m_TargetCellIndex) {
					//���łɓ����Z���ɂ���
					m_NextCellIndex = m_CellIndex;
				}
				else {
					//����Ă���
					m_NextCellIndex = m_CellIndex + 1;

				}
				return true;
			}
			else {
				//���s����
				m_CellIndex = -1;
				m_NextCellIndex = -1;
				m_TargetCellIndex = -1;
			}
		}
		return false;
	}

	CellSearchFlg Enemy::SeekBehavior(const Vec3& TargetPos) {
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto MapPtr = m_CelMap.lock();
		if (!MapPtr) {
			return CellSearchFlg::Failed;
		}
		if (Search(TargetPos)) {
			auto PtrSeek = GetBehavior<SeekSteering>();
			auto PtrArrive = GetBehavior<ArriveSteering>();
			if (m_NextCellIndex == 0) {
				m_Force = PtrArrive->Execute(m_Force, m_Velocity, TargetPos);
				//��������
				return CellSearchFlg::Arrived;
			}
			else {
				AABB ret;
				MapPtr->FindAABB(m_CellPath[m_NextCellIndex], ret);
				auto Pos = ret.GetCenter();
				Pos.y = TargetPos.y;
				m_Force = PtrSeek->Execute(m_Force, m_Velocity,Pos);
				return CellSearchFlg::Seek;
			}
		}
		else {
			auto PtrSeek = GetBehavior<SeekSteering>();
			CellIndex PlayerCell;
			if (MapPtr->FindCell(TargetPos, PlayerCell)) {
				AABB ret;
				MapPtr->FindAABB(PlayerCell, ret);
				auto Pos = ret.GetCenter();
				Pos.y = TargetPos.y;
				m_Force = PtrSeek->Execute(m_Force, m_Velocity, Pos);
				return CellSearchFlg::Seek;
			}
		}
		return CellSearchFlg::Failed;
	}

	void Enemy::ArriveBehavior(const Vec3& TargetPos) {
		auto PtrArrive = GetBehavior<ArriveSteering>();
		m_Force = PtrArrive->Execute(m_Force, m_Velocity, TargetPos);
	}


}
//end basecross
