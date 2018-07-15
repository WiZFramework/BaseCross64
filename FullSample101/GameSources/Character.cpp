/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
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
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);
		//OBB衝突j判定を付ける
		auto PtrColl = AddComponent<CollisionObb>();
		PtrColl->SetFixed(true);

		//タグをつける
		AddTag(L"FixedBox");

		//影をつける（シャドウマップを描画する）
	//	auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
	//	ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");



		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");

		PtrDraw->SetOwnShadowActive(true);


	}




}
//end basecross
