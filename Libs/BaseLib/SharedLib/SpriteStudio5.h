/*!
@file SpriteStudio5.h
@brief �X�v���C�g�X�^�W�I
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//---------------------------------------------------------------
	/// �\�[�g���[�h
	struct SsPartsSortMode
	{
		enum _enum
		{
			invalid = -1,
			prio,			///< �`�揇�͗D��x�Ő��䂷��B�D��x��\�����A�y���W���B���B
			z,				///< �`�揇�͂y���W�Ő��䂷��B�y���W��\�����A�D��x���B���B
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"prio") return prio;
			if (s == L"z") return z;
			return invalid;
		}
	};

	//---------------------------------------------------------------
	/// Animation Part Type
	struct SsPartType
	{
		enum _enum
		{
			invalid = -1,
			null,			///< null�B�̈��������SRT���̂݁B�������~�`�̓����蔻��͐ݒ�\�B
			normal,			///< �ʏ�p�[�c�B�̈�����B�摜�͖����Ă������B
			text,			///< �e�L�X�g(�\��@�������j
			instance,		///< �C���X�^���X�B���A�j���A�p�[�c�ւ̎Q�ƁB�V�[���ҏW���[�h�̑�ւɂȂ����
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"null") return null;
			if (s == L"normal") return normal;
			if (s == L"text") return text;
			if (s == L"instance") return instance;
			return invalid;
		}
	};


	//---------------------------------------------------------------
	/// �����蔻��`��
	struct SsBoundsType
	{
		enum _enum
		{
			invalid = -1,
			none,			///< �����蔻��Ƃ��Ďg��Ȃ��B
			quad,			///< ���݂ɕό`����l�ӌ`�B���_�ό`�ȂǓK�p��̂S�p�����񂾗̈�B�ł��d���B
			aabb,			///< ��]���Ȃ��S�̂��͂ދ�`�Ō�������
			circle,			///< �^�~�̔��a�ŋ����ɂ�蔻�肷��
			circle_smin,	///< �^�~�̔��a�ŋ����ɂ�蔻�肷�� (�X�P�[����x,y�̍ŏ��l���Ƃ�j
			circle_smax,	///< �^�~�̔��a�ŋ����ɂ�蔻�肷�� (�X�P�[����x,y�̍ő�l���Ƃ�j
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"none") return none;
			if (s == L"quad") return quad;
			if (s == L"aabb") return aabb;
			if (s == L"circle") return circle;
			if (s == L"circle_smin") return circle_smin;
			if (s == L"circle_smax") return circle_smax;
			return invalid;
		}
	};


	//---------------------------------------------------------------
	/// �p���^�C�v
	struct SsInheritType
	{
		enum _enum
		{
			invalid = -1,
			parent,			///< �e�̌p�����@�����̂܂܈����p��
			self,			///< ���g���A�g���r���[�g�ʂɎ��p�����@���g��
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"parent") return parent;
			if (s == L"self") return self;
			return invalid;
		}
	};

	//---------------------------------------------------------------
	/// �u�����h�^�C�v
	struct SsBlendType
	{
		enum _enum{
			invalid = -1,
			mix,			///< 0 �u�����h�i�~�b�N�X�j
			mul,			///< 1 ��Z
			add,			///< 2 ���Z
			sub,			///< 3 ���Z
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"mix") return mix;
			if (s == L"mul") return mul;
			if (s == L"add") return add;
			if (s == L"sub") return sub;
			return invalid;
		}
	};


	///�J���[�u�����h�L�[���g�p����Ă���ۂ̃J���[�K�p�͈͂̒�`
	struct SsColorBlendTarget
	{
		enum _enum{
			invalid = -1,
			whole,	///< �P�F�B�S�̂ɂ�����B
			vertex,	///< ���_�P��
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"whole") return whole;
			if (s == L"vertex") return vertex;
			return invalid;
		}
	};

	///��ԃ��[�h�̒�`
	struct SsInterpolationType
	{
		enum _enum
		{
			invalid = -1,
			none,			///< �Ȃ�
			linear,			///< ���`
			hermite,		///< �G���~�[�g
			bezier,			///< �x�W�F
			acceleration,	///< �����x
			deceleration,	///< �����x
			num,
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"none") return none;
			if (s == L"linear") return linear;
			if (s == L"hermite") return hermite;
			if (s == L"bezier") return bezier;
			if (s == L"acceleration") return acceleration;
			if (s == L"deceleration") return deceleration;
			return invalid;
		}
	};


	/// �e�N�X�`�����b�v���[�h
	struct SsTexWrapMode
	{
		enum _enum
		{
			invalid = -1,	/// �Ȃ�
			clamp,			/// �N�����v����
			repeat,			/// ���s�[�g
			mirror,			/// �~���[
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"clamp") return clamp;
			if (s == L"repeat") return repeat;
			if (s == L"mirror") return mirror;
			return invalid;
		}
	};

	/// �e�N�X�`���t�B���^�[���[�h ��f��ԕ��@
	struct SsTexFilterMode
	{
		enum _enum
		{
			invalid = -1,
			nearest,	///< �j�A���X�g�l�C�o�[
			linear,		///< ���j�A�A�o�C���j�A
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"nearest") return nearest;
			if (s == L"linear") return linear;
			return invalid;
		}
	};

	/// �A�g���r���[�g�̎��
	struct SsAttributeKind
	{
		enum _enum
		{
			invalid = -1,	///< �����l�B���f�[�^����̕ϊ����Ȃ�
			cell = 0,		///< [CELL]�Q�ƃZ��
			posx,		///< [POSX]�ʒu.X
			posy,		///< [POSY]�ʒu.Y
			posz,		///< [POSZ]�ʒu.Z
			rotx,		///< [ROTX]��].X
			roty,		///< [ROTY]��].Y
			rotz,		///< [ROTZ]��].Z
			sclx,		///< [SCLX]�X�P�[��.X
			scly,		///< [SCLY]�X�P�[��.Y
			alpha,		///< [ALPH]�s�����x
			prio,		///< [PRIO]�D��x
			fliph,		///< [FLPH]���E���](�Z���̌��_�����ɂ���)
			flipv,		///< [FLPV]�㉺���](�Z���̌��_�����ɂ���)
			hide,		///< [HIDE]��\��
			color,		///< [VCOL]�J���[�u�����h
			vertex,		///< [VERT]���_�ό`
			pivotx,		///< [PVTX]���_�I�t�Z�b�g.X
			pivoty,		///< [PVTY]���_�I�t�Z�b�g.Y
			anchorx,	///< [ANCX]�A���J�[�|�C���g.X
			anchory,	///< [ANCY]�A���J�[�|�C���g.Y
			sizex,		///< [SIZX]�\���T�C�Y.X
			sizey,		///< [SIZY]�\���T�C�Y.Y
			imgfliph,	///< [IFLH]�C���[�W���E���](��ɃC���[�W�̒��������_�Ƃ���)
			imgflipv,	///< [IFLV]�C���[�W�㉺���](��ɃC���[�W�̒��������_�Ƃ���)
			uvtx,		///< [UVTX]UV�A�j��.�ړ�.X
			uvty,		///< [UVTY]UV�A�j��.�ړ�.Y
			uvrz,		///< [UVRZ]UV�A�j��.��]
			uvsx,		///< [UVSX]UV�A�j��.�X�P�[��.X
			uvsy,		///< [UVSY]UV�A�j��.�X�P�[��.Y
			boundr,		///< [BNDR]�����蔻��p�̔��a
			user,		///< [USER]���[�U�[�f�[�^
			instance,	///< [IPRM]�C���X�^���X�p�[�c�p�����[�^
			num,
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"CELL") return cell;
			if (s == L"POSX") return posx;
			if (s == L"POSY") return posy;
			if (s == L"POSZ") return posz;
			if (s == L"ROTX") return rotx;
			if (s == L"ROTY") return roty;
			if (s == L"ROTZ") return rotz;
			if (s == L"SCLX") return sclx;
			if (s == L"SCLY") return scly;
			if (s == L"ALPH") return alpha;
			if (s == L"PRIO") return prio;
			if (s == L"FLPH") return fliph;
			if (s == L"FLPV") return flipv;
			if (s == L"HIDE") return hide;
			if (s == L"VCOL") return color;
			if (s == L"VERT") return vertex;
			if (s == L"PVTX") return pivotx;
			if (s == L"PVTY") return pivoty;
			if (s == L"ANCX") return anchorx;
			if (s == L"ANCY") return anchory;
			if (s == L"SIZX") return sizex;
			if (s == L"SIZY") return sizey;
			if (s == L"IFLH") return imgfliph;
			if (s == L"IFLV") return imgflipv;
			if (s == L"UVTX") return uvtx;
			if (s == L"UVTY") return uvty;
			if (s == L"UVRZ") return uvrz;
			if (s == L"UVSX") return uvsx;
			if (s == L"UVSY") return uvsy;
			if (s == L"BNDR") return boundr;
			if (s == L"USER") return user;
			if (s == L"IPRM") return instance;	//SDK�ɖ����B�����R�ꂩ�H
			return invalid;
		}
	};

	struct SsKeyValueType
	{
		enum _enum
		{
			_unkown = -1,
			_bool = 0,
			_float,
			_int,
			_string,
			_cellmap,
			_vertexAnime,
			_colorAnime,
			_userData,
		};
	};

	/// �Ȑ���Ԍv�Z�p�p�����[�^
	struct SsCurve
	{
		float	startTime;		///< �n�_�L�[�̎��Ԃ��琧��_�̎��Ԃւ̃I�t�Z�b�g�l�B�w���ɓ�����B
		float	startValue;		///< �n�_�L�[�̒l����	�V	�B�x��	�V
		float	endTime;		///< �I�_�L�[�̎��Ԃ��琧��_�̎��Ԃւ̃I�t�Z�b�g�l�B�w���ɓ�����B
		float	endValue;		///< �I�_�L�[�̒l����	�V	�B�x��	�V

		float	startKeyTime;	///< [���[�N�p�����[�^] �n�_�L�[�̎��� �v�Z���̂ݎg�p
		float	endKeyTime;		///< [���[�N�p�����[�^] �I�_�L�[�̎��� �v�Z���̂ݎg�p

		bool	syncStartEnd;	///< [�ҏW�p�p�����[�^]�J�[�u�G�f�B�^�ł̕ҏW���Ɏn�_�E�I�_�n���h���𓯊����ē��������H

		SsCurve() : startTime(0.f), startValue(0.f), endTime(0.f), endValue(0.f), startKeyTime(0.f), endKeyTime(0.f), syncStartEnd(false){}
		~SsCurve(){}

	};

	//--------------------------------------------------------------------------------------
	//	struct SS5Util;
	//	�p�r: SpriteStdio���[�e�B���e�B
	//--------------------------------------------------------------------------------------
	struct SS5Util{
		static bsm::Vec3 WstrToVector3(const wstring& src){
			bsm::Vec3 ret(0, 0, 0);
			vector<wstring> TokenVec;
			Util::WStrToTokenVector(TokenVec, src, L' ');
			if (TokenVec.size() >= 3){
				ret.x = (float)_wtof(TokenVec[0].c_str());
				ret.y = (float)_wtof(TokenVec[1].c_str());
				ret.z = (float)_wtof(TokenVec[2].c_str());
			}
			return ret;
		}

		static Point2D<float> WstrToPoint2DF(const wstring& src){
			Point2D<float> ret(0, 0);
			vector<wstring> TokenVec;
			Util::WStrToTokenVector(TokenVec, src, L' ');
			if (TokenVec.size() >= 2){
				ret.x = (float)_wtof(TokenVec[0].c_str());
				ret.y = (float)_wtof(TokenVec[1].c_str());
			}
			return ret;
		}
		static bsm::Col4 WstrARGBToColor4(const wstring& src){
			if (src.size() != 8){
				throw BaseException(
					L"�J���[���ɊԈႢ������܂�",
					src,
					L"SS5Util::WstrARGBTobsm::Col4()"
					);
			}
			wstring a = L"0x" + src.substr(0, 2);
			wstring r = L"0x" + src.substr(2, 2);
			wstring g = L"0x" + src.substr(4, 2);
			wstring b = L"0x" + src.substr(6, 2);


			bsm::Col4 ret(
				(float)wcstoul(r.c_str(), nullptr, 0) / 255.0f,
				(float)wcstoul(g.c_str(), nullptr, 0) / 255.0f,
				(float)wcstoul(b.c_str(), nullptr, 0) / 255.0f,
				(float)wcstoul(a.c_str(), nullptr, 0) / 255.0f
				);
			return ret;


		}
		//
		static wstring TextToWstr(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return XmlDocReader::GetText(pXMLDOMNode);
			}
			return wstring(L"");
		}

		static wstring AttributeToWstr(IXMLDOMNodePtr Node, const wstring& key){
			return XmlDocReader::GetAttribute(Node, key.c_str());
		}

		static bsm::Vec3 TextToVector3(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return SS5Util::WstrToVector3(XmlDocReader::GetText(pXMLDOMNode));
			}
			return bsm::Vec3(0, 0, 0);
		}

		static bsm::Vec3 AttributeToVector3(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return SS5Util::WstrToVector3(str);
			}
			return bsm::Vec3(0, 0, 0);
		}



		static Point2D<float> TextToPoint2DF(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return SS5Util::WstrToPoint2DF(XmlDocReader::GetText(pXMLDOMNode));
			}
			return Point2D<float>(0, 0);
		}

		static Point2D<float> AttributeToPoint2DF(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return SS5Util::WstrToPoint2DF(str);
			}
			return Point2D<float>(0, 0);
		}

		static size_t TextToSizet(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return wcstoul(XmlDocReader::GetText(pXMLDOMNode).c_str(), nullptr, 0);
			}
			return 0;
		}

		static size_t AttributeToSizet(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return wcstoul(str.c_str(), nullptr, 0);
			}
			return 0;
		}

		static int TextToInt(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return _wtoi(XmlDocReader::GetText(pXMLDOMNode).c_str());
			}
			return 0;
		}

		static int AttributeToInt(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return _wtoi(str.c_str());
			}
			return 0;
		}

		static float TextToFloat(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return (float)_wtof(XmlDocReader::GetText(pXMLDOMNode).c_str());
			}
			return 0.0f;
		}

		static float AttributeToFloat(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return (float)_wtof(str.c_str());
			}
			return 0.0f;
		}


		static bool TextToBool(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				if (XmlDocReader::GetText(pXMLDOMNode) == L"0"){
					return false;
				}
				else{
					return true;
				}
			}
			return false;
		}

		static bool AttributeToBool(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				if (str == L"0"){
					return false;
				}
				else{
					return true;
				}
			}
			return false;
		}


		static bsm::Col4 TextARGBToColor4(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				wstring Hex = XmlDocReader::GetText(pXMLDOMNode);
				return WstrARGBToColor4(Hex);
			}
			return bsm::Col4(0, 0, 0, 0);
		}

		static bsm::Col4 AttributeARGBToColor4(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return WstrARGBToColor4(str);
			}
			return bsm::Col4(0, 0, 0, 0);
		}


		// ���S��D3D���\�[�X���쐬����w���p�[�֐�.
		template<typename T, typename TCreateFunc>
		static T* DemandCreate(Microsoft::WRL::ComPtr<T>& comPtr, std::mutex& mutex, TCreateFunc createFunc)
		{
			T* result = comPtr.Get();
			// Double-checked lock pattern.
			MemoryBarrier();
			if (!result)
			{
				std::lock_guard<std::mutex> lock(mutex);
				result = comPtr.Get();
				if (!result)
				{
					createFunc(&result);
					MemoryBarrier();
					comPtr.Attach(result);
				}
			}
			return result;
		}

		///��ԂŃJ�[�u�p�����[�^���K�v�ȃ^�C�v�����肷��
		static bool SsNeedsCurveParams(SsInterpolationType::_enum type)
		{
			switch (type)
			{
			case SsInterpolationType::bezier:
			case SsInterpolationType::hermite:
				return true;
			}
			return false;
		}
		//----------------------------------------------------------------------------
		/**
		�^�C�v���w�肵�ĕ�Ԃ���
		*/
		//----------------------------------------------------------------------------
		static float SsInterpolate(SsInterpolationType::_enum type,
			float time, float start, float end, const SsCurve * curve = nullptr);
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeLine;
	//	�p�r: �A�j���[�V�������C���i���N���X�j
	//--------------------------------------------------------------------------------------
	class SS5AnimeLine{
	protected:
		SsAttributeKind::_enum type;	//�^�C�v
		SS5AnimeLine();
		virtual ~SS5AnimeLine();
	public:
		SsAttributeKind::_enum GetType()const{ return type; }
	private:
		//�R�s�[�֎~
		SS5AnimeLine(const  SS5AnimeLine&) = delete;
		SS5AnimeLine& operator=(const  SS5AnimeLine&) = delete;
		//���[�u�֎~
		SS5AnimeLine(const  SS5AnimeLine&&) = delete;
		SS5AnimeLine& operator=(const  SS5AnimeLine&&) = delete;

	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeBoolVal : public SS5AnimeLine;
	//	�p�r: bool�^�A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	class SS5AnimeBoolVal : public SS5AnimeLine{
	public:
		SS5AnimeBoolVal(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeBoolVal();
		//�w�莞�Ԃ̒l��Ԃ�
		bool GetValue(float Time);
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeFloatVal : public SS5AnimeLine;
	//	�p�r: float�^�̒l��ێ�����A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	class SS5AnimeFloatVal : public SS5AnimeLine{
	public:
		SS5AnimeFloatVal(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeFloatVal();
		//�ݒ肳�ꂽ��Ԃ����ƂɎw�莞�Ԃ̒l��Ԃ�
		float GetValue(float Time);
		float GetScaleValue(float Time);
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	struct AnimeCell{
		UINT id;
		wstring name;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeCellVal : public SS5AnimeLine;
	//	�p�r: AnimeCell�^�̒l��ێ�����A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	class SS5AnimeCellVal : public SS5AnimeLine{
	public:
		SS5AnimeCellVal(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeCellVal();
		//�w�莞�Ԃ̒l��Ԃ�
		AnimeCell GetValue(float Time);
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	//	class SS5AnimeWstrVal : public SS5AnimeLine;
	//	�p�r: wstrng�^�̒l��ێ�����A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	class SS5AnimeWstrVal : public SS5AnimeLine{
	public:
		SS5AnimeWstrVal(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeWstrVal();
		//�w�莞�Ԃ̒l��Ԃ�
		wstring GetValue(float Time);
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeColorVal : public SS5AnimeLine;
	//	�p�r: bsm::Col4�^�̒l��ێ�����A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	class SS5AnimeColorVal : public SS5AnimeLine{
	public:
		SS5AnimeColorVal(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeColorVal();
		//�ݒ肳�ꂽ��Ԃ����ƂɎw�莞�Ԃ̒l��Ԃ�
		bsm::Col4 GetValue(float Time);
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	struct Vertex2DAnimeData{
		Point2D<float> LeftTop;
		Point2D<float> RightTop;
		Point2D<float> LeftBottom;
		Point2D<float> RightBottom;
		Vertex2DAnimeData() :
			LeftTop(0, 0),
			RightTop(0, 0),
			LeftBottom(0, 0),
			RightBottom(0, 0){}
	};

	//--------------------------------------------------------------------------------------
	//	class SS5Animebsm::Vec3Val : public SS5AnimeLine;
	//	�p�r: bsm::Vec3�^�̒l��ێ�����A�j���[�V�������C��
	//--------------------------------------------------------------------------------------
	class SS5AnimeVector3Val : public SS5AnimeLine{
	public:
		SS5AnimeVector3Val(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeVector3Val();
		//�ݒ肳�ꂽ��Ԃ����ƂɎw�莞�Ԃ̒l��Ԃ�
		Vertex2DAnimeData GetValue(float Time);
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class SS5Cell;
	//	�p�r: SpriteStdio�Z��
	//--------------------------------------------------------------------------------------
	class SS5Cell{
	public:
		//�\�z�Ɣj��
		SS5Cell(IXMLDOMNodePtr Node);
		virtual ~SS5Cell();
		//�A�N�Z�b�T
		const wstring& get_name() const;
		const Point2D<float>& get_pos() const;
		const Point2D<float>& get_pivot() const;
		const Point2D<float>& get_size() const;
		bool get_rotated() const;
		bool is_rotated() const;
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		SS5Cell(const SS5Cell&) = delete;
		SS5Cell& operator=(const SS5Cell&) = delete;
		//���[�u�֎~
		SS5Cell(const SS5Cell&&) = delete;
		SS5Cell& operator=(const SS5Cell&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5ssce;
	//	�p�r: SpriteStdio��ssce�N���X
	//--------------------------------------------------------------------------------------
	class SS5ssce{
	public:
		//�\�z�Ə���
		SS5ssce(const wstring& BaseDir, const wstring& Xmlfilename);
		virtual ~SS5ssce();
		//�A�N�Z�b�T
		const Point2D<float>& get_pixelSize() const;
		//�Z���̌���
		shared_ptr<SS5Cell> GetCell(const wstring& name);
		//�e�N�X�`�����\�[�X��
		const wstring& get_textureResname() const;
		//�Z���}�b�v��
		const wstring& get_name() const;
		//�e�N�X�`��
		shared_ptr<TextureResource> GetTexture() const;

	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		SS5ssce(const SS5ssce&) = delete;
		SS5ssce& operator=(const SS5ssce&) = delete;
		//���[�u�֎~
		SS5ssce(const SS5ssce&&) = delete;
		SS5ssce& operator=(const SS5ssce&&) = delete;
	};





	class SS5Animation;
	//--------------------------------------------------------------------------------------
	//	class SS5PartAnimation;
	//	�p�r: �p�[�c���Ƃ̃A�j���[�V����
	//--------------------------------------------------------------------------------------
	class SS5PartAnimation{
	public:
		//�\�z�Ə���
		SS5PartAnimation(IXMLDOMNodePtr TgtNode, SS5Animation* pSS5Animation);
		~SS5PartAnimation();
		//�A�N�Z�b�T
		const wstring& GetpartName() const;

		//�O���b�h�T�C�Y
		float GetGridSize() const;

		//����
		bsm::Mat4x4 GetLocalMatrix();

		bool GetHide();
		float GetPrio();
		float GetAlpha();
		AnimeCell GetCell();
		Vertex2DAnimeData GetVirtex2DData();

	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		SS5PartAnimation(const SS5PartAnimation&) = delete;
		SS5PartAnimation& operator=(const SS5PartAnimation&) = delete;
		//���[�u�֎~
		SS5PartAnimation(const SS5PartAnimation&&) = delete;
		SS5PartAnimation& operator=(const SS5PartAnimation&&) = delete;
	};


	//--------------------------------------------------------------------------------------
	//	class SS5Animation;
	//	�p�r: �A�j���[�V����
	//--------------------------------------------------------------------------------------
	class SS5Animation{
	public:
		//�\�z�Ə���
		SS5Animation(IXMLDOMNodePtr TgtNode);
		~SS5Animation();

		//�A�N�Z�b�T
		const wstring& get_name() const;
		vector< shared_ptr<SS5PartAnimation> >& get_PartAnimationVec();
		const Point2D<float>& get_canvasSize() const;
		float get_gridSize() const;
		const Point2D<float>& get_pivot() const;

		float  get_fps() const;
		void set_fps(float f);

		//���[�v���邩�ǂ����𓾂�
		bool IsLooped() const;
		bool GetLooped() const;
		//���[�v���邩�ǂ�����ݒ肷��
		void SetLooped(bool b);
		//�A�j�����I���������ǂ���
		bool IsAnimeEnd() const;


		//����
		//�A�j���[�V�����^�C�}�[�̏�����
		void SetCurrentTime(float AnimeTime);
		//�A�j���[�V�����^�C�}�[�̍X�V
		void AddCurrentTime(float TimeSpan);
		//���݂̃A�j���[�V�����^�C�}�[�����ƂɃt���[�����v�Z���Ԃ�
		float GetAnimationTimer2Frame();
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		SS5Animation(SS5Animation const&);
		SS5Animation& operator=(SS5Animation const&);
	};

	class SS5ssae;

	//--------------------------------------------------------------------------------------
	//	class SSPart : public  GameObject;
	//	�p�r: SSPart�N���X(�e�p�[�c)
	//--------------------------------------------------------------------------------------
	class SSPart : public  GameObject{
	public:
		//�\�z�Ə���
		SSPart(const shared_ptr<Stage>& StagePtr, const wstring& SsaeName, vector< shared_ptr<SS5ssce> >& SS5sscePtrVec,
			IXMLDOMNodePtr ScanNode, IXMLDOMNodePtr TgtNode, const shared_ptr<SSPart>& Parent,bool SpriteType);
		virtual ~SSPart();
		//������
		virtual void OnCreate() override;
		//�A�N�Z�T
		int get_arrayIndex();
		float GetPrio();
		void SetSS5ssae(const shared_ptr<SS5ssae>& pSS5ssae);

		//�X�v���C�g���ǂ���(Getter�̂�)
		bool GetSpriteType() const;
		bool IsSpriteType() const;

		//����
		//�A�j���[�V������ύX����
		void ChangeAnimation(shared_ptr<SS5Animation> AnimationPtr);
		//�A�j���[�V������̍s����v�Z����(ZSort����)
		void CaluclateMatrix();
		void SetPartInVector(vector<SSPart*>& PartVec);

		virtual void OnDraw()override;

	private:
		//���_�̕ύX
		void ResetVirtex();
		void ResetSpriteVirtex();

		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5ssae :  public GameObject;
	//	�p�r: SpriteStdio��ssae�N���X�i�A�j���[�V�����p�b�N�j
	//--------------------------------------------------------------------------------------
	class SS5ssae : public  GameObject{
	public:
		//�\�z�Ə���
		SS5ssae(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir,
			const wstring& Xmlfilename, const wstring& StartAnimeName,bool SpriteType = false);
		virtual ~SS5ssae();
		//������
		virtual void OnCreate() override;
		//�A�N�Z�b�T
		//SS5ssae����A�j���[�V�����I�u�W�F�N�g�ւ̍s��
		const bsm::Mat4x4& GetToAnimeMatrix() const;
		void SetToAnimeMatrix(const bsm::Mat4x4& mat);

		//�X�v���C�g���ǂ���(Getter�̂�)
		bool GetSpriteType() const;
		bool IsSpriteType() const;

		//���ݑI������Ă���A�j���[�V���������[�v���邩�ǂ����𓾂�
		bool IsLooped() const;
		bool GetLooped() const;
		//���ݑI������Ă���A�j���[�V���������[�v���邩�ǂ�����ݒ肷��
		void SetLooped(bool b);
		//���ݑI������Ă���A�j���[�V���������s�I���������ǂ���
		bool IsAnimeEnd() const;

		void SetTextureOnlyNoLight(bool b);
		bool GetTextureOnlyNoLight() const;
		bool IsTextureOnlyNoLight() const;


		//����
		//�A�j���[�V������ύX����
		void ChangeAnimation(const wstring& AnimationName, float AnimeTime = 0);
		//�A�j���[�V�������X�V����
		void UpdateAnimeTime(float TimeSpan);
		//fps�i�Đ��X�s�[�h�j��ݒ肷��
		void SetFps(float f);
		//���[�g�̃p�[�c���擾����
		const shared_ptr<SSPart>& GetRootPart() const;
		//���ׂẴp�[�c�̃f�t���[�Y��ύX����
		void SetAllPartsDiffuse(const bsm::Col4& c);
		//���ׂẴp�[�c�̃G�~�b�V�u��ύX����
		void SetAllPartsEmissive(const bsm::Col4& c);

		virtual void OnUpdate() override{}
		virtual void OnDraw()override;
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

}
//end of namespace bascross.

