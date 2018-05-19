/*!
@file XmlDoc.h
@brief XML�֘A�N���X
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	XML�ǂݍ��݃I�����[�N���X
	//--------------------------------------------------------------------------------------
	class XmlDocReader {
	protected:
		IXMLDOMDocumentPtr m_pXMLDOMDocument;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StrReadContext XML�R���e�N�X�g
		@param[in]	IsMemory XML�R���e�N�X�g����������̃e�L�X�g���ǂ����i�����łȂ����XML�t�@�C���j
		*/
		//--------------------------------------------------------------------------------------
		XmlDocReader(const wstring& StrReadContext, bool IsMemory = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~XmlDocReader();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPath�ɂ��P��Node�̎擾
		@param[in]	Query	�N�G��������
		@return	Node�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodePtr GetSelectSingleNode(const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPath�ɂ�镡��Node�̎擾
		@param[in]	Query	�N�G��������
		@return	����Node�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodeListPtr GetSelectNodes(const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node�̐��擾
		@param[in]	NodeList	����Node
		@return	Node�̐�
		*/
		//--------------------------------------------------------------------------------------
		static long GetLength(const IXMLDOMNodeListPtr& NodeList);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C���f�b�N�X����Node�̎擾
		@param[in]	IXMLDOMNodeListPtr	����Node
		@param[in]	index	�C���f�b�N�X
		@return	�^�[�Q�b�g��Node
		*/
		//--------------------------------------------------------------------------------------
		static IXMLDOMNodePtr GetItem(const IXMLDOMNodeListPtr& NodeList, long index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�q��Node���X�g�̎擾
		@param[in]	Node	�eNode
		@return	�q��Node���X�g
		*/
		//--------------------------------------------------------------------------------------
		static IXMLDOMNodeListPtr GetChildNodes(const IXMLDOMNodePtr& Node);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node���̎擾
		@param[in]	Node	�^�[�Q�b�gNode
		@return	Node��
		*/
		//--------------------------------------------------------------------------------------
		static wstring GetBaseName(const IXMLDOMNodePtr& Node);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node�̃e�L�X�g�̎擾
		@param[in]	Node	�^�[�Q�b�gNode
		@return	Node�̃e�L�X�g
		*/
		//--------------------------------------------------------------------------------------
		static wstring GetText(const IXMLDOMNodePtr& Node);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node��XML�̎擾
		@param[in]	Node	�^�[�Q�b�gNode
		@return	Node��XML
		*/
		//--------------------------------------------------------------------------------------
		static wstring GetXML(const IXMLDOMNodePtr& Node);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node�̃A�g���r���[�g�̎擾
		@param[in]	Node	�^�[�Q�b�gNode
		@param[in]	AttTag	�A�g���r���[�g��
		@return	Node�̃A�g���r���[�g
		*/
		//--------------------------------------------------------------------------------------
		static wstring GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPath�ɂ��P��Node�̎擾
		@param[in]	Node	�^�[�Q�b�gNode
		@param[in]	Query	�N�G��������
		@return	Node
		*/
		//--------------------------------------------------------------------------------------
		static IXMLDOMNodePtr GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPath�ɂ�镡��Node�̎擾
		@param[in]	Node	�^�[�Q�b�gNode
		@param[in]	Query	�N�G��������
		@return	����Node
		*/
		//--------------------------------------------------------------------------------------
		static IXMLDOMNodeListPtr GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query);
	};



	//--------------------------------------------------------------------------------------
	///	XML�N���X
	//--------------------------------------------------------------------------------------
	class XmlDoc{
		void CheckXmlDoc() const;
		IXMLDOMNodePtr CreateNodeBase(const CComBSTR& NodeName);
		IXMLDOMNodePtr AddChildNodeBase(const IXMLDOMNodePtr& Node, const IXMLDOMNodePtr& NewChildNode);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	XML�t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		XmlDoc(const wstring& Filename);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~XmlDoc();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ۑ�����
		@param[in]	Filename	�ۑ�����t�@�C����
		@param[in]	Exp	��O�������邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Save(const wstring& Filename, bool Exp = true);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPath�ɂ��P��Node�̎擾
		@param[in]	Query	�N�G��������
		@return	Node�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodePtr GetSelectSingleNode(const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPath�ɂ�镡��Node�̎擾
		@param[in]	Query	�N�G��������
		@return	����Node�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodeListPtr GetSelectNodes(const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node�̐��擾
		@param[in]	NodeList	����Node
		@return	Node�̐�
		*/
		//--------------------------------------------------------------------------------------
		long GetLength(const IXMLDOMNodeListPtr& NodeList)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C���f�b�N�X����Node�̎擾
		@param[in]	IXMLDOMNodeListPtr	����Node
		@param[in]	index	�C���f�b�N�X
		@return	�^�[�Q�b�g��Node
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodePtr GetItem(const IXMLDOMNodeListPtr& NodeList, long index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�q��Node���X�g�̎擾
		@param[in]	Node	�eNode
		@return	�q��Node���X�g
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodeListPtr GetChildNodes(const IXMLDOMNodePtr& Node)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node���̎擾
		@param[in]	Node	�^�[�Q�b�gNode
		@return	Node��
		*/
		//--------------------------------------------------------------------------------------
		wstring GetBaseName(const IXMLDOMNodePtr& Node) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node�̃e�L�X�g�̎擾
		@param[in]	Node	�^�[�Q�b�gNode
		@return	Node�̃e�L�X�g
		*/
		//--------------------------------------------------------------------------------------
		wstring GetText(const IXMLDOMNodePtr& Node) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node�̃e�L�X�g�̐ݒ�
		@param[in]	Node	�^�[�Q�b�gNode
		@param[in]	text	�ݒ肷��e�L�X�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetText(const IXMLDOMNodePtr& Node, const CComBSTR& text);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node��XML�̎擾�i�m�[�h��XML�ւ̃A�N�Z�X�̓��[�h�I�����[�j
		@param[in]	Node	�^�[�Q�b�gNode
		@return	Node��XML
		*/
		//--------------------------------------------------------------------------------------
		wstring GetXML(const IXMLDOMNodePtr& Node) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node�̃A�g���r���[�g�̎擾
		@param[in]	Node	�^�[�Q�b�gNode
		@param[in]	AttTag	�A�g���r���[�g��
		@return	Node�̃A�g���r���[�g
		*/
		//--------------------------------------------------------------------------------------
		wstring GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node�̃A�g���r���[�g�̐ݒ�
		@param[in]	Node	�^�[�Q�b�gNode
		@param[in]	AttTag	�A�g���r���[�g��
		@param[in]	AttValue	�ݒ肷��l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag, const CComBSTR& AttValue);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPath�ɂ��P��Node�̎擾
		@param[in]	Node	�^�[�Q�b�gNode
		@param[in]	Query	�N�G��������
		@return	Node
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodePtr GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPath�ɂ�镡��Node�̎擾
		@param[in]	Node	�^�[�Q�b�gNode
		@param[in]	Query	�N�G��������
		@return	����Node
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodeListPtr GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�q��Node�̒ǉ�
		@param[in]	Node	�^�[�Q�b�gNode
		@param[in]	NodeName	Node��
		@return	�ǉ�����Node
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodePtr AddChildNode(const IXMLDOMNodePtr& Node, const CComBSTR& NodeName);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		XmlDoc(const XmlDoc&) = delete;
		XmlDoc& operator=(const XmlDoc&) = delete;
		//���[�u�֎~
		XmlDoc(const XmlDoc&&) = delete;
		XmlDoc& operator=(const XmlDoc&&) = delete;
	};



}
//endof  basecross

