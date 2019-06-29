/*!
@file StringComponents.h
@brief 文字列描画コンポーネント
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi,shike.y
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	用途: StringSpriteコンポーネント
	//	文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	class StringSprite : public Component{
	public:
		explicit StringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~StringSprite();
		//アクセサ
		enum TextAlignment{
			m_Left,		//左合わせ
			m_Center,	//中央合わせ
			m_Right		//右合わせ
		};
		//アクセサ
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

		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	複数ブロックの文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	class MultiStringSprite : public Component {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit MultiStringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MultiStringSprite();
		//アクセサ
		//ブロック共通
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントを設定する
		@param[in]	FontName フォント名
		@param[in]	FontSize フォントの大きさ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFont(const wstring& FontName, float FontSize);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントの色を取得する
		@return	フォントの色
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Col4& GetFontColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント色を設定する
		@param[in]	Col フォント色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFontColor(const bsm::Col4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント名を取得する
		@return	フォント名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetFontName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントサイズを取得する
		@return	フォントサイズ
		*/
		//--------------------------------------------------------------------------------------
		float GetFontSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景色を取得する
		@return	背景色
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Col4& GetBackColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景色を設定する
		@param[in]	Col 背景色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBackColor(const bsm::Col4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストマージンを取得する
		@return	テキストマージン
		*/
		//--------------------------------------------------------------------------------------
		Point2D<float> GetBackTextMargin() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストマージンを設定する
		@param[in]	p テキストマージン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBackTextMargin(Point2D<float> p);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストフォーマットを取得する
		@return	テキストフォーマット
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストアラインメントを取得する
		@return	テキストアラインメント
		*/
		//--------------------------------------------------------------------------------------
		StringSprite::TextAlignment GetTextAlignment() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストアラインメントを設定する
		@param[in]	Alignment テキストアラインメント
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextAlignment(StringSprite::TextAlignment Alignment);
		//ブロック単位
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを取得する
		@param[in]	Index ブロックのインデックス
		@return	テキスト
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetText(size_t Index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを設定する
		@param[in]	Index ブロックのインデックス
		@param[in]	str テキスト
		@param[in]	Clip クリッピングするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetText(size_t Index, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを追加する
		@param[in]	Index ブロックのインデックス
		@param[in]	str 追加するテキスト
		@param[in]	Clip クリッピングするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddText(size_t Index, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックを追加する
		@param[in]	Block ブロック
		@param[in]	str テキスト
		@param[in]	Clip クリッピングするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void InsertTextBlock(const Rect2D<float>& Block, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックを更新する
		@param[in]	Index ブロックのインデックス
		@param[in]	Block ブロック
		@param[in]	str テキスト
		@param[in]	Clip クリッピングするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateTextBlock(size_t Index, const Rect2D<float>& Block, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックをクリアする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ClearTextBlock();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストレイアウトを取得する
		@param[in]	Index ブロックのインデックス
		@return	テキストレイアウト
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDWriteTextLayout>& GetTextLayout(size_t Index)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストメトリクスを取得する
		@param[in]	Index ブロックのインデックス
		@return	テキストメトリクス
		*/
		//--------------------------------------------------------------------------------------
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics(size_t Index) const;
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理
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
