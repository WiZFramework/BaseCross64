/*!
@file Components.cpp
@brief �R���|�[�l���g����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct Component::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Component::Impl {
		weak_ptr<GameObject> m_GameObject;
		bool m_UpdateActive{ true };	//update���邩�ǂ���
		bool m_DrawActive{ true };		//Draw���邩�ǂ���
		explicit Impl(const shared_ptr<GameObject>& GameObjectPtr) :
			m_GameObject(GameObjectPtr)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class Component :public Object;
	//	�p�r: �R���|�[�l���g���N���X
	//--------------------------------------------------------------------------------------
	Component::Component(const shared_ptr<GameObject>& GameObjectPtr) :
		ObjectInterface(),
		ShapeInterface(),
		pImpl(new Impl(GameObjectPtr))
	{}
	Component::~Component() {}
	//�A�N�Z�T
	shared_ptr<GameObject> Component::GetGameObject() const {
		auto shptr = pImpl->m_GameObject.lock();
		if (!shptr) {
			throw BaseException(
				L"GameObject�͗L���ł͂���܂���",
				L"if (!shptr)",
				L"Component::GetGameObject()"
			);
		}
		else {
			return shptr;
		}
	}

	void Component::AttachGameObject(const shared_ptr<GameObject>& GameObjectPtr) {
		pImpl->m_GameObject = GameObjectPtr;
	}
	shared_ptr<Stage> Component::GetStage() const {
		return GetGameObject()->GetStage();
	}


	bool Component::IsUpdateActive() const {
		return pImpl->m_UpdateActive;
	}
	bool Component::GetUpdateActive() const {
		return pImpl->m_UpdateActive;
	}
	void Component::SetUpdateActive(bool b) {
		pImpl->m_UpdateActive = b;
	}

	bool Component::IsDrawActive() const {
		return pImpl->m_DrawActive;
	}
	bool Component::GetDrawActive() const {
		return pImpl->m_DrawActive;
	}
	void Component::SetDrawActive(bool b) {
		pImpl->m_DrawActive = b;
	}

	//--------------------------------------------------------------------------------------
	//	struct Transform::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Transform::Impl {
		//1�O�̕ϐ�
		bsm::Vec3 m_BeforeScale;
		bsm::Vec3 m_BeforePivot;
		bsm::Quat m_BeforeQuaternion;
		bsm::Vec3 m_BeforePosition;
		//���݂̕ϐ�
		bsm::Vec3 m_Scale;
		bsm::Vec3 m_Pivot;
		bsm::Quat m_Quaternion;
		bsm::Vec3 m_Position;
		//�e�I�u�W�F�N�g
		weak_ptr<GameObject> m_Parent;
		Impl():
			//�X�P�[���̂ݏ������i���̓f�t�H���g�����ł悢�j
			m_BeforeScale(1.0f,1.0f,1.0f),
			m_Scale(1.0f, 1.0f, 1.0f)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class Transform : public Component ;
	//	�p�r: �ω�
	//--------------------------------------------------------------------------------------
	Transform::Transform(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr), pImpl(new Impl())
	{}
	Transform::~Transform() {}

	//�A�N�Z�T
	//BeforeGetter
	bsm::Vec3 Transform::GetBeforeScale() const {
		return pImpl->m_BeforeScale;
	}

	bsm::Vec3 Transform::GetBeforePivot() const{
		return pImpl->m_BeforePivot;
	}

	bsm::Quat Transform::GetBeforeQuaternion() const {
		return pImpl->m_BeforeQuaternion;
	}

	bsm::Vec3 Transform::GetBeforeRotation() const {
		return pImpl->m_BeforeQuaternion.toRotVec();
	}

	bsm::Vec3 Transform::GetBeforePosition() const {
		return pImpl->m_BeforePosition;
	}

	bool Transform::IsSameBeforeWorldMatrix(const bsm::Mat4x4& mat) const {
		return mat.equalInt(GetBeforeWorldMatrix());
	}


	const bsm::Mat4x4 Transform::GetBeforeWorldMatrix() const{
		auto ParPtr = GetParent();
		Mat4x4 BefWorld;
		BefWorld.affineTransformation(
				pImpl->m_BeforeScale,
				pImpl->m_BeforePivot,
				pImpl->m_BeforeQuaternion,
				pImpl->m_BeforePosition
			);
		if (ParPtr) {
			auto ParBeforeWorld = ParPtr->GetComponent<Transform>()->GetBeforeWorldMatrix();
			ParBeforeWorld.scaleIdentity();
			BefWorld *= ParBeforeWorld;
		}
		return BefWorld;
	}


	//Getter&Setter

	bsm::Vec3 Transform::GetScale() const {
		return pImpl->m_Scale;
	}

	void Transform::SetScale(const bsm::Vec3& Scale) {
		pImpl->m_Scale = Scale;
	}
	void Transform::SetScale(float x, float y, float z) {
		SetScale(bsm::Vec3(x, y, z));
	}

	bsm::Vec3 Transform::GetPivot() const {
		return pImpl->m_Pivot;
	}
	void Transform::SetPivot(const bsm::Vec3& Pivot) {
		pImpl->m_Pivot = Pivot;
	}
	void Transform::SetPivot(float x, float y, float z) {
		SetPivot(bsm::Vec3(x, y, z));
	}

	bsm::Quat Transform::GetQuaternion() const {
		return pImpl->m_Quaternion;
	}
	void Transform::SetQuaternion(const bsm::Quat& quaternion) {
		pImpl->m_Quaternion = quaternion;
		pImpl->m_Quaternion.normalize();
	}
	bsm::Vec3 Transform::GetRotation() const {
		bsm::Vec3 r = pImpl->m_Quaternion.toRotVec();
		r.normalize();
		return r;
	}

	void Transform::SetRotation(const bsm::Vec3& Rot) {
		bsm::Quat Qt;
		Qt.rotationRollPitchYawFromVector(Rot);
		SetQuaternion(Qt);
	}
	void Transform::SetRotation(float x, float y, float z) {
		SetRotation(bsm::Vec3(x, y, z));
	}

	bsm::Vec3 Transform::GetPosition() const {
		return pImpl->m_Position;
	}

	void Transform::SetPosition(const bsm::Vec3& Position) {
		pImpl->m_Position = Position;
	}
	void Transform::SetPosition(float x, float y, float z) {
		SetPosition(bsm::Vec3(x, y, z));
	}

	void Transform::ResetPosition(const bsm::Vec3& Position) {
		pImpl->m_BeforePosition = Position;
		pImpl->m_Position = Position;
	}

	bsm::Vec3 Transform::GetWorldPosition() const {
		return GetWorldMatrix().transInMatrix();
	}
	void Transform::SetWorldPosition(const bsm::Vec3& Position) {
		auto SetPos = Position;
		auto ParPtr = GetParent();
		if (ParPtr) {
			auto ParWorldPos = ParPtr->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			SetPos -= ParWorldPos;
			auto ParQt = ParPtr->GetComponent<Transform>()->GetWorldMatrix().quatInMatrix();
			ParQt = bsm::inverse(ParQt);
			bsm::Mat4x4 ParQtMat(ParQt);
			SetPos *= ParQtMat;
		}
		SetPosition(SetPos);
	}
	void Transform::ResetWorldPosition(const bsm::Vec3& Position) {
		auto SetPos = Position;
		auto ParPtr = GetParent();
		if (ParPtr) {
			auto ParWorldPos = ParPtr->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			SetPos -= ParWorldPos;
			auto ParQt = ParPtr->GetComponent<Transform>()->GetWorldMatrix().quatInMatrix();
			ParQt = bsm::inverse(ParQt);
			bsm::Mat4x4 ParQtMat(ParQt);
			SetPos *= ParQtMat;
		}
		ResetPosition(SetPos);
	}

	bool Transform::IsSameWorldMatrix(const bsm::Mat4x4& mat) const {
		return mat.equalInt(GetWorldMatrix());
	}


	const bsm::Mat4x4 Transform::GetWorldMatrix() const{
		auto ParPtr = GetParent();
		Mat4x4 WorldMat;
		WorldMat.affineTransformation(
				pImpl->m_Scale,
				pImpl->m_Pivot,
				pImpl->m_Quaternion,
				pImpl->m_Position
			);
		if (ParPtr) {
			auto ParWorld = ParPtr->GetComponent<Transform>()->GetWorldMatrix();
			ParWorld.scaleIdentity();
			WorldMat *= ParWorld;
		}
		return WorldMat;
	}

	const bsm::Mat4x4 Transform::Get2DWorldMatrix() const {
		auto ParPtr = GetParent();
		pImpl->m_Scale.z = 1.0f;
		bsm::Vec4 temp_z(pImpl->m_Position.z);
		temp_z = XMVector4ClampLength(temp_z, 0.0f, 1.0f);
		pImpl->m_Position.z = temp_z.z;
		pImpl->m_Pivot.z = 0;
		Mat4x4 WorldMat;
		WorldMat.affineTransformation(
			pImpl->m_Scale,
			pImpl->m_Pivot,
			pImpl->m_Quaternion,
			pImpl->m_Position
		);
		if (ParPtr) {
			auto ParWorld = ParPtr->GetComponent<Transform>()->Get2DWorldMatrix();
			ParWorld.scaleIdentity();
			WorldMat *= ParWorld;
		}
		return WorldMat;
	}


	const shared_ptr<GameObject> Transform::GetParent()const {
		auto ShPtr = pImpl->m_Parent.lock();
		if (ShPtr) {
			return ShPtr;
		}
		return nullptr;
	}
	void Transform::SetParent(const shared_ptr<GameObject>& Obj) {
		if (GetParent() == Obj) {
			return;
		}
		if (Obj) {
			ClearParent();
			pImpl->m_Parent = Obj;
			auto ParWorld = Obj->GetComponent<Transform>()->GetWorldMatrix();
			ParWorld.scaleIdentity();
			auto PosSpan = GetPosition() - ParWorld.transInMatrix();
			auto QtSpan = ParWorld.quatInMatrix();
			QtSpan = bsm::inverse(QtSpan);
			bsm::Mat4x4 ParQtMat(QtSpan);
			PosSpan *= ParQtMat;

			bsm::Mat4x4 Mat = GetWorldMatrix() * ParWorld;
			bsm::Vec3 Scale, Pos;
			bsm::Quat Qt;
			Mat.decompose(Scale, Qt, Pos);
			SetScale(Scale);
			SetQuaternion(Qt);
			SetPosition(PosSpan);
			SetToBefore();

		}
		else {
			//nullptr���n���ꂽ
			ClearParent();
		}
	}

	void Transform::ClearParent() {
		if (auto ParPtr = GetParent()) {
			auto Pos = GetWorldPosition();
			SetPosition(Pos);
			SetToBefore();
		}
		pImpl->m_Parent.reset();
	}
	bsm::Vec3 Transform::GetVelocity() const {
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		bsm::Vec3 Velocity = pImpl->m_Position - pImpl->m_BeforePosition;
		Velocity /= ElapsedTime;
		return Velocity;
	}
	float Transform::GetMoveSize() const {
		bsm::Vec3 Move = pImpl->m_Position - pImpl->m_BeforePosition;
		return bsm::length(Move);
	}

	void Transform::SetToBefore() {
		if (pImpl->m_BeforeScale != pImpl->m_Scale) {
			pImpl->m_BeforeScale = pImpl->m_Scale;
		}
		if (pImpl->m_BeforePivot != pImpl->m_Pivot) {
			pImpl->m_BeforePivot = pImpl->m_Pivot;
		}
		if (pImpl->m_BeforeQuaternion != pImpl->m_Quaternion) {
			pImpl->m_BeforeQuaternion = pImpl->m_Quaternion;
		}
		if (pImpl->m_BeforePosition != pImpl->m_Position) {
			pImpl->m_BeforePosition = pImpl->m_Position;
		}
	}

	bsm::Vec3 Transform::GetForword() const {
		bsm::Vec3 ret = GetWorldMatrix().rotZInMatrix();
		ret.normalize();
		return ret;
	}

	bsm::Vec3 Transform::GetUp() const {
		bsm::Vec3 ret = GetWorldMatrix().rotYInMatrix();
		ret.normalize();
		return ret;
	}
	bsm::Vec3 Transform::GetRight() const {
		bsm::Vec3 ret = GetWorldMatrix().rotXInMatrix();
		ret.normalize();
		return ret;
	}



	//����
	void Transform::OnUpdate() {
	}

	struct CellNode {
		int x;
		int z;
		int px;
		int pz;
		int cost;
		int heuristic;
		int score;
	};


	//--------------------------------------------------------------------------------------
	//	struct PathSearch::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct PathSearch::Impl {
		weak_ptr<StageCellMap> m_StageCellMap;
		//�ڕW�̃Z���C���f�b�N�X
		CellIndex m_TargetIndex;
		//�����瑤�̃Z���C���f�b�N�X
		CellIndex m_BaseIndex;
		vector<CellNode> m_OpenVec;
		vector<CellNode> m_CloseVec;
		vector<vector<int>> m_WayArr = {
			{ -1, -1 },
			{ 0, -1 },
			{ 1, -1 },
			{ 1,  0 },
			{ 1,  1 },
			{ 0,  1 },
			{ -1,  1 },
			{ -1,  0 },
		};

		static bool NodeAsc(const CellNode& left, const CellNode& right) {
			return left.score < right.score;
		}
		bool IsInOpen(const CellIndex& Chk);
		bool AddOpenNode(const CellIndex& Base, const CellIndex& Target);
		int GetHeuristic(const CellIndex& Base, const CellIndex& Target);
		bool IsInVector(const CellIndex& Base, const vector<CellNode>& vec, CellNode& retNode);
		void InOpenScore(const CellIndex& Base, vector<CellNode>& retvec);
		bool SearchCellBase(const CellIndex& Start, const CellIndex& Target);
		shared_ptr<StageCellMap> GetStageCellMap() const;


		Impl()
		{}
		~Impl() {}
	};

	int PathSearch::Impl::GetHeuristic(const CellIndex& Base, const CellIndex& Target) {
		int spanX = abs((int)Base.x - (int)Target.x);
		int spanZ = abs((int)Base.z - (int)Target.z);
		//�ǂ��炩�����ق���Ԃ�
		return (spanX >= spanZ) ? spanX : spanZ;
	}

	bool PathSearch::Impl::IsInVector(const CellIndex& Base, const vector<CellNode>& vec, CellNode& retNode) {
		for (auto& v : vec) {
			if (Base.x == v.x && Base.z == v.z) {
				retNode = v;
				return true;
			}
		}
		return false;
	}

	bool PathSearch::Impl::IsInOpen(const CellIndex& Chk) {
		auto it = m_OpenVec.begin();
		while (it != m_OpenVec.end()) {
			if (it->x == Chk.x && it->z == Chk.z) {
				return true;
			}
			it++;
		}
		return false;
	}

	void PathSearch::Impl::InOpenScore(const CellIndex& Base, vector<CellNode>& retvec) {
		for (size_t i = 0; i < m_WayArr.size(); i++) {
			int posx = Base.x + m_WayArr[i][0];
			int posz = Base.z + m_WayArr[i][1];
			CellNode Node;
			if (IsInVector(CellIndex(posx, posz), m_OpenVec, Node)) {
				retvec.push_back(Node);
			}
		}
		if (!retvec.empty()) {
			std::sort(retvec.begin(), retvec.end(), NodeAsc);
		}
	}

	bool PathSearch::Impl::AddOpenNode(const CellIndex& Base, const CellIndex& Target) {
		//Open���X�g����w���Node�����o��
		auto it = m_OpenVec.begin();
		CellNode TempNode;
		bool find = false;
		while (it != m_OpenVec.end()) {
			if (it->x == Base.x && it->z == Base.z) {
				//���o������erase��Close���X�g�ɒǉ�
				TempNode = *it;
				m_CloseVec.push_back(TempNode);
				m_OpenVec.erase(it);
				find = true;
				break;
			}
			it++;
		}
		if (!find) {
			return false;
		}
		auto ShCellMap = m_StageCellMap.lock();
		if (!ShCellMap) {
			//�Z���}�b�v���Ȃ�
			throw BaseException(
				L"�Z���}�b�v���w�肳��Ă܂���",
				L"if (!ShCellMap)",
				L"PathFindSteering::AddOpenNode()"
			);
		}

		auto& CellVec = ShCellMap->GetCellVec();
		for (size_t i = 0; i < m_WayArr.size(); i++) {
			int posx = TempNode.x + m_WayArr[i][0];
			int posz = TempNode.z + m_WayArr[i][1];
			if (posx >= 0 && posx < (int)CellVec.size() &&
				posz >= 0 && posz < (int)CellVec[posx].size()) {
				//posx,posz���Z���}�b�v�͈͓̔�
				if (CellVec[posx][posz].m_Cost == -1) {
					//�R�X�g��-1�ł͂Ȃ�
					continue;
				}
				CellNode ret;
				if (IsInVector(CellIndex(posx, posz), m_OpenVec, ret)) {
					//OpenVec�ɂ�����
					continue;
				}
				if (IsInVector(CellIndex(posx, posz), m_CloseVec, ret)) {
					//CloseVec�ɂ�����
					continue;
				}
				//���݃I�[�v�����X�g�ɂȂ�
				//OpenVec�ɒǉ�
				CellIndex BaseIndex(posx, posz);
				CellNode Node;
				Node.x = posx;
				Node.z = posz;
				Node.px = TempNode.x;
				Node.pz = TempNode.z;
				Node.cost = TempNode.cost + CellVec[posx][posz].m_Cost;
				Node.heuristic = GetHeuristic(BaseIndex, Target);
				Node.score = Node.cost + Node.heuristic;
				m_OpenVec.push_back(Node);
			}
		}
		return true;
	}

	bool PathSearch::Impl::SearchCellBase(const CellIndex& Start, const CellIndex& Target) {
		if (Start == Target) {
			//���ǂ蒅����
			return true;
		}
		if (!AddOpenNode(Start, Target)) {
			return false;
		}
		//�אڂ���Open��Node���擾
		vector<CellNode> adjacentvec;
		InOpenScore(Start, adjacentvec);
		if (!adjacentvec.empty()) {
			//�אڂ���L����Node�����ԂɌ���
			for (auto& v : adjacentvec) {
				CellIndex Index;
				Index.x = v.x;
				Index.z = v.z;
				if (SearchCellBase(Index, Target)) {
					return true;
				}
			}
		}
		return false;
	}


	shared_ptr<StageCellMap> PathSearch::Impl::GetStageCellMap() const {
		auto ShCellMap = m_StageCellMap.lock();
		if (!ShCellMap) {
			//�Z���}�b�v���Ȃ�
			throw BaseException(
				L"�Z���}�b�v���w�肳��Ă܂���",
				L"if (!ShCellMap)",
				L"PathSearch::Impl::GetStageCellMap()"
			);
		}
		return ShCellMap;
	}

	//--------------------------------------------------------------------------------------
	///	 �o�H�������ǃR���|�[�l���g
	//--------------------------------------------------------------------------------------
	PathSearch::PathSearch(const shared_ptr<GameObject>& GameObjectPtr,
		const shared_ptr<StageCellMap>& StageCellMapPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{
		pImpl->m_StageCellMap = StageCellMapPtr;
	}

	PathSearch::~PathSearch() {}


	bool PathSearch::SearchCell(const bsm::Vec3& TargetPosition, vector<CellIndex>& RetCellIndexVec) {
		//�I�[�v���E�N���[�Y���X�g�̃N���A
		pImpl->m_OpenVec.clear();
		pImpl->m_CloseVec.clear();
		RetCellIndexVec.clear();
		auto ShCellMap = pImpl->GetStageCellMap();
		//�^�[�Q�b�g�̃C���f�b�N�X���擾
		if (!ShCellMap->FindCell(TargetPosition, pImpl->m_TargetIndex)) {
			//���������烊�^�[��
			return false;
		}
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
		//�������g�̃C���f�b�N�X���擾
		if (!ShCellMap->FindCell(Pos, pImpl->m_BaseIndex)) {
			//���������烊�^�[��
			return false;
		}
		//�ŏ���Node��ݒ�
		auto& CellVec = ShCellMap->GetCellVec();
		CellNode Node;
		Node.x = pImpl->m_BaseIndex.x;
		Node.z = pImpl->m_BaseIndex.z;
		Node.px = -1;
		Node.pz = -1;
		Node.cost = CellVec[pImpl->m_BaseIndex.x][pImpl->m_BaseIndex.z].m_Cost;
		Node.heuristic = pImpl->GetHeuristic(pImpl->m_BaseIndex, pImpl->m_TargetIndex);
		Node.score = Node.cost + Node.heuristic;
		if (Node.cost == -1) {
			return false;
		}
		bool DirectHit = false;
		for (UINT x = 0; x < CellVec.size(); x++) {
			for (UINT z = 0; z < CellVec[x].size(); z++) {
				if (CellVec[x][z].m_Cost == -1) {
					//��Q����������
					if (HitTest::SEGMENT_AABB(Pos, TargetPosition, CellVec[x][z].m_PieceRange)) {
						//��Q���ƃ��C���q�b�g���Ă���
						DirectHit = true;
						break;
					}
				}
			}
			if (DirectHit) {
				break;
			}
		}
		if (!DirectHit) {
			//�ǂ̏�Q���Ƃ��q�b�g���ĂȂ�
			//���ڃ^�[�Q�b�g�ɍs����
			//�܂��������g
			RetCellIndexVec.push_back(pImpl->m_BaseIndex);
			if (pImpl->m_BaseIndex != pImpl->m_TargetIndex) {
				//�����ă^�[�Q�b�g
				RetCellIndexVec.push_back(pImpl->m_TargetIndex);
			}
			//����
			return true;
		}
		pImpl->m_OpenVec.push_back(Node);
		if (pImpl->SearchCellBase(pImpl->m_BaseIndex, pImpl->m_TargetIndex)) {
			//�o�H����������
			CellNode TempNode;
			CellIndex TempCellIndex;
			//�ŏ���Node��Open���X�g�ɂ���
			pImpl->IsInVector(pImpl->m_TargetIndex, pImpl->m_OpenVec, TempNode);
			TempCellIndex.x = TempNode.x;
			TempCellIndex.z = TempNode.z;
			RetCellIndexVec.push_back(TempCellIndex);
			while (pImpl->m_BaseIndex != TempCellIndex) {
				//2�Ԗڈȍ~��Close���X�g�ɂ���
				TempCellIndex.x = TempNode.px;
				TempCellIndex.z = TempNode.pz;
				if (!pImpl->IsInVector(TempCellIndex, pImpl->m_CloseVec, TempNode)) {
					break;
				}
				RetCellIndexVec.push_back(TempCellIndex);
				TempCellIndex.x = TempNode.x;
				TempCellIndex.z = TempNode.z;
			}
			//�t���ɂ��ďI��
			std::reverse(RetCellIndexVec.begin(), RetCellIndexVec.end());
			return true;
		}
		//�o�H��������Ȃ�����
		return false;

	}


	//--------------------------------------------------------------------------------------
	//	struct Gravity::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Gravity::Impl {
		bsm::Vec3 m_Gravity;
		bsm::Vec3 m_GravityVelocity;
		Impl():
			m_Gravity(0),
			m_GravityVelocity(0)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	 �d�̓R���|�[�l���g
	//--------------------------------------------------------------------------------------
	Gravity::Gravity(const shared_ptr<GameObject>& GameObjectPtr,const bsm::Vec3& gravity):
		Component(GameObjectPtr),
		pImpl(new Impl())
	{
		pImpl->m_Gravity = gravity;
	}

	Gravity::~Gravity() {}

	bsm::Vec3 Gravity::GetGravity() const {
		return pImpl->m_Gravity;
	}
	void Gravity::SetGravity(const bsm::Vec3& gravity) {
		pImpl->m_Gravity = gravity;
	}
	void Gravity::SetGravityZero() {
		pImpl->m_Gravity = bsm::Vec3(0);
	}
	bsm::Vec3 Gravity::GetGravityVelocity() const {
		return pImpl->m_GravityVelocity;
	}
	void Gravity::SetGravityVerocity(const bsm::Vec3& GravityVerocity) {
		pImpl->m_GravityVelocity = GravityVerocity;
	}
	void Gravity::SetGravityVerocityZero() {
		pImpl->m_GravityVelocity = bsm::Vec3(0);
	}

	void Gravity::StartJump(const bsm::Vec3& JumpVerocity) {
		pImpl->m_GravityVelocity = JumpVerocity;
	}


	void Gravity::OnUpdate() {
		//�R���W�����������āA�X���[�v��ԂȂ�X�V���Ȃ�
		auto PtrCollision = GetGameObject()->GetComponent<Collision>(false);
		if (PtrCollision && PtrCollision->IsSleep()) {
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		pImpl->m_GravityVelocity += pImpl->m_Gravity * ElapsedTime;
		auto Pos = PtrTransform->GetPosition();
//		auto Pos = PtrTransform->GetWorldPosition();
		Pos += pImpl->m_GravityVelocity * ElapsedTime;
		PtrTransform->SetPosition(Pos);
	}



}
//end basecross
