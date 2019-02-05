
/*!
@file SpriteStudio5.cpp
@brief �X�v���C�g�X�^�W�I����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {
	//���[�J���֐�

	//---------------------------------------------------------------------------
	/**
	�����^��
	*/
	//---------------------------------------------------------------------------
	static float	// now�ɂ������Ԃ��ꂽ�l
		linear_(
		float	start,	// �n�_
		float	end,	// �I�_
		float	now)	// ���݂̎��� 0.f~1.f
	{
			return start + (now * (end - start));
		}

	//---------------------------------------------------------------------------
	/**
	�����^��
	*/
	//---------------------------------------------------------------------------
	static float	// now�ɂ������Ԃ��ꂽ�l
		accelerating_(
		float	start,	// �n�_
		float	end,	// �I�_
		float	now)	// ���݂̎��� 0.f~1.f
	{
			float rate = now * now;
			return start + (rate * (end - start));
	}

	//---------------------------------------------------------------------------
	/**
	�����^��
	*/
	//---------------------------------------------------------------------------
	static float	// now�ɂ������Ԃ��ꂽ�l
		decelerating_(
		float	start,	// �n�_
		float	end,	// �I�_
		float	now)	// ���݂̎��� 0.f~1.f
	{
			float time = 1 - now;
			float rate = 1 - time * time;
			return start + (rate * (end - start));
	}


	static float bezier_(float start, float end, float time, const SsCurve * c)
	{
		float fCurrentPos = (c->endKeyTime - c->startKeyTime) * time + c->startKeyTime;

		float fRet = end;
		float fCurrentCalc = 0.5f;
		float fCalcRange = 0.5f;

		float fTemp1;
		float fTemp2;
		float fTemp3;

		float fCurrentX;

		for (int iLoop = 0; iLoop < 8; iLoop++)
		{// more count of loop, better precision increase
			fTemp1 = 1.0f - fCurrentCalc;
			fTemp2 = fTemp1 * fTemp1;
			fTemp3 = fTemp2 * fTemp1;
			fCurrentX = (fTemp3 * c->startKeyTime) +
				(3 * fTemp2 * fCurrentCalc * (c->startTime + c->startKeyTime)) +
				(3 * fTemp1 * fCurrentCalc * fCurrentCalc * (c->endTime + c->endKeyTime)) +
				(fCurrentCalc * fCurrentCalc * fCurrentCalc * c->endKeyTime);

			fCalcRange /= 2.0f;
			if (fCurrentX > fCurrentPos)
			{
				fCurrentCalc -= fCalcRange;
			}
			else
			{
				fCurrentCalc += fCalcRange;
			}
		}

		// finally calculate with current value
		fTemp1 = 1.0f - fCurrentCalc;
		fTemp2 = fTemp1 * fTemp1;
		fTemp3 = fTemp2 * fTemp1;
		fRet = (fTemp3 * start) +
			(3 * fTemp2 * fCurrentCalc * (c->startValue + start)) +
			(3 * fTemp1 * fCurrentCalc * fCurrentCalc * (c->endValue + end)) +
			(fCurrentCalc * fCurrentCalc * fCurrentCalc * end);

		return fRet;
	}

	/**
	�G���~�[�g�ł� c->startTime, c->endTime �͕K�v�Ȃ�

	�X���[�v�l�����O�v�Z���Ă����΃J�[�u�v�Z�p�p�����[�^�͂P�ɂȂ�
	���A�x�W�F�Ƌ��p���邽�߂��̂܂܂̌`�ɂ��Ă����B
	*/
	static float hermite_(float start, float end, float time, const SsCurve * c)
	{
		float t2 = time * time;
		float t3 = t2 * time;
		float result =
			(2 * t3 - 3 * t2 + 1) * start +
			(-2 * t3 + 3 * t2) * end +
			(t3 - 2 * t2 + time) * (c->startValue - start) +
			(t3 - t2) * (c->endValue - end);
		return result;
	}

	//--------------------------------------------------------------------------------------
	//	struct SS5Util;
	//	�p�r: SpriteStdio���[�e�B���e�B
	//--------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	/**
	�^�C�v���w�肵�ĕ�Ԃ���
	*/
	//----------------------------------------------------------------------------
	float SS5Util::SsInterpolate(SsInterpolationType::_enum type,
		float time, float start, float end, const SsCurve * curve)
	{
		float r = 0.0f;
		switch (type)
		{
		case SsInterpolationType::none:
			r = start;
			break;
		case SsInterpolationType::linear:
			r = linear_(start, end, time);
			break;
		case SsInterpolationType::acceleration:
			r = accelerating_(start, end, time);
			break;
		case SsInterpolationType::deceleration:
			r = decelerating_(start, end, time);
			break;
		case SsInterpolationType::bezier:
			r = bezier_(start, end, time, curve);
			break;
		case SsInterpolationType::hermite:
			r = hermite_(start, end, time, curve);
			break;
		default:
			//SS_ASSERT_ID(type);
			break;
		}
		return r;
	}


	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeLine;
	//	�p�r: �A�j���[�V�������C���i���\���́j
	//--------------------------------------------------------------------------------------
	SS5AnimeLine::SS5AnimeLine() :
		type(SsAttributeKind::invalid){}
	SS5AnimeLine::~SS5AnimeLine(){}

	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeBoolVal::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SS5AnimeBoolVal::Impl{
		struct data{
			float time;	//�X�^�[�g�^�C��
			bool val;	//�l
		};
		vector< data > line;
	};
	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeBoolVal : public SS5AnimeLine;
	//	�p�r: bool�^�A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	SS5AnimeBoolVal::SS5AnimeBoolVal(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					im.val = SS5Util::TextToBool(pXMLDOMNode, L"value");
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5AnimeBoolVal::~SS5AnimeBoolVal(){}

	//�w�莞�Ԃ̒l��Ԃ�
	bool SS5AnimeBoolVal::GetValue(float Time){
		if (pImpl->line.size() <= 0){
			return false;
		}
		bool Start = false;
		float StartTime = 0;
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.val;
				StartTime = ptr.time;
			}
		}
		return Start;
	}


	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeFloatVal::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SS5AnimeFloatVal::Impl{
		struct data{
			float time;	//�X�^�[�g�^�C��
			SsInterpolationType::_enum ipType;
			float val;	//�l
			SsCurve curve;	//�J�[�u��񂪂���ꍇ
		};
		vector< data > line;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeFloatVal : public SS5AnimeLine;
	//	�p�r: float�^�̒l��ێ�����A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	SS5AnimeFloatVal::SS5AnimeFloatVal(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					wstring iptype = SS5Util::AttributeToWstr(pXMLDOMNode, L"ipType");
					im.ipType = SsInterpolationType::ws2e(iptype);
					if (SS5Util::SsNeedsCurveParams(im.ipType)){
						wstring curvestr = SS5Util::TextToWstr(pXMLDOMNode, L"curve");
						vector<wstring> strvec;
						Util::WStrToTokenVector(strvec, curvestr, L' ');
						im.curve.startTime = (float)_wtof(strvec[0].c_str());
						im.curve.startValue = (float)_wtof(strvec[1].c_str());
						im.curve.endTime = (float)_wtof(strvec[2].c_str());
						im.curve.endValue = (float)_wtof(strvec[3].c_str());
					}
					im.val = SS5Util::TextToFloat(pXMLDOMNode, L"value");
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5AnimeFloatVal::~SS5AnimeFloatVal(){}

	//�ݒ肳�ꂽ��Ԃ����ƂɎw�莞�Ԃ̒l��Ԃ�
	float SS5AnimeFloatVal::GetValue(float Time){
		if (pImpl->line.size() <= 0){
			return 0;
		}
		size_t TgtVecIndex = 0;
		SsInterpolationType::_enum Type;
		Type = SsInterpolationType::none;

		float Start = 0;
		float End = 0;

		float StartTime = 0;
		float EndTime = 0;
		SsCurve curve;
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.val;
				StartTime = ptr.time;
				Type = ptr.ipType;
				curve = ptr.curve;
			}
		}
		End = Start;
		EndTime = StartTime;
		for (auto ptr : pImpl->line){
			if (ptr.time > Time){
				End = ptr.val;
				EndTime = ptr.time;
				break;
			}
		}

		if (Type == SsInterpolationType::bezier)
		{
			// �x�W�F�̂݃L�[�̊J�n�E�I�����Ԃ��K�v
			curve.startKeyTime = StartTime;
			curve.endKeyTime = EndTime;
		}
		double spanFreme = Time - StartTime;
		double totalFrame = abs(EndTime - StartTime);
		double tgtFrame = 0;
		if (totalFrame > 0){
			tgtFrame = spanFreme / totalFrame;
		}
		if (SS5Util::SsNeedsCurveParams(Type)){
			return SS5Util::SsInterpolate(Type, (float)tgtFrame, Start, End, &curve);
		}
		else{
			return SS5Util::SsInterpolate(Type, (float)tgtFrame, Start, End, nullptr);
		}
	}

	float SS5AnimeFloatVal::GetScaleValue(float Time){
		if (pImpl->line.size() <= 0){
			return 1.0f;
		}
		size_t TgtVecIndex = 0;
		SsInterpolationType::_enum Type;
		Type = SsInterpolationType::none;

		float Start = 1.0f;
		float End = 1.0f;

		float StartTime = 0;
		float EndTime = 0;
		SsCurve curve;
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.val;
				StartTime = ptr.time;
				Type = ptr.ipType;
				curve = ptr.curve;
			}
		}
		End = Start;
		EndTime = StartTime;
		for (auto ptr : pImpl->line){
			if (ptr.time > Time){
				End = ptr.val;
				EndTime = ptr.time;
				break;
			}
		}

		if (Type == SsInterpolationType::bezier)
		{
			// �x�W�F�̂݃L�[�̊J�n�E�I�����Ԃ��K�v
			curve.startKeyTime = StartTime;
			curve.endKeyTime = EndTime;
		}
		double spanFreme = Time - StartTime;
		double totalFrame = abs(EndTime - StartTime);
		double tgtFrame = 0;
		if (totalFrame > 0){
			tgtFrame = spanFreme / totalFrame;
		}
		if (SS5Util::SsNeedsCurveParams(Type)){
			return SS5Util::SsInterpolate(Type, (float)tgtFrame, Start, End, &curve);
		}
		else{
			return SS5Util::SsInterpolate(Type, (float)tgtFrame, Start, End, nullptr);
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeCellVal::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SS5AnimeCellVal::Impl{
		struct data{
			float time;	//�X�^�[�g�^�C��
			wstring id;		//�}�b�vID
			wstring name;	//�l
		};
		vector< data > line;
	};



	//--------------------------------------------------------------------------------------
	//	class SS5AnimeCellVal : public SS5AnimeLine;
	//	�p�r: AnimeCell�^�̒l��ێ�����A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	SS5AnimeCellVal::SS5AnimeCellVal(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					auto ValueNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value");
					if (ValueNode){
						im.id = SS5Util::TextToWstr(ValueNode, L"mapId");
						im.name = SS5Util::TextToWstr(ValueNode, L"name");
					}
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5AnimeCellVal::~SS5AnimeCellVal(){}

	AnimeCell SS5AnimeCellVal::GetValue(float Time){
		AnimeCell ret;
		ret.id = 0;
		ret.name = L"";
		if (pImpl->line.size() <= 0){
			return ret;
		}
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				ret.id = wcstoul(ptr.id.c_str(), nullptr, 10);
				ret.name = ptr.name;
			}
		}
		return ret;
	}




	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeWstrVal::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SS5AnimeWstrVal::Impl{
		struct data{
			float time;	//�X�^�[�g�^�C��
			wstring val;	//�l
		};
		vector< data > line;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeWstrVal : public SS5AnimeLine;
	//	�p�r: wstrng�^�̒l��ێ�����A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	SS5AnimeWstrVal::SS5AnimeWstrVal(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					auto ValueNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value");
					if (ValueNode){
						im.val = SS5Util::TextToWstr(ValueNode, L"name");
					}
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5AnimeWstrVal::~SS5AnimeWstrVal(){}

	//�w�莞�Ԃ̒l��Ԃ�
	wstring SS5AnimeWstrVal::GetValue(float Time){
		if (pImpl->line.size() <= 0){
			return wstring(L"");
		}
		wstring Start(L"");
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.val;
			}
		}
		return Start;
	}

	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeColorVal::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SS5AnimeColorVal::Impl{
		struct data{
			float time;	//�X�^�[�g�^�C��
			SsInterpolationType::_enum ipType;
			bsm::Col4 val;	//�l
		};
		vector< data > line;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeColorVal : public SS5AnimeLine;
	//	�p�r: bsm::Col4�^�̒l��ێ�����A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	SS5AnimeColorVal::SS5AnimeColorVal(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					wstring iptype = SS5Util::AttributeToWstr(pXMLDOMNode, L"ipType");
					im.ipType = SsInterpolationType::ws2e(iptype);
					im.val = SS5Util::TextARGBToColor4(pXMLDOMNode, L"value");
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}

	SS5AnimeColorVal::~SS5AnimeColorVal(){}


	//�ݒ肳�ꂽ��Ԃ����ƂɎw�莞�Ԃ̒l��Ԃ�
	bsm::Col4 SS5AnimeColorVal::GetValue(float Time){
		if (pImpl->line.size() <= 0){
			return bsm::Col4(0, 0, 0, 0);
		}
		size_t TgtVecIndex = 0;
		SsInterpolationType::_enum Type;
		Type = SsInterpolationType::none;

		bsm::Col4 Start(0, 0, 0, 0);
		bsm::Col4 End(0, 0, 0, 0);

		float StartTime = 0;
		float EndTime = 0;
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.val;
				StartTime = ptr.time;
				Type = ptr.ipType;
			}
		}
		End = Start;
		EndTime = StartTime;
		for (auto ptr : pImpl->line){
			if (ptr.time > Time){
				End = ptr.val;
				EndTime = ptr.time;
				break;
			}
		}
		if (SS5Util::SsNeedsCurveParams(Type)){
			throw BaseException(
				L"���݂��̕�ԕ��@�͎�������܂���",
				L"�x�W�F�������̓G���~�[�g���g�p���Ă܂��񂩁H",
				L"SS5AnimeColorVal::GetValue()"
				);
		}
		double spanFreme = Time - StartTime;
		double totalFrame = abs(EndTime - StartTime);
		double tgtFrame = 0;
		if (totalFrame > 0){
			tgtFrame = spanFreme / totalFrame;
		}
		bsm::Col4 ret(0, 0, 0, 0);
		ret.x = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.x, End.x, nullptr);
		ret.y = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.y, End.y, nullptr);
		ret.z = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.z, End.z, nullptr);
		ret.w = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.w, End.w, nullptr);
		ret = XMVector4ClampLength(ret, 0.0f, 1.0f);
		return ret;
	}


	//--------------------------------------------------------------------------------------
	//	struct SS5Animebsm::Vec3Val::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SS5AnimeVector3Val::Impl{
		struct data{
			float time;	//�X�^�[�g�^�C��
			SsInterpolationType::_enum ipType;
			Vertex2DAnimeData tgt;
		};
		vector< data > line;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5Animebsm::Vec3Val : public SS5AnimeLine;
	//	�p�r: bsm::Vec3�^�̒l��ێ�����A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	SS5AnimeVector3Val::SS5AnimeVector3Val(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					wstring iptype = SS5Util::AttributeToWstr(pXMLDOMNode, L"ipType");
					im.ipType = SsInterpolationType::ws2e(iptype);

					auto VertNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value/LT");
					if (VertNode){
						im.tgt.LeftTop = SS5Util::WstrToPoint2DF(XmlDocReader::GetText(VertNode));
					}
					else{
						im.tgt.LeftTop = Point2D<float>(0, 0);
					}

					VertNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value/RT");
					if (VertNode){
						im.tgt.RightTop = SS5Util::WstrToPoint2DF(XmlDocReader::GetText(VertNode));
					}
					else{
						im.tgt.RightTop = Point2D<float>(0, 0);
					}

					VertNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value/LB");
					if (VertNode){
						im.tgt.LeftBottom = SS5Util::WstrToPoint2DF(XmlDocReader::GetText(VertNode));
					}
					else{
						im.tgt.LeftBottom = Point2D<float>(0, 0);
					}

					VertNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value/RB");
					if (VertNode){
						im.tgt.RightBottom = SS5Util::WstrToPoint2DF(XmlDocReader::GetText(VertNode));
					}
					else{
						im.tgt.RightBottom = Point2D<float>(0, 0);
					}
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5AnimeVector3Val::~SS5AnimeVector3Val(){}
	//�ݒ肳�ꂽ��Ԃ����ƂɎw�莞�Ԃ̒l��Ԃ�
	Vertex2DAnimeData SS5AnimeVector3Val::GetValue(float Time){
		Vertex2DAnimeData Ret, Start, End;
		if (pImpl->line.size() <= 0){
			return Ret;
		}
		size_t TgtVecIndex = 0;
		SsInterpolationType::_enum Type;
		Type = SsInterpolationType::none;


		float StartTime = 0;
		float EndTime = 0;
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.tgt;
				StartTime = ptr.time;
				Type = ptr.ipType;
			}
		}
		End = Start;
		EndTime = StartTime;
		for (auto ptr : pImpl->line){
			if (ptr.time > Time){
				End = ptr.tgt;
				EndTime = ptr.time;
				break;
			}
		}
		if (SS5Util::SsNeedsCurveParams(Type)){
			throw BaseException(
				L"���݂��̕�ԕ��@�͎�������܂���",
				L"�x�W�F�������̓G���~�[�g���g�p���Ă܂��񂩁H",
				L"SS5Animebsm::Vec3Val::GetValue()"
				);
		}
		double spanFreme = Time - StartTime;
		double totalFrame = abs(EndTime - StartTime);
		double tgtFrame = 0;
		if (totalFrame > 0){
			tgtFrame = spanFreme / totalFrame;
		}
		bsm::Vec3 ret(0, 0, 0);
		Ret.LeftTop.x = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.LeftTop.x, End.LeftTop.x, nullptr);
		Ret.LeftTop.y = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.LeftTop.y, End.LeftTop.y, nullptr);

		Ret.RightTop.x = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.RightTop.x, End.RightTop.x, nullptr);
		Ret.RightTop.y = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.RightTop.y, End.RightTop.y, nullptr);

		Ret.LeftBottom.x = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.LeftBottom.x, End.LeftBottom.x, nullptr);
		Ret.LeftBottom.y = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.LeftBottom.y, End.LeftBottom.y, nullptr);

		Ret.RightBottom.x = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.RightBottom.x, End.RightBottom.x, nullptr);
		Ret.RightBottom.y = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.RightBottom.y, End.RightBottom.y, nullptr);


		return Ret;
	}

	//--------------------------------------------------------------------------------------
	//	struct SS5Cell::Impl;
	//	�p�r: SpriteStdio�Z�����C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SS5Cell::Impl{
		wstring	name;			///< �Z������
		Point2D<float> pos;			///< ����̍��W
		Point2D<float> size;			///< WH�s�N�Z���T�C�Y
		Point2D<float> pivot;			///< WH�s�N�Z���T�C�Y
		bool	 rotated;		///< �������ɂX�O�x��]����Ă���Buvs �̊��蓖�Ă��ς��B

		Impl(IXMLDOMNodePtr Node);
		~Impl(){}
	};
	SS5Cell::Impl::Impl(IXMLDOMNodePtr Node){
		try{
			//XML�̓ǂݍ���
			name = SS5Util::TextToWstr(Node, L"name");
			pos = SS5Util::TextToPoint2DF(Node, L"pos");
			size = SS5Util::TextToPoint2DF(Node, L"size");
			pivot = SS5Util::TextToPoint2DF(Node, L"pivot");
			rotated = SS5Util::TextToBool(Node, L"rotated");
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class SS5Cell;
	//	�p�r: SpriteStdio�Z��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SS5Cell::SS5Cell(IXMLDOMNodePtr Node) :
		pImpl(new Impl(Node))
	{
	}
	SS5Cell::~SS5Cell(){}
	//�A�N�Z�b�T
	const wstring& SS5Cell::get_name() const { return pImpl->name; }
	const Point2D<float>& SS5Cell::get_pos() const { return pImpl->pos; }
	const Point2D<float>& SS5Cell::get_size() const { return pImpl->size; }
	const Point2D<float>& SS5Cell::get_pivot() const { return pImpl->pivot; }
	bool SS5Cell::get_rotated() const { return pImpl->rotated; }
	bool SS5Cell::is_rotated() const { return pImpl->rotated; }

	//--------------------------------------------------------------------------------------
	//	struct SS5ssce::Impl;
	//	�p�r: SpriteStdio��ssce�N���X���C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SS5ssce::Impl{
		wstring	basedir;			///< ��f�B���N�g���i\���Ō�ɂ��j
		wstring	fname;				///< �Z���}�b�v�̃t�@�C���l�[��
		wstring	name;				///< ���̃Z���}�b�v�̖��̂ł��B
		wstring	imagePath;			///< �Q�Ɖ摜�t�@�C���p�X�B�v���W�F�N�g�̉摜�����
		wstring textureResname;		///�e�N�X�`���̃��\�[�X��
		Point2D<float> pixelSize;			///< �摜�̃s�N�Z��WH�T�C�Y
		bool overrideTexSettings;///< �e�N�X�`���ݒ���v���W�F�N�g�̐ݒ�ł͂Ȃ����L�ݒ���g��
		SsTexWrapMode::_enum		wrapMode;			///< �e�N�X�`���̃��b�v���[�h
		SsTexFilterMode::_enum		filterMode;			///< �e�N�X�`���̃t�B���^���[�h
		vector< shared_ptr<SS5Cell> > m_CellVec;
		shared_ptr<TextureResource> m_Texture;			//�e�N�X�`��
		//�\�z�Ə���
		Impl(const wstring& BaseDir, const wstring& Xmlfilename);
		~Impl(){}
	};
	SS5ssce::Impl::Impl(const wstring& BaseDir, const wstring& Xmlfilename) :
		basedir(BaseDir),
		fname(Xmlfilename)
	{
		try{

			XmlDocReader doc(basedir + fname);
			//��{�f�[�^
			auto Setting = doc.GetSelectSingleNode(L"SpriteStudioCellMap");
			name = SS5Util::TextToWstr(Setting, L"name");
			imagePath = SS5Util::TextToWstr(Setting, L"imagePath");
			pixelSize = SS5Util::TextToPoint2DF(Setting, L"pixelSize");
			overrideTexSettings = SS5Util::TextToBool(Setting, L"overrideTexSettings");

			wrapMode = SsTexWrapMode::ws2e(SS5Util::TextToWstr(Setting, L"wrapMode"));
			filterMode = SsTexFilterMode::ws2e(SS5Util::TextToWstr(Setting, L"filterMode"));

			//�e�N�X�`���̃��\�[�X��
			wstring strTexture = basedir + imagePath;
			textureResname = name;
			m_Texture = App::GetApp()->RegisterTexture(textureResname, strTexture);
			//�e�Z��
			auto Cells = doc.GetSelectNodes(L"SpriteStudioCellMap/cells/cell");
			// �m�[�h���X�g�̃m�[�h�̐��̎擾
			long lCountNode = XmlDocReader::GetLength(Cells);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(Cells, i);
				//�Z���̓I�u�W�F�N�g�z��ɒǉ����Ȃ�
				shared_ptr<SS5Cell> cell(new SS5Cell(pXMLDOMNode));
				m_CellVec.push_back(cell);
			}

		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class SS5ssce;
	//	�p�r: SpriteStdio��ssce�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	SS5ssce::SS5ssce(const wstring& BaseDir, const wstring& Xmlfilename) :
		pImpl(new Impl(BaseDir, Xmlfilename))
	{
	}
	SS5ssce::~SS5ssce(){}
	//�A�N�Z�b�T
	const Point2D<float>& SS5ssce::get_pixelSize() const{
		return pImpl->pixelSize;
	}
	//�Z���̌���
	shared_ptr<SS5Cell> SS5ssce::GetCell(const wstring& name){
		for (auto ptr : pImpl->m_CellVec){
			if (name == ptr->get_name()){
				return ptr;
			}
		}
		//������Ȃ�
		return nullptr;
	}
	//�e�N�X�`�����\�[�X��
	const wstring& SS5ssce::get_textureResname() const{
		return pImpl->textureResname;
	}
	//�Z���}�b�v��
	const wstring& SS5ssce::get_name() const{
		return pImpl->name;
	}

	//�e�N�X�`��
	shared_ptr<TextureResource> SS5ssce::GetTexture() const{
		return pImpl->m_Texture;
	}


	//--------------------------------------------------------------------------------------
	//	struct SS5PartAnimation::Impl;
	//	�p�r: �p�[�c���Ƃ̃A�j���[�V�������C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SS5PartAnimation::Impl{
		wstring	partName;
		//�A�j���[�V�����̃|�C���^�ithis��n���̂Ő��|�C���^���g���j
		SS5Animation* m_pSS5Animation;
		vector< shared_ptr<SS5AnimeLine> > m_LineVec;
		Impl(IXMLDOMNodePtr TgtNode, SS5Animation* pSS5Animation);
		~Impl(){}
		//����
		void FloatAnimationSub(IXMLDOMNodePtr TgtNode, const wstring& Key);
		//���݂̎p���𓾂�
		bsm::Mat4x4 GetLocalMatrix();

		bool GetHide();
		float GetPrio();
		float GetAlpha();

		AnimeCell GetCell();

		Vertex2DAnimeData GetVirtex2DData();

		void GetIfFloatVal(bool b, shared_ptr<SS5AnimeLine>& ptr, float& dest, float AnimeFrame);
		void GetIfFloatScaleVal(bool b, shared_ptr<SS5AnimeLine>& ptr, float& dest, float AnimeFrame);

	};

	SS5PartAnimation::Impl::Impl(IXMLDOMNodePtr TgtNode, SS5Animation* pSS5Animation) :
		m_pSS5Animation(pSS5Animation)
	{
		try{
			partName = SS5Util::TextToWstr(TgtNode, L"partName");
			auto Node = XmlDocReader::GetSelectSingleNode(TgtNode, L"attributes/attribute[@tag=\'HIDE\']");
			if (Node){
				auto ptr = make_shared<SS5AnimeBoolVal>(Node, L"HIDE");
				m_LineVec.push_back(ptr);
			}
			Node = XmlDocReader::GetSelectSingleNode(TgtNode, L"attributes/attribute[@tag=\'CELL\']");
			if (Node){
				auto ptr = make_shared<SS5AnimeCellVal>(Node, L"CELL");
				m_LineVec.push_back(ptr);
			}

			Node = XmlDocReader::GetSelectSingleNode(TgtNode, L"attributes/attribute[@tag=\'VERT\']");
			if (Node){
				auto ptr = make_shared<SS5AnimeVector3Val>(Node, L"VERT");
				m_LineVec.push_back(ptr);
			}




			FloatAnimationSub(TgtNode, L"POSX");
			FloatAnimationSub(TgtNode, L"POSY");
			FloatAnimationSub(TgtNode, L"POSZ");
			FloatAnimationSub(TgtNode, L"ROTX");
			FloatAnimationSub(TgtNode, L"ROTY");
			FloatAnimationSub(TgtNode, L"ROTZ");
			FloatAnimationSub(TgtNode, L"SCLX");
			FloatAnimationSub(TgtNode, L"SCLY");
			FloatAnimationSub(TgtNode, L"ALPH");
			FloatAnimationSub(TgtNode, L"PRIO");

		}
		catch (...){
			throw;
		}
	}

	void SS5PartAnimation::Impl::FloatAnimationSub(IXMLDOMNodePtr TgtNode, const wstring& Key){
		wstring str = L"attributes/attribute[@tag=\'" + Key + L"\']";
		auto Node = XmlDocReader::GetSelectSingleNode(TgtNode, str.c_str());
		if (Node){
			auto ptr = make_shared<SS5AnimeFloatVal>(Node, Key.c_str());
			m_LineVec.push_back(ptr);
		}
	}

	//���݂̎p���s��𓾂�
	void SS5PartAnimation::Impl::GetIfFloatVal(bool b, shared_ptr<SS5AnimeLine>& ptr, float& dest, float AnimeFrame){
		if (b){
			auto v = dynamic_pointer_cast<SS5AnimeFloatVal>(ptr);
			dest = v->GetValue(AnimeFrame);
		}
	}

	void SS5PartAnimation::Impl::GetIfFloatScaleVal(bool b, shared_ptr<SS5AnimeLine>& ptr, float& dest, float AnimeFrame){
		if (b){
			auto v = dynamic_pointer_cast<SS5AnimeFloatVal>(ptr);
			dest = v->GetScaleValue(AnimeFrame);
		}
	}


	bool SS5PartAnimation::Impl::GetHide(){
		bool ret = false;
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();
		for (auto ptr : m_LineVec){
			if (ptr->GetType() == SsAttributeKind::hide){
				auto v = dynamic_pointer_cast<SS5AnimeBoolVal>(ptr);
				ret = v->GetValue(AnimeFrame);
			}
		}
		return ret;
	}
	float SS5PartAnimation::Impl::GetPrio(){
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();
		float prio = 0;
		for (auto ptr : m_LineVec){
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::prio, ptr, prio, AnimeFrame);
		}
		return prio;
	}

	float SS5PartAnimation::Impl::GetAlpha(){
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();
		float Alpha = 1.0f;
		for (auto ptr : m_LineVec){
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::alpha, ptr, Alpha, AnimeFrame);
		}
		return Alpha;
	}


	AnimeCell SS5PartAnimation::Impl::GetCell(){
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();
		AnimeCell cell;
		for (auto ptr : m_LineVec){
			if (ptr->GetType() == SsAttributeKind::cell){
				auto v = dynamic_pointer_cast<SS5AnimeCellVal>(ptr);
				cell = v->GetValue(AnimeFrame);
			}
		}
		return cell;
	}

	Vertex2DAnimeData SS5PartAnimation::Impl::GetVirtex2DData(){
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();
		Vertex2DAnimeData data;
		for (auto ptr : m_LineVec){
			if (ptr->GetType() == SsAttributeKind::vertex){
				auto v = dynamic_pointer_cast<SS5AnimeVector3Val>(ptr);
				data = v->GetValue(AnimeFrame);
			}
		}
		return data;
	}



	//���݂̍s��𓾂�B
	//�A�j���[�V��������̃f�[�^�̎��o��
	bsm::Mat4x4 SS5PartAnimation::Impl::GetLocalMatrix(){
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();

		bsm::Mat4x4 LocalMatrix;
		bsm::Vec3 Pos(0, 0, 0);
		bsm::Vec3 Rot(0, 0, 0);
		bsm::Vec3 Scale(1.0f, 1.0f, 1.0f);
		for (auto ptr : m_LineVec){
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::posx, ptr, Pos.x, AnimeFrame);
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::posy, ptr, Pos.y, AnimeFrame);
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::posz, ptr, Pos.z, AnimeFrame);
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::rotx, ptr, Rot.x, AnimeFrame);
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::roty, ptr, Rot.y, AnimeFrame);
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::rotz, ptr, Rot.z, AnimeFrame);
			GetIfFloatScaleVal(ptr->GetType() == SsAttributeKind::sclx, ptr, Scale.x, AnimeFrame);
			GetIfFloatScaleVal(ptr->GetType() == SsAttributeKind::scly, ptr, Scale.y, AnimeFrame);
		}

		Pos.x /= m_pSS5Animation->get_gridSize();
		Pos.y /= m_pSS5Animation->get_gridSize();
		//���W�A���ɕϊ�
		Rot.x = Rot.x * XM_PI / 180.0f;
		Rot.y = Rot.y * XM_PI / 180.0f;
		Rot.z = Rot.z * XM_PI / 180.0f;
		LocalMatrix.affineTransformation(Scale, bsm::Vec3(0,0,0),Rot, Pos);
		return LocalMatrix;
	}

	//--------------------------------------------------------------------------------------
	//	class SS5PartAnimation;
	//	�p�r: �p�[�c���Ƃ̃A�j���[�V����
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	SS5PartAnimation::SS5PartAnimation(IXMLDOMNodePtr TgtNode, SS5Animation* pSS5Animation) :
		pImpl(new Impl(TgtNode, pSS5Animation))
	{
	}
	SS5PartAnimation::~SS5PartAnimation(){}

	//�A�N�Z�b�T
	const wstring& SS5PartAnimation::GetpartName() const{
		return pImpl->partName;
	}

	//�O���b�h�T�C�Y
	float SS5PartAnimation::GetGridSize() const{
		return pImpl->m_pSS5Animation->get_gridSize();
	}


	bsm::Mat4x4 SS5PartAnimation::GetLocalMatrix(){
		return pImpl->GetLocalMatrix();
	}

	bool SS5PartAnimation::GetHide(){
		return pImpl->GetHide();
	}
	float SS5PartAnimation::GetPrio(){
		return pImpl->GetPrio();
	}

	float SS5PartAnimation::GetAlpha(){
		return pImpl->GetAlpha();
	}


	AnimeCell SS5PartAnimation::GetCell(){
		return pImpl->GetCell();
	}

	Vertex2DAnimeData SS5PartAnimation::GetVirtex2DData(){
		return pImpl->GetVirtex2DData();
	}



	//--------------------------------------------------------------------------------------
	//	struct SS5Animation::Impl;
	//	�p�r: �A�j���[�V�����̃C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SS5Animation::Impl{
		wstring	name;
		bool overrideSettings;
		float fps;
		float frameCount;
		SsPartsSortMode::_enum sortMode;
		Point2D<float> canvasSize;
		Point2D<float> pivot;
		bsm::Col4 bgColor;
		float gridSize;
		bsm::Col4 gridColor;
		int outStartNum;
		float m_CurrentTime;
		bool m_Looped;
		bool m_AnimeEnd;
		vector< shared_ptr<SS5PartAnimation> > m_PartAnimationVec;
		//�\�z�Ɣj��
		Impl(IXMLDOMNodePtr TgtNode);
		~Impl(){}
		//�A�j���[�V�����^�C�}�[�̏�����
		void SetCurrentTime(float AnimeTime){
			if (AnimeTime < 0){
				m_CurrentTime = 0;
			}
			else{
				m_CurrentTime = AnimeTime;
			}
		}
		//�A�j���[�V�����^�C�}�[�̍X�V
		void AddCurrentTime(float TimeSpan){
			m_CurrentTime += TimeSpan;
		}
		//���݂̃A�j���[�V�����^�C�}�[�����ƂɃt���[�����v�Z���Ԃ�
		float GetAnimationTimer2Frame();
	};

	//�\�z�Ɣj��
	SS5Animation::Impl::Impl(IXMLDOMNodePtr TgtNode) :
		m_CurrentTime(0),
		m_Looped(true),
		m_AnimeEnd(false)
	{
		try{
			name = SS5Util::TextToWstr(TgtNode, L"name");
			overrideSettings = SS5Util::TextToBool(TgtNode, L"overrideSettings");
			//��{�f�[�^
			auto Setting = XmlDocReader::GetSelectSingleNode(TgtNode, L"settings");
			if (!Setting){
				throw BaseException(
					L"settings��������܂���",
					L"settings",
					L"SS5Animation::Impl::Impl()"
					);
			}
			//�f�t�H���g��FPS
			fps = SS5Util::TextToFloat(Setting, L"fps");
			frameCount = SS5Util::TextToFloat(Setting, L"frameCount");
			sortMode = SsPartsSortMode::ws2e(SS5Util::TextToWstr(Setting, L"sortMode"));
			canvasSize = SS5Util::TextToPoint2DF(Setting, L"canvasSize");
			pivot = SS5Util::TextToPoint2DF(Setting, L"pivot");
			bgColor = SS5Util::TextARGBToColor4(Setting, L"bgColor");
			gridSize = SS5Util::TextToFloat(Setting, L"gridSize");
			gridColor = SS5Util::TextARGBToColor4(Setting, L"gridColor");
			outStartNum = SS5Util::TextToInt(Setting, L"outStartNum");

		}
		catch (...){
			throw;
		}
	}

	//���݂̃A�j���[�V�����^�C�}�[�����ƂɃt���[�����v�Z���Ԃ�
	float SS5Animation::Impl::GetAnimationTimer2Frame(){
		//�o�ߎ��Ԃ���t���[�������߂�
		float fCurrentFrame = m_CurrentTime * fps;
		//�A�j���[�V�������Ō�܂ł����Ă邩�ǂ������m�F
		bool ret = false;
		if (!m_Looped && fCurrentFrame >= frameCount){
			//���[�v���Ȃ��ŁA�A�j�����Ō�ɓ��B������
			ret = true;
		}
		int iAnimationFrame = static_cast< int >(fCurrentFrame * 10000.0f);
		if (frameCount > 0) {
			if (m_Looped){
				//���[�v����
				iAnimationFrame %= ((int)frameCount) * 10000;
				iAnimationFrame += outStartNum * 10000;
				m_AnimeEnd = false;
			}
			else{
				//���[�v���Ȃ�
				if (ret){
					//�Ō�̃t���[���Ƀ|�[�Y��ݒ肷��
					iAnimationFrame = (outStartNum + ((int)frameCount)) * 10000;
					m_AnimeEnd = true;
				}
				else{
					iAnimationFrame += outStartNum * 10000;
					m_AnimeEnd = false;
				}
			}
		}
		else{
			m_AnimeEnd = false;
		}
		return ((float)iAnimationFrame) / 10000.0f;
	}




	//--------------------------------------------------------------------------------------
	//	class SS5Animation;
	//	�p�r: �A�j���[�V����
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	SS5Animation::SS5Animation(IXMLDOMNodePtr TgtNode) :
		pImpl(new Impl(TgtNode))
	{
		try{
			auto ModelNodes = XmlDocReader::GetSelectNodes(TgtNode, L"partAnimes/partAnime");
			long lCountNode = XmlDocReader::GetLength(ModelNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(ModelNodes, i);
				if (pXMLDOMNode){
					auto ptr = make_shared<SS5PartAnimation>(pXMLDOMNode, this);
					pImpl->m_PartAnimationVec.push_back(ptr);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5Animation::~SS5Animation(){}

	//�A�N�Z�b�T
	const wstring& SS5Animation::get_name() const { return pImpl->name; }
	vector< shared_ptr<SS5PartAnimation> >& SS5Animation::get_PartAnimationVec(){
		return pImpl->m_PartAnimationVec;
	}
	const Point2D<float>& SS5Animation::get_canvasSize() const{
		return pImpl->canvasSize;
	}

	float SS5Animation::get_gridSize() const{
		return pImpl->gridSize;
	}

	const Point2D<float>& SS5Animation::get_pivot() const{
		return pImpl->pivot;
	}

	float  SS5Animation::get_fps() const{
		return pImpl->fps;
	}
	void SS5Animation::set_fps(float f){
		pImpl->fps = f;
	}

	//���[�v���邩�ǂ����𓾂�
	bool SS5Animation::IsLooped() const{
		return pImpl->m_Looped;
	}
	bool SS5Animation::GetLooped() const{
		return pImpl->m_Looped;
	}
	//���[�v���邩�ǂ�����ݒ肷��
	void SS5Animation::SetLooped(bool b){
		pImpl->m_Looped = b;
	}
	//�A�j���I���������ǂ���
	bool SS5Animation::IsAnimeEnd() const{
		return pImpl->m_AnimeEnd;
	}


	//����
	//�A�j���[�V�����^�C�}�[�̏�����
	void SS5Animation::SetCurrentTime(float AnimeTime){
		pImpl->SetCurrentTime(AnimeTime);
	}
	//�A�j���[�V�����^�C�}�[�̍X�V
	void  SS5Animation::AddCurrentTime(float TimeSpan){
		pImpl->AddCurrentTime(TimeSpan);
	}
	//���݂̃A�j���[�V�����^�C�}�[�����ƂɃt���[�����v�Z���Ԃ�
	float SS5Animation::GetAnimationTimer2Frame(){
		return pImpl->GetAnimationTimer2Frame();
	}

	//--------------------------------------------------------------------------------------
	//	struct SSPart::Impl;
	//	�p�r: SSPart�N���X���C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SSPart::Impl{
		wstring	m_SsaeName;
		IXMLDOMNodePtr m_ScanNode;
		IXMLDOMNodePtr m_TgtNode;

		//���b�V��
		shared_ptr<MeshResource> m_SquareRes;
		//�e�N�X�`��
		shared_ptr<TextureResource> m_TextureResource;

		wstring	name;
		int arrayIndex;
		int parentIndex;
		SsPartType::_enum type;
		SsBoundsType::_enum boundsType;
		SsInheritType::_enum inheritType;
		struct ineheritRatesTag{
			bool flph;	//FLPH ���E���](�Z���̌��_�����ɂ���)
			bool flpv;	//FLPV �㉺���](�Z���̌��_�����ɂ���)
			bool hide;	//HIDE ��\��
			bool iflh;	//IFLH �C���[�W���E���](��ɃC���[�W�̒��������_�Ƃ���)
			bool iflv;	//IFLV �C���[�W���E���](��ɃC���[�W�̒��������_�Ƃ���)
		};
		ineheritRatesTag ineheritRates;
		SsBlendType::_enum alphaBlendType;
		bool show;
		bool locked;
		bool expandAttribute;
		bool expandChildren;
		//�Z���}�b�v�̔z��
		vector< shared_ptr<SS5ssce> >& m_SS5sscePtrVec;
		size_t m_TgtSS5ssceIndex;
		//�Ή�����Z��
		shared_ptr<SS5Cell> m_SS5CellPtr;
		//�e�p�[�c(this�|�C���^���g���̂Ő��|�C���^���g��)
		weak_ptr<SSPart> m_Parent;
		//SS5ssae�N���X
		weak_ptr<SS5ssae> m_SS5ssae;
		//�q�p�[�c�̔z��
		vector< shared_ptr<SSPart> > m_Childlen;
		//���݂̑Ή�����A�j���[�V����
		shared_ptr<SS5PartAnimation> m_PartAnime;
		//����Hide��Ԃ��ǂ���
		bool m_Hide;
		//���݂̗D��
		float m_Prio;
		//�A���t�@�l
		float m_Alpha;

		//�X�v���C�g���ǂ���
		bool m_SpriteType;

		//���_�ύX���̃f�[�^
		Vertex2DAnimeData m_Vertex2DAnimeData;
		//�\�z�Ɣj��
		Impl(const wstring& SsaeName, vector< shared_ptr<SS5ssce> >& SS5sscePtrVec, 
			IXMLDOMNodePtr ScanNode, IXMLDOMNodePtr TgtNode, const shared_ptr<SSPart>& Parent, bool SpriteType);
		~Impl(){}
		//����
		bsm::Mat4x4 CaluclateMatrix();
	};
	//�\�z�Ɣj��
	SSPart::Impl::Impl(const wstring& SsaeName, 
		vector< shared_ptr<SS5ssce> >& SS5sscePtrVec,
		IXMLDOMNodePtr ScanNode,IXMLDOMNodePtr TgtNode,
		const shared_ptr<SSPart>& Parent, bool SpriteType) :
		m_SsaeName(SsaeName),
		m_SS5sscePtrVec(SS5sscePtrVec),
		m_ScanNode(ScanNode),
		m_TgtNode(TgtNode),
		m_TgtSS5ssceIndex(0),
		m_SS5CellPtr(nullptr),
		m_Parent(Parent),
		m_Hide(false),
		m_Prio(0),
		m_Alpha(1.0f),
		m_SpriteType(SpriteType)
	{
		try{
			name = SS5Util::TextToWstr(TgtNode, L"name");
			arrayIndex = SS5Util::TextToInt(TgtNode, L"arrayIndex");
			parentIndex = SS5Util::TextToInt(TgtNode, L"parentIndex");
			type = SsPartType::ws2e(SS5Util::TextToWstr(TgtNode, L"type"));
			boundsType = SsBoundsType::ws2e(SS5Util::TextToWstr(TgtNode, L"boundsType"));
			inheritType = SsInheritType::ws2e(SS5Util::TextToWstr(TgtNode, L"inheritType"));
			auto ineheritRatesNodes = XmlDocReader::GetSelectNodes(TgtNode, L"ineheritRates/*");
			long lCountNode = XmlDocReader::GetLength(ineheritRatesNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(ineheritRatesNodes, i);
				if (pXMLDOMNode){
					wstring Name = XmlDocReader::GetBaseName(pXMLDOMNode);
					SsAttributeKind::_enum key = SsAttributeKind::ws2e(Name);
					switch (key){
					case SsAttributeKind::fliph:
						ineheritRates.flph = SS5Util::TextToBool(pXMLDOMNode, Name);
						break;
					case SsAttributeKind::flipv:
						ineheritRates.flpv = SS5Util::TextToBool(pXMLDOMNode, Name);
						break;
					case SsAttributeKind::hide:
						ineheritRates.hide = SS5Util::TextToBool(pXMLDOMNode, Name);
						break;
					case SsAttributeKind::imgfliph:
						ineheritRates.iflh = SS5Util::TextToBool(pXMLDOMNode, Name);
						break;
					case SsAttributeKind::imgflipv:
						ineheritRates.iflv = SS5Util::TextToBool(pXMLDOMNode, Name);
						break;
					}
				}
			}
			alphaBlendType = SsBlendType::ws2e(SS5Util::TextToWstr(TgtNode, L"alphaBlendType"));
			show = SS5Util::TextToBool(TgtNode, L"show");
			locked = SS5Util::TextToBool(TgtNode, L"locked");
			expandAttribute = SS5Util::TextToBool(TgtNode, L"expandAttribute");
			expandChildren = SS5Util::TextToBool(TgtNode, L"expandChildren");

		}
		catch (...){
			throw;
		}
	}

	//�A�j���[�V������̍s����v�Z����
	bsm::Mat4x4 SSPart::Impl::CaluclateMatrix(){
		bsm::Mat4x4 ret;
		ret.identity();
		if (m_PartAnime){
			m_Prio = -m_PartAnime->GetPrio();
			m_Hide = m_PartAnime->GetHide();
			m_Alpha = m_PartAnime->GetAlpha();
			m_Vertex2DAnimeData = m_PartAnime->GetVirtex2DData();

			AnimeCell TgtCell = m_PartAnime->GetCell();
			if (TgtCell.name != L""){
				m_TgtSS5ssceIndex = TgtCell.id;
				m_SS5CellPtr = m_SS5sscePtrVec[m_TgtSS5ssceIndex]->GetCell(TgtCell.name);
			}
			else{
				m_TgtSS5ssceIndex = 0;
				m_SS5CellPtr.reset();
			}
			ret = m_PartAnime->GetLocalMatrix();
			if (!m_Parent.expired()){
				auto ParPtr = m_Parent.lock();
				auto PtrT = ParPtr->GetComponent<Transform>();
				ret *= PtrT->GetWorldMatrix();
			}
			else{
				if (!m_SS5ssae.expired()){
					auto SS5ssaePtr = m_SS5ssae.lock();
					ret *= SS5ssaePtr->GetToAnimeMatrix();
					auto mat = SS5ssaePtr->GetComponent<Transform>()->GetWorldMatrix();
					ret *= mat;
				}
				else{
					throw BaseException(
						L"Ssae���擾�ł��܂���",
						L"if (m_SS5ssae.expired())",
						L"SSPart::Impl::CaluclateMatrix()"
						);

				}
			}
		}
		return ret;
	}



	//--------------------------------------------------------------------------------------
	//	class SSPart : public  GameObject;
	//	�p�r: SSPart�N���X(�e�p�[�c)
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	SSPart::SSPart(const shared_ptr<Stage>& StagePtr, const wstring& SsaeName, vector< shared_ptr<SS5ssce> >& SS5sscePtrVec,
		IXMLDOMNodePtr ScanNode, IXMLDOMNodePtr TgtNode, const shared_ptr<SSPart>& Parent, bool SpriteType) :
		GameObject(StagePtr),
		pImpl(new Impl(SsaeName, SS5sscePtrVec, ScanNode, TgtNode, Parent, SpriteType))
	{}
	SSPart::~SSPart(){}
	//������
	void SSPart::OnCreate(){
		try{
			AddComponent<Transform>();
			//���_��ύX�ł���悤�ɂ���̂Ńf�t�H���g�̃��\�[�X�͎g���Ȃ�
			wstring ResName = pImpl->m_SsaeName + L"_" + pImpl->name;
			//���\�[�X�쐬�B�ʂ̃C���X�^���X�ł��������\�[�X���g��
			if (!App::GetApp()->CheckResource<MeshResource>(ResName)){
				//���_�z��
				vector<VertexPositionNormalTexture> vertices;
				//�C���f�b�N�X���쐬���邽�߂̔z��
				vector<uint16_t> indices;
				//Square�̍쐬(�w���p�[�֐��𗘗p)
				MeshUtill::CreateSquare(1.0f, vertices, indices);
				//���_�̌^��ς����V�������_���쐬
				vector<VertexPositionColorTexture> new_vertices;
				for (auto& v : vertices){
					VertexPositionColorTexture nv;
					nv.position = v.position;
					nv.color = bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f);
					nv.textureCoordinate = v.textureCoordinate;
					new_vertices.push_back(nv);
				}
				//�V�������_���g���ă��b�V�����\�[�X�̍쐬
				pImpl->m_SquareRes = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, indices, true);
				//���\�[�X�̓o�^
				App::GetApp()->RegisterResource(ResName, pImpl->m_SquareRes);
			}
			else{
				pImpl->m_SquareRes = App::GetApp()->GetResource<MeshResource>(ResName);
			}
			wstring Query = L"value[parentIndex/text()=" + SS5Util::TextToWstr(pImpl->m_TgtNode, L"arrayIndex") + L"]";
			auto ChildNodes = XmlDocReader::GetSelectNodes(pImpl->m_ScanNode, Query.c_str());
			long lCountNode = XmlDocReader::GetLength(ChildNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(ChildNodes, i);
				if (pXMLDOMNode){
					auto ChildPtr = ObjectFactory::Create<SSPart>(GetStage(),pImpl->m_SsaeName, pImpl->m_SS5sscePtrVec, 
						pImpl->m_ScanNode, pXMLDOMNode, GetThis<SSPart>(),pImpl->m_SpriteType);
					pImpl->m_Childlen.push_back(ChildPtr);
				}
			}
			//�X�N�G�A�p
			auto PtrDraw = AddComponent<PCTStaticDraw>();
			//�X�v���C�g�p
			auto PtrSprite = AddComponent<PCTSpriteDraw>();
			//��������������
			SetAlphaActive(true);
		}
		catch (...){
			throw;
		}
	}

	int SSPart::get_arrayIndex(){ 
		return pImpl->arrayIndex; 
	}
	float SSPart::GetPrio(){ return pImpl->m_Prio; }

	void SSPart::SetSS5ssae(const shared_ptr<SS5ssae>& pSS5ssae){
		pImpl->m_SS5ssae = pSS5ssae;
		//�q�����ݒ�
		for (auto ptr : pImpl->m_Childlen){
			ptr->SetSS5ssae(pSS5ssae);
		}
	}

	//�X�v���C�g���ǂ���(Getter�̂�)
	bool SSPart::GetSpriteType() const{
		return pImpl->m_SpriteType;
	}
	bool SSPart::IsSpriteType() const{
		return pImpl->m_SpriteType;
	}


	//�A�j���[�V������ύX����
	void SSPart::ChangeAnimation(shared_ptr<SS5Animation> AnimationPtr){
		//�q���z��A�j���[�V������ύX����
		for (auto ptr : pImpl->m_Childlen){
			ptr->ChangeAnimation(AnimationPtr);
		}
		auto& Vec = AnimationPtr->get_PartAnimationVec();
		for (auto& PartAnime : Vec){
			if (PartAnime->GetpartName() == pImpl->name){
				//���f������������
				pImpl->m_PartAnime = PartAnime;
				return;
			}
		}
		//������Ȃ�
		pImpl->m_PartAnime.reset();
	}

	//���_�̕ύX
	void SSPart::ResetVirtex(){

#if BASECROSS_DXVERSION == 11

		//���W��ύX����
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//���_�o�b�t�@�����\�[�X������o��
		auto pVertexBuffer = pImpl->m_SquareRes->GetVertexBuffer().Get();
		//�o�b�N�A�b�v�̒��_�����o��
		vector<VertexPositionColorTexture>& BacukVertices = pImpl->m_SquareRes->GetBackupVerteces<VertexPositionColorTexture>();
		//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//���_�̃}�b�v
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map���s
			throw BaseException(
				L"���_��Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"SSPart::ResetVirtex()"
				);
		}
		//UV�l�̕ύX
		VertexPositionColorTexture* vertices = (VertexPositionColorTexture*)mappedBuffer.pData;
		Rect2D<float> UVRectBase(0, 0, pImpl->m_SS5CellPtr->get_size().x, pImpl->m_SS5CellPtr->get_size().y);
		UVRectBase += pImpl->m_SS5CellPtr->get_pos();

		Rect2D<float> UVRect(
			UVRectBase.left / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().x,
			UVRectBase.top / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().y,
			UVRectBase.right / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().x,
			UVRectBase.bottom / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().y
			);

		float grid = 32.0f;
		if (pImpl->m_PartAnime){
			grid = pImpl->m_PartAnime->GetGridSize();
		}
		float CellWidth = pImpl->m_SS5CellPtr->get_size().x / grid;
		float CelHeight = pImpl->m_SS5CellPtr->get_size().y / grid;

		float pivotx = pImpl->m_SS5CellPtr->get_size().x * pImpl->m_SS5CellPtr->get_pivot().x / grid;
		float pivoty = pImpl->m_SS5CellPtr->get_size().y * pImpl->m_SS5CellPtr->get_pivot().y / grid;

		Point2D<float> Pivot(pivotx, pivoty);


		Rect2D<float> CellRect(-CellWidth / 2.0f, CelHeight / 2.0f, CellWidth / 2.0f, -CelHeight / 2.0f);


		CellRect -= Pivot;

		vertices[0] = VertexPositionColorTexture(
			bsm::Vec3(
			CellRect.left + pImpl->m_Vertex2DAnimeData.LeftTop.x / grid,
			CellRect.top + pImpl->m_Vertex2DAnimeData.LeftTop.y / grid,
			0),
			bsm::Col4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			bsm::Vec2(UVRect.left, UVRect.top));

		vertices[1] = VertexPositionColorTexture(
			bsm::Vec3(
			CellRect.right + pImpl->m_Vertex2DAnimeData.RightTop.x / grid,
			CellRect.top + pImpl->m_Vertex2DAnimeData.RightTop.y / grid,
			0), 
			bsm::Col4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			bsm::Vec2(UVRect.right, UVRect.top));

		vertices[2] = VertexPositionColorTexture(
			bsm::Vec3(
			CellRect.left + pImpl->m_Vertex2DAnimeData.LeftBottom.x / grid,
			CellRect.bottom + +pImpl->m_Vertex2DAnimeData.LeftBottom.y / grid,
			0), 
			bsm::Col4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			bsm::Vec2(UVRect.left, UVRect.bottom));
		vertices[3] = VertexPositionColorTexture(
			bsm::Vec3(
			CellRect.right + pImpl->m_Vertex2DAnimeData.RightBottom.x / grid,
			CellRect.bottom + pImpl->m_Vertex2DAnimeData.RightBottom.y / grid,
			0), 
			bsm::Col4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			bsm::Vec2(UVRect.right, UVRect.bottom));
		//�A���}�b�v
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);
#endif
	}

	void SSPart::ResetSpriteVirtex(){

#if BASECROSS_DXVERSION == 11

		vector<VertexPositionColorTexture> m_VertexVec;
		//���_�̕ύX
		Rect2D<float> UVRectBase(0, 0, pImpl->m_SS5CellPtr->get_size().x, pImpl->m_SS5CellPtr->get_size().y);
		UVRectBase += pImpl->m_SS5CellPtr->get_pos();
		Rect2D<float> UVRect(
			UVRectBase.left / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().x,
			UVRectBase.top / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().y,
			UVRectBase.right / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().x,
			UVRectBase.bottom / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().y
			);

		float grid = 32.0f;
		if (pImpl->m_PartAnime){
			grid = pImpl->m_PartAnime->GetGridSize();
		}
		float CellWidth = pImpl->m_SS5CellPtr->get_size().x / grid;
		float CelHeight = pImpl->m_SS5CellPtr->get_size().y / grid;

		float pivotx = pImpl->m_SS5CellPtr->get_size().x * pImpl->m_SS5CellPtr->get_pivot().x / grid;
		float pivoty = pImpl->m_SS5CellPtr->get_size().y * pImpl->m_SS5CellPtr->get_pivot().y / grid;
		Point2D<float> Pivot(pivotx, pivoty);
		Rect2D<float> CellRect(-CellWidth / 2.0f, CelHeight / 2.0f, CellWidth / 2.0f, -CelHeight / 2.0f);
		CellRect -= Pivot;

		m_VertexVec.push_back(
			VertexPositionColorTexture(
			bsm::Vec3(CellRect.left + pImpl->m_Vertex2DAnimeData.LeftTop.x / grid,CellRect.top + pImpl->m_Vertex2DAnimeData.LeftTop.y / grid,0),
			bsm::Col4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			bsm::Vec2(UVRect.left, UVRect.top)
			)
		);
		m_VertexVec.push_back(
			VertexPositionColorTexture(
			bsm::Vec3(CellRect.right + pImpl->m_Vertex2DAnimeData.RightTop.x / grid,CellRect.top + pImpl->m_Vertex2DAnimeData.RightTop.y / grid,0),
			bsm::Col4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			bsm::Vec2(UVRect.right, UVRect.top)
			)
			);

		m_VertexVec.push_back(
			VertexPositionColorTexture(
			bsm::Vec3(CellRect.left + pImpl->m_Vertex2DAnimeData.LeftBottom.x / grid,CellRect.bottom + +pImpl->m_Vertex2DAnimeData.LeftBottom.y / grid,0),
			bsm::Col4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			bsm::Vec2(UVRect.left, UVRect.bottom)
			)
			);

		m_VertexVec.push_back(
			VertexPositionColorTexture(
			bsm::Vec3(CellRect.right + pImpl->m_Vertex2DAnimeData.RightBottom.x / grid,CellRect.bottom + pImpl->m_Vertex2DAnimeData.RightBottom.y / grid,0),
			bsm::Col4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			bsm::Vec2(UVRect.right, UVRect.bottom)
			)
			);

		auto PtrSprite = GetComponent<PCTSpriteDraw>();
		auto Mesh = PtrSprite->GetMeshResource();
		Mesh->UpdateVirtexBuffer(m_VertexVec);

#endif

	}

	//�A�j���[�V������̍s����v�Z����
	void SSPart::CaluclateMatrix(){
		bsm::Mat4x4 ret = pImpl->CaluclateMatrix();
		if (pImpl->m_SS5CellPtr){
			//���_�̕ύX�iUV�̕ύX�j
			ResetVirtex();
			pImpl->m_TextureResource = pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->GetTexture();
			ResetSpriteVirtex();
		}
		auto PtrT = GetComponent<Transform>();
		PtrT->SetScale(ret.scaleInMatrix());
		PtrT->SetQuaternion(ret.quatInMatrix());
		PtrT->SetWorldPosition(ret.transInMatrix());
		//�q���̍s����v�Z����
		for (auto ptr : pImpl->m_Childlen){
			ptr->CaluclateMatrix();
		}
	}

	void SSPart::SetPartInVector(vector<SSPart*>& PartVec){
		PartVec.push_back(this);
		for (auto ptr : pImpl->m_Childlen){
			ptr->SetPartInVector(PartVec);
		}
	}

	void SSPart::OnDraw(){
		if (pImpl->type == SsPartType::normal && pImpl->m_SS5CellPtr){
			if (!pImpl->m_Hide && pImpl->show){
				//�`�悷��
				if (pImpl->m_SpriteType){
					auto PtrSprite = GetComponent<PCTSpriteDraw>();
					PtrSprite->SetTextureResource(pImpl->m_TextureResource);
					PtrSprite->OnDraw();
				}
				else{
					auto PtrDraw = GetComponent<PCTStaticDraw>();
					PtrDraw->SetMeshResource(pImpl->m_SquareRes);
					PtrDraw->SetTextureResource(pImpl->m_TextureResource);
					PtrDraw->OnDraw();
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct SS5ssae::Impl;
	//	�p�r: SS5ssae�N���X���C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SS5ssae::Impl{
		wstring m_StartAnimeName;


		wstring	basedir;			///< ��f�B���N�g���i\���Ō�ɂ��j
		wstring	fname;				///< �A�j���[�V�����p�b�N�̃t�@�C���l�[��
		wstring	name;
		float fps;
		float frameCount;
		SsPartsSortMode::_enum sortMode;
		Point2D<float> canvasSize;
		Point2D<float> pivot;
		bsm::Col4 bgColor;
		float gridSize;
		bsm::Col4 gridColor;
		int outStartNum;
		//�����Z���}�b�v
		vector< shared_ptr<SS5ssce> > m_SS5ssces;
		//���[�g�̃p�[�c
		shared_ptr<SSPart> m_RootPart;
		//�A�j���[�V����
		map<wstring, shared_ptr<SS5Animation> > m_AnimeMap;
		//���݂̃A�j���[�V�����i���O�j
		wstring m_NowAnimation;
		//���݂̃A�j���[�V�����i�|�C���^�j
		shared_ptr<SS5Animation> m_NowAnimationPtr;
		//SS5ssae����A�j���[�V�����I�u�W�F�N�g�ւ̍s��
		bsm::Mat4x4 m_ToAnimeMatrix;
		//�X�v���C�g���ǂ���
		bool m_SpriteType;

		//���C�e�B���O���Ȃ����ǂ����i�f�t�H���gtrue�j
		bool m_TextureOnlyNoLight;

		//�\�z�Ɣj��
		Impl(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir, const wstring& Xmlfilename, const wstring& StartAnimeName,bool SpriteType);
		~Impl(){}
		//����
		void ChangeAnimation(const wstring& AnimationName, float AnimeTime);
		void UpdateAnimeTime(float TimeSpan);

	};

	SS5ssae::Impl::Impl(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir, const wstring& Xmlfilename, const wstring& StartAnimeName, bool SpriteType) :
		basedir(BaseDir),
		fname(Xmlfilename),
		m_StartAnimeName(StartAnimeName),
		m_NowAnimation(L""),
		m_NowAnimationPtr(nullptr),
		m_ToAnimeMatrix(),
		m_SpriteType(SpriteType),
		m_TextureOnlyNoLight(true)
	{
		try{

			//�C���X�^���X�i���o�[
			//�X�^�e�B�b�N�ϐ�
			static size_t stInstansNumber = 0;

			XmlDocReader doc(basedir + fname);
			//�x�[�X�f�[�^
			auto BaseSetting = doc.GetSelectSingleNode(L"SpriteStudioAnimePack");
			if (!BaseSetting){
				throw BaseException(
					L"�A�j���p�b�N��������܂���",
					fname,
					L"SS5ssae::Impl::Impl()"
					);
			}
			name = SS5Util::TextToWstr(BaseSetting, L"name") + Util::SizeTToWStr(stInstansNumber);
			//�C���X�^���X�i���o�[�̃C���N�������g
			stInstansNumber++;

			//��{�f�[�^
			auto Setting = doc.GetSelectSingleNode(L"SpriteStudioAnimePack/settings");
			if (!Setting){
				throw BaseException(
					L"settings��������܂���",
					fname,
					L"SS5ssae::Impl::Impl()"
					);
			}

			fps = SS5Util::TextToFloat(Setting, L"fps");
			frameCount = SS5Util::TextToFloat(Setting, L"frameCount");
			sortMode = SsPartsSortMode::ws2e(SS5Util::TextToWstr(Setting, L"sortMode"));
			canvasSize = SS5Util::TextToPoint2DF(Setting, L"canvasSize");
			pivot = SS5Util::TextToPoint2DF(Setting, L"pivot");
			bgColor = SS5Util::TextARGBToColor4(Setting, L"bgColor");
			gridSize = SS5Util::TextToFloat(Setting, L"gridSize");
			gridColor = SS5Util::TextARGBToColor4(Setting, L"gridColor");
			outStartNum = SS5Util::TextToInt(Setting, L"outStartNum");
			//�Z���}�b�v�̓ǂݍ���
			//�����Z���t�@�C���ւ̑Ή�
			auto CellMaps = doc.GetSelectNodes(L"SpriteStudioAnimePack/cellmapNames/value");
			if (!CellMaps){
				throw BaseException(
					L"�Z���}�b�v��������܂���",
					L"SpriteStudioAnimePack/cellmapNames",
					L"SS5ssae::Impl::Impl()"
					);
			}
			long lCountNode = XmlDocReader::GetLength(CellMaps);
			if (lCountNode <= 0){
				throw BaseException(
					L"�Z���}�b�v��������܂���",
					L"if(lCountNode <= 0)",
					L"SS5ssae::Impl::Impl()"
					);
			}
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(CellMaps, i);
				wstring SsceFilename = XmlDocReader::GetText(pXMLDOMNode);
				if (SsceFilename != L""){
					//�Z���}�b�v�̍쐬�Ɛݒu
					auto ssce = make_shared<SS5ssce>(BaseDir, SsceFilename);
					m_SS5ssces.push_back(ssce);
				}
				else{
					throw BaseException(
						L"�Z���}�b�v�����󔒂ł�",
						L"SpriteStudioAnimePack/cellmapNames",
						L"SS5ssae::Impl::Impl()"
						);
				}
			}
			auto PartsRootNode = doc.GetSelectSingleNode(L"SpriteStudioAnimePack/Model/partList");
			if (!PartsRootNode){
				throw BaseException(
					L"�p�[�c��������܂���",
					fname,
					L"SS5ssae::Impl::Impl()"
					);
			}
			//���[�g�̃m�[�h��T��
			auto RootNode = XmlDocReader::GetSelectSingleNode(PartsRootNode, L"value[parentIndex/text()=-1]");
			if (!RootNode){
				throw BaseException(
					L"�p�[�c��root��������܂���",
					fname,
					L"SS5ssae::Impl::Impl()"
					);
			}
			//�p�[�c�̓ǂݍ���
			m_RootPart = ObjectFactory::Create<SSPart>(StagePtr, name, m_SS5ssces, PartsRootNode, RootNode, nullptr, SpriteType);
			//�A�j���[�V�����̓ǂݍ���
			auto Animations = doc.GetSelectNodes(L"SpriteStudioAnimePack/animeList/anime");
			if (!Animations){
				throw BaseException(
					L"�A�j���[�V������������܂���",
					L"SpriteStudioAnimePack/animeList/anime",
					L"SS5ssae::Impl::Impl()"
					);
			}

			lCountNode = XmlDocReader::GetLength(Animations);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(Animations, i);
				if (pXMLDOMNode){
					auto ptr = make_shared<SS5Animation>(pXMLDOMNode);
					m_AnimeMap[ptr->get_name()] = ptr;
				}
			}



		}
		catch (...){
			throw;
		}
	}
	void SS5ssae::Impl::ChangeAnimation(const wstring& AnimationName, float AnimeTime){
		try{
			if (AnimationName == L""){
				throw BaseException(
					L"�A�j���[�V���������󔒂ł�",
					L"null",
					L"SS5ssae::Impl::ChangeAnimation()"
					);
			}
			if (m_NowAnimation == AnimationName){
				//�������O�Ȃ�ύX���Ȃ�
				//�^�C�}�[�̂ݕύX
				m_NowAnimationPtr->SetCurrentTime(AnimeTime);
				return;
			}
			map<wstring, shared_ptr<SS5Animation> >::const_iterator it;
			it = m_AnimeMap.find(AnimationName);
			if (it != m_AnimeMap.end()) {
				m_NowAnimation = AnimationName;
				m_NowAnimationPtr = it->second;
				m_NowAnimationPtr->SetCurrentTime(AnimeTime);
				//�p�[�c�̃A�j���[�V������ύX����
				m_RootPart->ChangeAnimation(m_NowAnimationPtr);
			}
			else{
				throw BaseException(
					L"�w��̃A�j���[�V������������܂���",
					AnimationName,
					L"SS5ssae::Impl::ChangeAnimation()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	void SS5ssae::Impl::UpdateAnimeTime(float TimeSpan){
		//�ŏ��ɃA�j���[�V�����^�C�����X�V����
		m_NowAnimationPtr->AddCurrentTime(TimeSpan);
		//�����ă��f���̍s����v�Z����
		m_RootPart->CaluclateMatrix();
	}

	//--------------------------------------------------------------------------------------
	//	class SS5ssae :  public GameObject;
	//	�p�r: SpriteStdio��ssae�N���X�i�A�j���[�V�����p�b�N�j
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	SS5ssae::SS5ssae(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir,
		const wstring& Xmlfilename, const wstring& StartAnimeName, bool SpriteType) :
		GameObject(StagePtr),
		pImpl(new Impl(StagePtr, BaseDir, Xmlfilename, StartAnimeName, SpriteType))
	{}
	SS5ssae::~SS5ssae(){}
	//������
	void SS5ssae::OnCreate(){
		AddComponent<Transform>();
		pImpl->m_RootPart->SetSS5ssae(GetThis<SS5ssae>());
		//�ŏ��̃A�j���[�V�����̐ݒu
		ChangeAnimation(pImpl->m_StartAnimeName);
		SetAlphaActive(true);
		if (IsSpriteType()){
			//�X�v���C�g�Ƃ��ĕ`�悷��
			SetSpriteDraw(true);
		}
	}


	//���ݑI������Ă���A�j���[�V���������[�v���邩�ǂ����𓾂�
	bool SS5ssae::IsLooped() const{
		if (pImpl->m_NowAnimationPtr){
			return pImpl->m_NowAnimationPtr->IsLooped();
		}
		else{
			return false;
		}
	}
	bool SS5ssae::GetLooped() const{
		return IsLooped();
	}
	//���ݑI������Ă���A�j���[�V���������[�v���邩�ǂ�����ݒ肷��
	void SS5ssae::SetLooped(bool b){
		if (pImpl->m_NowAnimationPtr){
			return pImpl->m_NowAnimationPtr->SetLooped(b);
		}
	}
	//���ݑI������Ă���A�j���[�V�������I���������ǂ���
	bool SS5ssae::IsAnimeEnd() const{
		if (pImpl->m_NowAnimationPtr){
			return pImpl->m_NowAnimationPtr->IsAnimeEnd();
		}
		return false;
	}



	void SS5ssae::ChangeAnimation(const wstring& AnimationName, float AnimeTime){
		pImpl->ChangeAnimation(AnimationName, AnimeTime);
	}

	void SS5ssae::UpdateAnimeTime(float TimeSpan){
		pImpl->UpdateAnimeTime(TimeSpan);
	}

	//fps�i�Đ��X�s�[�h�j��ݒ肷��
	void SS5ssae::SetFps(float f){
		if (pImpl->m_NowAnimationPtr){
			pImpl->m_NowAnimationPtr->set_fps(f);
		}
	}
	//SS5ssae����A�j���[�V�����I�u�W�F�N�g�ւ̍s��
	const bsm::Mat4x4& SS5ssae::GetToAnimeMatrix() const{
		return pImpl->m_ToAnimeMatrix;
	}
	void SS5ssae::SetToAnimeMatrix(const bsm::Mat4x4& mat){
		pImpl->m_ToAnimeMatrix = mat;
	}

	//�X�v���C�g���ǂ���
	bool SS5ssae::GetSpriteType() const{
		return pImpl->m_SpriteType;
	}
	bool SS5ssae::IsSpriteType() const{
		return pImpl->m_SpriteType;
	}

	void SS5ssae::SetTextureOnlyNoLight(bool b){
		pImpl->m_TextureOnlyNoLight = b;
	}
	bool SS5ssae::GetTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}

	bool SS5ssae::IsTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}

	const shared_ptr<SSPart>&  SS5ssae::GetRootPart() const {
		return pImpl->m_RootPart;
	}

	//���ׂẴp�[�c�̃f�t���[�Y��ύX����
	void SS5ssae::SetAllPartsDiffuse(const bsm::Col4& c) {
		//�p�[�c�̃f�t���[�Y�ύX
		auto& rootParts = GetRootPart();
		vector<SSPart*> PartVec;
		rootParts->SetPartInVector(PartVec);
		for (auto ptr : PartVec) {
			if (IsSpriteType()) {
				auto DComp = ptr->GetComponent<PCTSpriteDraw>();
				DComp->SetDiffuse(c);
			}
			else {
				auto DComp = ptr->GetComponent<PCTStaticDraw>();
				DComp->SetDiffuse(c);
			}
		}
	}

	void SS5ssae::SetAllPartsEmissive(const bsm::Col4& c) {
		//�p�[�c�̃G�~�b�V�u�ύX
		auto& rootParts = GetRootPart();
		vector<SSPart*> PartVec;
		rootParts->SetPartInVector(PartVec);
		for (auto ptr : PartVec) {
			if (IsSpriteType()) {
				auto DComp = ptr->GetComponent<PCTSpriteDraw>();
				DComp->SetEmissive(c);
			}
			else {
				auto DComp = ptr->GetComponent<PCTStaticDraw>();
				DComp->SetEmissive(c);
			}
		}

	}




	bool ZSortModelFunc(SSPart* rLeft, SSPart* rRight){
		if (rLeft->GetPrio() == rRight->GetPrio()){
			return rLeft->get_arrayIndex() < rRight->get_arrayIndex();
		}
		return rLeft->GetPrio() > rRight->GetPrio();
	}

	void SS5ssae::OnDraw(){
		ComponentDraw();
		vector<SSPart*> PartVec;
		pImpl->m_RootPart->SetPartInVector(PartVec);
		std::sort(PartVec.begin(), PartVec.end(), ZSortModelFunc);
		if (IsSpriteType()){
			for (auto ptr : PartVec){
				ptr->OnDraw();
			}
		}
		else{
			float count = 0;
			for (auto ptr : PartVec){
				auto PtrT = ptr->GetComponent<Transform>();
				auto TempPos = PtrT->GetWorldPosition();
				auto Pos = PtrT->GetWorldPosition();
				bsm::Vec3 Z(0, 0, count);
				Pos += Z;
				PtrT->SetWorldPosition(Pos);
				ptr->OnDraw();
				PtrT->SetWorldPosition(TempPos);
				count -= 0.001f;
			}
		}
	}

}
//endof  basecross
