/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	// �����̃X�p�[�N�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiSpark::MultiSpark(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiSpark::~MultiSpark() {}

	//������
	void MultiSpark::OnCreate() {
	}

	void MultiSpark::OnUpdate() {
		for (auto ptrParticle : GetParticleVec()) {
			for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					rParticleSprite.m_Color.z += 0.05f;
					if (rParticleSprite.m_Color.z >= 1.0f) {
						rParticleSprite.m_Color.z = 1.0f;
					}
				}
			}
		}
		//�e�N���X��OnUpdate()���Ă�
		MultiParticle::OnUpdate();
	}



	void MultiSpark::InsertSpark(const Vec3& Pos) {
		auto ptrParticle = InsertParticle(4);
		ptrParticle->SetEmitterPos(Pos);
		ptrParticle->SetTextureResource(L"SPARK_TX");
		ptrParticle->SetMaxTime(0.5f);
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
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
	///	�����̉��N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiFire::MultiFire(shared_ptr<Stage>& StagePtr) :
		MultiParticle(StagePtr)
	{}
	MultiFire::~MultiFire() {}

	//������
	void MultiFire::OnCreate() {
		//���Z�`�揈��������
		SetAddType(true);
	}

	void MultiFire::InsertFire(const Vec3& Pos) {
		auto ptrParticle = InsertParticle(4);
		ptrParticle->SetEmitterPos(Pos);
		ptrParticle->SetTextureResource(L"FIRE_TX");
		ptrParticle->SetMaxTime(0.5f);
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
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
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

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
	///	�㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
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

	MoveBox::~MoveBox() {}

	//������
	void MoveBox::OnCreate() {
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);
		AddTag(L"MoveBox");

		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//�A�N�V�����̓o�^
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vec3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vec3(-5.0f, -5.0f, 0));
		//���[�v����
		PtrAction->SetLooped(true);
		//�A�N�V�����J�n
		PtrAction->Run();



		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		auto Group = GetStage()->GetSharedObjectGroup(L"MoveBox");
		Group->IntoGroup(GetThis<GameObject>());


	}


	//--------------------------------------------------------------------------------------
	//	�ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_StateChangeSize(5.0f),
		m_Force(0),
		m_Velocity(0)
	{
	}
	SeekObject::~SeekObject() {}

	//������
	void SeekObject::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_StartPos);
		ptrTransform->SetScale(0.125f, 0.25f, 0.25f);
		ptrTransform->SetRotation(0.0f, 0.0f, 0.0f);

		AddTag(L"SeekGroup");

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto group = GetStage()->GetSharedObjectGroup(L"SeekGroup");
		//�O���[�v�Ɏ������g��ǉ�
		group->IntoGroup(GetThis<SeekObject>());
		//Obb�̏Փ˔��������
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetDrawActive(true);
		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();
		//�����s��������
		auto ptrSep = GetBehavior<SeparationSteering>();
		ptrSep->SetGameObjectGroup(group);
		//�e������
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"TRACE_TX");
		//��������������
		SetAlphaActive(true);

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine.reset(new StateMachine<SeekObject>(GetThis<SeekObject>()));
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(FarState::Instance());
	}

	void SeekObject::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//�t�@�C�A�̕��o
		auto ptriFire = GetStage()->GetSharedGameObject<MultiFire>(L"MultiFire", false);
		if (ptriFire) {
			ptriFire->InsertFire(GetComponent<Transform>()->GetPosition());
		}

	}



	//����
	void SeekObject::OnUpdate() {
		m_Force = Vec3(0);
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
		auto ptrUtil = GetBehavior<UtilBehavior>();
		ptrUtil->RotToHead(1.0f);
	}

	void SeekObject::OnUpdate2() {
	}

	Vec3 SeekObject::GetTargetPos()const {
		auto targetPtr = GetStage()->GetSharedObject(L"Player");
		return targetPtr->GetComponent<Transform>()->GetPosition();
	}


	void SeekObject::ApplyForce() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		m_Velocity += m_Force * elapsedTime;
		auto ptrTransform = GetComponent<Transform>();
		auto pos = ptrTransform->GetPosition();
		pos += m_Velocity * elapsedTime;
		ptrTransform->SetPosition(pos);
	}



	//--------------------------------------------------------------------------------------
	//	�v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance() {
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto ptrSeek = Obj->GetBehavior<SeekSteering>();
		auto ptrSep = Obj->GetBehavior<SeparationSteering>();
		auto force = Obj->GetForce();
		force = ptrSeek->Execute(force, Obj->GetVelocity(), Obj->GetTargetPos());
		force += ptrSep->Execute(force);
		Obj->SetForce(force);
		Obj->ApplyForce();
		float f = bsm::length(Obj->GetComponent<Transform>()->GetPosition() - Obj->GetTargetPos());
		if (f < Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}

	void FarState::Exit(const shared_ptr<SeekObject>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	//	�v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance() {
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj) {
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj) {
		auto ptrArrive = Obj->GetBehavior<ArriveSteering>();
		auto ptrSep = Obj->GetBehavior<SeparationSteering>();
		auto force = Obj->GetForce();
		force = ptrArrive->Execute(force, Obj->GetVelocity(), Obj->GetTargetPos());
		force += ptrSep->Execute(force);
		Obj->SetForce(force);
		Obj->ApplyForce();
		float f = bsm::length(Obj->GetComponent<Transform>()->GetPosition() - Obj->GetTargetPos());
		if (f >= Obj->GetStateChangeSize()) {
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj) {
	}


	//--------------------------------------------------------------------------------------
	//	�����̃X�N�G�A
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	NumberSquare::NumberSquare(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<SeekObject>& SeekObjectPtr, size_t Number) :
		GameObject(StagePtr),
		m_SeekObject(SeekObjectPtr),
		m_Number(Number)
	{}
	NumberSquare::~NumberSquare() {}

	//������
	void NumberSquare::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();
		if (!m_SeekObject.expired()) {
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 0.75f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
			PtrTransform->SetQuaternion(SeekTransPtr->GetQuaternion());
			//�ύX�ł���X�N�G�A���\�[�X���쐬

			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Square�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateSquare(1.0f, vertices, indices);
			//UV�l�̕ύX
			float from = ((float)m_Number) / 10.0f;
			float to = from + (1.0f / 10.0f);
			//���㒸�_
			vertices[0].textureCoordinate = Vec2(from, 0);
			//�E�㒸�_
			vertices[1].textureCoordinate = Vec2(to, 0);
			//�������_
			vertices[2].textureCoordinate = Vec2(from, 1.0f);
			//�E�����_
			vertices[3].textureCoordinate = Vec2(to, 1.0f);
			//���_�̌^��ς����V�������_���쐬
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : vertices) {
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//�V�������_���g���ă��b�V�����\�[�X�̍쐬
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(L"NUMBER_TX");
			SetAlphaActive(true);
			SetDrawLayer(1);
		}

	}


	//�ω�
	void NumberSquare::OnUpdate() {

		if (!m_SeekObject.expired()) {
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 0.75f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);

			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();

			Quat Qt;
			//�������r���{�[�h�ɂ���
			Qt = Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			PtrTransform->SetQuaternion(Qt);

		}

	}




	//--------------------------------------------------------------------------------------
	///	�^�C�g���X�v���C�g
	//--------------------------------------------------------------------------------------
	TitleSprite::TitleSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec2& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos)
	{}

	TitleSprite::~TitleSprite() {}
	void TitleSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��(�c��5���\��)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	//--------------------------------------------------------------------------------------
	///	�A�j���X�v���C�g
	//--------------------------------------------------------------------------------------
	AnimeSprite::AnimeSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec2& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0.0f)
	{}

	AnimeSprite::~AnimeSprite() {}

	void AnimeSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��
		vector<VertexPositionColorTexture> vertex = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertex, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	void AnimeSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime * 5.0f;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		auto PtrDraw = GetComponent<PCTSpriteDraw>();
		Col4 col(1.0, 1.0, 1.0, 1.0);
		col.w = sin(m_TotalTime) * 0.5f + 0.5f;
		PtrDraw->SetDiffuse(col);
	}

	//--------------------------------------------------------------------------------------
	///	�A�ԃA�j���X�v���C�g
	//--------------------------------------------------------------------------------------
	SerialAnimeSprite::SerialAnimeSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec2& StartPos, uint32_t PieceXCount, uint32_t PieceYCount, float AnimeTime) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0.0f),
		m_PieceXCount(PieceXCount),
		m_PieceYCount(PieceYCount),
		m_PieceIndex(0),
		m_AnimeTime(AnimeTime)

	{}

	SerialAnimeSprite::~SerialAnimeSprite() {}

	void SerialAnimeSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��(�c��5���\��)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto PtrDraw = AddComponent<PCTSpriteDraw>(m_BackupVertices, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	void SerialAnimeSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= m_AnimeTime) {
			m_PieceIndex++;
			if (m_PieceIndex >= m_PieceXCount * m_PieceYCount) {
				m_PieceIndex = 0;
			}
			m_TotalTime = 0;
		}
		vector<VertexPositionColorTexture> newVertices;
		uint32_t pieceX = m_PieceIndex % m_PieceXCount;
		uint32_t pieceY = m_PieceIndex / m_PieceXCount;
		float pieceWidth = 1.0f / (float)m_PieceXCount;
		float pieceHeight = 1.0f / (float)m_PieceYCount;

		float pieceStartX = (float)pieceX * pieceWidth;
		float pieceEndX = pieceStartX + pieceWidth;

		float pieceStartY = (float)pieceY * pieceHeight;
		float pieceEndY = pieceStartY + pieceHeight;

		for (size_t i = 0; i < m_BackupVertices.size(); i++) {
			Vec2 uv = m_BackupVertices[i].textureCoordinate;
			switch (i) {
			case 0:
				uv.x = pieceStartX;
				uv.y = pieceStartY;
				break;
			case 1:
				uv.x = pieceEndX;
				uv.y = pieceStartY;
				break;
			case 2:
				uv.x = pieceStartX;
				uv.y = pieceEndY;
				break;
			case 3:
				uv.x = pieceEndX;
				uv.y = pieceEndY;
				break;
			}
			auto v = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				m_BackupVertices[i].color,
				uv
			);
			newVertices.push_back(v);
		}
		auto ptrDraw = GetComponent<PCTSpriteDraw>();
		ptrDraw->UpdateVertices(newVertices);
	}




}
//end basecross


