/*!
@file GameObject.cpp
@brief ゲームオブジェクト、ステージ実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ゲーム配置オブジェクト親クラス実体
	//--------------------------------------------------------------------------------------
	GameObject::GameObject(const shared_ptr<Stage>& StagePtr) :
		ObjectInterface(),
		ShapeInterface(),
		m_Stage(StagePtr)
	{}

	GameObject::~GameObject() {}

	shared_ptr<Stage> GameObject::GetStage(bool ExceptionActive) const {
		auto shptr = m_Stage.lock();
		if (shptr) {
			return shptr;
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"所属ステージがnullです。自分自身がステージではありませんか？",
					L"if (!shptr)",
					L"GameObject::GetStage()"
				);
			}
			else {
				return nullptr;
			}
		}
		// 所属ステージがnullだった
		// 自分自身がステージの可能性がある
		return nullptr;
	}

	shared_ptr<Stage> GameObject::GetStage(bool ExceptionActive) {
		auto shptr = m_Stage.lock();
		if (shptr) {
			return shptr;
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"所属ステージがnullです。自分自身がステージではありませんか？",
					L"if (!shptr)",
					L"GameObject::GetStage()"
				);
			}
			else {
				return nullptr;
			}
		}
		// 所属ステージがnullだった
		// 自分自身がステージの可能性がある
		return nullptr;
	}

	void GameObject::TransformInit() {
		auto Transptr = GetComponent<Transform>();
		Transptr->SetToBefore();
	}

	void GameObject::ComponentUpdate() {
		auto Transptr = GetComponent<Transform>();
		auto RightPtr = GetComponent<Rigidbody>(false);
		//マップを検証してUpdate
		list<type_index>::iterator it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//指定の型のコンポーネントが見つかった
				if (it2->second->IsUpdateActive()
					&& (it2->second != Transptr)
					&& (it2->second != RightPtr)
					) {
					it2->second->OnUpdate();
				}
			}
			it++;
		}
		if (RightPtr && RightPtr->IsUpdateActive()) {
			RightPtr->OnUpdate();
		}
		//TransformのUpdate
		if (Transptr->IsUpdateActive()) {
			Transptr->OnUpdate();
		}
	}

	void GameObject::DrawShadowmap() {
		auto shadowptr = GetComponent<Shadowmap>(false);
		if (shadowptr) {
			shadowptr->OnDraw();
		}
	}

	void GameObject::ComponentDraw() {
		//Transformがなければ例外
		auto Transptr = GetComponent<Transform>();
		auto RightPtr = GetComponent<Rigidbody>(false);
		auto CollisionPtr = GetComponent<Collision>(false);
		auto StringSpritePtr = GetComponent<StringSprite>(false);

		//マップを検証してDraw
		list<type_index>::iterator it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			//指定の型のコンポーネントが見つかった
			if (it2 != m_CompMap.end() && !dynamic_pointer_cast<Shadowmap>(it2->second)) {
				//シャドウマップ以外なら実行
				//そのコンポーネントの子コンポーネントの描画
				if (it2->second->IsDrawActive()
					&& (it2->second != Transptr)
					&& (it2->second != RightPtr)
					&& (it2->second != CollisionPtr)
					&& (it2->second != StringSpritePtr)
					) {
					it2->second->OnDraw();
				}
			}
			it++;
		}
		if (RightPtr && RightPtr->IsDrawActive()) {
			//RigidbodyがあればDraw()
			RightPtr->OnDraw();
		}
		if (CollisionPtr && CollisionPtr->IsDrawActive()) {
			//CollisionがあればDraw()
			CollisionPtr->OnDraw();
		}
		if (Transptr->IsDrawActive()) {
			Transptr->OnDraw();
		}
		if (StringSpritePtr && StringSpritePtr->IsDrawActive()) {
			//StringSpriteのDraw()
			StringSpritePtr->OnDraw();
		}
	}

	void GameObject::OnPreCreate(){
		//Transform必須
		AddComponent<Transform>();
	}

	const shared_ptr<Camera>& GameObject::OnGetDrawCamera()const{
		//デフォルトはビューのカメラから取り出す
		auto StageView = GetStage()->GetView();
		return StageView->GetTargetCamera();
	}

	const Light& GameObject::OnGetDrawLight() const {
		//ステージからライトを取り出す
		auto StageLight = GetStage()->GetLight();
		return StageLight->GetTargetLight();
	}

	void GameObject::OnGet2DDrawProjMatrix(bsm::Mat4x4& ProjMatrix) const {
		auto viewport = GetStage()->GetView()->GetTargetViewport();
		float w = static_cast<float>(viewport.Width);
		float h = static_cast<float>(viewport.Height);
		ProjMatrix = XMMatrixOrthographicLH(w, h, viewport.MinDepth, viewport.MaxDepth);
	}

	void GameObject::OnDraw(){
		//コンポーネント描画
		//派生クラスで多重定義する場合は
		//コンポーネント描画する場合は
		//GameObject::Draw()を呼び出す
		ComponentDraw();
	}

	void GameObject::DestroyGameObject() {
		auto TMptr = GetComponent<Transform>();
		auto RightPtr = GetComponent<Rigidbody>(false);
		auto CollisionPtr = GetComponent<Collision>(false);
		//マップを検証してOnDestroy
		list<type_index>::iterator it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//指定の型のコンポーネントが見つかった
				if (it2->second->IsUpdateActive()) {
					it2->second->OnDestroy();
				}
			}
			it++;
		}
		if (RightPtr) {
			//RigidbodyがあればOnDestroy()
			RightPtr->OnDestroy();
		}
		if (CollisionPtr) {
			//CollisionがあればOnDestroy()
			CollisionPtr->OnDestroy();
		}
		//TransformのOnDestroy
		TMptr->OnDestroy();
		//自分自身のOnDestroy()
		OnDestroy();
	}

	//--------------------------------------------------------------------------------------
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct GameObjectGroup::Impl {
		vector< weak_ptr<GameObject> > m_Group;
		Impl() {}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class GameObjectGroup;
	//--------------------------------------------------------------------------------------
	GameObjectGroup::GameObjectGroup() :
		ObjectInterface(),
		pImpl(new Impl())
	{}
	GameObjectGroup::~GameObjectGroup() {}
	//アクセサ
	const vector< weak_ptr<GameObject> >& GameObjectGroup::GetGroupVector() const {
		return pImpl->m_Group;
	}
	shared_ptr<GameObject> GameObjectGroup::at(size_t index) {
		if (index >= pImpl->m_Group.size()) {
			wstring msg = Util::SizeTToWStr(index);
			msg += L" >= ";
			msg += Util::SizeTToWStr(pImpl->m_Group.size());
			throw BaseException(
				L"インデックスが範囲外です",
				msg,
				L"GameObjectGroup::at()"
			);
		}
		if (pImpl->m_Group.at(index).expired()) {
			wstring msg = Util::SizeTToWStr(index);
			throw BaseException(
				L"そのインデックスのオブジェクトは無効です。",
				msg,
				L"GameObjectGroup::at()"
			);
		}
		return pImpl->m_Group.at(index).lock();
	}
	size_t GameObjectGroup::size() const {
		return pImpl->m_Group.size();
	}
	//操作
	void GameObjectGroup::IntoGroup(const shared_ptr<GameObject>& Obj) {
		pImpl->m_Group.push_back(Obj);
	}

	void GameObjectGroup::AllClear() {
		pImpl->m_Group.clear();
	}

	//--------------------------------------------------------------------------------------
	//	struct Particle::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Particle::Impl {
		DrawOption m_DrawOption;		//表示オプション
		vector<ParticleSprite> m_ParticleSpriteVec;	//保存しておくスプライトの配列
		bsm::Vec3 m_EmitterPos;			//エミッター位置
		float m_TotalTime;				//タイマー制御する場合に使用する変数
		float m_MaxTime;				//このパーティクル集合体の表示時間
		weak_ptr<TextureResource> m_TextureResource;	//テクスチャ
		Impl(size_t Count, DrawOption Option) :
			m_DrawOption(Option),
			m_ParticleSpriteVec(Count),
			m_EmitterPos(0, 0, 0),
			m_TotalTime(0),
			m_MaxTime(0)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class Particle;
	//	用途: パーティクル
	//	*1つのエミッターを持ち、複数のParticleSpriteを保持する
	//--------------------------------------------------------------------------------------
	Particle::Particle(size_t Count, DrawOption Option) :
		ObjectInterface(),
		pImpl(new Impl(Count, Option))
	{}
	Particle::~Particle() {}

	Particle::DrawOption Particle::GetDrawOption()const {
		return pImpl->m_DrawOption;
	}
	void Particle::SetDrawOption(DrawOption Option) {
		pImpl->m_DrawOption = Option;
	}

	const bsm::Vec3& Particle::GetEmitterPos() const {
		return pImpl->m_EmitterPos;
	}
	void Particle::SetEmitterPos(const bsm::Vec3& Pos) {
		pImpl->m_EmitterPos = Pos;
	}

	float Particle::GetTotalTime() const {
		return pImpl->m_TotalTime;
	}
	void Particle::SetTotalTime(float f) {
		pImpl->m_TotalTime = f;
	}
	void Particle::AddTotalTime(float f) {
		pImpl->m_TotalTime += f;
	}

	float Particle::GetMaxTime() const {
		return pImpl->m_MaxTime;
	}
	void Particle::SetMaxTime(float f) {
		pImpl->m_MaxTime = f;
	}



	bool Particle::IsActive() const {
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			if (Psp.m_Active) {
				//1つでもアクティブがあればtrue
				return true;
			}
		}
		return false;
	}
	bool Particle::IsAllActive() const {
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			if (!Psp.m_Active) {
				//1つでも非アクティブがあればfalse
				return false;
			}
		}
		return true;
	}
	void Particle::SetAllActive() {
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			Psp.m_Active = true;
		}
	}
	void Particle::SetAllNoActive() {
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			Psp.m_Active = false;
		}
	}

	void Particle::Reflesh(size_t Count, Particle::DrawOption Option) {
		pImpl->m_DrawOption = Option;
		pImpl->m_EmitterPos = bsm::Vec3(0, 0, 0);
		pImpl->m_TotalTime = 0;
		pImpl->m_MaxTime = 0;
		pImpl->m_ParticleSpriteVec.clear();
		pImpl->m_ParticleSpriteVec.resize(Count);
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			Psp.Reflesh();
		}
	}

	vector<ParticleSprite>& Particle::GetParticleSpriteVec() const {
		return pImpl->m_ParticleSpriteVec;
	}

	shared_ptr<TextureResource> Particle::GetTextureResource(bool ExceptionActive) const {
		if (!pImpl->m_TextureResource.expired()) {
			return pImpl->m_TextureResource.lock();
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"テクスチャリソースが見つかりません",
					L"if (pImpl->m_Texture.expired())",
					L"Particle::GetTextureResource()"
				);
			}
		}
		return nullptr;
	}
	void Particle::SetTextureResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"テクスチャキーが空白です",
					L"if (ResKey == L\"\"",
					L"Particle::SetTextureResource()"
				);
			}
			pImpl->m_TextureResource = App::GetApp()->GetResource<TextureResource>(ResKey);
		}
		catch (...) {
			throw;
		}
	}
	void Particle::SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr) {
		pImpl->m_TextureResource = TextureResourcePtr;
	}

	void Particle::Draw(const shared_ptr<ParticleManager>& Manager) {
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			if (Psp.m_Active && !pImpl->m_TextureResource.expired()) {
				Manager->AddParticle(Psp, GetDrawOption(),
					GetEmitterPos(), pImpl->m_TextureResource.lock());
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct MultiParticle::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct MultiParticle::Impl {
		vector< shared_ptr<Particle> > m_ParticleVec;
		//加算処理するかどうか
		bool m_AddType;
		Impl():
			m_AddType(false)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class MultiParticle : public GameObject;
	//	用途: マルチエフェクト 
	//--------------------------------------------------------------------------------------
	MultiParticle::MultiParticle(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		pImpl(new Impl())
	{}
	MultiParticle::~MultiParticle() {}

	vector< shared_ptr<Particle> >& MultiParticle::GetParticleVec() const {
		return pImpl->m_ParticleVec;
	}

	bool  MultiParticle::GetAddType() const {
		return pImpl->m_AddType;
	}
	bool MultiParticle::IsAddType() const {
		return pImpl->m_AddType;
	}
	void MultiParticle::SetAddType(bool b) {
		pImpl->m_AddType = b;
	}



	shared_ptr<Particle> MultiParticle::InsertParticle(size_t Count, Particle::DrawOption Option) {
		for (size_t i = 0; i < pImpl->m_ParticleVec.size(); i++) {
			//もし非アクティブのパーティクルがあれば初期化してリターン
			if (!pImpl->m_ParticleVec[i]->IsActive()) {
				pImpl->m_ParticleVec[i]->Reflesh(Count, Option);
				return pImpl->m_ParticleVec[i];
			}
		}
		//新しいパーティクルを追加
		shared_ptr<Particle> ParticlePtr = ObjectFactory::Create<Particle>(Count, Option);
		pImpl->m_ParticleVec.push_back(ParticlePtr);
		return ParticlePtr;
	}

	void MultiParticle::OnPreCreate() {
		GameObject::OnPreCreate();
		//透明処理のみ指定しておく
		SetAlphaActive(true);
	}


	void MultiParticle::OnUpdate() {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()) {
			ParticlePtr->AddTotalTime(ElapsedTime);
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					//移動速度に従って移動させる
					rParticleSprite.m_LocalPos += rParticleSprite.m_Velocity * ElapsedTime;
					if (ParticlePtr->GetTotalTime() >= ParticlePtr->GetMaxTime()) {
						//制限時間になったら
						rParticleSprite.m_Active = false;
					}
				}
			}
		}
	}


	void MultiParticle::OnDraw() {
		if (pImpl->m_ParticleVec.size() > 0) {
			for (auto Ptr : pImpl->m_ParticleVec) {
				if (Ptr->IsActive()) {
					Ptr->Draw(GetStage()->GetParticleManager(IsAddType()));
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ParticleManager::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct ParticleManager::Impl {
		bool m_ZBufferUse;				//Zバッファを使用するかどうか
		bool m_SamplerWrap;				//サンプラーのラッピングするかどうか
		//加算処理するかどうか
		bool m_AddType;
		Impl(bool AddType) :
			m_ZBufferUse(true),
			m_SamplerWrap(false),
			m_AddType(AddType)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class ParticleManager : public GameObject;
	//	用途: パーティクルマネージャ
	//--------------------------------------------------------------------------------------
	//構築と消滅
	ParticleManager::ParticleManager(const shared_ptr<Stage>& StagePtr, bool AddType) :
		GameObject(StagePtr),
		pImpl(new Impl(AddType))
	{}
	ParticleManager::~ParticleManager() {}

	//初期化
	void ParticleManager::OnCreate() {
		try {
			//上限2000でマネージャ作成
			AddComponent<PCTParticleDraw>(2000,pImpl->m_AddType);
			//透明処理のみ指定しておく
			SetAlphaActive(true);
		}
		catch (...) {
			throw;
		}

	}

	bool ParticleManager::GetZBufferUse() const {
		return pImpl->m_ZBufferUse;
	}
	bool ParticleManager::IsZBufferUse() const {
		return pImpl->m_ZBufferUse;
	}
	void ParticleManager::SetZBufferUse(bool b) {
		pImpl->m_ZBufferUse = b;
	}

	bool ParticleManager::GetSamplerWrap() const {
		return pImpl->m_SamplerWrap;
	}
	bool ParticleManager::IsSamplerWrap() const {
		return pImpl->m_SamplerWrap;
	}
	void ParticleManager::SetSamplerWrap(bool b) {
		pImpl->m_SamplerWrap = b;
	}

	void ParticleManager::AddParticle(const ParticleSprite& rParticleSprite, Particle::DrawOption Option,
		const bsm::Vec3& EmitterPos, const shared_ptr<TextureResource>& TextureRes) {
		auto DrawCom = GetComponent<PCTParticleDraw>();

		auto StageView = GetStage()->GetView();

		auto PtrCamera = StageView->GetTargetCamera();
		//カメラの位置
		bsm::Vec3 CameraEye = PtrCamera->GetEye();
		bsm::Vec3 CameraAt = PtrCamera->GetAt();


		bsm::Vec3 WorldPos = rParticleSprite.m_LocalPos + EmitterPos;
		float ToCaneraLength = bsm::length(CameraEye - WorldPos);

		bsm::Vec3 LocalScale;
		LocalScale.x = rParticleSprite.m_LocalScale.x;
		LocalScale.y = rParticleSprite.m_LocalScale.y;
		LocalScale.z = 1.0f;

		bsm::Vec3 Temp;
		bsm::Quat Qt;
		bsm::Mat4x4 RotMatrix;
	//	bsm::Vec4 dammi(0, 0, 0, 0);
		bsm::Vec3 DefUp(0, 1.0f, 0);
		switch (Option) {
		case Particle::DrawOption::Billboard:
		{
			Temp = CameraAt - CameraEye;
			bsm::Vec2 TempVec2(Temp.x, Temp.z);
			if (bsm::length(TempVec2) < 0.1f) {
				DefUp = bsm::Vec3(0, 0, 1.0f);
			}
			Temp.normalize();
			RotMatrix = XMMatrixLookAtLH(bsm::Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix = bsm::inverse(RotMatrix);
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
		}
		break;
		case Particle::DrawOption::Faceing:
		{
			Temp = WorldPos - CameraEye;
			bsm::Vec2 TempVec2(Temp.x, Temp.z);
			if (bsm::length(TempVec2) < 0.1f) {
				DefUp = bsm::Vec3(0, 0, 1.0f);
			}
			RotMatrix = XMMatrixLookAtLH(bsm::Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix = bsm::inverse(RotMatrix);
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
		}
		break;
		case Particle::DrawOption::FaceingY:
			Temp = WorldPos - CameraEye;
			Temp.normalize();
			Qt = XMQuaternionRotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
			Qt.normalize();
			break;
		case Particle::DrawOption::Normal:
			Qt = rParticleSprite.m_LocalQt;
			Qt.normalize();
			break;
		}

		bsm::Mat4x4 matrix;
		matrix.affineTransformation(
			LocalScale,
			bsm::Vec3(0,0,0),
			Qt,
			WorldPos
		);
		DrawCom->AddParticle(ToCaneraLength, matrix, TextureRes, rParticleSprite.m_Color);
	}

	void ParticleManager::OnDraw() {
		auto DrawCom = GetComponent<PCTParticleDraw>();
		DrawCom->OnDraw();
	}


	//--------------------------------------------------------------------------------------
	//	struct Stage::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Stage::Impl {
		//updateするかどうか
		bool m_UpdateActive;
		//パーティクルマネージャ(透明処理)
		shared_ptr<ParticleManager> m_AlphaParticleManager;
		//パーティクルマネージャ(加算処理)
		shared_ptr<ParticleManager> m_AddParticleManager;
		//コリジョン管理者
		shared_ptr<CollisionManager> m_CollisionManager;
		//オブジェクトの配列
		vector< shared_ptr<GameObject> > m_GameObjectVec;
		//途中にオブジェクトが追加された場合、ターンの開始まで待つ配列
		vector< shared_ptr<GameObject> > m_WaitAddObjectVec;
		//途中にオブジェクトが削除された場合、ターンの開始まで待つ配列
		vector< shared_ptr<GameObject> > m_WaitRemoveObjectVec;
		//Spriteかそうでないかを分離する配列
		vector< shared_ptr<GameObject> > m_SpriteVec;
		vector< shared_ptr<GameObject> > m_Object3DVec;
		//3Dの透明と非透明を分離する配列
		vector< shared_ptr<GameObject> > m_Object3DNormalVec;
		vector< shared_ptr<GameObject> > m_Object3DAlphaVec;
		//物理計算
		BasePhysics m_BasePhysics;
		//物理計算で使う空物理オブジェクトのset
		set<uint16_t> m_VacantPhysicsIndices;
		//現在Drawされているビューのインデックス
		size_t m_DrawViewIndex;
		//ビューのポインタ
		shared_ptr<ViewBase> m_ViewBase;
		//ライトのポインタ
		shared_ptr<LightBase> m_LightBase;
		//シェアオブジェクトポインタのマップ
		map<const wstring, weak_ptr<GameObject> > m_SharedMap;
		//シェアグループのポインタのマップ
		map<const wstring, shared_ptr<GameObjectGroup> >  m_SharedGroupMap;
		vector< shared_ptr<Stage> > m_ChildStageVec;	//子供ステージの配列
		weak_ptr<Stage> m_ParentStage;		//親ステージ
		//シャドウマップを使うかどうか
		bool m_IsShadowmapDraw;
		//物理計算を使うかどうか
		bool m_IsPhysicsActive;
		//衝突判定マネージャのパフォーマンスカウンタ
		PerformanceCounter m_CollisionPerformance;
		//Updateのパフォーマンスカウンタ
		PerformanceCounter m_UpdatePerformance;
		//Drawのパフォーマンスカウンタ
		PerformanceCounter m_DrawPerformance;
		Impl() :
			m_UpdateActive(true),
			m_DrawViewIndex(0),
			m_IsShadowmapDraw(true),
			m_IsPhysicsActive(false)
		{}
		~Impl() {}
		void RemoveTargetGameObject(const shared_ptr<GameObject>& targetobj);
	};
	void Stage::Impl::RemoveTargetGameObject(const shared_ptr<GameObject>& targetobj) {
		auto it = m_GameObjectVec.begin();
		while (it != m_GameObjectVec.end()) {
			if (*it == targetobj) {
				//削除されることをオブジェクトに伝える
				targetobj->DestroyGameObject();
				m_GameObjectVec.erase(it);
				return;
			}
			it++;
		}
	}


	//--------------------------------------------------------------------------------------
	//	ステージクラス
	//--------------------------------------------------------------------------------------
	Stage::Stage() :
		ObjectInterface(),
		ShapeInterface(),
		pImpl(new Impl())
	{}
	Stage::~Stage() {}

	//プライベートサブ関数
	void Stage::PushBackGameObject(const shared_ptr<GameObject>& Ptr) {
		//このステージはクリエイト後である
		if (IsCreated()) {
			pImpl->m_WaitAddObjectVec.push_back(Ptr);
		}
		else {
			//クリエイト前
			pImpl->m_GameObjectVec.push_back(Ptr);
		}
		Ptr->TransformInit();
	}

	//削除オブジェクトの設定
	void Stage::RemoveBackGameObject(const shared_ptr<GameObject>& Ptr) {
		pImpl->m_WaitRemoveObjectVec.push_back(Ptr);
	}

	shared_ptr<GameObject> Stage::GetSharedGameObjectEx(const wstring& Key, bool ExceptionActive)const {
		map<const wstring, weak_ptr<GameObject> >::const_iterator it;
		//重複キーの検査
		it = pImpl->m_SharedMap.find(Key);
		if (it != pImpl->m_SharedMap.end()) {
			if (it->second.expired()) {
				//すでに無効
				if (ExceptionActive) {
					//例外発生
					wstring keyerr = Key;
					throw BaseException(
						L"オブジェクトが無効です",
						keyerr,
						L"Stage::GetSharedGameObject()"
					);
				}
			}
			return it->second.lock();
		}
		else {
			//指定の名前が見つからなかった
			if (ExceptionActive) {
				//例外発生
				wstring keyerr = Key;
				throw BaseException(
					L"オブジェクトが見つかりません",
					keyerr,
					L"Stage::GetSharedGameObject()"
				);
			}
		}
		return nullptr;
	}

	shared_ptr<ParticleManager> Stage::GetParticleManager(bool Addtype) const { 
		if (Addtype) {
			return pImpl->m_AddParticleManager;
		}
		else {
			return pImpl->m_AlphaParticleManager;
		}
	}

	BasePhysics& Stage::GetBasePhysics() const {
		if (!IsPhysicsActive()) {
			throw BaseException(
				L"物理演算が無効になっています。有効にしてから取得してください。",
				L"if (!IsPhysicsActive())",
				L"Stage::GetBasePhysics()()"
			);
		}
		return pImpl->m_BasePhysics;
	}


	bool Stage::IsPhysicsActive() const {
		return pImpl->m_IsPhysicsActive;
		
	}
	void Stage::SetPhysicsActive(bool b) {
		pImpl->m_IsPhysicsActive = b;
	}


	uint16_t Stage::GetVacantPhysicsIndex(){
		uint16_t ret = UINT16_MAX;
		if (pImpl->m_VacantPhysicsIndices.size() > 0) {
			//空いているIDがある。先頭を取得
			auto it = pImpl->m_VacantPhysicsIndices.begin();
			ret = *it;
			//IDを使うのでそのIDを削除
			pImpl->m_VacantPhysicsIndices.erase(ret);
		}
		return ret;
	}

	void Stage::SetVacantPhysicsIndex(uint16_t index) {
		pImpl->m_VacantPhysicsIndices.insert(index);
	}

	shared_ptr<CollisionManager> Stage::GetCollisionManager() const {
		return pImpl->m_CollisionManager;
	}


	vector< shared_ptr<GameObject> >& Stage::GetGameObjectVec() { return pImpl->m_GameObjectVec; }
	vector< shared_ptr<GameObject> >& Stage::GetGameObjectVec() const{ return pImpl->m_GameObjectVec; }


	//追加や削除待ちになってるオブジェクトを追加・削除する
	void Stage::SetWaitToObjectVec(){
		if (!pImpl->m_WaitRemoveObjectVec.empty()) {
			for (auto Ptr : pImpl->m_WaitRemoveObjectVec) {
				pImpl->RemoveTargetGameObject(Ptr);
			}
		}
		pImpl->m_WaitRemoveObjectVec.clear();
		if (!pImpl->m_WaitAddObjectVec.empty()){
			for (auto Ptr : pImpl->m_WaitAddObjectVec){
				pImpl->m_GameObjectVec.push_back(Ptr);
			}
		}
		pImpl->m_WaitAddObjectVec.clear();
	}



	shared_ptr<GameObject> Stage::GetSharedObject(const wstring& Key, bool ExceptionActive)const {
		shared_ptr<GameObject> Ptr = GetSharedGameObjectEx(Key, ExceptionActive);
		return Ptr;
	}


	void Stage::SetSharedGameObject(const wstring& Key, const shared_ptr<GameObject>& Ptr) {
		map<const wstring, weak_ptr<GameObject> >::iterator it;
		//重複キーの検査
		it = pImpl->m_SharedMap.find(Key);
		if (it != pImpl->m_SharedMap.end()) {
			//既に存在した
			//例外発生
			wstring keyerr = Key;
			throw BaseException(
				L"同名のシェアオブジェクトがあります",
				keyerr,
				L"Stage::SetSharedGameObjectEx()"
			);
		}
		else {
			pImpl->m_SharedMap[Key] = Ptr;
		}
	}

	shared_ptr<GameObjectGroup> Stage::CreateSharedObjectGroup(const wstring& Key) {
		try {
			map<const wstring, shared_ptr<GameObjectGroup> >::iterator it;
			//重複キーの検査
			it = pImpl->m_SharedGroupMap.find(Key);
			if (it != pImpl->m_SharedGroupMap.end()) {
				//既に存在した
				//例外発生
				wstring keyerr = Key;
				throw BaseException(
					L"同名のシェアオブジェクト配列があります",
					keyerr,
					L"Stage::CreateSharedObjectGroup()"
				);
			}
			else {
				auto Ptr = ObjectFactory::Create<GameObjectGroup>();
				pImpl->m_SharedGroupMap[Key] = Ptr;
				return Ptr;
			}
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<GameObjectGroup> Stage::GetSharedObjectGroup(const wstring& Key, bool ExceptionActive)const {
		//重複キーの検査
		auto it = pImpl->m_SharedGroupMap.find(Key);
		if (it != pImpl->m_SharedGroupMap.end()) {
			//ペアのsecondを返す
			return it->second;
		}
		else {
			//指定の名前が見つからなかった
			if (ExceptionActive) {
				//例外発生
				wstring keyerr = Key;
				throw BaseException(
					L"指定のキーが見つかりません",
					keyerr,
					L"Stage::GetSharedObjectGroup() const"
				);
			}
		}
		return nullptr;
	}

	void Stage::SetSharedObjectGroup(const wstring& Key, const shared_ptr<GameObjectGroup>& NewPtr) {
		//重複キーの検査
		auto it = pImpl->m_SharedGroupMap.find(Key);
		if (it != pImpl->m_SharedGroupMap.end()) {
			//例外発生
			wstring keyerr = Key;
			throw BaseException(
				L"同名のシェアオブジェクト配列があります",
				keyerr,
				L"Stage::SetSharedObjectGroup()"
			);
		}
		else {
			//指定の名前が見つからなかった
			//登録できる
			pImpl->m_SharedGroupMap[Key] = NewPtr;
		}
	}


	vector< shared_ptr<Stage> >& Stage::GetChileStageVec() {
		return pImpl->m_ChildStageVec;
	}
	vector< shared_ptr<Stage> >& Stage::GetChileStageVec() const {
		return pImpl->m_ChildStageVec;
	}

	void Stage::AddChileStageBase(const shared_ptr<Stage>& ChildStage) {
		pImpl->m_ChildStageVec.push_back(ChildStage);
		ChildStage->SetParentStage(GetThis<Stage>());
	}

	shared_ptr<Stage> Stage::GetParentStage() const {
		if (!pImpl->m_ParentStage.expired()) {
			return  pImpl->m_ParentStage.lock();
		}
		return nullptr;
	}
	void Stage::SetParentStage(const shared_ptr<Stage>& ParentStage) {
		pImpl->m_ParentStage = ParentStage;
	}


	void Stage::SetView(const shared_ptr<ViewBase>& v) {
		pImpl->m_ViewBase = v;
	}

	const shared_ptr<ViewBase>& Stage::GetView(bool ExceptionActive)const {
		if (ExceptionActive) {
			if (!pImpl->m_ViewBase) {
				throw BaseException(
					L"ステージにビューが設定されていません。",
					L"if (!pImpl->m_ViewBase)",
					L"Stage::GetView()"
				);
			}
		}
		return pImpl->m_ViewBase;
	}

	void Stage::SetLight(const shared_ptr<LightBase>& L) {
		pImpl->m_LightBase = L;

	}
	const shared_ptr<LightBase>& Stage::GetLight()const {
		if (!pImpl->m_LightBase) {
			throw BaseException(
				L"ステージにライトが設定されていません。",
				L"if (!pImpl->m_LightBase)",
				L"Stage::GetLight()"
			);
		}
		return pImpl->m_LightBase;
	}


	//アクセサ
	bool Stage::IsUpdateActive() const { return pImpl->m_UpdateActive; }
	bool Stage::GetUpdateActive() const { return pImpl->m_UpdateActive; }
	void Stage::SetUpdateActive(bool b) { pImpl->m_UpdateActive = b; }

	void Stage::OnPreCreate() {
		//パーティクルマネージャの作成(透明処理)
		pImpl->m_AlphaParticleManager = ObjectFactory::Create<ParticleManager>(GetThis<Stage>(),false);
		//パーティクルマネージャの作成(加算処理)
		pImpl->m_AddParticleManager = ObjectFactory::Create<ParticleManager>(GetThis<Stage>(),true);
		//コリジョン管理者の作成
		pImpl->m_CollisionManager = ObjectFactory::Create<CollisionManager>(GetThis<Stage>());
		//物理計算リセット
		pImpl->m_BasePhysics.Reset();
	}


	//ステージ内の更新（シーンからよばれる）
	void Stage::UpdateStage() {
		if (IsUpdatePerformanceActive()) {
			pImpl->m_UpdatePerformance.Start();
		}
		//追加・削除まちオブジェクトの追加と削除
		SetWaitToObjectVec();
		//Transformコンポーネントの値をバックアップにコピー
		for (auto& ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				auto ptr2 = ptr->GetComponent<Transform>();
				ptr2->SetToBefore();
			}
		}
		//物理オブジェクトのフォースの初期化
		if (IsPhysicsActive()) {
			pImpl->m_BasePhysics.InitForce();
		}

		//配置オブジェクトの更新処理
		for (auto& ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->OnUpdate();
			}
		}

		//自身の更新処理
		if (IsUpdateActive()) {
			OnUpdate();
		}
		//物理オブジェクトの更新
		if (IsPhysicsActive()) {
			pImpl->m_BasePhysics.Update(false);
		}
		//配置オブジェクトのコンポーネント更新
		for (auto& ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->ComponentUpdate();
			}
		}

		////衝突判定
		UpdateCollision();
		//配置オブジェクトの更新後処理
		for (auto& ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->OnUpdate2();
			}
		}
		//自身の更新後処理
		if (IsUpdateActive()) {
			OnUpdate2();
		}

		//自身のビューをアップデート
		auto& ViewPtr = GetView(false);
		if (ViewPtr && ViewPtr->IsUpdateActive()) {
			ViewPtr->OnUpdate();
		}
		//子供ステージの更新
		for (auto& PtrChileStage : GetChileStageVec()) {
			PtrChileStage->UpdateStage();
		}
		if (IsUpdatePerformanceActive()) {
			pImpl->m_UpdatePerformance.End();
		}
	}


	//衝突判定の更新（ステージから呼ぶ）
	//衝突判定をカスタマイズするためには
	//この関数を多重定義する
	void Stage::UpdateCollision() {
		if (pImpl->m_CollisionPerformance.IsAvtive()) {
			pImpl->m_CollisionPerformance.Start();
			//衝突判定管理者のUpdate(判定)
			pImpl->m_CollisionManager->OnUpdate();
			pImpl->m_CollisionPerformance.End();
		}
		else {
			//衝突判定管理者のUpdate(判定)
			pImpl->m_CollisionManager->OnUpdate();
		}
	}


	//シャドウマップを使うかどうか
	bool Stage::IsShadowmapDraw() const {
		return pImpl->m_IsShadowmapDraw;
	}
	void Stage::SetShadowmapDraw(bool b) {
		pImpl->m_IsShadowmapDraw = b;
	}

	void Stage::SetCollisionPerformanceActive(bool b) {
		pImpl->m_CollisionPerformance.SetActive(b);
	}

	bool Stage::IsCollisionPerformanceActive() const {
		return pImpl->m_CollisionPerformance.IsAvtive();
	}

	float Stage::GetCollisionPerformanceTime() const {
		return pImpl->m_CollisionPerformance.GetPerformanceTime();
	}

	void Stage::SetUpdatePerformanceActive(bool b) {
		pImpl->m_UpdatePerformance.SetActive(b);
	}
	bool Stage::IsUpdatePerformanceActive() const {
		return pImpl->m_UpdatePerformance.IsAvtive();

	}
	float Stage::GetUpdatePerformanceTime() const {
		return pImpl->m_UpdatePerformance.GetPerformanceTime();

	}

	void Stage::SetDrawPerformanceActive(bool b) {
		pImpl->m_DrawPerformance.SetActive(b);
	}
	bool Stage::IsDrawPerformanceActive() const {
		return pImpl->m_DrawPerformance.IsAvtive();

	}
	float Stage::GetDrawPerformanceTime() const {
		return pImpl->m_DrawPerformance.GetPerformanceTime();
	}

	//ステージ内のシャドウマップ描画（ステージからよばれる）
	void Stage::DrawShadowmapStage() {
		for (auto ptr : pImpl->m_GameObjectVec) {
			if (ptr->IsDrawActive()) {
				ptr->DrawShadowmap();
			}
		}
	}

	//ステージ内の描画（ステージからよばれる）
	void Stage::DrawStage() {
		//レイヤーの取得と設定
		set<int> DrawLayers;
		//Spriteかそうでないかを分離
		for (auto& ptr : GetGameObjectVec()) {
			if (ptr->IsDrawActive()) {
				//描画レイヤーに登録
				DrawLayers.insert(ptr->GetDrawLayer());
				//Spriteかその派生クラスなら分離
				if (ptr->GetComponent<SpriteBaseDraw>(false) || ptr->IsSpriteDraw()) {
					pImpl->m_SpriteVec.push_back(ptr);
				}
				else {
					pImpl->m_Object3DVec.push_back(ptr);
				}
			}
		}
		//3Dの透明と非透明を分離
		for (auto& ptr : pImpl->m_Object3DVec) {
			if (ptr->IsDrawActive()) {
				if (ptr->IsAlphaActive()) {
					pImpl->m_Object3DAlphaVec.push_back(ptr);
				}
				else {
					pImpl->m_Object3DNormalVec.push_back(ptr);
				}
			}
		}

		auto PtrCamera = pImpl->m_ViewBase->GetTargetCamera();
		//カメラの位置
		bsm::Vec3 CameraEye = PtrCamera->GetEye();
		//透明の3Dオブジェクトをカメラからの距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto func = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool {
			auto PtrLeftTrans = Left->GetComponent<Transform>();
			auto PtrRightTrans = Right->GetComponent<Transform>();

			auto LeftPos = PtrLeftTrans->GetWorldMatrix().transInMatrix();
			auto RightPos = PtrRightTrans->GetWorldMatrix().transInMatrix();

			auto LeftLen = bsm::length(LeftPos - CameraEye);
			auto RightLen = bsm::length(RightPos - CameraEye);

			return (LeftLen > RightLen);
		};

		std::sort(pImpl->m_Object3DAlphaVec.begin(), pImpl->m_Object3DAlphaVec.end(), func);

		//3Dノーマルオブジェクトの描画準備
		for (auto& ptr : pImpl->m_Object3DNormalVec) {
			ptr->OnPreDraw();
		}
		//3D透明オブジェクトの描画準備
		for (auto& ptr : pImpl->m_Object3DAlphaVec) {
			ptr->OnPreDraw();
		}
		//パーティクルの描画準備（透明）
		GetParticleManager(false)->OnPreDraw();
		//パーティクルの描画準備（加算）
		GetParticleManager(true)->OnPreDraw();
		//スプライトオブジェクトの描画準備
		for (auto& ptr : pImpl->m_SpriteVec) {
			ptr->OnPreDraw();
		}

		//--------------------------------------------------------
		//スプライトをZ座標距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto funcSprite = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool {
			auto PtrLeftTrans = Left->GetComponent<Transform>();
			auto PtrRightTrans = Right->GetComponent<Transform>();

			auto LeftPos = PtrLeftTrans->GetWorldMatrix().transInMatrix();
			auto RightPos = PtrRightTrans->GetWorldMatrix().transInMatrix();

			float LeftZ = LeftPos.z;
			float RightZ = RightPos.z;
			return (LeftZ > RightZ);
		};
		std::sort(pImpl->m_SpriteVec.begin(), pImpl->m_SpriteVec.end(), funcSprite);

		for (auto it = DrawLayers.begin(); it != DrawLayers.end(); it++) {
			int Tgt = *it;
			//3Dノーマルオブジェクトの描画
			for (auto& ptr : pImpl->m_Object3DNormalVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
			//3D透明オブジェクトの描画
			for (auto& ptr : pImpl->m_Object3DAlphaVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
			//パーティクルの描画
			//パーティクルマネージャは描画レイヤーごとに初期化されるので
			//毎レイヤー描画する
			//透明処理
			GetParticleManager(false)->OnDraw();
			//加算処理
			GetParticleManager(true)->OnDraw();
			//スプライトオブジェクトの描画
			for (auto& ptr : pImpl->m_SpriteVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
		}
		//ステージのDraw();
		OnDraw();
		//ワーク用配列のクリア
		//ワーク配列は毎ターンごとに初期化されるが、
		//最大値は減らないので2回目のターン以降は高速に動作する
		pImpl->m_Object3DVec.clear();
		pImpl->m_SpriteVec.clear();
		pImpl->m_Object3DNormalVec.clear();
		pImpl->m_Object3DAlphaVec.clear();
	}

	//ステージ内のすべての描画（シーンからよばれる）
	void Stage::RenderStage() {
		if (IsDrawPerformanceActive()) {
			pImpl->m_DrawPerformance.Start();
		}
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto MultiPtr = dynamic_pointer_cast<MultiView>(GetView());
		if (MultiPtr) {
			for (size_t i = 0; i < MultiPtr->GetViewSize(); i++) {
				MultiPtr->SetTargetIndex(i);
				if (IsShadowmapDraw()) {
					Dev->ClearShadowmapViews();
					Dev->StartShadowmapDraw();
					DrawShadowmapStage();
					Dev->EndShadowmapDraw();
				}
				//デフォルト描画の開始
				Dev->StartDefaultDraw();
#if (BASECROSS_DXVERSION == 11)
				RsSetViewport(MultiPtr->GetTargetViewport());
#endif
				DrawStage();
				//デフォルト描画の終了
				Dev->EndDefaultDraw();
			}
			//描画が終わったら更新処理用に先頭のカメラにターゲットを設定する
			MultiPtr->SetTargetIndex(0);
		}
		else {
			if (IsShadowmapDraw()) {
				Dev->ClearShadowmapViews();
				Dev->StartShadowmapDraw();
				DrawShadowmapStage();
				Dev->EndShadowmapDraw();
			}
			//デフォルト描画の開始
			Dev->StartDefaultDraw();
#if (BASECROSS_DXVERSION == 11)
			RsSetViewport(GetView()->GetTargetViewport());
#endif
			DrawStage();
			//デフォルト描画の終了
			Dev->EndDefaultDraw();
		}
		//子供ステージの描画
		for (auto PtrChileStage : GetChileStageVec()) {
			PtrChileStage->RenderStage();
		}
		if (IsDrawPerformanceActive()) {
			pImpl->m_DrawPerformance.End();
		}
	}

	void Stage::DestroyStage() {
		//子供ステージの削除処理
		for (auto PtrChileStage : pImpl->m_ChildStageVec) {
			PtrChileStage->DestroyStage();
		}
		//配置オブジェクトの削除処理
		for (auto ptr : GetGameObjectVec()) {
				ptr->DestroyGameObject();
		}
		//自身の削除処理
		OnDestroy();
	}


	//--------------------------------------------------------------------------------------
	//	struct MovieStage::Impl;
	//--------------------------------------------------------------------------------------
	struct MovieStage::Impl {
		wstring m_MovieFileName;
		bool m_AutoRepeat;
		Impl() :
			m_AutoRepeat(true)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	ムービーステージクラス
	//--------------------------------------------------------------------------------------
	MovieStage::MovieStage():
		Stage(),
		pImpl(new Impl())
	{}

	MovieStage::MovieStage(const wstring& MivieFileName) :
		Stage(),
		pImpl(new Impl())
	{
		pImpl->m_MovieFileName = MivieFileName;
	}
	MovieStage::~MovieStage() {}

	void MovieStage::OnCreate() {
		auto ptrView = CreateView<SingleView>();
		if (pImpl->m_MovieFileName != L"") {
			App::GetApp()->PlayMovie(pImpl->m_MovieFileName);
		}
	}

	void MovieStage::SetMovieFileName(const wstring& MivieFileName) {
		pImpl->m_MovieFileName = MivieFileName;
	}

	void MovieStage::Play() {
		if (pImpl->m_MovieFileName != L"") {
			App::GetApp()->PlayMovie(pImpl->m_MovieFileName);
		}
	}

	bool MovieStage::IsAutoRepeat() const {
		return App::GetApp()->IsMovieAutoRepeat();
	}
	void MovieStage::SetAutoRepeat(bool b) {
		App::GetApp()->SetMovieAutoRepeat(b);
	}

	void MovieStage::OnDestroy() {
		App::GetApp()->ClearMovie();
	}

	//--------------------------------------------------------------------------------------
	//	struct SceneBase::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct SceneBase::Impl {
		//アクティブなステージ
		shared_ptr<Stage> m_ActiveStage;
		//クリアする色
		bsm::Col4 m_ClearColor;
		Impl():
			m_ActiveStage(),
			m_ClearColor(0,0,0,1.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	シーン親クラス
	//--------------------------------------------------------------------------------------
	void SceneBase::ConvertVertex(const vector<VertexPositionNormalTexture>& vertices,
		vector<VertexPositionColor>& new_pc_vertices,
		vector<VertexPositionNormal>& new_pn_vertices,
		vector<VertexPositionTexture>& new_pt_vertices,
		vector<VertexPositionNormalTangentTexture>& new_pntnt_vertices
	) {
		new_pc_vertices.clear();
		new_pn_vertices.clear();
		new_pt_vertices.clear();
		new_pntnt_vertices.clear();
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionColor new_pc_v;
			VertexPositionNormal new_pn_v;
			VertexPositionTexture new_pt_v;
			VertexPositionNormalTangentTexture new_pntnt_v;

			new_pc_v.position = vertices[i].position;
			new_pc_v.color = bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f);

			new_pn_v.position = vertices[i].position;
			new_pn_v.normal = vertices[i].normal;

			new_pt_v.position = vertices[i].position;
			new_pt_v.textureCoordinate = vertices[i].textureCoordinate;

			new_pntnt_v.position = vertices[i].position;
			new_pntnt_v.normal = vertices[i].normal;
			new_pntnt_v.textureCoordinate = vertices[i].textureCoordinate;
			bsm::Vec3 n = bsm::cross((bsm::Vec3)new_pntnt_v.normal, bsm::Vec3(0, 1, 0));
			new_pntnt_v.tangent = bsm::Vec4(n.x, n.y, n.z, 0.0f);
			new_pntnt_v.tangent.w = 0.0f;

			new_pc_vertices.push_back(new_pc_v);
			new_pn_vertices.push_back(new_pn_v);
			new_pt_vertices.push_back(new_pt_v);
			new_pntnt_vertices.push_back(new_pntnt_v);

		}

	}



	SceneBase::SceneBase() :
		SceneInterface(),
		pImpl(new Impl())
	{
		try {
			//デフォルトのリソースの作成
			App::GetApp()->RegisterResource(L"DEFAULT_SQUARE", MeshResource::CreateSquare(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_CUBE", MeshResource::CreateCube(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_SPHERE", MeshResource::CreateSphere(1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CAPSULE", MeshResource::CreateCapsule(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CYLINDER", MeshResource::CreateCylinder(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CONE", MeshResource::CreateCone(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_TORUS", MeshResource::CreateTorus(1.0f, 0.3f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_TETRAHEDRON", MeshResource::CreateTetrahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_OCTAHEDRON", MeshResource::CreateOctahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_DODECAHEDRON", MeshResource::CreateDodecahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_ICOSAHEDRON", MeshResource::CreateIcosahedron(1.0f));

			vector<VertexPositionNormalTexture> vertices;
			vector<VertexPositionColor> new_pc_vertices;
			vector<VertexPositionNormal> new_pn_vertices;
			vector<VertexPositionTexture> new_pt_vertices;
			vector<VertexPositionNormalTangentTexture> new_pntnt_vertices;

			vector<uint16_t> indices;

			MeshUtill::CreateSquare(1.0f, vertices, indices);
			ConvertVertex(vertices, new_pc_vertices, new_pn_vertices, new_pt_vertices, new_pntnt_vertices);
			MeshUtill::SetNormalTangent(new_pntnt_vertices);
			App::GetApp()->RegisterResource(L"DEFAULT_PC_SQUARE", MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PN_SQUARE", MeshResource::CreateMeshResource(new_pn_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PT_SQUARE", MeshResource::CreateMeshResource(new_pt_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PNTnT_SQUARE", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
			vertices.clear();
			indices.clear();

			MeshUtill::CreateCube(1.0f, vertices, indices);
			ConvertVertex(vertices, new_pc_vertices, new_pn_vertices, new_pt_vertices, new_pntnt_vertices);
			MeshUtill::SetNormalTangent(new_pntnt_vertices);
			App::GetApp()->RegisterResource(L"DEFAULT_PC_CUBE", MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PN_CUBE", MeshResource::CreateMeshResource(new_pn_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PT_CUBE", MeshResource::CreateMeshResource(new_pt_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PNTnT_CUBE", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
			vertices.clear();
			indices.clear();

			MeshUtill::CreateSphere(1.0f,18, vertices, indices);
			ConvertVertex(vertices, new_pc_vertices, new_pn_vertices, new_pt_vertices, new_pntnt_vertices);
			MeshUtill::SetNormalTangent(new_pntnt_vertices);
			App::GetApp()->RegisterResource(L"DEFAULT_PC_SPHERE", MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PN_SPHERE", MeshResource::CreateMeshResource(new_pn_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PT_SPHERE", MeshResource::CreateMeshResource(new_pt_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PNTnT_SPHERE", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
			vertices.clear();
			indices.clear();

			bsm::Vec3 PointA(0, -1.0f / 2.0f, 0);
			bsm::Vec3 PointB(0, 1.0f / 2.0f, 0);
			//Capsuleの作成(ヘルパー関数を利用)
			MeshUtill::CreateCapsule(1.0f, PointA, PointB,18, vertices, indices);
			ConvertVertex(vertices, new_pc_vertices, new_pn_vertices, new_pt_vertices, new_pntnt_vertices);
			MeshUtill::SetNormalTangent(new_pntnt_vertices);
			App::GetApp()->RegisterResource(L"DEFAULT_PC_CAPSULE", MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PN_CAPSULE", MeshResource::CreateMeshResource(new_pn_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PT_CAPSULE", MeshResource::CreateMeshResource(new_pt_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PNTnT_CAPSULE", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
			vertices.clear();
			indices.clear();

			MeshUtill::CreateCylinder(1.0f, 1.0f, 18, vertices, indices);
			ConvertVertex(vertices, new_pc_vertices, new_pn_vertices, new_pt_vertices, new_pntnt_vertices);
			MeshUtill::SetNormalTangent(new_pntnt_vertices);
			App::GetApp()->RegisterResource(L"DEFAULT_PC_CYLINDER", MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PN_CYLINDER", MeshResource::CreateMeshResource(new_pn_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PT_CYLINDER", MeshResource::CreateMeshResource(new_pt_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PNTnT_CYLINDER", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
			vertices.clear();
			indices.clear();

			MeshUtill::CreateCone(1.0f, 1.0f, 18, vertices, indices);
			ConvertVertex(vertices, new_pc_vertices, new_pn_vertices, new_pt_vertices, new_pntnt_vertices);
			MeshUtill::SetNormalTangent(new_pntnt_vertices);
			App::GetApp()->RegisterResource(L"DEFAULT_PC_CONE", MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PN_CONE", MeshResource::CreateMeshResource(new_pn_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PT_CONE", MeshResource::CreateMeshResource(new_pt_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PNTnT_CONE", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
			vertices.clear();
			indices.clear();

			MeshUtill::CreateTorus(1.0f, 0.3f, 18, vertices, indices);
			ConvertVertex(vertices, new_pc_vertices, new_pn_vertices, new_pt_vertices, new_pntnt_vertices);
			MeshUtill::SetNormalTangent(new_pntnt_vertices);
			App::GetApp()->RegisterResource(L"DEFAULT_PC_TORUS", MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PN_TORUS", MeshResource::CreateMeshResource(new_pn_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PT_TORUS", MeshResource::CreateMeshResource(new_pt_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PNTnT_TORUS", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
			vertices.clear();
			indices.clear();

			MeshUtill::CreateTetrahedron(1.0f, vertices, indices);
			ConvertVertex(vertices, new_pc_vertices, new_pn_vertices, new_pt_vertices, new_pntnt_vertices);
			MeshUtill::SetNormalTangent(new_pntnt_vertices);
			App::GetApp()->RegisterResource(L"DEFAULT_PC_TETRAHEDRON", MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PN_TETRAHEDRON", MeshResource::CreateMeshResource(new_pn_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PT_TETRAHEDRON", MeshResource::CreateMeshResource(new_pt_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PNTnT_TETRAHEDRON", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
			vertices.clear();
			indices.clear();

			MeshUtill::CreateOctahedron(1.0f, vertices, indices);
			ConvertVertex(vertices, new_pc_vertices, new_pn_vertices, new_pt_vertices, new_pntnt_vertices);
			MeshUtill::SetNormalTangent(new_pntnt_vertices);
			App::GetApp()->RegisterResource(L"DEFAULT_PC_OCTAHEDRON", MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PN_OCTAHEDRON", MeshResource::CreateMeshResource(new_pn_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PT_OCTAHEDRON", MeshResource::CreateMeshResource(new_pt_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PNTnT_OCTAHEDRON", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
			vertices.clear();
			indices.clear();

			MeshUtill::CreateDodecahedron(1.0f, vertices, indices);
			ConvertVertex(vertices, new_pc_vertices, new_pn_vertices, new_pt_vertices, new_pntnt_vertices);
			MeshUtill::SetNormalTangent(new_pntnt_vertices);
			App::GetApp()->RegisterResource(L"DEFAULT_PC_DODECAHEDRON", MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PN_DODECAHEDRON", MeshResource::CreateMeshResource(new_pn_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PT_DODECAHEDRON", MeshResource::CreateMeshResource(new_pt_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PNTnT_DODECAHEDRON", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
			vertices.clear();
			indices.clear();

			MeshUtill::CreateIcosahedron(1.0f, vertices, indices);
			ConvertVertex(vertices, new_pc_vertices, new_pn_vertices, new_pt_vertices, new_pntnt_vertices);
			MeshUtill::SetNormalTangent(new_pntnt_vertices);
			App::GetApp()->RegisterResource(L"DEFAULT_PC_ICOSAHEDRON", MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PN_ICOSAHEDRON", MeshResource::CreateMeshResource(new_pn_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PT_ICOSAHEDRON", MeshResource::CreateMeshResource(new_pt_vertices, indices, false));
			App::GetApp()->RegisterResource(L"DEFAULT_PNTnT_ICOSAHEDRON", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
			vertices.clear();
			indices.clear();
			//物理ワイフレーム用
			MeshUtill::CreateSphere(2.0f, 6, vertices, indices);
			vector<VertexPositionColor> col_vertices;
			for (auto& v : vertices) {
				VertexPositionColor vertex;
				vertex.position = v.position;
				vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				col_vertices.push_back(vertex);
			}
			App::GetApp()->RegisterResource(L"PSWIRE_PC_SPHERE", MeshResource::CreateMeshResource(col_vertices, indices, false));
			vertices.clear();
			indices.clear();
			col_vertices.clear();

			MeshUtill::CreateCube(2.0f, vertices, indices);
			for (auto& v : vertices) {
				VertexPositionColor vertex;
				vertex.position = v.position;
				vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				col_vertices.push_back(vertex);
			}
			App::GetApp()->RegisterResource(L"PSWIRE_PC_CUBE", MeshResource::CreateMeshResource(col_vertices, indices, false));
			vertices.clear();
			indices.clear();
			col_vertices.clear();


			MeshUtill::CreateCylinder(2.0f, 2.0f, 12,vertices,indices);
			for (auto& v : vertices) {
				VertexPositionColor vertex;
				vertex.position = v.position;
				vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				col_vertices.push_back(vertex);
			}
			App::GetApp()->RegisterResource(L"PSWIRE_PC_CYLINDER", MeshResource::CreateMeshResource(col_vertices, indices, false));
			vertices.clear();
			indices.clear();
			col_vertices.clear();



		}
		catch (...) {
			throw;
		}
	}
	SceneBase::~SceneBase() {}

	shared_ptr<Stage> SceneBase::GetActiveStage(bool ExceptionActive) const {
		if (!pImpl->m_ActiveStage) {
			//アクティブなステージが無効なら
			if (ExceptionActive) {
				throw BaseException(
					L"アクティブなステージがありません",
					L"if(!m_ActiveStage.get())",
					L"SceneBase::GetActiveStage()"
				);
			}
			else {
				return nullptr;
			}
		}
		return pImpl->m_ActiveStage;
	}
	void SceneBase::SetActiveStage(const shared_ptr<Stage>& stage) {
		pImpl->m_ActiveStage = stage;
	}

	bsm::Col4 SceneBase::GetClearColor() const {
		return pImpl->m_ClearColor;

	}
	void SceneBase::SetClearColor(const bsm::Col4& col) {
		pImpl->m_ClearColor = col;
	}


	void SceneBase::OnUpdate() {
		if (pImpl->m_ActiveStage) {
			//ステージのアップデート
			pImpl->m_ActiveStage->UpdateStage();
		}
	}
	void SceneBase::OnDraw() {
		if (pImpl->m_ActiveStage) {
			//描画デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->ClearDefaultViews(GetClearColor());
			pImpl->m_ActiveStage->RenderStage();

		}
	}

	void SceneBase::OnDestroy() {
		if (pImpl->m_ActiveStage) {
			pImpl->m_ActiveStage->DestroyStage();
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct StageCellMap::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct StageCellMap::Impl {
		//セルのポインタの配列（2次元）
		vector<vector<CellPiece>> m_CellVec;
		const UINT m_MaxCellSize = 100;
		//セルのXZ方向の数
		UINT m_SizeX;
		UINT m_SizeZ;
		//セル1個のステージ上のサイズ
		float m_PieceSize;
		//デフォルトのコスト
		int m_DefaultCost;
		//ステージ上でこのセルマップを展開するAABB
		AABB m_MapAABB;
		//メッシュ
		shared_ptr<MeshResource> m_LineMesh;
		//メッシュ作成のための頂点の配列
		vector<VertexPositionColor> m_Vertices;
		//セル文字を描画するかどうか
		bool m_IsCellStringActive;
		Impl() :
			m_IsCellStringActive(false)
		{}
		~Impl() {}
		void Init(const bsm::Vec3& MiniPos,
			float PieceSize, UINT PieceCountX, UINT PieceCountZ, int DefaultCost);
		void Create(const shared_ptr<MultiStringSprite>& StringPtr, const shared_ptr<Stage>& StagePtr);

		bsm::Vec3 WorldToSCreen(const bsm::Vec3& v,const bsm::Mat4x4& m, float ViewWidth, float ViewHeight) {
			Vec4 Pos4(v,1.0f);
			Pos4.w = 1.0f;
			//座標変換
			Pos4 *= m;
			//遠近
			Pos4.x /= Pos4.w;
			Pos4.y /= Pos4.w;
			Pos4.z /= Pos4.w;
			//座標単位の修正
			Pos4.x += 1.0f;
			Pos4.y += 1.0f;
			Pos4.y = 2.0f - Pos4.y;
			//ビューポート変換
			Pos4.x *= (ViewWidth * 0.5f);
			Pos4.y *= (ViewHeight * 0.5f);
			return (Vec3)Pos4;
		}

	};
	void StageCellMap::Impl::Init(const bsm::Vec3& MiniPos,
		float PieceSize, UINT PieceCountX, UINT PieceCountZ, int DefaultCost) {
		m_PieceSize = PieceSize;
		m_DefaultCost = DefaultCost;
		m_SizeX = PieceCountX;
		if (m_SizeX <= 0) {
			m_SizeX = 1;
		}
		if (m_SizeX >= m_MaxCellSize) {
			throw BaseException(
				L"セルのX方向が最大値を超えました",
				L"if (m_SizeX >= m_MaxCellSize)",
				L"StageCellMap::Impl::Init()"
			);
		}
		m_SizeZ = PieceCountZ;
		if (m_SizeZ <= 0) {
			m_SizeZ = 1;
		}
		if (m_SizeZ >= m_MaxCellSize) {
			throw BaseException(
				L"セルのZ方向が最大値を超えました",
				L"if (m_SizeZ >= m_MaxCellSize)",
				L"StageCellMap::Impl::Init()"
			);
		}
		m_MapAABB.m_Min = MiniPos;
		m_MapAABB.m_Max.x = m_MapAABB.m_Min.x + m_PieceSize * (float)m_SizeX;
		m_MapAABB.m_Max.y = m_MapAABB.m_Min.y + m_PieceSize;
		m_MapAABB.m_Max.z = m_MapAABB.m_Min.z + m_PieceSize * (float)m_SizeZ;
		bsm::Vec3 PieceVec(m_PieceSize, m_PieceSize, m_PieceSize);
		//配列の初期化
		m_CellVec.resize(m_SizeX);
		for (UINT x = 0; x < m_SizeX; x++) {
			m_CellVec[x].resize(m_SizeZ);
			for (UINT z = 0; z < m_SizeZ; z++) {
				m_CellVec[x][z].m_Index.x = x;
				m_CellVec[x][z].m_Index.z = z;
				m_CellVec[x][z].m_Cost = m_DefaultCost;
				AABB Piece;
				Piece.m_Min.x = m_MapAABB.m_Min.x + (float)x * m_PieceSize;
				Piece.m_Min.y = m_MapAABB.m_Min.y;
				Piece.m_Min.z = m_MapAABB.m_Min.z + (float)z * m_PieceSize;
				Piece.m_Max = Piece.m_Min + PieceVec;
				m_CellVec[x][z].m_PieceRange = Piece;
			}
		}
	}

	void StageCellMap::Impl::Create(const shared_ptr<MultiStringSprite>& StringPtr, const shared_ptr<Stage>& StagePtr) {
		bsm::Vec3 Min = m_MapAABB.m_Min;
		bsm::Vec3 Max = m_MapAABB.m_Max;
		bsm::Col4 Col(1.0f, 1.0f, 1.0f, 1.0f);

		bsm::Vec3 LineFrom(Min);
		bsm::Vec3 LineTo(Min);
		LineTo.x = Max.x;

		m_Vertices.clear();
		for (UINT z = 0; z <= m_SizeZ; z++) {
			m_Vertices.push_back(VertexPositionColor(LineFrom, Col));
			m_Vertices.push_back(VertexPositionColor(LineTo, Col));
			LineFrom.z += m_PieceSize;
			LineTo.z += m_PieceSize;
		}

		LineFrom = Min;
		LineTo = Min;
		LineTo.z = Max.z;
		for (UINT x = 0; x <= m_SizeX; x++) {
			m_Vertices.push_back(VertexPositionColor(LineFrom, Col));
			m_Vertices.push_back(VertexPositionColor(LineTo, Col));
			LineFrom.x += m_PieceSize;
			LineTo.x += m_PieceSize;
		}
		//メッシュの作成（変更できない）
		m_LineMesh = MeshResource::CreateMeshResource(m_Vertices, false);

		//スプライト文字列の初期化
		bsm::Mat4x4 World, View, Proj;

		//ワールド行列の決定
		bsm::Quat Qt;
		Qt.normalize();
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Qt,				//回転角度
			bsm::Vec3(0, 0.01f, 0)				//位置
		);

		auto PtrCamera = StagePtr->GetView()->GetTargetCamera();
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();
		auto viewport = StagePtr->GetView()->GetTargetViewport();
		World *= View;
		World *= Proj;

		StringPtr->ClearTextBlock();
		for (UINT x = 0; x < m_CellVec.size(); x++) {
			for (UINT z = 0; z < m_CellVec[x].size(); z++) {
				bsm::Vec3 Pos = m_CellVec[x][z].m_PieceRange.GetCenter();

				Pos.y = m_CellVec[x][z].m_PieceRange.m_Min.y;
				WorldToSCreen(Pos,World, viewport.Width, viewport.Height);
				Rect2D<float> rect(Pos.x, Pos.y, Pos.x + 50, Pos.y + 20);

				wstring str(L"");
				str += Util::IntToWStr(x);
				str += L",";
				str += Util::IntToWStr(z);

				if (Pos.z < viewport.MinDepth || Pos.z > viewport.MaxDepth) {
					StringPtr->InsertTextBlock(rect, str, true);
				}
				else {
					StringPtr->InsertTextBlock(rect, str, false);
				}
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	ステージのセルマップ（派生クラスを作るかインスタンスを作成する）
	//--------------------------------------------------------------------------------------



	StageCellMap::StageCellMap(const shared_ptr<Stage>& StagePtr, const bsm::Vec3& MiniPos,
		float PieceSize, UINT PieceCountX, UINT PieceCountZ, int DefaultCost):
		GameObject(StagePtr),
		pImpl(new Impl())
	{
		pImpl->Init(MiniPos, PieceSize, PieceCountX, PieceCountZ, DefaultCost);
	}



	StageCellMap::~StageCellMap() {}

	bool StageCellMap::IsCellStringActive() {
		return pImpl->m_IsCellStringActive;

	}
	void StageCellMap::SetCellStringActive(bool b) {
		pImpl->m_IsCellStringActive = b;
	}

	vector<vector<CellPiece>>& StageCellMap::GetCellVec() const {
		return pImpl->m_CellVec;
	}
	//初期化
	void StageCellMap::OnCreate(){
		pImpl->Create(AddComponent<MultiStringSprite>(), GetStage());
		SetDrawActive(false);
	}

	bool StageCellMap::FindCell(const bsm::Vec3& Pos, CellIndex& ret) {
		for (UINT x = 0; x < pImpl->m_CellVec.size(); x++) {
			for (UINT z = 0; z < pImpl->m_CellVec[x].size(); z++) {
				if (pImpl->m_CellVec[x][z].m_PieceRange.PtInAABB(Pos)) {
					ret = pImpl->m_CellVec[x][z].m_Index;
					return true;
				}
			}
		}
		return false;
	}

	void StageCellMap::FindNearCell(const bsm::Vec3& Pos, CellIndex& ret) {
		if (FindCell(Pos, ret)) {
			return;
		}
		float len = 0;
		bool isset = false;
		for (UINT x = 0; x < pImpl->m_CellVec.size(); x++) {
			for (UINT z = 0; z < pImpl->m_CellVec[x].size(); z++) {
				if (!isset) {
					auto cellcenter = pImpl->m_CellVec[x][z].m_PieceRange.GetCenter();
					len = bsm::length(Pos - cellcenter);
					ret = pImpl->m_CellVec[x][z].m_Index;
					isset = true;
				}
				else {
					auto cellcenter = pImpl->m_CellVec[x][z].m_PieceRange.GetCenter();
					auto templen = bsm::length(Pos - cellcenter);
					if (len > templen) {
						len = templen;
						ret = pImpl->m_CellVec[x][z].m_Index;
					}
				}
			}
		}
	}


	bool StageCellMap::FindAABB(const CellIndex& Index, AABB& ret) {
		for (UINT x = 0; x < pImpl->m_CellVec.size(); x++) {
			for (UINT z = 0; z < pImpl->m_CellVec[x].size(); z++) {
				if (pImpl->m_CellVec[x][z].m_Index == Index) {
					ret = pImpl->m_CellVec[x][z].m_PieceRange;
					return true;
				}
			}
		}
		return false;
	}

	void StageCellMap::FindNearAABB(const bsm::Vec3& Pos, AABB& ret) {
		CellIndex retcell;
		FindNearCell(Pos, retcell);
		ret = pImpl->m_CellVec[retcell.x][retcell.z].m_PieceRange;
	}


	void StageCellMap::GetMapAABB(AABB& ret) const {
		ret =  pImpl->m_MapAABB;
	}

	void StageCellMap::RefleshCellMap(const bsm::Vec3& MiniPos,
		float PieceSize, UINT PieceCountX, UINT PieceCountZ, int DefaultCost) {
		pImpl->Init(MiniPos,PieceSize,PieceCountX, PieceCountZ,DefaultCost);
		pImpl->Create(GetComponent<MultiStringSprite>(), GetStage());
	}



	void  StageCellMap::OnUpdate() {
		if (pImpl->m_IsCellStringActive) {
			auto StringPtr = GetComponent<MultiStringSprite>();
			Mat4x4 World, View, Proj;
			World.identity();
			auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
			View = PtrCamera->GetViewMatrix();
			Proj = PtrCamera->GetProjMatrix();
			auto viewport = GetStage()->GetView()->GetTargetViewport();

			World *= View;
			World *= Proj;

			size_t count = 0;
			for (UINT x = 0; x < pImpl->m_CellVec.size(); x++) {
				for (UINT z = 0; z < pImpl->m_CellVec[x].size(); z++) {
					Vec3 Pos = pImpl->m_CellVec[x][z].m_PieceRange.GetCenter();
					Pos.y = pImpl->m_CellVec[x][z].m_PieceRange.m_Min.y;
					Pos = pImpl->WorldToSCreen(Pos, World, viewport.Width, viewport.Height);

					Rect2D<float> rect(Pos.x, Pos.y, Pos.x + 50, Pos.y + 50);

					wstring str(L"");
					str += Util::IntToWStr(x);
					str += L",";
					str += Util::IntToWStr(z);
					str += L"\nCost: ";
					str += Util::IntToWStr(pImpl->m_CellVec[x][z].m_Cost);

					if (Pos.z < viewport.MinDepth || Pos.z > viewport.MaxDepth) {
						StringPtr->UpdateTextBlock(count, rect, str, true);
					}
					else {
						StringPtr->UpdateTextBlock(count, rect, str, false);
					}
					count++;
				}
			}
		}
	}


	void StageCellMap::OnDraw() {
		//Dx12が対応できるまで記述を消す
#if (BASECROSS_DXVERSION == 11)

		if (pImpl->m_IsCellStringActive) {
			GameObject::OnDraw();
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//行列の定義
		bsm::Mat4x4 World, View, Proj;
		//ワールド行列の決定
		bsm::Quat Qt;
		Qt.normalize();
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Qt,				//回転角度
			bsm::Vec3(0, 0.01f, 0)				//位置
		);
		//転置する
		World = bsm::transpose(World);
		//カメラを得る
		auto CameraPtr = OnGetDrawCamera();
		//ビューと射影行列を得る
		View = CameraPtr->GetViewMatrix();
		//転置する
		View = bsm::transpose(View);
		//転置する
		Proj = CameraPtr->GetProjMatrix();
		Proj = bsm::transpose(Proj);

		//コンスタントバッファの準備
		SimpleConstants sb;
		sb.World = World;
		sb.View = View;
		sb.Projection = Proj;
		//エミッシブ加算は行わない。
		sb.Emissive = bsm::Col4(0, 0, 0, 0);
		sb.Diffuse = bsm::Col4(1, 1, 1, 1);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, pImpl->m_LineMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);

		//描画方法（ライン）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCStatic::GetPtr()->GetInputLayout());
		//ブレンドステート
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//ラスタライザステート
		pD3D11DeviceContext->RSSetState(RenderState->GetCullNone());

		//描画
		pD3D11DeviceContext->Draw((UINT)pImpl->m_Vertices.size(), 0);
		//後始末
		Dev->InitializeStates();
#endif

	}



	//--------------------------------------------------------------------------------------
	//	struct GameObjecttCSVBuilder::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct GameObjecttCSVBuilder::Impl {
		map<wstring, shared_ptr<GameObjectCreatorBaseCSV> > m_CreatorMap;
		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	ゲームオブジェクトビルダーCSV
	//--------------------------------------------------------------------------------------

	GameObjecttCSVBuilder::GameObjecttCSVBuilder() :
		pImpl(new Impl())
	{

	}
	GameObjecttCSVBuilder::~GameObjecttCSVBuilder() {}

	map<wstring, shared_ptr<GameObjectCreatorBaseCSV>>& GameObjecttCSVBuilder::GetCreatorMap() const {
		return pImpl->m_CreatorMap;
	}

	shared_ptr<GameObject> GameObjecttCSVBuilder::CreateFromCSV(const wstring& ClsName, const shared_ptr<Stage>& StagePtr, const wstring& Line) {
		auto it = pImpl->m_CreatorMap.find(ClsName);
		if (it == pImpl->m_CreatorMap.end()) {
			return nullptr;
		}
		else {
			auto ptr = (*it).second->Create(StagePtr, Line);
			return ptr;
		}
	}

	void GameObjecttCSVBuilder::Build(const shared_ptr<Stage>& StagePtr, const wstring& CSVFileName) {
		try {
			//CSVファイル
			CsvFile GameStageCsv(CSVFileName);
			GameStageCsv.ReadCsv();
			//CSVの全体の配列
			//CSVからすべての行を抜き出す
			auto& LineVec = GameStageCsv.GetCsvVec();
			for (auto& v : LineVec) {
				//トークン（カラム）の配列
				vector<wstring> Tokens;
				Util::WStrToTokenVector(Tokens,v, L',');
				CreateFromCSV(Tokens[0], StagePtr, v);
			}
		}
		catch (...) {
			throw;
		}

	}

	//--------------------------------------------------------------------------------------
	//	struct GameObjecttXMLBuilder::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct GameObjecttXMLBuilder::Impl {
		map<wstring, shared_ptr<GameObjectCreatorBaseXML> > m_CreatorMap;
		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	ゲームオブジェクトビルダーXML
	//--------------------------------------------------------------------------------------

	GameObjecttXMLBuilder::GameObjecttXMLBuilder() :
		pImpl(new Impl())
	{

	}
	GameObjecttXMLBuilder::~GameObjecttXMLBuilder() {}

	map<wstring, shared_ptr<GameObjectCreatorBaseXML>>& GameObjecttXMLBuilder::GetCreatorMap() const {
		return pImpl->m_CreatorMap;
	}

	shared_ptr<GameObject>  GameObjecttXMLBuilder::CreateFromXML(const wstring& ClsName, const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode) {
		auto it = pImpl->m_CreatorMap.find(ClsName);
		if (it == pImpl->m_CreatorMap.end()) {
			return nullptr;
		}
		else {
			auto ptr = (*it).second->Create(StagePtr, pNode);
			return ptr;
		}
	}

	void GameObjecttXMLBuilder::Build(const shared_ptr<Stage>& StagePtr, const wstring& XMLFileName, const wstring& GameObjectsPath) {
		try {
			//XMLリーダー
			XmlDocReader Reader(XMLFileName);
			auto Nodes = Reader.GetSelectNodes(GameObjectsPath.c_str());
			long CountNode = XmlDocReader::GetLength(Nodes);
			for (long i = 0; i < CountNode; i++) {
				auto Node = XmlDocReader::GetItem(Nodes, i);
				auto TypeStr = XmlDocReader::GetAttribute(Node, L"Type");
				CreateFromXML(TypeStr, StagePtr, Node);
			}
		}
		catch (...) {
			throw;
		}
	}








}
//end basecross
