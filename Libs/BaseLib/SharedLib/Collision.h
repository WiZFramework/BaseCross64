/*!
@file Collision.h
@brief 衝突判定コンポーネント
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {
	class Collision;
	class CollisionSphere;
	class CollisionCapsule;
	class CollisionObb;
	class CollisionManager;

	//--------------------------------------------------------------------------------------
	///	 スケールが一つだった場合の計算に使用するスケール
	//--------------------------------------------------------------------------------------
	enum class CalcScaling {
		XScale,
		YScale,
		ZScale,
	};

	//--------------------------------------------------------------------------------------
	///	 衝突後の処理
	//--------------------------------------------------------------------------------------
	enum class AfterCollision {
		Auto,
		None,
	};


	//--------------------------------------------------------------------------------------
	///	 衝突判定コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	class Collision : public Component {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit Collision(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Collision();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	固定衝突オブジェクトかどうかを得る。
		@return	固定衝突オブジェクトならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetFixed() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	固定衝突オブジェクトかどうかを得る。
		@return	固定衝突オブジェクトならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsFixed() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	固定衝突オブジェクトかどうかを設定する
		@param[in]	b	固定衝突オブジェクトならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetFixed(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突後の処理を得る。
		@return	AfterCollision
		*/
		//--------------------------------------------------------------------------------------
		AfterCollision GetAfterCollision() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突後の処理を設定する
		@param[in]	a	衝突後の処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAfterCollision(AfterCollision a);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 判定から除外するグループの取得。
		@return	 判定から除外するグループ（指定されてなければnullptr）
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObjectGroup> GetExcludeCollisionGroup() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 判定から除外するグループの設定。
		@return	 なし
		*/
		//--------------------------------------------------------------------------------------
		void SetExcludeCollisionGroup(const shared_ptr<GameObjectGroup>& Group);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	判定から除外するタグが存在するかどうかを得る
		@param[in]	tagstr	検証するタグ
		@return	存在すればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool FindExcludeCollisionTag(const wstring& tagstr) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	判定から除外するタグを追加する
		@param[in]	tagstr	追加するタグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  AddExcludeCollisionTag(const wstring& tagstr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	判定から除外するタグが存在したら削除する（存在しない場合は何もしない）
		@param[in]	tagstr	削除するタグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  RemoveExcludeCollisionTag(const wstring& tagstr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 判定から除外するオブジェクトかどうかを取得。
		@param[in]	Obj	調査するゲームオブジェクト
		@return	 判定から除外する設定ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsExcludeCollisionObject(const shared_ptr<GameObject>& Obj) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンコール。ダブルデスパッチ関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionCall(const shared_ptr<Collision>& Src) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionObb>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前と現在の連結させたSPHEREを得る
		@return	連結させたSPHERE
		*/
		//--------------------------------------------------------------------------------------
		virtual SPHERE GetEnclosingSphere()const = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 自分と相手の連結したSPHEREの衝突判定（前処理用）
		@tparam	T	相手のコリジョンの型
		@param[in]	DestColl	相手のコリジョン
		@return	ヒットしてればtrue
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		bool CillisionWrappedSphere(const shared_ptr<T>& DestColl) {
			SPHERE Src = GetEnclosingSphere();
			SPHERE Dest = DestColl->GetEnclosingSphere();
			return HitTest::SPHERE_SPHERE(Src, Dest);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の包み込むAABBを返す。仮想関数
		@return	包み込むAABB(1つ前のターン時の内容は見ない)
		*/
		//--------------------------------------------------------------------------------------
//		virtual AABB GetWrappingAABB()const = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心位置を返す。仮想関数
		@return	中心位置
		*/
		//--------------------------------------------------------------------------------------
		virtual bsm::Vec3 GetCenterPosition()const = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突判定マネージャを返す
		@return	衝突判定マネージャ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<CollisionManager> GetCollisionManager() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スライドさせる
		@param[in]	Vec	現在の方向
		@param[in]	Norm	衝突法線
		@return	スライドする方向
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3 Slide(const bsm::Vec3& Vec, const bsm::Vec3& Norm) {
			//thisと法線から直行線の長さ（内積で求める）
			float Len = bsm::dot(Vec, Norm);
			//その長さに伸ばす
			bsm::Vec3 Contact = Norm * Len;
			//スライドする方向は現在のベクトルから引き算
			return (Vec - Contact);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	球体コリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionSphere>& Other, bsm::Vec3& SrcHitNormal) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	カプセルコリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionCapsule>& Other, bsm::Vec3& SrcHitNormal) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	Obbコリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionObb>& Other, bsm::Vec3& SrcHitNormal) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。デフォルトは何も行わない
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionSphere : public Collision ;
	//	用途: 球衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	class CollisionSphere : public Collision {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit CollisionSphere(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CollisionSphere();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を得る
		@return	作成時の直径
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedDiameter() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を設定する
		@param[in]	f	作成時の直径
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedDiameter(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の半径を得る。内部では直径のみ持っているので、直径の0.5を返す
		@return	作成時の半径
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedRadius() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を半径をもとに設定する。内部では直径のみ持っているので、2倍したものを直径に設定する
		@param[in]	f	作成時の半径
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedRadius(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	計算時、XYZどのスケールを使用するかを得る(x: 0,y: 1,z: 2)。デフォルトx
		@return	計算時、XYZどのスケールを使用するか
		*/
		//--------------------------------------------------------------------------------------
		CalcScaling GetCalcScaling() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	計算時、XYZどのスケールを使用するかを設定する(x: 0,y: 1,z: 2)。デフォルトx
		@param[in]	s	計算時、XYZどのスケールを使用するか
		@return	
		*/
		//--------------------------------------------------------------------------------------
		void SetCalcScaling(CalcScaling s);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のSPHER境界ボリュームを得る
		@return	現在のSPHER境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetSphere() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1ターン前のSPHER境界ボリュームを得る
		@return	1ターン前のSPHER境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetBeforeSphere() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンコール。ダブルデスパッチ関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionCall(const shared_ptr<Collision>& Src)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionSphere>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionCapsule>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionObb>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前と現在の連結させたSPHEREを得る
		@return	連結させたSPHERE
		*/
		//--------------------------------------------------------------------------------------
		virtual SPHERE GetEnclosingSphere()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の包み込むAABBを返す。仮想関数
		@return	包み込むAABB(1つ前のターン時の内容は見ない)
		*/
		//--------------------------------------------------------------------------------------
//		virtual AABB GetWrappingAABB()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心位置を返す。仮想関数
		@return	中心位置
		*/
		//--------------------------------------------------------------------------------------
		virtual bsm::Vec3 GetCenterPosition()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	球体コリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionSphere>& Other, bsm::Vec3& SrcHitNormal) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	カプセルコリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionCapsule>& Other, bsm::Vec3& SrcHitNormal) override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	Obbコリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionObb>& Other, bsm::Vec3& SrcHitNormal)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。DrawActiveがtrue時に呼ばれる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionCapsule : public Collision ;
	//	用途: カプセル衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	class CollisionCapsule : public Collision {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit CollisionCapsule(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CollisionCapsule();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を得る
		@return	作成時の直径
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedDiameter() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を設定する
		@param[in]	f	作成時の直径
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedDiameter(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の半径を得る。内部では直径のみ持っているので、直径の0.5を返す
		@return	作成時の半径
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedRadius() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を半径をもとに設定する。内部では直径のみ持っているので、2倍したものを直径に設定する
		@param[in]	f	作成時の半径
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedRadius(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の高さを得る
		@return	作成時の高さ
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedHeight() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の高さを得る
		@param[in]	f	作成時の高さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedHeight(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のCAPSULE境界ボリュームを得る
		@return	現在のCAPSULE境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE GetCapsule() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1ターン前のCAPSULE境界ボリュームを得る
		@return	1ターン前のCAPSULE境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE GetBeforeCapsule() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンコール。ダブルデスパッチ関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionCall(const shared_ptr<Collision>& Src)override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionSphere>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionCapsule>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionObb>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前と現在の連結させたSPHEREを得る
		@return	連結させたSPHERE
		*/
		//--------------------------------------------------------------------------------------
		virtual SPHERE GetEnclosingSphere()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の包み込むAABBを返す。仮想関数
		@return	包み込むAABB(1つ前のターン時の内容は見ない)
		*/
		//--------------------------------------------------------------------------------------
//		virtual AABB GetWrappingAABB()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心位置を返す。仮想関数
		@return	中心位置
		*/
		//--------------------------------------------------------------------------------------
		virtual bsm::Vec3 GetCenterPosition()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	球体コリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionSphere>& Other, bsm::Vec3& SrcHitNormal) override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	カプセルコリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionCapsule>& Other, bsm::Vec3& SrcHitNormal) override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	Obbコリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionObb>& Other, bsm::Vec3& SrcHitNormal)override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。DrawActiveがtrue時に呼ばれる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};





	//--------------------------------------------------------------------------------------
	//	class CollisionObb : public Collision ;
	//	用途: Obb衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	class CollisionObb : public Collision {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit CollisionObb(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CollisionObb();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の1辺の長さを得る
		@return	作成時の1辺の長さ
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の1辺の長さを設定する
		@param[in]	f	作成時の1辺の長さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedSize(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のOBB境界ボリュームを得る
		@return	現在のOBB境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		OBB GetObb() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1ターン前のOBB境界ボリュームを得る
		@return	1ターン前のOBB境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		OBB GetBeforeObb() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンコール。ダブルデスパッチ関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionCall(const shared_ptr<Collision>& Src)override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionSphere>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionCapsule>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionObb>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
//		virtual void GetHitNormal(const shared_ptr<CollisionSphere>& DestColl, bsm::Vec3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
//		virtual void GetHitNormal(const shared_ptr<CollisionCapsule>& DestColl, bsm::Vec3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
//		virtual void GetHitNormal(const shared_ptr<CollisionObb>& DestColl, bsm::Vec3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前と現在の連結させたSPHEREを得る
		@return	連結させたSPHERE
		*/
		//--------------------------------------------------------------------------------------
		virtual SPHERE GetEnclosingSphere()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 線分と自分の衝突チェック（判定するのみ）
		@param[in]	Pos1	線分開始
		@param[in]	Pos2	線分終了
		@return	ヒットしてればtrue
		*/
		//--------------------------------------------------------------------------------------
//		virtual bool HitTestWithSegment(const bsm::Vec3& Pos1, const bsm::Vec3& Pos2)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の包み込むAABBを返す。仮想関数
		@return	包み込むAABB(1つ前のターン時の内容は見ない)
		*/
		//--------------------------------------------------------------------------------------
//		virtual AABB GetWrappingAABB()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心位置を返す。仮想関数
		@return	中心位置
		*/
		//--------------------------------------------------------------------------------------
		virtual bsm::Vec3 GetCenterPosition()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	球体コリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionSphere>& Other, bsm::Vec3& SrcHitNormal) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	カプセルコリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionCapsule>& Other, bsm::Vec3& SrcHitNormal) override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手からエスケープする
		@param[in]	Other	Obbコリジョン
		@param[in]	SrcHitNormal	衝突法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EscapeCollision(const shared_ptr<CollisionObb>& Other, bsm::Vec3& SrcHitNormal)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。DrawActiveがtrue時に呼ばれる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




}
//end basecross
