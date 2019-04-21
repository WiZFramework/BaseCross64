/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//�@�_�ō\�����ꂽ����
	//-----------------------------------------------ix---------------------------------------
	PointsBall::PointsBall(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Position(Position)
	{}

	PointsBall::~PointsBall() {}
	//������
	void PointsBall::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//���̂��쐬
		MeshUtill::CreateSphere(1.0f, 28,vertices, indices);
		for (auto& v : vertices) {
			LocalData tempLocalData;
			tempLocalData.m_LocalPosition = v.position;
			//�e���_���C���X�^���X�̈ʒu�ɐݒ�
			m_LocalDataVec.push_back(tempLocalData);
		}
		//�`��p���b�V���̍쐬
		float helfSize = 0.04f;
		Col4 col(1.0f, 1.0f, 0.0f, 1.0f);
		//���_�z��
		vector<VertexPositionColorTexture> meshVertices = {
			{ VertexPositionColorTexture(Vec3(-helfSize, helfSize, 0),col, Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, helfSize, 0),col, Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-helfSize, -helfSize, 0),col, Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, -helfSize, 0),col, Vec2(1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> meshIndex = { 0, 1, 2, 1, 3, 2 };
		//2�������ʂƂ���i���_�������Ȃ����߁j
		m_MeshRes = MeshResource::CreateMeshResource(meshVertices, meshIndex, true);
		//�S�̂̈ʒu�֘A
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(Vec3(m_Scale));
		ptrTransform->SetRotation(Vec3(0));
		ptrTransform->SetPosition(m_Position);
		//�`��R���|�[�l���g�̒ǉ��i�C���X�^���X�`��j
		auto PtrDraw = AddComponent<PCTStaticInstanceDraw>();
		PtrDraw->SetMeshResource(m_MeshRes);
		PtrDraw->SetTextureResource(L"SPARK_TX");
		PtrDraw->SetDepthStencilState(DepthStencilState::Read);
		//�e���_���Ƃɍs����쐬
		for (auto& v : m_LocalDataVec) {
			Mat4x4 tempMat;
			tempMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Quat(),
				v.m_LocalPosition
			);
			//�C���X�^���X�`��̍s��Ƃ��Đݒ�
			PtrDraw->AddMatrix(tempMat);
		}
		SetAlphaActive(true);
	}

	//�X�e�[�g��Update
	void PointsBall::UpdateState() {
		if (m_MatVec.size() != m_LocalDataVec.size()) {
			//m_MatVec���܂�����������ĂȂ��\��������
			return;
		}
		//�e�萔
		const float baseY = m_Scale * 0.02f;
		const float velocityPower = 3.0f;
		const Vec3 gravity(0, -9.8f, 0);
		float elapsedTime = App::GetApp()->GetElapsedTime();
		auto playerSh = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto playerPos = playerSh->GetComponent<Transform>()->GetWorldPosition();
		//�e���_�Ń��[�v
		for (size_t i = 0; i < m_LocalDataVec.size(); i++) {
			switch (m_LocalDataVec[i].m_State) {
			case LocalState::Roll:
				//��]���Ă�����
				{
					auto len = length(playerPos - m_MatVec[i].transInMatrix());
					if (len < 0.4f) {
						//len��0.4�����Ȃ�Փ˂��Ă�Ɣ��f
						//�X�e�[�g��ύX
						//�Փ˂��Ă����狅�����яo���悤�ɑ��x��ݒ�
						m_LocalDataVec[i].m_State = LocalState::Down;
						m_LocalDataVec[i].m_Velocity = playerPos - m_MatVec[i].transInMatrix();
						m_LocalDataVec[i].m_Velocity.normalize();
						m_LocalDataVec[i].m_Velocity.y = 1.0f;
						m_LocalDataVec[i].m_Velocity *= velocityPower;
					}
				}
				break;
			case LocalState::Down:
				//�������̏��
				if (m_MatVec[i].transInMatrix().y <= baseY) {
					//�����I��
					m_LocalDataVec[i].m_State = LocalState::Fix;
					//�I�����̍s���ۑ�
					//Y�l��0.1�ɂ���
					m_MatVec[i]._42 = baseY;
					//m_LocalDataVec��m_MatVec�͊e�C���X�^���X�͓����C���f�b�N�X�ł���
					m_LocalDataVec[i].m_FixedMatrix = m_MatVec[i];
				}
				else {
					m_LocalDataVec[i].m_Velocity += gravity * elapsedTime;
					m_LocalDataVec[i].m_LocalPosition += m_LocalDataVec[i].m_Velocity * elapsedTime;
				}
				break;
			case LocalState::Fix:
				//�����I���̏��
				break;
			}
		}
	}

	void PointsBall::UpdateInstances() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		auto ptrDraw = GetComponent<PCTStaticInstanceDraw>();
		auto camera = OnGetDrawCamera();
		//�J�����̃��C���쐬���Ă���
		auto lay = camera->GetAt() - camera->GetEye();
		lay.normalize();
		Quat qtCamera;
		//��]�͏�ɃJ�����������悤�ɂ���
		qtCamera.facing(lay);
		auto ptrTransform = GetComponent<Transform>();
		//�S�̂���]������
		auto worldQt = ptrTransform->GetQuaternion();
		Quat spanQt(Vec3(0, 1, 0), elapsedTime);
		worldQt *= spanQt;
		ptrTransform->SetQuaternion(worldQt);
		//�s��̔z����N���A
		m_MatVec.clear();
		Mat4x4 worldMat;
		for (auto& v : m_LocalDataVec) {
			if (v.m_State == LocalState::Fix) {
				//�����I���̏��
				worldMat.affineTransformation(
					v.m_FixedMatrix.scaleInMatrix(),
					Vec3(0.0f),
					qtCamera,
					v.m_FixedMatrix.transInMatrix()
				);
			}
			else {
				Mat4x4 localMat;
				localMat.affineTransformation(
					Vec3(1.0f),
					Vec3(0.0f),
					Quat(),
					v.m_LocalPosition
				);
				worldMat = localMat * ptrTransform->GetWorldMatrix();
				worldMat.affineTransformation(
					worldMat.scaleInMatrix(),
					Vec3(0.0f),
					qtCamera,
					worldMat.transInMatrix()
				);
			}
			m_MatVec.push_back(worldMat);
		}
		//�C���X�^���X�s��̍X�V
		ptrDraw->UpdateMultiMatrix(m_MatVec);
	}

	void PointsBall::OnUpdate() {
		//�X�e�[�g��Update
		UpdateState();
		//�e�C���X�^���X��Update
		UpdateInstances();
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

}
//end basecross
