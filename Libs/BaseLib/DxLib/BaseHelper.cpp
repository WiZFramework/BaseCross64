/*!
@file BaseHelper.cpp
@brief ��O�����A�w���p�[�֐�������
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross{

	BinaryReader::BinaryReader(const wstring& fileName)
	{
		size_t dataSize;

		HRESULT hr = ReadEntireFile(fileName, mOwnedData, &dataSize);
		if (FAILED(hr))
		{
			// ���������s
			throw BaseException(
				L"�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B",
				L"if (FAILED(hr))",
				L"BinaryReader::BinaryReader()"
				);
		}

		mPos = mOwnedData.get();
		mEnd = mOwnedData.get() + dataSize;
	}


	BinaryReader::BinaryReader(_In_reads_bytes_(dataSize) uint8_t const* dataBlob, size_t dataSize)
	{
		mPos = dataBlob;
		mEnd = dataBlob + dataSize;
	}


	HRESULT BinaryReader::ReadEntireFile(const wstring& fileName, _Inout_ std::unique_ptr<uint8_t[]>& data, _Out_ size_t* dataSize)
	{
		//�t�@�C���̃I�[�v��
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
		ScopedHandle hFile(safe_handle(CreateFile2(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr)));
#else
		ScopedHandle hFile(safe_handle(CreateFileW(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)));
#endif

		if (!hFile)
			return HRESULT_FROM_WIN32(GetLastError());

		// �T�C�Y�̎擾
		LARGE_INTEGER fileSize = { 0 };

#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
		FILE_STANDARD_INFO fileInfo;

		if (!GetFileInformationByHandleEx(hFile.get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		fileSize = fileInfo.EndOfFile;
#else
		GetFileSizeEx(hFile.get(), &fileSize);
#endif

		// �t�@�C���������𒴂�����G���[
		if (fileSize.HighPart > 0)
			return E_FAIL;

		// �����T�C�Y�̃��������쐬
		data.reset(new uint8_t[fileSize.LowPart]);

		if (!data)
			return E_OUTOFMEMORY;

		// �f�[�^�̓ǂݍ���
		DWORD bytesRead = 0;

		if (!ReadFile(hFile.get(), data.get(), fileSize.LowPart, &bytesRead, nullptr))
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		if (bytesRead < fileSize.LowPart)
			return E_FAIL;

		*dataSize = bytesRead;

		return S_OK;
	}


	//--------------------------------------------------------------------------------------
	//	struct CsvFile::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct CsvFile::Impl{
		wstring m_FileName;					//�t�@�C����
		vector< wstring > m_CsvVec;	//�������ێ�����z��
		Impl()
		{}
		~Impl(){
			m_CsvVec.clear();
		}
	};


	//--------------------------------------------------------------------------------------
	//	class CsvFile;
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	CsvFile();
	//--------------------------------------------------------------------------------------
	CsvFile::CsvFile():
		pImpl(new Impl)
	{
	}
	//--------------------------------------------------------------------------------------
	//	explicit CsvFile(
	//	const wstring& FileName	//CSV�t�@�C����
	//	);
	//--------------------------------------------------------------------------------------
	CsvFile::CsvFile(const wstring& FileName):
		pImpl(new Impl)
	{
		try{
			if (FileName == L""){
				throw BaseException(
					L"Csv�t�@�C�������s��ł��B",
					L"FileName == L\"\"",
					L"CsvFile::CsvFile()"
					);
			}
			pImpl->m_FileName = FileName;
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	virtual CsvFile::~CsvFile();
	//--------------------------------------------------------------------------------------
	CsvFile::~CsvFile(){}
	//--------------------------------------------------------------------------------------
	//	const wstring& CsvFile::GetFileName() const;
	//--------------------------------------------------------------------------------------
	const wstring& CsvFile::GetFileName() const{
		return pImpl->m_FileName;
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::SetFileName(
	//	const wstring& FileName	//CSV�t�@�C����
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::SetFileName(const wstring& FileName){
		try{
			if (FileName == L""){
				throw BaseException(
					L"Csv�t�@�C�������s��ł��B",
					L"FileName == L\"\"",
					L"CsvFile::SetFileName()"
					);
			}
			pImpl->m_FileName = FileName;
			pImpl->m_CsvVec.clear();
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	vector< wstring >& CsvFile::GetCsvVec()const;
	//--------------------------------------------------------------------------------------
	vector< wstring >& CsvFile::GetCsvVec()const{
		return pImpl->m_CsvVec;
	}

	//--------------------------------------------------------------------------------------
	//	bool CsvFile::ReadCsv();
	//--------------------------------------------------------------------------------------
	bool CsvFile::ReadCsv(){
		try{
			if (pImpl->m_FileName == L""){
				throw BaseException(
					L"Csv�t�@�C�������s��ł��B",
					L"(m_FileName == L\"\")",
					L"CsvFile::ReadCsv()"
					);
			}
			//CSV�t�@�C���̑��݂��m�F
			DWORD RetCode;
			RetCode = GetFileAttributes(pImpl->m_FileName.c_str());
			if (RetCode == 0xFFFFFFFF){
				//�t�@�C�����Ȃ��B
				//�ǂݍ��ݏ����I��
				return false;
			}
			wifstream ifs(pImpl->m_FileName.c_str());
			if (!ifs){
				//�I�[�v���Ɏ��s�����B
				//�t�@�C�������݂��邪���s�������Ƃ��l������
				throw BaseException(
					L"Csv�t�@�C�����̃I�[�v���Ɏ��s���܂����B",
					pImpl->m_FileName.c_str(),
					L"CsvFile::ReadCsv()"
					);
			}
			locale loc("japanese");
			ifs.imbue(loc);
			//�f�[�^���N���A����
			pImpl->m_CsvVec.clear();
			wstring line;
			//�P�s����line�ɓǂݍ���
			while (getline(ifs, line)){
				//�s���f�[�^�ɒǉ�����
				pImpl->m_CsvVec.push_back(line);
			}
			ifs.close();
			return true;
		}
		catch (...){
			throw;
		}
	}

	bool CsvFile::ReadCsv(const wstring& str){
		//�f�[�^���N���A����
		pImpl->m_CsvVec.clear();
		vector<wstring> lineVec;
		Util::WStrToTokenVector(lineVec, str, L'\n');
		for (auto& v : lineVec){
			auto Len = v.size();
			wstring setstr;
			if (Len > 0){
				if (v[Len - 1] == L'\r'){
					setstr = v.substr(0, Len - 1);

				}
				else{
					setstr = v;
				}
				pImpl->m_CsvVec.push_back(setstr);
			}
		}
		return true;
	}


	//--------------------------------------------------------------------------------------
	//	void CsvFile::SaveCsv(
	//	bool Exp = true	//��O�������邩�ǂ���
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::SaveCsv(bool Exp){
		try{
			if (pImpl->m_FileName == L""){
				if (Exp){
					//��O��������ꍇ
					throw BaseException(
						L"Csv�t�@�C�������s��ł��B",
						L"m_FileName == L\"\"",
						L"CsvFile::SaveCsv()"
						);
				}
				else{
					//��O�������Ȃ�
					return;
				}
			}
			wofstream ofs(pImpl->m_FileName.c_str());
			//��O��������ꍇ
			if (!ofs){
				if (Exp){
					throw BaseException(
						L"Csv�t�@�C�����̃I�[�v���Ɏ��s���܂����B",
						pImpl->m_FileName.c_str(),
						L"CsvFile::SaveCsv()"
						);
				}
				else{
					//��O�������Ȃ�
					return;
				}
			}
			locale loc("japanese");
			ofs.imbue(loc);
			size_t lsz = pImpl->m_CsvVec.size();
			for (size_t i = 0; i < lsz; i++){
				ofs << pImpl->m_CsvVec[i] << endl;
			}
			ofs.close();
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	size_t AddRow(
	//	wstring& Row	//Csv��1�s�i������́A�u,�v�ŋ�؂��Ă���K�v������j
	//	);
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	size_t CsvFile::AddRow(
	//	vector<wstring>& Row	//Csv��1�s�i�e�Z���͔z�񉻂���Ă���K�v������j
	//	);
	//--------------------------------------------------------------------------------------
	size_t CsvFile::AddRow(vector<wstring>& Row){
		wstring temp = L"";
		bool flg = false;
		for (auto l : Row){
			if (flg){
				temp += L',';
			}
			temp += l;
			flg = true;
		}
		size_t ret = pImpl->m_CsvVec.size();
		pImpl->m_CsvVec.push_back(temp);
		return ret;
	}
	//--------------------------------------------------------------------------------------
	//	size_t CsvFile::AddRow(
	//	wstring& Row	//Csv��1�s�i������́A�u,�v�ŋ�؂��Ă���K�v������j
	//	);
	//--------------------------------------------------------------------------------------
	size_t CsvFile::AddRow(wstring& Row){
		size_t ret = pImpl->m_CsvVec.size();
		//�f�[�^�ɒǉ�����
		pImpl->m_CsvVec.push_back(Row);
		return ret;
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::UpdateRow(
	//	size_t RowNum,			//�ύX�������s
	//	vector<wstring>& Row.	//Csv��1�s�i�e�Z���͔z�񉻂���Ă���K�v������j
	//	bool Exp = true			//��O�������邩�ǂ���
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::UpdateRow(size_t RowNum, vector<wstring>& Row, bool Exp){
		try{
			if (pImpl->m_CsvVec.size() <= RowNum){
				if (Exp){
					//��O��������ꍇ
					throw BaseException(
						L"�w��̍s���͈͊O�ł��B",
						MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"RowNum", RowNum).c_str(),
						L"CsvFile::UpdateRow()"
						);
				}
				else{
					//��O�������Ȃ�
					return;
				}
			}
			wstring temp = L"";
			bool flg = false;
			for (auto l : Row){
				if (flg){
					temp += L',';
				}
				temp += l;
				flg = true;
			}
			pImpl->m_CsvVec[RowNum] = temp;
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::UpdateRow(
	//	size_t RowNum,	//�ύX�������s
	//	wstring& Row,	//Csv��1�s�i������́A�u,�v�ŋ�؂��Ă���K�v������j
	//	bool Exp = true	//��O�������邩�ǂ���
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::UpdateRow(size_t RowNum, wstring& Row, bool Exp){
		try{
			if (pImpl->m_CsvVec.size() <= RowNum){
				if (Exp){
					//��O��������ꍇ
					throw BaseException(
						L"�w��̍s���͈͊O�ł��B",
						MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"RowNum", RowNum).c_str(),
						L"CsvFile::UpdateRow()"
						);
				}
				else{
					//��O�������Ȃ�
					return;
				}
			}
			pImpl->m_CsvVec[RowNum] = Row;
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::UpdateCell(
	//	size_t RowNum,	//�ύX�s
	//	size_t ColNum,	//�ύX��
	//	wstring& Cell,	//�����ւ��镶����
	//	bool Exp = true			//��O�������邩�ǂ���
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::UpdateCell(size_t RowNum, size_t ColNum, wstring& Cell, bool Exp){
		try{
			if (pImpl->m_CsvVec.size() <= RowNum){
				if (Exp){
					//��O��������ꍇ
					throw BaseException(
						L"�w��̍s���͈͊O�ł��B",
						MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"RowNum", RowNum).c_str(),
						L"CsvFile::UpdateCell()"
						);
				}
				else{
					//��O�������Ȃ�
					return;
				}
			}
			vector<wstring> LineVec;
			Util::WStrToTokenVector(LineVec, pImpl->m_CsvVec[RowNum], L',');
			if (LineVec.size() <= ColNum){
				if (Exp){
					//��O��������ꍇ
					throw BaseException(
						L"�w��̗񂪔͈͊O�ł��B",
						MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"ColNum", ColNum).c_str(),
						L"CsvFile::UpdateCell()"
						);
				}
				else{
					//��O�������Ȃ�
					return;
				}
			}
			//�w��̈ʒu�̃Z���������ւ�
			LineVec[ColNum] = Cell;
			wstring temp = L"";
			bool flg = false;
			for (auto l : LineVec){
				if (flg){
					temp += L',';
				}
				temp += l;
				flg = true;
			}
			pImpl->m_CsvVec[RowNum] = temp;
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	size_t CsvFile::GetRowCount() const;
	//--------------------------------------------------------------------------------------
	size_t CsvFile::GetRowCount() const{
		return pImpl->m_CsvVec.size();
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::GetRowVec(
	//	size_t RowNum,			//�󂯎�肽���s
	//	vector<wstring>& Row	//1�s���󂯎�镶����̔z��
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::GetRowVec(size_t RowNum, vector<wstring>& Row){
		try{
			if (pImpl->m_CsvVec.size() <= RowNum){
				throw BaseException(
					L"�w��̍s���͈͊O�ł��B",
					MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"RowNum", RowNum).c_str(),
					L"CsvFile::GetLineVec()"
					);
			}
			//�n���ꂽ�z��ɒl�������Ă�ꍇ������̂ŃN���A
			Row.clear();
			//�w��s���A�f���~�^�u,�v�ŋ�؂��ĕԂ�
			Util::WStrToTokenVector(Row, pImpl->m_CsvVec[RowNum], L',');
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	wstring CsvFile::GetCell(
	//	size_t RowNum,	//�󂯎�肽���s
	//	size_t ColNum	//�󂯎�肽����
	//	);
	//--------------------------------------------------------------------------------------
	wstring CsvFile::GetCell(size_t RowNum, size_t ColNum){
		try{
			if (pImpl->m_CsvVec.size() <= RowNum){
				throw BaseException(
					L"�w��̍s���͈͊O�ł��B",
					MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"RowNum", RowNum).c_str(),
					L"CsvFile::GetCell()"
					);
			}
			vector<wstring> LineVec;
			Util::WStrToTokenVector(LineVec, pImpl->m_CsvVec[RowNum], L',');
			if (LineVec.size() <= ColNum){
				throw BaseException(
					L"�w��̗񂪔͈͊O�ł��B",
					MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"ColNum", ColNum).c_str(),
					L"CsvFile::GetCell()"
					);
			}
			return LineVec[ColNum];
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	void GetSelect(
	//	vector< wstring >& RetVec	//���ʂ��󂯎�镶����̔z��
	//	size_t ColNum,			//���������������
	//	const wstring& Key,		//�����L�[
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::GetSelect(vector< wstring >& RetVec,size_t ColNum, const wstring& Key){
		//�n���ꂽ�z��ɒl�������Ă�ꍇ������̂ŃN���A
		RetVec.clear();
		size_t sz = pImpl->m_CsvVec.size();
		for (size_t i = 0; i < sz; i++){
			vector<wstring> LineVec;
			Util::WStrToTokenVector(LineVec, pImpl->m_CsvVec[i], L',');
			if (LineVec.size() > ColNum){
				if (LineVec[ColNum] == Key){
					RetVec.push_back(pImpl->m_CsvVec[i]);
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::GetSelect(
	//	vector< wstring >& RetVec,		//���ʂ��󂯎�镶����̔z��
	//	bool (Func)(const wstring&),	//��������������R�[���o�b�N�֐�
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::GetSelect(vector< wstring >& RetVec,bool(Func)(const wstring&)){
		//�n���ꂽ�z��ɒl�������Ă�ꍇ������̂ŃN���A
		RetVec.clear();
		size_t sz = pImpl->m_CsvVec.size();
		for (size_t i = 0; i < sz; i++){
			//�R�[���o�b�N�֐����Ăяo��
			if (Func(pImpl->m_CsvVec[i])){
				//����������Βǉ�
				RetVec.push_back(pImpl->m_CsvVec[i]);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	///	Create��PreCreate�������AThis�X�}�[�g�|�C���^���Ƃ��C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	void ObjectInterface::PostEvent(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
		const wstring& MsgStr,const shared_ptr<void>& Info) {
		App::GetApp()->GetEventDispatcher()->PostEvent(DispatchTime, Sender, Receiver, MsgStr, Info);
	}
	void ObjectInterface::PostEvent(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
		const wstring& MsgStr, const  shared_ptr<void>& Info) {
		App::GetApp()->GetEventDispatcher()->PostEvent(DispatchTime, Sender, ReceiverKey, MsgStr, Info);

	}

	void ObjectInterface::SendEvent(const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
		const wstring& MsgStr, const shared_ptr<void>& Info) {
		App::GetApp()->GetEventDispatcher()->SendEvent(Sender, Receiver, MsgStr, Info);
	}

	void ObjectInterface::SendEvent(const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
		const wstring& MsgStr, const shared_ptr<void>& Info) {
		App::GetApp()->GetEventDispatcher()->SendEvent(Sender, ReceiverKey, MsgStr, Info);
	}





}
//endof  basecross
