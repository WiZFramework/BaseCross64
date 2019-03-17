/*!
@file PhysicsManager.cpp
@brief 物理計算用マネージャなど実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	物理計算コンポーネントの親
	//--------------------------------------------------------------------------------------
	Rigidbody::Rigidbody(const shared_ptr<GameObject>& GameObjectPtr):
		Component(GameObjectPtr),
		m_AutoTransform(true)
	{
		SetDrawActive(false);
	}
	Rigidbody::~Rigidbody() {}

	void Rigidbody::DrawShapeWireFrame(const shared_ptr<MeshResource>& res, const bsm::Mat4x4& meshtotrans) {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), res, meshtotrans);
	}

	void Rigidbody::DrawShapeWireFrameWithWorldMatrix(const shared_ptr<MeshResource>& res, const bsm::Mat4x4& WorldMatrix) {
		GenericDraw Draw;
		Draw.DrawWireFrameWithWorldMatrix(GetGameObject(), res, WorldMatrix);

	}


	shared_ptr<MeshResource> Rigidbody::CreateCapsuleMesh(const PsCapsuleParam& param) {
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionColor> new_pc_vertices;
		vector<uint16_t> indices;
		bsm::Vec3 PointA(0, 0, 0);
		bsm::Vec3 PointB(0, 0, 0);
		PointA -= bsm::Vec3(0, param.m_HalfLen, 0);
		PointB += bsm::Vec3(0, param.m_HalfLen, 0);
		MeshUtill::CreateCapsule(param.m_Radius * 2.0f,
			PointA, PointB, 6, vertices, indices, true);
		for (auto& v : vertices) {
			VertexPositionColor vertex;
			vertex.position = v.position;
			vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			new_pc_vertices.push_back(vertex);
		}
		return MeshResource::CreateMeshResource(new_pc_vertices, indices, false);
	}

	shared_ptr<MeshResource> Rigidbody::CreateCylinderMesh(const PsCylinderParam& param) {
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionColor> new_pc_vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCylinder(param.m_HalfLen * 2.0f, param.m_Radius * 2.0f, 6, vertices, indices, true);
		for (auto& v : vertices) {
			VertexPositionColor vertex;
			vertex.position = v.position;
			vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			new_pc_vertices.push_back(vertex);
		}
		return MeshResource::CreateMeshResource(new_pc_vertices, indices, false);
	}

	shared_ptr<MeshResource> Rigidbody::CreateConvexMesh(const PsConvexParam& param) {
		vector<VertexPositionColor> new_pc_vertices;
		for (auto& v : param.m_ConvexMeshResource->GetVertices()) {
			VertexPositionColor vertex;
			vertex.position = v.position;
			vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			new_pc_vertices.push_back(vertex);
		}
		return MeshResource::CreateMeshResource(new_pc_vertices, param.m_ConvexMeshResource->GetIndices(), false);
	}

	//--------------------------------------------------------------------------------------
	///	単体の物理計算コンポーネント
	//--------------------------------------------------------------------------------------
	RigidbodySingle::RigidbodySingle(const shared_ptr<GameObject>& GameObjectPtr) :
		Rigidbody(GameObjectPtr)
	{}
	RigidbodySingle::~RigidbodySingle() {
	}

	bsm::Vec3	RigidbodySingle::GetPosition() const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyPosition(GetIndex());
	}

	bsm::Quat	RigidbodySingle::GetOrientation() const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyOrientation(GetIndex());
	}

	bsm::Vec3	RigidbodySingle::GetLinearVelocity() const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyLinearVelocity(GetIndex());
	}

	bsm::Vec3	RigidbodySingle::GetAngularVelocity() const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyAngularVelocity(GetIndex());
	}

	void RigidbodySingle::GetStatus(PsBodyStatus& st) const {
		GetGameObject()->GetStage()->GetBasePhysics().GetBodyStatus(GetIndex(), st);
	}

	void RigidbodySingle::SetStatus(const PsBodyUpdateStatus& st) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyStatus(GetIndex(), st);
	}

	void RigidbodySingle::WakeUp() {
		GetGameObject()->GetStage()->GetBasePhysics().WakeUpBody(GetIndex());
	}

	bool RigidbodySingle::IsAutoGravity()const {
		return GetGameObject()->GetStage()->GetBasePhysics().IsAutoGravity(GetIndex());
	}

	void RigidbodySingle::SetAutoGravity(bool b) {
		GetGameObject()->GetStage()->GetBasePhysics().SetAutoGravity(GetIndex(),b);
	}


	void RigidbodySingle::SetPosition(const bsm::Vec3& pos) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyPosition(GetIndex(), pos);
	}

	void RigidbodySingle::SetOrientation(const bsm::Quat& qt) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyOrientation(GetIndex(), qt);
	}

	void RigidbodySingle::SetLinearVelocity(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyLinearVelocity(GetIndex(), v);

	}
	void RigidbodySingle::SetAngularVelocity(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyAngularVelocity(GetIndex(), v);
	}

	void RigidbodySingle::MovePosition(const bsm::Vec3 &pos, float timeStep) {
		GetGameObject()->GetStage()->GetBasePhysics().MoveBodyPosition(GetIndex(), pos, timeStep);
	}
	void RigidbodySingle::MoveOrientation(const bsm::Quat& qt, float timeStep) {
		GetGameObject()->GetStage()->GetBasePhysics().MoveBodyOrientation(GetIndex(), qt, timeStep);
	}


	void RigidbodySingle::ApplyForce(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().ApplyBodyForce(GetIndex(), v);

	}
	void RigidbodySingle::ApplyTorque(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().ApplyBodyTorque(GetIndex(), v);
	}

	void RigidbodySingle::SetContactFilterSelf(uint32_t val) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyContactFilterSelf(GetIndex(), val);
	}
	void RigidbodySingle::SetContactFilterTarget(uint32_t val) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyContactFilterTarget(GetIndex(), val);
	}

	void RigidbodySingle::SetMotionType(PsMotionType t) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyMotionType(GetIndex(), t);

	}
	PsMotionType RigidbodySingle::GetMotionType()const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyMotionType(GetIndex());
	}



	bool RigidbodySingle::GetContactsVec(vector<uint16_t>& contacts)const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetContactsVec(GetIndex(), contacts);
	}
	bool RigidbodySingle::GetContactsSet(set<uint16_t>& contacts)const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetContactsSet(GetIndex(), contacts);

	}


	bool RigidbodySingle::GetContactGameObjects(vector<shared_ptr<GameObject>>& objects)const {
		set<uint16_t> indeces;
		if (GetContactsSet(indeces)) {
			auto& ObjVec = GetGameObject()->GetStage()->GetGameObjectVec();
			for (auto& v : ObjVec) {
				auto Comp = v->GetComponent<RigidbodySingle>(false);
				if (Comp) {
					auto index = Comp->GetIndex();
					if (indeces.find(index) != indeces.end()) {
						objects.push_back(v);
					}
				}
			}
		}
		if (objects.size() > 0) {
			return true;
		}
		else {
			return false;
		}
	}



	void RigidbodySingle::OnUpdate() {
		if (IsAutoTransform()) {
			PsBodyStatus Status;
			GetGameObject()->GetStage()->GetBasePhysics().GetBodyStatus(GetIndex(), Status);
			auto Ptr = GetGameObject()->GetComponent<Transform>();
			Ptr->SetPosition(Status.m_Position);
			Ptr->SetQuaternion(Status.m_Orientation);
		}
	}

	void RigidbodySingle::OnDestroy() {
		//計算や衝突をしないようにする
		SetContactFilterTarget(0);
		SetMotionType(PsMotionType::MotionTypeFixed);
		//空IDにセット
		GetGameObject()->GetStage()->SetVacantPhysicsIndex(GetIndex());
	}





	//--------------------------------------------------------------------------------------
	///	単体の球体コンポーネント
	//--------------------------------------------------------------------------------------
	RigidbodySphere::RigidbodySphere(const shared_ptr<GameObject>& GameObjectPtr, const PsSphereParam& param):
		RigidbodySingle(GameObjectPtr)
	{
		//空IDの取得
		auto index = GameObjectPtr->GetStage()->GetVacantPhysicsIndex();
		m_PsSphere = GameObjectPtr->GetStage()->GetBasePhysics().AddSphere(param, index);
	}

	uint16_t RigidbodySphere::GetIndex() const {
		return m_PsSphere->GetIndex();
	}

	const PsSphereParam& RigidbodySphere::GetParam() const {
		return m_PsSphere->GetParam();
	}

	SPHERE RigidbodySphere::GetSPHERE() const {
		SPHERE sp;
		auto& param = m_PsSphere->GetParam();
		sp.m_Radius = param.m_Radius;
		sp.m_Center = GetPosition();
		return sp;
	}

	bool RigidbodySphere::CollisionTestBase(const SPHERE& src) {
		return HitTest::SPHERE_SPHERE(src, GetSPHERE());
	}
	bool RigidbodySphere::CollisionTestBase(const CAPSULE& src) {
		bsm::Vec3 ret;
		return HitTest::SPHERE_CAPSULE(GetSPHERE(),src,ret);
	}
	bool RigidbodySphere::CollisionTestBase(const OBB& src) {
		bsm::Vec3 ret;
		return HitTest::SPHERE_OBB(GetSPHERE(), src, ret);
	}

	void RigidbodySphere::Reset(const PsSphereParam& param, uint16_t index) {
		m_PsSphere = GetGameObject()->GetStage()->GetBasePhysics().AddSphere(param, index);
	}

	void RigidbodySphere::OnDraw() {
		//トランスフォームからの差分
		bsm::Mat4x4 meshtotrans;
		meshtotrans.affineTransformation(
			bsm::Vec3(0.5f),			//スケーリングは0.5f
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Quat(),				//回転角度
			bsm::Vec3(0, 0, 0)				//位置
		);
		auto MeshRes = App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_SPHERE");
		DrawShapeWireFrame(MeshRes, meshtotrans);
	}

	//--------------------------------------------------------------------------------------
	///	単体のボックスコンポーネント
	//--------------------------------------------------------------------------------------
	RigidbodyBox::RigidbodyBox(const shared_ptr<GameObject>& GameObjectPtr, const PsBoxParam& param) :
		RigidbodySingle(GameObjectPtr)
	{
		//空IDの取得
		auto index = GameObjectPtr->GetStage()->GetVacantPhysicsIndex();
		m_PsBox = GameObjectPtr->GetStage()->GetBasePhysics().AddBox(param, index);
	}

	uint16_t RigidbodyBox::GetIndex() const {
		return m_PsBox->GetIndex();
	}

	const PsBoxParam& RigidbodyBox::GetParam() const {
		return m_PsBox->GetParam();
	}

	OBB RigidbodyBox::GetOBB() const {
		OBB obb(m_PsBox->GetParam().m_HalfSize * 2,
			GetOrientation(), GetPosition());
		return obb;
	}

	bool RigidbodyBox::CollisionTestBase(const SPHERE& src) {
		bsm::Vec3 ret;
		return HitTest::SPHERE_OBB(src, GetOBB(),ret);
	}
	bool RigidbodyBox::CollisionTestBase(const CAPSULE& src) {
		bsm::Vec3 ret;
		return HitTest::CAPSULE_OBB( src, GetOBB(), ret);
	}
	bool RigidbodyBox::CollisionTestBase(const OBB& src) {
		return HitTest::OBB_OBB(src, GetOBB());
	}

	void RigidbodyBox::Reset(const PsBoxParam& param, uint16_t index) {
		m_PsBox = GetGameObject()->GetStage()->GetBasePhysics().AddBox(param, index);
	}

	void RigidbodyBox::OnDraw() {
		auto MeshRes = App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_CUBE");
		//トランスフォームからの差分
		bsm::Mat4x4 meshtotrans;
		meshtotrans.affineTransformation(
			bsm::Vec3(0.5f),			//スケーリングは0.5f
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Quat(),				//回転角度
			bsm::Vec3(0, 0, 0)				//位置
		);
		DrawShapeWireFrame(MeshRes, meshtotrans);
	}

	//--------------------------------------------------------------------------------------
	///	単体のカプセルコンポーネント
	//--------------------------------------------------------------------------------------
	RigidbodyCapsule::RigidbodyCapsule(const shared_ptr<GameObject>& GameObjectPtr, const PsCapsuleParam& param):
		RigidbodySingle(GameObjectPtr)
	{
		//空IDの取得
		auto index = GameObjectPtr->GetStage()->GetVacantPhysicsIndex();
		m_PsCapsule = GetGameObject()->GetStage()->GetBasePhysics().AddCapsule(param, index);
		m_CapsuleMesh = Rigidbody::CreateCapsuleMesh(param);
	}


	uint16_t RigidbodyCapsule::GetIndex() const {
		return m_PsCapsule->GetIndex();
	}

	const PsCapsuleParam& RigidbodyCapsule::GetParam() const {
		return m_PsCapsule->GetParam();
	}

	CAPSULE RigidbodyCapsule::GetCAPSULE() const {
		//ワールド行列の決定
		Mat4x4 World;
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			GetOrientation(),				//回転角度
			GetPosition()			//位置
		);
		CAPSULE cap(m_PsCapsule->GetParam().m_Radius,
			Vec3(0, -m_PsCapsule->GetParam().m_HalfLen, 0),
			Vec3(0, m_PsCapsule->GetParam().m_HalfLen, 0),
			World);
		return cap;
	}

	bool RigidbodyCapsule::CollisionTestBase(const SPHERE& src) {
		bsm::Vec3 ret;
		return HitTest::SPHERE_CAPSULE(src, GetCAPSULE(), ret);
	}
	bool RigidbodyCapsule::CollisionTestBase(const CAPSULE& src) {
		bsm::Vec3 ret1,ret2;
		return HitTest::CAPSULE_CAPSULE(src, GetCAPSULE(), ret1,ret2);
	}
	bool RigidbodyCapsule::CollisionTestBase(const OBB& src) {
		bsm::Vec3 ret;
		return HitTest::CAPSULE_OBB(GetCAPSULE(), src,ret);
	}

	void RigidbodyCapsule::Reset(const PsCapsuleParam& param, uint16_t index) {
		m_PsCapsule = GetGameObject()->GetStage()->GetBasePhysics().AddCapsule(param, index);
		m_CapsuleMesh = Rigidbody::CreateCapsuleMesh(param);
	}

	void RigidbodyCapsule::OnDraw() {
		//トランスフォームからの差分
		bsm::Mat4x4 meshtotrans;
		meshtotrans.affineTransformation(
			bsm::Vec3(1.0f),			//スケーリングは1.0f
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Quat(),				//回転角度
			bsm::Vec3(0, 0, 0)				//位置
		);
		auto MeshRes = m_CapsuleMesh;
		DrawShapeWireFrame(MeshRes, meshtotrans);

	}

	//--------------------------------------------------------------------------------------
	///	単体のシリンダーコンポーネント
	//--------------------------------------------------------------------------------------
	RigidbodyCylinder::RigidbodyCylinder(const shared_ptr<GameObject>& GameObjectPtr, const PsCylinderParam& param):
		RigidbodySingle(GameObjectPtr)
	{
		//空IDの取得
		auto index = GameObjectPtr->GetStage()->GetVacantPhysicsIndex();
		m_PsCylinder = GetGameObject()->GetStage()->GetBasePhysics().AddCylinder(param, index);
		m_CylinderMesh = Rigidbody::CreateCylinderMesh(param);
	}


	uint16_t RigidbodyCylinder::GetIndex() const {
		return m_PsCylinder->GetIndex();
	}

	const PsCylinderParam& RigidbodyCylinder::GetParam() const {
		return m_PsCylinder->GetParam();
	}


	void RigidbodyCylinder::Reset(const PsCylinderParam& param, uint16_t index) {
		m_PsCylinder = GetGameObject()->GetStage()->GetBasePhysics().AddCylinder(param, index);
		m_CylinderMesh = Rigidbody::CreateCylinderMesh(param);
	}

	void RigidbodyCylinder::OnDraw() {
		//トランスフォームからの差分
		bsm::Mat4x4 meshtotrans;
		meshtotrans.affineTransformation(
			bsm::Vec3(0.5f),			//スケーリングは0.5f
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Quat(),				//回転角度
			bsm::Vec3(0, 0, 0)				//位置
		);
		auto MeshRes = m_CylinderMesh;
		DrawShapeWireFrame(MeshRes, meshtotrans);
	}

	//--------------------------------------------------------------------------------------
	///	単体の凸面コンポーネント
	//--------------------------------------------------------------------------------------
	RigidbodyConvex::RigidbodyConvex(const shared_ptr<GameObject>& GameObjectPtr, const PsConvexParam& param):
		RigidbodySingle(GameObjectPtr)
	{
		//空IDの取得
		auto index = GameObjectPtr->GetStage()->GetVacantPhysicsIndex();
		m_PsConvex = GetGameObject()->GetStage()->GetBasePhysics().AddConvex(param, index);
		m_ConvexMesh = Rigidbody::CreateConvexMesh(param);
	}

	uint16_t RigidbodyConvex::GetIndex() const {
		return m_PsConvex->GetIndex();
	}

	const PsConvexParam& RigidbodyConvex::GetParam() const {
		return m_PsConvex->GetParam();
	}


	void RigidbodyConvex::Reset(const PsConvexParam& param, uint16_t index) {
		m_PsConvex = GetGameObject()->GetStage()->GetBasePhysics().AddConvex(param, index);
		m_ConvexMesh = Rigidbody::CreateConvexMesh(param);
	}

	void RigidbodyConvex::OnDraw() {
		//トランスフォームからの差分
		bsm::Mat4x4 meshtotrans;
		meshtotrans.affineTransformation(
			bsm::Vec3(1.0f),			//スケーリングは1.0f
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Quat(),				//回転角度
			bsm::Vec3(0, 0, 0)				//位置
		);
		DrawShapeWireFrame(m_ConvexMesh, meshtotrans);
	}

	//--------------------------------------------------------------------------------------
	///	プリミティブ合成コンポーネント
	//--------------------------------------------------------------------------------------
	RigidbodyCombined::RigidbodyCombined(const shared_ptr<GameObject>& GameObjectPtr, const PsCombinedParam& param):
		RigidbodySingle(GameObjectPtr)
	{
		//空IDの取得
		auto index = GameObjectPtr->GetStage()->GetVacantPhysicsIndex();
		m_PsCombined = GetGameObject()->GetStage()->GetBasePhysics().AddCombined(param, index);
		CreateMesh(param);
	}

	void RigidbodyCombined::CreateMesh(const PsCombinedParam& param) {
		m_CombinedMeshVec.clear();
		for (auto& Prim : param.m_Primitives){
			vector<VertexPositionNormalTexture> vertices;
			vector<VertexPositionColor> new_pc_vertices;
			vector<uint16_t> indices;
			switch (Prim.m_CombinedType) {
				case PsCombinedType::TypeSphere:
				{
					m_CombinedMeshVec.push_back(App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_SPHERE"));
				}
				break;
				case PsCombinedType::TypeBox:
				{
					m_CombinedMeshVec.push_back(App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_CUBE"));
				}
				break;
				case PsCombinedType::TypeCapsule:
				{
					bsm::Vec3 PointA(0, 0, 0);
					bsm::Vec3 PointB(0, 0, 0);
					PointA -= bsm::Vec3(0, Prim.m_HalfLen, 0);
					PointB += bsm::Vec3(0, Prim.m_HalfLen, 0);
					MeshUtill::CreateCapsule(Prim.m_Radius * 2.0f,
						PointA, PointB, 6, vertices, indices, true);
					for (auto& v : vertices) {
						VertexPositionColor vertex;
						vertex.position = v.position;
						vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
						new_pc_vertices.push_back(vertex);
					}
					m_CombinedMeshVec.push_back(MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
				}
				break;
				case PsCombinedType::TypeCylinder:
				{
					MeshUtill::CreateCylinder(Prim.m_HalfLen * 2.0f, Prim.m_Radius * 2.0f, 6, vertices, indices, true);
					for (auto& v : vertices) {
						VertexPositionColor vertex;
						vertex.position = v.position;
						vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
						new_pc_vertices.push_back(vertex);
					}
					m_CombinedMeshVec.push_back(MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
				}
				break;
				case PsCombinedType::TypeConvex:
				{
					for (auto& v : Prim.m_ConvexMeshResource->GetVertices()) {
						VertexPositionColor vertex;
						vertex.position = v.position;
						vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
						new_pc_vertices.push_back(vertex);
					}
					m_CombinedMeshVec.push_back(MeshResource::CreateMeshResource(new_pc_vertices, Prim.m_ConvexMeshResource->GetIndices(), false));
				}
				break;
			}
		}

	}

	uint16_t RigidbodyCombined::GetIndex() const {
		return m_PsCombined->GetIndex();
	}

	const PsCombinedParam& RigidbodyCombined::GetParam() const {
		return m_PsCombined->GetParam();
	}


	void RigidbodyCombined::Reset(const PsCombinedParam& param, uint16_t index) {
		m_PsCombined = GetGameObject()->GetStage()->GetBasePhysics().AddCombined(param, index);
		CreateMesh(param);
	}

	void RigidbodyCombined::OnDraw() {
		auto index = GetIndex();
		//行列の定義
		bsm::Mat4x4 Local;
		PsBodyStatus Status;
		auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
		BasePs.GetBodyStatus(index, Status);
		auto& param = m_PsCombined->GetParam();
		for (size_t i = 0; i < param.m_Primitives.size(); i++) {
			auto& prim = param.m_Primitives[i];
			bsm::Vec3 LocalPos;
			bsm::Quat LocalQt;
			BasePs.GetShapeOffsetQuatPos(index, (uint16_t)i, LocalQt, LocalPos);
			switch (prim.m_CombinedType) {
				case PsCombinedType::TypeSphere:
				{
					Local.affineTransformation(
						Vec3(prim.m_Radius),	//スケーリング
						bsm::Vec3(0, 0, 0),		//回転の中心（重心）
						LocalQt,				//回転角度
						LocalPos				//位置
					);
				}
				break;
				case PsCombinedType::TypeBox:
				{
					Local.affineTransformation(
						prim.m_HalfSize,			//スケーリング
						bsm::Vec3(0, 0, 0),		//回転の中心（重心）
						LocalQt,				//回転角度
						LocalPos				//位置
					);
				}
				break;
				default:
				{
					//球とボックス以外はスケーリング1.0
					Local.affineTransformation(
						bsm::Vec3(1.0f),			//スケーリングは1.0f
						bsm::Vec3(0, 0, 0),		//回転の中心（重心）
						LocalQt,				//回転角度
						LocalPos				//位置
					);
				}
				break;
			}
			bsm::Mat4x4 DrawWorld = Local;
			DrawShapeWireFrame(m_CombinedMeshVec[i], Local);
		}
	}

	//--------------------------------------------------------------------------------------
	///	マルチ物理コンポーネント
	//--------------------------------------------------------------------------------------
	RigidbodyMulti::RigidbodyMulti(const shared_ptr<GameObject>& GameObjectPtr):
		Rigidbody(GameObjectPtr)
	{}

	RigidbodyMulti::~RigidbodyMulti() {
	}


	void RigidbodyMulti::CheckObjIndex(size_t index) const{
		if (index >= m_PsObjectVec.size()) {
			throw BaseException(
				L"RigidbodyMultiのオブジェクトインデックスが範囲外です",
				L"if (index >= m_PsObjectVec.size())",
				L"RigidbodyMulti::CheckObjIndex()"
			);
		}
	}

	void  RigidbodyMulti::CheckJointIndex(size_t index) const {
		if (index >= m_PsJointVec.size()) {
			throw BaseException(
				L"PsMultiJointのオブジェクトインデックスが範囲外です",
				L"if (index >= m_PsJointVec.size())",
				L"RigidbodyMulti::CheckJointIndex()"
			);
		}

	}



	size_t RigidbodyMulti::AddSphere(const PsSphereParam& param) {
		size_t obj_index = m_PsObjectVec.size();
		auto PsSpherePtr = GetGameObject()->GetStage()->GetBasePhysics().AddSphere(param);
		m_PsObjectVec.push_back(PsSpherePtr);
		return obj_index;
	}

	size_t RigidbodyMulti::AddBox(const PsBoxParam& param) {
		size_t obj_index = m_PsObjectVec.size();
		auto PsBoxPtr = GetGameObject()->GetStage()->GetBasePhysics().AddBox(param);
		m_PsObjectVec.push_back(PsBoxPtr);
		return obj_index;
	}

	size_t RigidbodyMulti::AddCapsule(const PsCapsuleParam& param) {
		size_t obj_index = m_PsObjectVec.size();
		auto PsCapsuleObj = GetGameObject()->GetStage()->GetBasePhysics().AddCapsule(param);
		m_PsObjectVec.push_back(PsCapsuleObj);
		auto index = PsCapsuleObj->GetIndex();
		auto CapsuleMesh = Rigidbody::CreateCapsuleMesh(param);
		//Mapに登録
		m_MeshMap[index] =  CapsuleMesh;
		return obj_index;
	}

	size_t RigidbodyMulti::AddCylinder(const PsCylinderParam& param) {
		size_t obj_index = m_PsObjectVec.size();
		auto PsCylinderObj = GetGameObject()->GetStage()->GetBasePhysics().AddCylinder(param);
		m_PsObjectVec.push_back(PsCylinderObj);
		auto index = PsCylinderObj->GetIndex();
		auto CylinderMesh = Rigidbody::CreateCylinderMesh(param);
		//Mapに登録
		m_MeshMap[index] = CylinderMesh;
		return obj_index;
	}

	size_t RigidbodyMulti::AddConvex(const PsConvexParam& param) {
		size_t obj_index = m_PsObjectVec.size();
		auto PsConvexObj = GetGameObject()->GetStage()->GetBasePhysics().AddConvex(param);
		m_PsObjectVec.push_back(PsConvexObj);
		auto index = PsConvexObj->GetIndex();
		auto ConvexMesh = Rigidbody::CreateConvexMesh(param);
		//Mapに登録
		m_MeshMap[index] = ConvexMesh;
		return obj_index;
	}

	size_t RigidbodyMulti::AddBallJoint(const PsBallJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddBallJoint(param));
		return j_index;
	}

	size_t RigidbodyMulti::AddSwingTwistJoint(const PsSwingTwistJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddSwingTwistJoint(param));
		return j_index;
	}

	size_t RigidbodyMulti::AddHingeJoint(const PsHingeJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddHingeJoint(param));
		return j_index;
	}

	size_t RigidbodyMulti::AddSliderJoint(const PsSliderJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddSliderJoint(param));
		return j_index;
	}

	size_t RigidbodyMulti::AddFixJoint(const PsFixJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddFixJoint(param));
		return j_index;
	}

	size_t RigidbodyMulti::AddUniversalJoint(const PsUniversalJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddUniversalJoint(param));
		return j_index;
	}

	uint16_t RigidbodyMulti::GetJointIndex(size_t index) const {
		CheckJointIndex(index);
		return m_PsJointVec[index]->GetIndex();
	}

	shared_ptr<PsJoint> RigidbodyMulti::GetPsJoint(size_t index) const {
		CheckJointIndex(index);
		return m_PsJointVec[index];
	}



	uint16_t RigidbodyMulti::GetIndex(size_t index) const {
		CheckObjIndex(index);
		return m_PsObjectVec[index]->GetIndex();
	}

	shared_ptr<PsObject> RigidbodyMulti::GetPsObject(size_t index) const {
		CheckObjIndex(index);
		return  m_PsObjectVec[index];
	}


	void RigidbodyMulti::GetStatus(PsBodyStatus& st, size_t index) const {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().GetBodyStatus(GetIndex(index), st);
	}
	bsm::Vec3	RigidbodyMulti::GetPosition(size_t index) const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyPosition(GetIndex(index));
	}
	bsm::Quat	RigidbodyMulti::GetOrientation(size_t index) const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyOrientation(GetIndex(index));
	}
	bsm::Vec3	RigidbodyMulti::GetLinearVelocity(size_t index)const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyLinearVelocity(GetIndex(index));

	}

	bsm::Vec3	RigidbodyMulti::GetAngularVelocity(size_t index)const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyAngularVelocity(GetIndex(index));

	}
	void RigidbodyMulti::SetStatus(const PsBodyUpdateStatus& st, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyStatus(GetIndex(index), st);

	}
	void RigidbodyMulti::WakeUp(size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().WakeUpBody(GetIndex(index));

	}
	void RigidbodyMulti::WakeUpAll() {
		for (auto& v : m_PsObjectVec) {
			GetGameObject()->GetStage()->GetBasePhysics().WakeUpBody(v->GetIndex());
		}
	}

	bool RigidbodyMulti::IsAutoGravity(size_t index)const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().IsAutoGravity(GetIndex(index));
	}
	void RigidbodyMulti::SetAutoGravity(bool b, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetAutoGravity(GetIndex(index), b);
	}


	void RigidbodyMulti::SetPosition(const bsm::Vec3& pos, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyPosition(GetIndex(index), pos);
	}
	void RigidbodyMulti::SetOrientation(const bsm::Quat& qt, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyOrientation(GetIndex(index), qt);

	}
	void RigidbodyMulti::SetLinearVelocity(const bsm::Vec3& v, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyLinearVelocity(GetIndex(index), v);

	}
	void RigidbodyMulti::SetAngularVelocity(const bsm::Vec3& v, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyAngularVelocity(GetIndex(index), v);

	}
	void RigidbodyMulti::MovePosition(const bsm::Vec3 &pos, float timeStep, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().MoveBodyPosition(GetIndex(index), pos, timeStep);

	}
	void RigidbodyMulti::MoveOrientation(const bsm::Quat& qt, float timeStep, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().MoveBodyOrientation(GetIndex(index), qt, timeStep);

	}
	void RigidbodyMulti::ApplyForce(const bsm::Vec3& v, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().ApplyBodyForce(GetIndex(index), v);

	}
	void RigidbodyMulti::ApplyTorque(const bsm::Vec3& v, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().ApplyBodyTorque(GetIndex(index), v);

	}
	void RigidbodyMulti::SetContactFilterSelf(uint32_t val, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyContactFilterSelf(GetIndex(index), val);

	}
	void RigidbodyMulti::SetContactFilterTarget(uint32_t val, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyContactFilterTarget(GetIndex(index), val);

	}

	void RigidbodyMulti::SetMotionType(PsMotionType t, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyMotionType(GetIndex(index), t);
	}
	PsMotionType RigidbodyMulti::GetMotionType(size_t index)const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyMotionType(GetIndex(index));
	}


	bool RigidbodyMulti::GetContactsVec(vector<uint16_t>& contacts, size_t index)const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetContactsVec(GetIndex(index), contacts);

	}
	bool RigidbodyMulti::GetContactsSet(set<uint16_t>& contacts, size_t index)const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetContactsSet(GetIndex(index), contacts);
	}

	void RigidbodyMulti::OnDraw() {
		for (auto& v : m_PsObjectVec) {
			auto index = v->GetIndex();
			switch (v->GetShapeType()) {
				case PsShapeType::Sphere:
				{
					auto PsSpherePtr = dynamic_pointer_cast<PsSphere>(v);
					if (PsSpherePtr) {
						//行列の定義
						bsm::Mat4x4 World, Local;
						PsBodyStatus Status;
						auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
						auto MeshRes = App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_SPHERE");
						BasePs.GetBodyStatus(index, Status);
						//ワールド行列の決定
						World.affineTransformation(
							bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							Status.m_Orientation,				//回転角度
							Status.m_Position			//位置
						);
						bsm::Vec3 LocalPos;
						bsm::Quat LocalQt;
						BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);
						Local.affineTransformation(
							bsm::Vec3(PsSpherePtr->GetParam().m_Radius),			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							LocalQt,				//回転角度
							LocalPos				//位置
						);
						bsm::Mat4x4 DrawWorld = Local * World;
						DrawShapeWireFrameWithWorldMatrix(MeshRes, DrawWorld);
					}
				}
				break;
				case PsShapeType::Box:
				{
					auto PsBoxPtr = dynamic_pointer_cast<PsBox>(v);
					if (PsBoxPtr) {
						//行列の定義
						bsm::Mat4x4 World, Local;
						PsBodyStatus Status;
						auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
						auto MeshRes = App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_CUBE");
						BasePs.GetBodyStatus(index, Status);
						//ワールド行列の決定
						World.affineTransformation(
							bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							Status.m_Orientation,				//回転角度
							Status.m_Position			//位置
						);
						bsm::Vec3 LocalPos;
						bsm::Quat LocalQt;
						BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);
						Local.affineTransformation(
							PsBoxPtr->GetParam().m_HalfSize,			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							LocalQt,				//回転角度
							LocalPos				//位置
						);
						bsm::Mat4x4 DrawWorld = Local * World;
						DrawShapeWireFrameWithWorldMatrix(MeshRes, DrawWorld);
					}
				}
				break;
				case PsShapeType::Capsule:
				{
					auto PsCapsulePtr = dynamic_pointer_cast<PsCapsule>(v);
					if (PsCapsulePtr && m_MeshMap.find(index) != m_MeshMap.end()) {
						//行列の定義
						bsm::Mat4x4 World, Local;
						PsBodyStatus Status;
						auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
						auto MeshRes = m_MeshMap[index];
						BasePs.GetBodyStatus(index, Status);
						//ワールド行列の決定
						World.affineTransformation(
							bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							Status.m_Orientation,				//回転角度
							Status.m_Position			//位置
						);
						bsm::Vec3 LocalPos;
						bsm::Quat LocalQt;
						BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);

						Local.affineTransformation(
							bsm::Vec3(1.0f),			//スケーリングは1.0f
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							LocalQt,				//回転角度
							LocalPos				//位置
						);
						bsm::Mat4x4 DrawWorld = Local * World;
						DrawShapeWireFrameWithWorldMatrix(MeshRes, DrawWorld);
					}
				}
				break;
				case PsShapeType::Cylinder:
				{
					auto PsCylinderPtr = dynamic_pointer_cast<PsCylinder>(v);
					if (PsCylinderPtr && m_MeshMap.find(index) != m_MeshMap.end()) {
						//行列の定義
						bsm::Mat4x4 World, Local;
						PsBodyStatus Status;
						auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
						auto MeshRes = m_MeshMap[index];
						BasePs.GetBodyStatus(index, Status);
						//ワールド行列の決定
						World.affineTransformation(
							bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							Status.m_Orientation,				//回転角度
							Status.m_Position			//位置
						);
						bsm::Vec3 LocalPos;
						bsm::Quat LocalQt;
						BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);
						Local.affineTransformation(
							bsm::Vec3(1.0f),			//スケーリングは1.0f
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							LocalQt,				//回転角度
							LocalPos				//位置
						);
						bsm::Mat4x4 DrawWorld = Local * World;
						DrawShapeWireFrameWithWorldMatrix(MeshRes, DrawWorld);
					}
				}
				break;
				case PsShapeType::Convex:
				break;
			}
		}
	}

	void RigidbodyMulti::OnDestroy() {
		//計算や衝突をしないようにする
		for (auto& v : m_PsObjectVec) {
			auto BodyIndex = v->GetIndex();
			GetGameObject()->GetStage()->GetBasePhysics().SetBodyContactFilterTarget(BodyIndex, 0);
			GetGameObject()->GetStage()->GetBasePhysics().SetBodyMotionType(BodyIndex, PsMotionType::MotionTypeFixed);
		}
	}




}
