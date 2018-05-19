/*!
@file XmlDoc.cpp
@brief XML�֘A�N���X����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class XmlDocReader;
	//	�p�r: XML�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	XmlDocReader::XmlDocReader(const wstring& StrReadContext, bool IsMemory){
		try{
			HRESULT hr = m_pXMLDOMDocument.CreateInstance(CLSID_DOMDocument);
			if (FAILED(hr))
			{
				// ���������s
				throw BaseException(
					L"XML�������Ɏ��s���܂����B",
					L"if (FAILED(hr))",
					L"XmlDocReader::XmlDoc()"
					);
			}
			m_pXMLDOMDocument->put_async(VARIANT_FALSE);
			VARIANT_BOOL varbResult;
			if (!IsMemory){
				//�t�@�C���Ƃ��ēǂݍ���
				hr = m_pXMLDOMDocument->load(CComVariant(StrReadContext.c_str()), &varbResult);
				if (FAILED(hr) || !varbResult)
				{
					// ���������s
					throw BaseException(
						L"XML�t�@�C���ǂݍ��݂Ɏ��s���܂����B",
						L"if (FAILED(hr) || !varbResult)",
						L"XmlDocReader::XmlDoc()"
						);
				}
			}
			else{
				//������Ƃ��ēǂݍ���
				hr = m_pXMLDOMDocument->loadXML(BSTR(StrReadContext.c_str()), &varbResult);
				if (FAILED(hr) || !varbResult)
				{
					// ���������s
					throw BaseException(
						L"XML�f�[�^�ǂݍ��݂Ɏ��s���܂����B",
						L"if (FAILED(hr) || !varbResult)",
						L"XmlDocReader::XmlDoc()"
						);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	XmlDocReader::~XmlDocReader(){
	}

	//����
	//XPath�ɂ��擾
	IXMLDOMNodePtr XmlDocReader::GetSelectSingleNode(const CComBSTR& Query) const{
		if (Query.Length() <= 0){
			// �N�G�����s
			throw BaseException(
				L"XPath�N�G������ł��B",
				L"if (Query.Length() <= 0)",
				L"XmlDocReader::GetSelectSingleNode()"
				);
		}
		// �m�[�h�̎擾
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = m_pXMLDOMDocument->selectSingleNode(Query, &pXMLDOMNode);
		if (FAILED(hr))
		{
			wstring errstr(Query);
			// �N�G�����s
			throw BaseException(
				L"�m�[�h�̎擾�Ɏ��s���܂���",
				errstr,
				L"XmlDocReader::GetSelectSingleNode()"
				);
		}
		return pXMLDOMNode;
	}

	IXMLDOMNodeListPtr XmlDocReader::GetSelectNodes(const CComBSTR& Query)const{
		if (Query.Length() <= 0){
			// �N�G�����s
			throw BaseException(
				L"XPath�N�G������ł��B",
				L"if (Query.Length() <= 0)",
				L"XmlDocReader::GetSelectNodes()"
				);
		}
		// �^�O���̃m�[�h���X�g�̎擾
		IXMLDOMNodeListPtr pXMLDOMNodeList = nullptr;
		HRESULT hr = m_pXMLDOMDocument->selectNodes(Query, &pXMLDOMNodeList);
		if (FAILED(hr))
		{
			wstring errstr(Query);
			// �N�G�����s
			throw BaseException(
				L"�m�[�h���X�g�̎擾�Ɏ��s���܂���",
				errstr,
				L"XmlDocReader::GetSelectNodes()"
				);
		}
		return pXMLDOMNodeList;
	}

	//static�֐��Q
	long XmlDocReader::GetLength(const IXMLDOMNodeListPtr& NodeList){
		long lCountNode = 0;
		HRESULT hr = NodeList->get_length(&lCountNode);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�m�[�h���̎擾�Ɏ��s���܂���",
				L"NodeList->get_length()",
				L"XmlDocReader::GetLength()"
				);
		}
		return lCountNode;
	}
	IXMLDOMNodePtr XmlDocReader::GetItem(const IXMLDOMNodeListPtr& NodeList, long index){
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = NodeList->get_item(index, &pXMLDOMNode);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�m�[�h�̎擾�Ɏ��s���܂���",
				L"NodeList->get_item()",
				L"XmlDocReader::GetItem()"
				);
		}
		return pXMLDOMNode;

	}


	IXMLDOMNodeListPtr XmlDocReader::GetChildNodes(const IXMLDOMNodePtr& Node){
		IXMLDOMNodeListPtr NodeList = nullptr;
		HRESULT hr = Node->get_childNodes(&NodeList);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�m�[�h���X�g�̎擾�Ɏ��s���܂���",
				L"Node->get_childNodes()",
				L"XmlDocReader::GetChildNodes()"
				);
		}
		return NodeList;
	}
	wstring XmlDocReader::GetBaseName(const IXMLDOMNodePtr& Node){
		CComBSTR bstrText;
		HRESULT hr = Node->get_baseName(&bstrText);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"���O�̎擾�Ɏ��s���܂���",
				L"Node->get_baseName()",
				L"XmlDocReader::GetBaseName()"
				);
		}
		wstring ret(bstrText);
		return ret;
	}

	wstring XmlDocReader::GetText(const IXMLDOMNodePtr& Node){
		CComBSTR bstrText;
		HRESULT hr = Node->get_text(&bstrText);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�e�L�X�g�̎擾�Ɏ��s���܂���",
				L"Node->get_text()",
				L"XmlDocReader::GetText()"
				);
		}
		wstring ret(bstrText);
		return ret;
	}

	wstring XmlDocReader::GetXML(const IXMLDOMNodePtr& Node){
		CComBSTR bstrText;
		HRESULT hr = Node->get_xml(&bstrText);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�e�L�X�g�̎擾�Ɏ��s���܂���",
				L"Node->get_xml()",
				L"XmlDocReader::GetXML()"
				);
		}
		wstring ret(bstrText);
		return ret;
	}


	wstring XmlDocReader::GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag){
		CComBSTR bstrText;
		IXMLDOMNamedNodeMap* pMap = nullptr;
		HRESULT hr = Node->get_attributes(&pMap);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�A�g���r���[�g�̎擾�Ɏ��s���܂���",
				L"Node->get_attributes()",
				L"XmlDocReader::GetAttribute()"
				);
		}
		if (pMap){
			IXMLDOMNodePtr pXMLDOMNode = nullptr;
			pMap->getNamedItem(AttTag, &pXMLDOMNode);
			if (pXMLDOMNode){
				wstring ret = XmlDocReader::GetText(pXMLDOMNode);
				return ret;
			}
		}
		return wstring(L"");
	}

	IXMLDOMNodePtr XmlDocReader::GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query){
		if (Query.Length() <= 0){
			// �N�G�����s
			throw BaseException(
				L"XPath�N�G������ł��B",
				L"if (Query.Length() <= 0)",
				L"XmlDocReader::GetSelectSingleNode()"
				);
		}
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = Node->selectSingleNode(Query, &pXMLDOMNode);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�m�[�h�̎擾�Ɏ��s���܂���",
				L"Node->selectSingleNode()",
				L"XmlDocReader::GetSelectSingleNode()"
				);
		}
		return pXMLDOMNode;
	}

	IXMLDOMNodeListPtr XmlDocReader::GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query){
		if (Query.Length() <= 0){
			// �N�G�����s
			throw BaseException(
				L"XPath�N�G������ł��B",
				L"if (Query.Length() <= 0)",
				L"XmlDocReader::GetSelectNodes()"
				);
		}
		IXMLDOMNodeListPtr pXMLDOMNodeList = nullptr;
		HRESULT hr = Node->selectNodes(Query, &pXMLDOMNodeList);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�m�[�h���X�g�̎擾�Ɏ��s���܂���",
				L"Node->selectNodes()",
				L"XmlDocReader::GetSelectNodes()"
				);
		}
		return pXMLDOMNodeList;
	}

	//--------------------------------------------------------------------------------------
	//	struct XmlDoc::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct XmlDoc::Impl{
		IXMLDOMDocumentPtr m_pXMLDOMDocument;
		Impl(){}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class XmlDoc;
	//	�p�r: XML�N���X(�X�^�e�B�b�N���g��Ȃ��o�[�W����)
	//--------------------------------------------------------------------------------------
	void XmlDoc::CheckXmlDoc()const{
		if (!pImpl->m_pXMLDOMDocument){
			// ���������s
			throw BaseException(
				L"XML������������Ă܂���B",
				L"if (!pImpl->m_pXMLDOMDocument)",
				L"XmlDoc::CheckXmlDoc()"
				);
		}
	}

	//�\�z�Ə���
	XmlDoc::XmlDoc(const wstring& Filename) :
		pImpl(new Impl){
		try{
			HRESULT hr = pImpl->m_pXMLDOMDocument.CreateInstance(CLSID_DOMDocument);
			if (FAILED(hr))
			{
				// ���������s
				throw BaseException(
					L"XML�������Ɏ��s���܂����B",
					L"if (FAILED(hr))",
					L"XmlDoc::XmlDoc()"
					);
			}
			pImpl->m_pXMLDOMDocument->put_async(VARIANT_FALSE);
			VARIANT_BOOL varbResult;
			hr = pImpl->m_pXMLDOMDocument->load(CComVariant(Filename.c_str()), &varbResult);
			if (FAILED(hr) || !varbResult)
			{
				// ���������s
				throw BaseException(
					L"XML�ǂݍ��݂Ɏ��s���܂����B",
					L"if (FAILED(hr) || !varbResult)",
					L"XmlDoc::XmlDoc()"
					);
			}
		}
		catch (...){
			throw;
		}
	}
	XmlDoc::~XmlDoc(){}

	//����
	void XmlDoc::Save(const wstring& Filename, bool Exp){
		try{
			if (!pImpl->m_pXMLDOMDocument || Filename == L""){
				if (Exp){
					//��O�h��
					throw BaseException(
						L"XML�ۑ��Ɏ��s���܂����B",
						Filename,
						L"XmlDoc::Save()"
						);

				}
				else{
					//��O���������Ƀ��^�[��
					return;
				}
			}
			HRESULT hr = pImpl->m_pXMLDOMDocument->save(CComVariant(Filename.c_str()));
			if (FAILED(hr) && Exp){
				//��O�h��
				throw BaseException(
					L"XMLsave�Ɏ��s���܂����B",
					Filename,
					L"XmlDoc::Save()"
					);
			}
		}
		catch (...){
			throw;
		}

	}
	IXMLDOMNodePtr XmlDoc::CreateNodeBase(const CComBSTR& NodeName){
		try{
			if (!pImpl->m_pXMLDOMDocument || NodeName.Length() <= 0){
				//��O�h��
				wstring errstr(NodeName);
				throw BaseException(
					L"Node�̍쐬�Ɏ��s���܂����B",
					errstr,
					L"XmlDoc::CreateNode()"
					);

			}
			CComVariant NodeType = NODE_ELEMENT;
			CComBSTR	NamespaceURI = "";
			IXMLDOMNodePtr NewChild;
			ThrowIfFailed(
				pImpl->m_pXMLDOMDocument->createNode(NodeType, NodeName, NamespaceURI, &NewChild),
				L"Node�̍쐬�Ɏ��s���܂���",
				L"FAILED(m_pXMLDOMDocument->createNode(NodeType, NodeName, NamespaceURI, &NewChild))",
				L"XmlDoc::CreateNode()"
				);
			return NewChild;
		}
		catch (...){
			throw;
		}
	}
	IXMLDOMNodePtr XmlDoc::AddChildNodeBase(const IXMLDOMNodePtr& Node, const IXMLDOMNodePtr& NewChildNode){
		CheckXmlDoc();
		//�ǉ������m�[�h�Ǝ��ۂɔz�u�����m�[�h�͈Ⴄ
		IXMLDOMNodePtr RetNewChild;
		Node->appendChild(NewChildNode, &RetNewChild);
		//�z�u���ꂽ�m�[�h��Ԃ�
		return RetNewChild;
	}




	//XPath�ɂ��擾
	IXMLDOMNodePtr XmlDoc::GetSelectSingleNode(const CComBSTR& Query) const{
		CheckXmlDoc();
		if (Query.Length() <= 0){
			// �N�G�����s
			throw BaseException(
				L"XPath�N�G������ł��B",
				L"if (Query.Length() <= 0)",
				L"XmlDoc::GetSelectSingleNode()"
				);
		}
		// �m�[�h�̎擾
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = pImpl->m_pXMLDOMDocument->selectSingleNode(Query, &pXMLDOMNode);
		if (FAILED(hr))
		{
			wstring errstr(Query);
			// �N�G�����s
			throw BaseException(
				L"�m�[�h�̎擾�Ɏ��s���܂���",
				errstr,
				L"XmlDoc::GetSelectSingleNode()"
				);
		}
		return pXMLDOMNode;
	}
	IXMLDOMNodeListPtr XmlDoc::GetSelectNodes(const CComBSTR& Query) const{
		CheckXmlDoc();
		if (Query.Length() <= 0){
			// �N�G�����s
			throw BaseException(
				L"XPath�N�G������ł��B",
				L"if (Query.Length() <= 0)",
				L"XmlDoc::GetSelectNodes()"
				);
		}
		// �^�O���̃m�[�h���X�g�̎擾
		IXMLDOMNodeListPtr pXMLDOMNodeList = nullptr;
		HRESULT hr = pImpl->m_pXMLDOMDocument->selectNodes(Query, &pXMLDOMNodeList);
		if (FAILED(hr))
		{
			wstring errstr(Query);
			// �N�G�����s
			throw BaseException(
				L"�m�[�h���X�g�̎擾�Ɏ��s���܂���",
				errstr,
				L"XmlDoc::GetSelectNodes()"
				);
		}
		return pXMLDOMNodeList;
	}

	//Node������擾����static�֐�
	long XmlDoc::GetLength(const IXMLDOMNodeListPtr& NodeList)const{
		CheckXmlDoc();
		long lCountNode = 0;
		HRESULT hr = NodeList->get_length(&lCountNode);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�m�[�h���̎擾�Ɏ��s���܂���",
				L"NodeList->get_length()",
				L"XmlDoc::GetLength()"
				);
		}
		return lCountNode;
	}

	IXMLDOMNodePtr XmlDoc::GetItem(const IXMLDOMNodeListPtr& NodeList, long index)const{
		CheckXmlDoc();
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = NodeList->get_item(index, &pXMLDOMNode);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�m�[�h�̎擾�Ɏ��s���܂���",
				L"NodeList->get_item()",
				L"XmlDoc::GetItem()"
				);
		}
		return pXMLDOMNode;

	}
	IXMLDOMNodeListPtr XmlDoc::GetChildNodes(const IXMLDOMNodePtr& Node)const{
		CheckXmlDoc();
		IXMLDOMNodeListPtr NodeList = nullptr;
		HRESULT hr = Node->get_childNodes(&NodeList);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�m�[�h���X�g�̎擾�Ɏ��s���܂���",
				L"Node->get_childNodes()",
				L"XmlDoc::GetChildNodes()"
				);
		}
		return NodeList;
	}
	wstring XmlDoc::GetBaseName(const IXMLDOMNodePtr& Node) const{
		CheckXmlDoc();
		CComBSTR bstrText;
		HRESULT hr = Node->get_baseName(&bstrText);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"���O�̎擾�Ɏ��s���܂���",
				L"Node->get_baseName()",
				L"XmlDoc::GetBaseName()"
				);
		}
		wstring ret(bstrText);
		return ret;
	}
	wstring XmlDoc::GetText(const IXMLDOMNodePtr& Node)const{
		CheckXmlDoc();
		CComBSTR bstrText;
		HRESULT hr = Node->get_text(&bstrText);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�e�L�X�g�̎擾�Ɏ��s���܂���",
				L"Node->get_text()",
				L"XmlDoc::GetText()"
				);
		}
		wstring ret(bstrText);
		return ret;
	}
	void XmlDoc::SetText(const IXMLDOMNodePtr& Node, const CComBSTR& text){
		CheckXmlDoc();
		ThrowIfFailed(
			Node->put_text(text),
			L"�e�L�X�g�̐ݒ�Ɏ��s���܂���",
			L"Node->put_text()",
			L"XmlDoc::SetText()"
			);
	}

	//�m�[�h��XML�ւ̃A�N�Z�X�̓��[�h�I�����[
	wstring XmlDoc::GetXML(const IXMLDOMNodePtr& Node)const{
		CheckXmlDoc();
		CComBSTR bstrText;
		HRESULT hr = Node->get_xml(&bstrText);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�e�L�X�g�̎擾�Ɏ��s���܂���",
				L"Node->get_xml()",
				L"XmlDoc::GetXML()"
				);
		}
		wstring ret(bstrText);
		return ret;
	}

	wstring XmlDoc::GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag) const{
		CheckXmlDoc();
		CComBSTR bstrText;
		IXMLDOMNamedNodeMap* pMap = nullptr;
		HRESULT hr = Node->get_attributes(&pMap);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�A�g���r���[�g�̎擾�Ɏ��s���܂���",
				L"Node->get_attributes()",
				L"XmlDoc::GetAttribute()"
				);
		}
		if (pMap){
			IXMLDOMNodePtr pXMLDOMNode = nullptr;
			pMap->getNamedItem(AttTag, &pXMLDOMNode);
			if (pXMLDOMNode){
				wstring ret = XmlDocReader::GetText(pXMLDOMNode);
				return ret;
			}
		}
		return wstring(L"");
	}

	void XmlDoc::SetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag, const CComBSTR& AttValue){
		CheckXmlDoc();
		CComBSTR bstrText;
		IXMLDOMNamedNodeMapPtr MapPtr = nullptr;
		HRESULT hr = Node->get_attributes(&MapPtr);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�A�g���r���[�g�̎擾�Ɏ��s���܂���",
				L"Node->get_attributes()",
				L"XmlDoc::SetAttribute()"
				);
		}
		if (MapPtr){
			//Map������
			IXMLDOMNodePtr pXMLDOMNode = nullptr;
			MapPtr->getNamedItem(AttTag, &pXMLDOMNode);
			if (pXMLDOMNode){
				//���łɂ��̃A�g���r���[�g������
				SetText(pXMLDOMNode, AttValue);
			}
			else{
				//���̃A�g���r���[�g���Ȃ�
				IXMLDOMAttributePtr AttPtr;
				pImpl->m_pXMLDOMDocument->createAttribute(AttTag, &AttPtr);
				IXMLDOMNodePtr pXMLDOMNode = nullptr;
				MapPtr->setNamedItem(AttPtr, &pXMLDOMNode);
				SetText(pXMLDOMNode, AttValue);
			}
		}
		else{
			//Map���Ȃ�
			int a = 0;
		}
	}


	IXMLDOMNodePtr XmlDoc::GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query)const{
		CheckXmlDoc();
		if (Query.Length() <= 0){
			// �N�G�����s
			throw BaseException(
				L"XPath�N�G������ł��B",
				L"if (Query.Length() <= 0)",
				L"XmlDoc::GetSelectSingleNode()"
				);
		}
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = Node->selectSingleNode(Query, &pXMLDOMNode);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�m�[�h�̎擾�Ɏ��s���܂���",
				L"Node->selectSingleNode()",
				L"XmlDoc::GetSelectSingleNode()"
				);
		}
		return pXMLDOMNode;

	}
	IXMLDOMNodeListPtr XmlDoc::GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query)const{
		CheckXmlDoc();
		if (Query.Length() <= 0){
			// �N�G�����s
			throw BaseException(
				L"XPath�N�G������ł��B",
				L"if (Query.Length() <= 0)",
				L"XmlDoc::GetSelectNodes()"
				);
		}
		IXMLDOMNodeListPtr pXMLDOMNodeList = nullptr;
		HRESULT hr = Node->selectNodes(Query, &pXMLDOMNodeList);
		if (FAILED(hr))
		{
			// ���s
			throw BaseException(
				L"�m�[�h���X�g�̎擾�Ɏ��s���܂���",
				L"Node->selectNodes()",
				L"XmlDoc::GetSelectNodes()"
				);
		}
		return pXMLDOMNodeList;
	}

	IXMLDOMNodePtr XmlDoc::AddChildNode(const IXMLDOMNodePtr& Node, const CComBSTR& NodeName){
		CheckXmlDoc();
		auto NewNode = CreateNodeBase(NodeName);
		return AddChildNodeBase(Node, NewNode);
	}



}

//endof  basecross

