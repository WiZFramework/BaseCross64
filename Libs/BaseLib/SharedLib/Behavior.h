/*!
@file Behavior.h
@brief �s���N���X
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class GameObject;
	//--------------------------------------------------------------------------------------
	///	�s���N���X�̐e�N���X
	//--------------------------------------------------------------------------------------
	class Behavior : public ObjectInterface {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�R���X�g���N�^
		@param[in]	GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit Behavior(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Behavior();
	public:
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Q�[���I�u�W�F�N�g�̎擾
		@return	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetGameObject() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������X�e�[�W�̎擾
		@return	�Q�[���I�u�W�F�N�g����������X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������(��֐��A�K�v�Ȃ瑽�d��`����)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() {}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	�s�����[�e�B���e�B�N���X
	//--------------------------------------------------------------------------------------
	class UtilBehavior : public Behavior {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		UtilBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~UtilBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �i�s�����������悤�ɂ���
		@param[in]	LerpFact	��ԌW���i0.0����1.0�̊ԁj
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void RotToHead(float LerpFact);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �i�s�����������悤�ɂ���(���x�w�����)
		@param[in]	LerpFact	��ԌW���i0.0����1.0�̊ԁj
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void RotToHead(const bsm::Vec3& Velocity, float LerpFact);

	};

	//--------------------------------------------------------------------------------------
	///	�s���N���X�̐e�N���X(�e���v���[�g��)
	//--------------------------------------------------------------------------------------
	template <typename T>
	class ObjBehavior : public ObjectInterface {
		weak_ptr<T> m_GameObject;
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�R���X�g���N�^
		@param[in]	GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit ObjBehavior(const shared_ptr<T>& GameObjectPtr):
			m_GameObject(GameObjectPtr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ObjBehavior() {}
	public:
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Q�[���I�u�W�F�N�g�̎擾
		@return	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<T> GetGameObject() const {
			auto shptr = m_GameObject.lock();
			if (!shptr) {
				throw BaseException(
					L"GameObject�͗L���ł͂���܂���",
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
		@brief	��������X�e�[�W�̎擾
		@return	�Q�[���I�u�W�F�N�g����������X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage() const {
			return GetGameObject()->GetStage();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������(��֐��A�K�v�Ȃ瑽�d��`����)
		@return	�Ȃ�
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
	///	Easing�s���N���X�e���v���[�g��
	//--------------------------------------------------------------------------------------
	template<typename T>
	class Easing {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		Easing(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		~Easing() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief�@Linear���v�Z����
		@param[in]	Start	�J�n�l�iT�^�j
		@param[in]	End		�I���l�iT�^�j
		@param[in]	TgtTime	�v�Z����^�C��
		@param[in]	AllTime	�g�[�^���^�C��
		@return	�v�Z���ʃx�N�g��
		*/
		//--------------------------------------------------------------------------------------
		T Linear(const T& Start, T& End, float TgtTime, float AllTime) {
			auto SpanVec = End - Start;
			return EasingBase<T>::Linear(TgtTime, Start, SpanVec, AllTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief�@EaseIn���v�Z����
		@param[in]	type	EasingType
		@param[in]	Start	�J�n�l�iT�^�j
		@param[in]	End		�I���l�iT�^�j
		@param[in]	TgtTime	�v�Z����^�C��
		@param[in]	AllTime	�g�[�^���^�C��
		@return	�v�Z���ʃx�N�g��
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
			//�G���[�̏ꍇ�̓X�^�[�g��߂�
			return Start;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief�@EaseOut���v�Z����
		@param[in]	type	EasingType
		@param[in]	Start	�J�n�l�iT�^�j
		@param[in]	End		�I���l�iT�^�j
		@param[in]	TgtTime	�v�Z����^�C��
		@param[in]	AllTime	�g�[�^���^�C��
		@return	�v�Z���ʃx�N�g��
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
			//�G���[�̏ꍇ�̓X�^�[�g��߂�
			return Start;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief�@EaseInOut���v�Z����
		@param[in]	Start	�J�n�l�iT�^�j
		@param[in]	End		�I���l�iT�^�j
		@param[in]	End		�I���x�N�g��
		@param[in]	TgtTime	�v�Z����^�C��
		@param[in]	AllTime	�g�[�^���^�C��
		@return	�v�Z���ʃx�N�g��
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
			//�G���[�̏ꍇ�̓X�^�[�g��߂�
			return Start;
		}
	};



}
//end basecross
