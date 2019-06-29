/*!
@file StringComponents.h
@brief ������`��R���|�[�l���g
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi,shike.y
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	�p�r: StringSprite�R���|�[�l���g
	//	������\���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class StringSprite : public Component{
	public:
		explicit StringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~StringSprite();
		//�A�N�Z�T
		enum TextAlignment{
			m_Left,		//�����킹
			m_Center,	//�������킹
			m_Right		//�E���킹
		};
		//�A�N�Z�T
		void SetFont(const wstring& FontName, float FontSize);

		StringSprite::TextAlignment GetTextAlignment() const;
		void SetTextAlignment(StringSprite::TextAlignment Alignment);
		const wstring& GetText() const;
		void SetText(const wstring& str);
		void AddText(const wstring& str);


		const bsm::Col4& GetFontColor() const;
		void SetFontColor(const bsm::Col4& Col);
		const wstring& GetFontName() const;
		float GetFontSize() const;

		const bsm::Col4& GetBackColor() const;
		void SetBackColor(const bsm::Col4& Col);

		Point2D<float> GetBackTextMargin() const;
		void SetBackTextMargin(Point2D<float> p);

		float GetTextBlockWidth() const;
		void SetTextBlockWidth(float f);
		float GetTextBlockHeight() const;
		void SetTextBlockHeight(float f);

		ComPtr<IDWriteTextLayout>& GetTextLayout()const;
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics() const;

		const Point2D<float>& GetStartPosition() const;
		void SetStartPosition(const Point2D<float>& pos);

		Rect2D<float> GetTextRect() const;
		void SetTextRect(const Rect2D<float>& rect);

		//����
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	�����u���b�N�̕�����\���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class MultiStringSprite : public Component {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit MultiStringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MultiStringSprite();
		//�A�N�Z�T
		//�u���b�N����
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H���g��ݒ肷��
		@param[in]	FontName �t�H���g��
		@param[in]	FontSize �t�H���g�̑傫��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetFont(const wstring& FontName, float FontSize);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H���g�̐F���擾����
		@return	�t�H���g�̐F
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Col4& GetFontColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H���g�F��ݒ肷��
		@param[in]	Col �t�H���g�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetFontColor(const bsm::Col4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H���g�����擾����
		@return	�t�H���g��
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetFontName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H���g�T�C�Y���擾����
		@return	�t�H���g�T�C�Y
		*/
		//--------------------------------------------------------------------------------------
		float GetFontSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w�i�F���擾����
		@return	�w�i�F
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Col4& GetBackColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w�i�F��ݒ肷��
		@param[in]	Col �w�i�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBackColor(const bsm::Col4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g�}�[�W�����擾����
		@return	�e�L�X�g�}�[�W��
		*/
		//--------------------------------------------------------------------------------------
		Point2D<float> GetBackTextMargin() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g�}�[�W����ݒ肷��
		@param[in]	p �e�L�X�g�}�[�W��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBackTextMargin(Point2D<float> p);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g�t�H�[�}�b�g���擾����
		@return	�e�L�X�g�t�H�[�}�b�g
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g�A���C�������g���擾����
		@return	�e�L�X�g�A���C�������g
		*/
		//--------------------------------------------------------------------------------------
		StringSprite::TextAlignment GetTextAlignment() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g�A���C�������g��ݒ肷��
		@param[in]	Alignment �e�L�X�g�A���C�������g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTextAlignment(StringSprite::TextAlignment Alignment);
		//�u���b�N�P��
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g���擾����
		@param[in]	Index �u���b�N�̃C���f�b�N�X
		@return	�e�L�X�g
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetText(size_t Index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g��ݒ肷��
		@param[in]	Index �u���b�N�̃C���f�b�N�X
		@param[in]	str �e�L�X�g
		@param[in]	Clip �N���b�s���O���邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetText(size_t Index, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g��ǉ�����
		@param[in]	Index �u���b�N�̃C���f�b�N�X
		@param[in]	str �ǉ�����e�L�X�g
		@param[in]	Clip �N���b�s���O���邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddText(size_t Index, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g�u���b�N��ǉ�����
		@param[in]	Block �u���b�N
		@param[in]	str �e�L�X�g
		@param[in]	Clip �N���b�s���O���邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void InsertTextBlock(const Rect2D<float>& Block, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g�u���b�N���X�V����
		@param[in]	Index �u���b�N�̃C���f�b�N�X
		@param[in]	Block �u���b�N
		@param[in]	str �e�L�X�g
		@param[in]	Clip �N���b�s���O���邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateTextBlock(size_t Index, const Rect2D<float>& Block, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g�u���b�N���N���A����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ClearTextBlock();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g���C�A�E�g���擾����
		@param[in]	Index �u���b�N�̃C���f�b�N�X
		@return	�e�L�X�g���C�A�E�g
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDWriteTextLayout>& GetTextLayout(size_t Index)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�L�X�g���g���N�X���擾����
		@param[in]	Index �u���b�N�̃C���f�b�N�X
		@return	�e�L�X�g���g���N�X
		*/
		//--------------------------------------------------------------------------------------
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics(size_t Index) const;
		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�揈��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




}

//end basecross
