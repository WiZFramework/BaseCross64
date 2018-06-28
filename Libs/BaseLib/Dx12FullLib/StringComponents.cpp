/*!
@file StringComponents.cpp
@brief 文字列描画コンポーネント実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct StringSprite::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct StringSprite::Impl{
		// テキスト レンダリングに関連するリソース。
		wstring m_text;
		float m_FoneSize;
		float m_TextBlockWidth;
		float m_TextBlockHeight;
		wstring m_FontName;
		bsm::Col4 m_FontColor;
		Point2D<float> m_StartPosition;
		StringSprite::TextAlignment m_TextAlignment;

		bsm::Col4 m_BackColor;		//背景色
		Point2D<float> m_BackTextMargin;	//背景色を塗りつぶすテキストのマージン(左右と上下)

		DWRITE_TEXT_METRICS m_textMetrics;
		ComPtr<ID2D1SolidColorBrush>    m_Brush;
		ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		ComPtr<IDWriteTextLayout>       m_textLayout;
		ComPtr<IDWriteTextFormat>		m_textFormat;

		ComPtr<ID2D1SolidColorBrush>    m_BackBrush;


		Impl():
			m_text(),
			m_FoneSize(16.0f),
			m_TextBlockWidth(128.0f),
			m_TextBlockHeight(32.0f),
			m_FontName(L"ＭＳゴシック"),
			m_FontColor(bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_StartPosition{16.0f,16.0f},
			m_TextAlignment(StringSprite::TextAlignment::m_Left),
			m_BackColor(bsm::Col4(0.0f, 0.0f, 0.0f, 0.0f)),
			m_BackTextMargin(4.0f,0.0f)
			{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	用途: StringSpriteコンポーネント
	//	文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	StringSprite::StringSprite(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl()){
		try{

		}
		catch (...){
			throw;
		}
	}
	StringSprite::~StringSprite(){}
	//アクセサ
	void StringSprite::SetFont(const wstring& FontName, float FontSize){

	}
	StringSprite::TextAlignment StringSprite::GetTextAlignment() const{
		return pImpl->m_TextAlignment;
	}
	void StringSprite::SetTextAlignment(StringSprite::TextAlignment Alignment){
		pImpl->m_TextAlignment = Alignment;
	}


	const wstring& StringSprite::GetText() const{
		return pImpl->m_text;
	}
	void StringSprite::SetText(const wstring& str){
		pImpl->m_text = str;
	}


	//最後尾に追加
	void StringSprite::AddText(const wstring& str){
		wstring TempText = GetText();
		TempText += str;
		SetText(TempText);
	}


	const bsm::Col4& StringSprite::GetFontColor() const{
		return pImpl->m_FontColor;
	}
	void StringSprite::SetFontColor(const bsm::Col4& Col){

	}

	const bsm::Col4& StringSprite::GetBackColor() const{
		return pImpl->m_BackColor;
	}
	void StringSprite::SetBackColor(const bsm::Col4& Col){
	}

	Point2D<float> StringSprite::GetBackTextMargin() const{
		return pImpl->m_BackTextMargin;
	}
	void StringSprite::SetBackTextMargin(Point2D<float> p){
		pImpl->m_BackTextMargin = p;
	}


	const wstring& StringSprite::GetFontName() const{
		return pImpl->m_FontName;
	}
	float StringSprite::GetFontSize() const{
		return pImpl->m_FoneSize;
	}

	float StringSprite::GetTextBlockWidth() const{
		return pImpl->m_TextBlockWidth;
	}
	void StringSprite::SetTextBlockWidth(float f){
		pImpl->m_TextBlockWidth = f;
	}
	float StringSprite::GetTextBlockHeight() const{
		return pImpl->m_TextBlockHeight;
	}
	void StringSprite::SetTextBlockHeight(float f){
		pImpl->m_TextBlockHeight = f;
	}




	ComPtr<IDWriteTextLayout>& StringSprite::GetTextLayout()const{
		return pImpl->m_textLayout;
	}
	ComPtr<IDWriteTextFormat>&	StringSprite::GetTextFormat()const{
		return pImpl->m_textFormat;
	}

	const DWRITE_TEXT_METRICS& StringSprite::GetDriteTextMetrics() const{
		return pImpl->m_textMetrics;
	}

	const Point2D<float>& StringSprite::GetStartPosition() const{
		return pImpl->m_StartPosition;
	}
	void StringSprite::SetStartPosition(const Point2D<float>& pos){
		pImpl->m_StartPosition = pos;
	}

	Rect2D<float> StringSprite::GetTextRect() const{
		Rect2D<float> ret;
		ret.left = GetStartPosition().x;
		ret.top = GetStartPosition().y;
		ret.right = ret.left + GetTextBlockWidth();
		ret.bottom = ret.top + GetTextBlockHeight();
		return ret;
	}
	void StringSprite::SetTextRect(const Rect2D<float>& rect){
		SetStartPosition(Point2D<float>(rect.left, rect.top));
		SetTextBlockWidth(rect.Width());
		SetTextBlockHeight(rect.Height());
	}

	void StringSprite::OnUpdate(){

	}



	void StringSprite::OnDraw(){
	}

	struct StringSpriteItem {
		ComPtr<IDWriteTextLayout>  m_textLayout;
		DWRITE_TEXT_METRICS m_textMetrics;
		Rect2D<float> m_Rect;
		wstring m_String;
		bool m_Updated;
		//クリップされるかどうか
		bool m_Clip;
		StringSpriteItem() :
			m_textLayout(nullptr),
			m_Rect(),
			m_String(L""),
			m_Updated(true),
			m_Clip(false)
		{
			ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
		}
	};


	//--------------------------------------------------------------------------------------
	//	struct MultiStringSprite::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct MultiStringSprite::Impl {
		// テキスト レンダリングに関連するリソース。
		float m_FoneSize;
		wstring m_FontName;
		bsm::Col4 m_FontColor;
		StringSprite::TextAlignment m_TextAlignment;

		bsm::Col4 m_BackColor;		//背景色
		Point2D<float> m_BackTextMargin;	//背景色を塗りつぶすテキストのマージン(左右と上下)

		ComPtr<ID2D1SolidColorBrush>    m_Brush;
		ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		ComPtr<IDWriteTextFormat>		m_textFormat;

		ComPtr<ID2D1SolidColorBrush>    m_BackBrush;

		vector<StringSpriteItem> m_Items;


		Impl() :
			m_FoneSize(16.0f),
			m_FontName(L"ＭＳゴシック"),
			m_FontColor(bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_TextAlignment(StringSprite::TextAlignment::m_Left),
			m_BackColor(bsm::Col4(0.0f, 0.0f, 0.0f, 0.0f)),
			m_BackTextMargin(4.0f, 0.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class MultiStringSprite : public Component;
	//	用途: MultiStringSpriteコンポーネント
	//	文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	MultiStringSprite::MultiStringSprite(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl()) {
	}
	MultiStringSprite::~MultiStringSprite() {}
	//アクセサ
	void MultiStringSprite::SetFont(const wstring& FontName, float FontSize) {



	}
	StringSprite::TextAlignment MultiStringSprite::GetTextAlignment() const {
		return pImpl->m_TextAlignment;
	}
	void MultiStringSprite::SetTextAlignment(StringSprite::TextAlignment Alignment) {
		pImpl->m_TextAlignment = Alignment;
	}


	const bsm::Col4& MultiStringSprite::GetFontColor() const {
		return pImpl->m_FontColor;
	}
	void MultiStringSprite::SetFontColor(const bsm::Col4& Col) {
		pImpl->m_FontColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);
		// デバイスに依存するリソースを作成します。
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
				ColBrush,
				&pImpl->m_Brush
			),
			L"フォントブラシ設定に失敗しました。",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"MultiStringSprite::SetFontColor()"
		);

	}

	const bsm::Col4& MultiStringSprite::GetBackColor() const {
		return pImpl->m_BackColor;
	}
	void MultiStringSprite::SetBackColor(const bsm::Col4& Col) {
	}

	Point2D<float> MultiStringSprite::GetBackTextMargin() const {
		return pImpl->m_BackTextMargin;
	}
	void MultiStringSprite::SetBackTextMargin(Point2D<float> p) {
		pImpl->m_BackTextMargin = p;
	}


	const wstring& MultiStringSprite::GetFontName() const {
		return pImpl->m_FontName;
	}
	float MultiStringSprite::GetFontSize() const {
		return pImpl->m_FoneSize;
	}

	ComPtr<IDWriteTextFormat>&	MultiStringSprite::GetTextFormat()const {
		return pImpl->m_textFormat;
	}



	const wstring& MultiStringSprite::GetText(size_t Index) const {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::GetText()"
			);
		}
		return pImpl->m_Items[Index].m_String;
	}
	void MultiStringSprite::SetText(size_t Index, const wstring& str, bool Clip) {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::SetText()"
			);
		}
		pImpl->m_Items[Index].m_String = str;
		pImpl->m_Items[Index].m_Updated = true;
		pImpl->m_Items[Index].m_Clip = Clip;
	}
	void MultiStringSprite::AddText(size_t Index, const wstring& str, bool Clip) {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::AddText()"
			);
		}
		pImpl->m_Items[Index].m_String += str;
		pImpl->m_Items[Index].m_Updated = true;
		pImpl->m_Items[Index].m_Clip = Clip;
	}
	void MultiStringSprite::InsertTextBlock(const Rect2D<float>& Block, const wstring& str, bool Clip) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();

		StringSpriteItem Item;
		Item.m_Rect = Block;
		Item.m_String = str;
		Item.m_Updated = true;
		Item.m_Clip = Clip;
		ZeroMemory(&Item.m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
		pImpl->m_Items.push_back(Item);
	}
	void MultiStringSprite::UpdateTextBlock(size_t Index, const Rect2D<float>& Block, const wstring& str, bool Clip) {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::UpdateTextBlock()"
			);
		}
		StringSpriteItem Item;
		Item.m_Rect = Block;
		Item.m_String = str;
		Item.m_Updated = true;
		Item.m_Clip = Clip;
		ZeroMemory(&Item.m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
		pImpl->m_Items[Index] = Item;

	}

	void MultiStringSprite::ClearTextBlock() {
		pImpl->m_Items.clear();
	}


	ComPtr<IDWriteTextLayout>& MultiStringSprite::GetTextLayout(size_t Index)const {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::GetTextLayout()"
			);
		}
		return pImpl->m_Items[Index].m_textLayout;
	}
	const DWRITE_TEXT_METRICS& MultiStringSprite::GetDriteTextMetrics(size_t Index) const {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::GetDriteTextMetrics()"
			);
		}
		return pImpl->m_Items[Index].m_textMetrics;
	}


	void MultiStringSprite::OnUpdate() {
	}

	void MultiStringSprite::OnDraw() {
	}


}

//end basecross
