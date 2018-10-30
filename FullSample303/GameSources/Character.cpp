/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	//構築と破棄
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

	//初期化
	void FixedBox::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(m_Scale);
		ptrTrans->SetRotation(m_Rotation);
		ptrTrans->SetPosition(m_Position);
		//OBB衝突j判定を付ける
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//タグをつける
		AddTag(L"FixedBox");
		//影をつける（シャドウマップを描画する）
		auto ptrShadow = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetTextureResource(L"SKY_TX");
		ptrDraw->SetOwnShadowActive(true);

	}


	//--------------------------------------------------------------------------------------
	///	PC球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PcSphere::PcSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PcSphere::~PcSphere() {}

	//初期化
	void PcSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//描画コンポーネント
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
		//影をつける
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(ptrDraw->GetOriginalMeshResource());
	}

	void PcSphere::OnUpdate() {
		auto beh = GetBehavior<VertexBehavior>();
		beh->ExpandAndContract<VertexPositionColor, BcPCStaticDraw>();
	}

	//--------------------------------------------------------------------------------------
	///	PN球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PnSphere::PnSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PnSphere::~PnSphere() {}

	//初期化
	void PnSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//描画コンポーネント
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
		//影をつける
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(ptrDraw->GetOriginalMeshResource());
	}

	void PnSphere::OnUpdate() {
		auto beh = GetBehavior<VertexBehavior>();
		beh->ExpandAndContract<VertexPositionNormal, BcPNStaticDraw>();
	}


	//--------------------------------------------------------------------------------------
	///	PT球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PtSphere::PtSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PtSphere::~PtSphere() {}

	//初期化
	void PtSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//描画コンポーネント
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
		//影をつける
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(ptrDraw->GetOriginalMeshResource());
	}
	void PtSphere::OnUpdate() {
		auto beh = GetBehavior<VertexBehavior>();
		beh->ExpandAndContract<VertexPositionTexture, BcPTStaticDraw>();
	}


	//--------------------------------------------------------------------------------------
	///	PCT球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PctSphere::PctSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	PctSphere::~PctSphere() {}

	//初期化
	void PctSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//描画コンポーネント
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
		//影をつける
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(ptrDraw->GetOriginalMeshResource());
	}

	void PctSphere::OnUpdate() {
		auto beh = GetBehavior<VertexBehavior>();
		beh->ExpandAndContract<VertexPositionColorTexture, BcPCTStaticDraw>();
	}



	//--------------------------------------------------------------------------------------
	///	Pnt球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PntSphere::PntSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TextureUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TextureUse(TextureUse)
	{
	}
	PntSphere::~PntSphere() {}

	//初期化
	void PntSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//影をつける
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");
		//描画コンポーネント
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetOwnShadowActive(true);
		if (m_TextureUse) {
			ptrDraw->SetTextureResource(L"SKY_TX");
		}
	}

	//--------------------------------------------------------------------------------------
	///	Pntスペキュラー球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PntSpSphere::PntSpSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TextureUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TextureUse(TextureUse)
	{
	}
	PntSpSphere::~PntSpSphere() {}
	//初期化
	void PntSpSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat qt;
		qt.identity();
		ptrTrans->SetQuaternion(qt);
		ptrTrans->SetPosition(m_StartPos);

		//影をつける
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネント
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetSpecularColorAndPower(Col4(1.0f, 1.0f, 1.0f, 1.0f));
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		if (m_TextureUse) {
			ptrDraw->SetTextureResource(L"SKY_TX");
		}
	}


	//--------------------------------------------------------------------------------------
	///	Staticキャラ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	StaticChara::StaticChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{
	}
	StaticChara::~StaticChara() {}
	//初期化
	void StaticChara::OnCreate() {
		//初期位置などの設定
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(0.5f, 0.5f, 0.5f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetPosition(m_StartPos);

		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ptrShadow = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
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
	///	Staticキャラ(マルチメッシュ版)
	//--------------------------------------------------------------------------------------
	//構築と破棄
	StaticMultiMeshChara::StaticMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{}
	StaticMultiMeshChara::~StaticMultiMeshChara() {}

	//初期化
	void StaticMultiMeshChara::OnCreate() {
		//初期位置などの設定
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(0.5f, 0.5f, 0.5f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetPosition(m_StartPos);

		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ptrShadow = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
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
	///	Boneキャラ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BoneChara::BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{
	}
	BoneChara::~BoneChara() {}

	//初期化
	void BoneChara::OnCreate() {
		//初期位置などの設定
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(0.5f, 0.5f, 0.5f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetPosition(m_StartPos);

		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ptrShadow = AddComponent<Shadowmap>();

		//影の形（メッシュ）を設定
		ptrShadow->SetMeshResource(L"Chara_R_MESH");
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		if (m_TamgentUse) {
			//描画コンポーネントの設定
			auto ptrDraw = AddComponent<BcPNTnTBoneModelDraw>();
			ptrDraw->SetFogEnabled(true);
			//描画するメッシュを設定
			ptrDraw->SetMeshResource(L"Chara_R_MESH_WITH_TAN");
			ptrDraw->SetNormalMapTextureResource(L"Chara_R_NORMAL_TX");
			ptrDraw->SetMeshToTransformMatrix(spanMat);
			ptrDraw->AddAnimation(L"Default", 0, 100, true, 20.0f);
			ptrDraw->ChangeCurrentAnimation(L"Default");
		}
		else {
			//描画コンポーネントの設定
			auto ptrDraw = AddComponent<BcPNTBoneModelDraw>();
			ptrDraw->SetFogEnabled(true);
			//描画するメッシュを設定
			ptrDraw->SetMeshResource(L"Chara_R_MESH");
			ptrDraw->SetMeshToTransformMatrix(spanMat);

			ptrDraw->AddAnimation(L"Default", 0, 100, true, 20.0f);
			ptrDraw->ChangeCurrentAnimation(L"Default");

		}
		//透明処理
		SetAlphaActive(true);
	}

	//更新
	void BoneChara::OnUpdate() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		//アニメーションを更新する
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
	///	Boneキャラ(マルチメッシュ版)
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BoneMultiMeshChara::BoneMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TamgentUse(TamgentUse)
	{
	}
	BoneMultiMeshChara::~BoneMultiMeshChara() {}

	//初期化
	void BoneMultiMeshChara::OnCreate() {
		//初期位置などの設定
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(0.5f, 0.5f, 0.5f);
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetPosition(m_StartPos);

		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ptrShadow = AddComponent<Shadowmap>();

		//影の形（メッシュ）を設定
		ptrShadow->SetMultiMeshResource(L"Object_WalkAnimation_MESH");
		ptrShadow->SetMeshToTransformMatrix(spanMat);

		if (m_TamgentUse) {
			//描画コンポーネントの設定
			auto ptrDraw = AddComponent<BcPNTnTBoneModelDraw>();
			ptrDraw->SetFogEnabled(true);
			//描画するメッシュを設定
			ptrDraw->SetMultiMeshResource(L"Object_WalkAnimation_MESH_WITH_TAN");
			ptrDraw->SetNormalMapTextureResource(L"OBJECT_NORMAL_TX");
			ptrDraw->SetMeshToTransformMatrix(spanMat);
			ptrDraw->AddAnimation(L"Default", 0, 30, true, 10.0f);
			ptrDraw->ChangeCurrentAnimation(L"Default");
		}
		else {
			//描画コンポーネントの設定
			auto ptrDraw = AddComponent<BcPNTBoneModelDraw>();
			ptrDraw->SetFogEnabled(true);
			//描画するメッシュを設定
			ptrDraw->SetMultiMeshResource(L"Object_WalkAnimation_MESH");
			ptrDraw->SetSamplerState(SamplerState::LinearWrap);
			ptrDraw->SetMeshToTransformMatrix(spanMat);

			ptrDraw->AddAnimation(L"Default", 0, 30, true, 10.0f);
			ptrDraw->ChangeCurrentAnimation(L"Default");

		}
	}

	//更新
	void BoneMultiMeshChara::OnUpdate() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		//アニメーションを更新する
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
