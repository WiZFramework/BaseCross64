/*!
@file Behavior.h
@brief 行動クラス
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class GameObject;
	//--------------------------------------------------------------------------------------
	///	行動クラスの親クラス
	//--------------------------------------------------------------------------------------
	class Behavior : public ObjectInterface {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit Behavior(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Behavior();
	public:
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトの取得
		@return	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetGameObject() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	所属するステージの取得
		@return	ゲームオブジェクトが所属するステージ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化(空関数、必要なら多重定義する)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() {}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	行動ユーティリティクラス
	//--------------------------------------------------------------------------------------
	class UtilBehavior : public Behavior {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		UtilBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~UtilBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 進行方向を向くようにする
		@param[in]	LerpFact	補間係数（0.0から1.0の間）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RotToHead(float LerpFact);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 進行方向を向くようにする(速度指定方式)
		@param[in]	LerpFact	補間係数（0.0から1.0の間）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RotToHead(const bsm::Vec3& Velocity, float LerpFact);

	};

	//--------------------------------------------------------------------------------------
	///	行動クラスの親クラス(テンプレート版)
	//--------------------------------------------------------------------------------------
	template <typename T>
	class ObjBehavior : public ObjectInterface {
		weak_ptr<T> m_GameObject;
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit ObjBehavior(const shared_ptr<T>& GameObjectPtr):
			m_GameObject(GameObjectPtr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ObjBehavior() {}
	public:
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトの取得
		@return	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<T> GetGameObject() const {
			auto shptr = m_GameObject.lock();
			if (!shptr) {
				throw BaseException(
					L"GameObjectは有効ではありません",
					L"if (!shptr)",
					L"ObjBehavior::GetGameObject()"
				);
			}
			else {
				return shptr;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	所属するステージの取得
		@return	ゲームオブジェクトが所属するステージ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage() const {
			return GetGameObject()->GetStage();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化(空関数、必要なら多重定義する)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() {}
	};

	enum class EasingType {
		Quadratic,
		Cubic,
		Quartic,
		Quintic,
		Sinusoidal,
		Exponential,
		Circular,
	};


	//--------------------------------------------------------------------------------------
	///	Easing行動クラステンプレート版
	//--------------------------------------------------------------------------------------
	template<typename T>
	class Easing {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		Easing(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		~Easing() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief　Linearを計算する
		@param[in]	Start	開始値（T型）
		@param[in]	End		終了値（T型）
		@param[in]	TgtTime	計算するタイム
		@param[in]	AllTime	トータルタイム
		@return	計算結果ベクトル
		*/
		//--------------------------------------------------------------------------------------
		T Linear(const T& Start, T& End, float TgtTime, float AllTime) {
			auto SpanVec = End - Start;
			return EasingBase<T>::Linear(TgtTime, Start, SpanVec, AllTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief　EaseInを計算する
		@param[in]	type	EasingType
		@param[in]	Start	開始値（T型）
		@param[in]	End		終了値（T型）
		@param[in]	TgtTime	計算するタイム
		@param[in]	AllTime	トータルタイム
		@return	計算結果ベクトル
		*/
		//--------------------------------------------------------------------------------------
		T EaseIn(EasingType type, const T& Start, const T& End, float TgtTime, float AllTime) {
			auto SpanVec = End - Start;
			switch (type) {
			case EasingType::Quadratic:
			{
				EaseQuad<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Cubic:
			{
				EaseCubic<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quartic:
			{
				EaseQuart<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quintic:
			{
				EaseQuint<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Sinusoidal:
			{
				EaseSin<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Exponential:
			{
				EaseExpo<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Circular:
			{
				EaseCirc<T> es;
				return es.EaseIn(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			}
			//エラーの場合はスタートを戻す
			return Start;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief　EaseOutを計算する
		@param[in]	type	EasingType
		@param[in]	Start	開始値（T型）
		@param[in]	End		終了値（T型）
		@param[in]	TgtTime	計算するタイム
		@param[in]	AllTime	トータルタイム
		@return	計算結果ベクトル
		*/
		//--------------------------------------------------------------------------------------
		T EaseOut(EasingType type, const T& Start, const T& End, float TgtTime, float AllTime) {
			auto SpanVec = End - Start;
			switch (type) {
			case EasingType::Quadratic:
			{
				EaseQuad<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Cubic:
			{
				EaseCubic<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quartic:
			{
				EaseQuart<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quintic:
			{
				EaseQuint<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Sinusoidal:
			{
				EaseSin<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Exponential:
			{
				EaseExpo<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Circular:
			{
				EaseCirc<T> es;
				return es.EaseOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			}
			//エラーの場合はスタートを戻す
			return Start;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief　EaseInOutを計算する
		@param[in]	Start	開始値（T型）
		@param[in]	End		終了値（T型）
		@param[in]	End		終了ベクトル
		@param[in]	TgtTime	計算するタイム
		@param[in]	AllTime	トータルタイム
		@return	計算結果ベクトル
		*/
		//--------------------------------------------------------------------------------------
		T EaseInOut(EasingType type, T& Start, const T& End, float TgtTime, float AllTime) {
			auto SpanVec = End - Start;
			switch (type) {
			case EasingType::Quadratic:
			{
				EaseQuad<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Cubic:
			{
				EaseCubic<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quartic:
			{
				EaseQuart<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Quintic:
			{
				EaseQuint<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			case EasingType::Sinusoidal:
			{
				EaseSin<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Exponential:
			{
				EaseExpo<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);

			}
			break;
			case EasingType::Circular:
			{
				EaseCirc<T> es;
				return es.EaseInOut(TgtTime, Start, SpanVec, AllTime);
			}
			break;
			}
			//エラーの場合はスタートを戻す
			return Start;
		}
	};



}
//end basecross
