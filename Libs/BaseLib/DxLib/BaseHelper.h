/*!
@file BaseHelper.h
@brief ��O�����A�w���p�[�֐���
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross{

	/// �X�}�[�g�|�C���^�̃w���p�[
	struct handle_closer { void operator()(HANDLE h) { if (h) CloseHandle(h); } };
	typedef public std::unique_ptr<void, handle_closer> ScopedHandle;
	inline HANDLE safe_handle(HANDLE h) { return (h == INVALID_HANDLE_VALUE) ? 0 : h; }

	//--------------------------------------------------------------------------------------
	/// ��O�N���X
	//--------------------------------------------------------------------------------------
	class BaseException : public exception
	{
		// ���b�Z�[�W�ϐ�
		wstring m_Message;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	m1  ��1���b�Z�[�W�i�x�����j
		@param[in]	m2  ��2���b�Z�[�W�i���ƂȂ����f�[�^�j
		@param[in]	m3  ��3���b�Z�[�W�i���������ʒu�j
		*/
		//--------------------------------------------------------------------------------------
		BaseException(const wstring&  m1, const wstring&  m2, const wstring& m3){
			m_Message = m1;
			m_Message += L"\r\n";
			m_Message += m2;
			m_Message += L"\r\n";
			m_Message += m3;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���b�Z�[�W�̎擾
		@return	���b�Z�[�W������̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const wstring& what_w() const throw(){
			return m_Message;
		}
	};

	//--------------------------------------------------------------------------------------
	///	��O�N���X�i�}���`�o�C�g�Łj
	//--------------------------------------------------------------------------------------
	class BaseMBException : public exception{
		//���b�Z�[�W�ϐ�
		string m_Message;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	m1  ��1���b�Z�[�W�i�x�����j
		@param[in]	m2  ��2���b�Z�[�W�i���ƂȂ����f�[�^�j
		@param[in]	m3  ��3���b�Z�[�W�i���������ʒu�j
		*/
		//--------------------------------------------------------------------------------------
		BaseMBException(const string& m1, const string& m2, const string& m3)
		{
			m_Message = m1;
			m_Message += "\r\n";
			m_Message += m2;
			m_Message += "\r\n";
			m_Message += m3;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�Z�[�W�̎擾�i�}���`�o�C�g�j
		@return	���b�Z�[�W������̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const string& what_m() const throw(){
			return m_Message;
		}
	};

	//--------------------------------------------------------------------------------------
	/*!
	@brief �C���^�[�t�F�C�X�̗�O
	@param[in]	hr	�߂�l�B�ʏ�͂�����HRESULT��Ԃ��֐����L�q����
	@param[in]	Str1	��1���b�Z�[�W�i�x�����j
	@param[in]	Str2	��2���b�Z�[�W�i���ƂȂ����f�[�^�j
	@param[in]	Str3	��3���b�Z�[�W�i���������ʒu�j
	@return	HRESULT
	*/
	//--------------------------------------------------------------------------------------
	inline HRESULT ThrowIfFailed(HRESULT hr, const wstring& Str1, const wstring& Str2, const wstring& Str3)
	{
		if (FAILED(hr))
		{
			throw BaseException(
				Str1,
				Str2,
				Str3
				);
		}
		return hr;
	}


	//--------------------------------------------------------------------------------------
	///	�o�C�i���f�[�^�ǂݍ��݁BDirectXTK�J�X�^�}�C�Y
	//--------------------------------------------------------------------------------------
	class BinaryReader
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	fileName	�t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		explicit BinaryReader(const wstring& fileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^�i����������ǂݍ��ށj
		@param[in]	dataBlob	�f�[�^�̐擪�|�C���^
		@param[in]	dataSize	�T�C�Y
		*/
		//--------------------------------------------------------------------------------------
		BinaryReader(_In_reads_bytes_(dataSize) uint8_t const* dataBlob, size_t dataSize);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T�^�̃f�[�^��1�ǂݍ���
		@tparam	T	�f�[�^�̌^
		@return	T�^�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		template<typename T> T const& Read()
		{
			return *ReadArray<T>(1);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T�^�̔z����w�萔�����ǂݍ���
		@tparam	T	�z��̌^
		@param[in]	elementCount	�ǂݍ��ރT�C�Y�i���j
		@return	T�^�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		template<typename T> T const* ReadArray(size_t elementCount)
		{
			static_assert(std::is_pod<T>::value, "Can only read plain-old-data types");
			uint8_t const* newPos = mPos + sizeof(T)* elementCount;
			if (newPos > mEnd){
				throw std::exception("End of file");
			}
			auto result = reinterpret_cast<T const*>(mPos);
			mPos = newPos;
			return result;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �჌�x���w���p�[�i�t�@�C�����烁�����ւ̓ǂݍ��݁j
		@param[in]	fileName	�t�@�C����
		@param[in,out]	data	�f�[�^�̎Q��
		@param[out]	dataSize	�ǂݍ��񂾐�
		@return	HRESULT�^
		*/
		//--------------------------------------------------------------------------------------
		static HRESULT ReadEntireFile(const wstring& fileName, _Inout_ std::unique_ptr<uint8_t[]>& data, _Out_ size_t* dataSize);
	private:
		// The data currently being read.
		uint8_t const* mPos;
		uint8_t const* mEnd;
		std::unique_ptr<uint8_t[]> mOwnedData;
		//�R�s�[�֎~
		BinaryReader(const BinaryReader&) = delete;
		BinaryReader& operator=(const BinaryReader&) = delete;
		//���[�u�֎~
		BinaryReader(const BinaryReader&&) = delete;
		BinaryReader& operator=(const BinaryReader&&) = delete;
	};




	//--------------------------------------------------------------------------------------
	///	Csv�t�@�C���A�N�Z�X�N���X
	//--------------------------------------------------------------------------------------
	class CsvFile{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		CsvFile();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	FileName	CSV�t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		explicit CsvFile(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CsvFile();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�@�C�����𓾂�
		@return	csv�t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetFileName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�@�C�������Z�b�g����
		@param[in]	FileName	CSV�t�@�C����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetFileName(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CSV�z��𓾂�
		@return	CSV�z��
		*/
		//--------------------------------------------------------------------------------------
		vector< wstring >& GetCsvVec()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Csv��ǂݍ���<br />
		�����łɎ����Ă�f�[�^�͔j�������<br/>
		��Csv�t�@�C�������݂��Ȃ��ꍇ�͏���������������false��Ԃ��B<br />
		�����݂��Ă��ǂݍ��݂Ɏ��s�������O
		@return	���s������false
		*/
		//--------------------------------------------------------------------------------------
		bool ReadCsv();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Csv��wstring����ǂݍ���<br />
		�����łɎ����Ă�f�[�^�͔j�������<br/>
		@return	���s������false
		*/
		//--------------------------------------------------------------------------------------
		bool ReadCsv(const wstring& str);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Csv�������o��<br />
		��Csv�t�@�C�������݂��Ȃ���΍쐬����A���݂��Ă�����A�f�[�^�͏����������<br/>
		���A�N�Z�X�Ɏ��s���A��O�����t���O�������Ă����O<br />
		���V�[����X�e�[�W�̏I�����Ƀf�X�g���N�^�ŕۑ��������ł���悤�ɁA��O������I�ׂ�<br />
		@param[in]	Exp = true	��O�������邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SaveCsv(bool Exp = true);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1�s�Ō���ɒǉ�����.
		@param[in]	Row	Csv��1�s�i�e�Z���͔z�񉻂���Ă���K�v������j
		@return	�ǉ������s�ԍ�
		*/
		//--------------------------------------------------------------------------------------
		size_t AddRow(vector<wstring>& Row);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�s�Ō���ɒǉ�����.
		@param[in]	Row	Csv��1�s�i������́A�u,�v�ŋ�؂��Ă���K�v������j
		@return	�ǉ������s�ԍ�
		*/
		//--------------------------------------------------------------------------------------
		size_t AddRow(wstring& Row);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w�肵��1�s��ύX����.<br />
		���K�������X�V�O�Ɨ�̐��͓����łȂ��Ă��悢�B<br />
		���w�肵���s�����݂��Ȃ��ꍇ�A��O�𔭐�����<br />
		���V�[����X�e�[�W�̏I�����Ƀf�X�g���N�^�ŕۑ��������ł���悤�ɁA��O������I�ׂ�
		@param[in]	RowNum	�ύX�������s
		@param[in]	Row	Csv��1�s�i�e�Z���͔z�񉻂���Ă���K�v������j
		@param[in]	Exp = true	��O�������邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateRow(size_t RowNum, vector<wstring>& Row, bool Exp = true);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �w�肵��1�s��ύX����.
		��Row�́u,�v�ɂ���ċ�؂��z�񉻂���ۑ������<br />
		���K�������X�V�O�Ɨ�̐��͓����łȂ��Ă��悢�B<br />
		���w�肵���s�����݂��Ȃ��ꍇ�A��O�𔭐�����<br />
		���V�[����X�e�[�W�̏I�����Ƀf�X�g���N�^�ŕۑ��������ł���悤�ɁA��O������I�ׂ�
		@param[in]	RowNum	�ύX�������s
		@param[in]	Row	Csv��1�s�i������́A�u,�v�ŋ�؂��Ă���K�v������j
		@param[in]	Exp = true	��O�������邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateRow(size_t RowNum, wstring& Row, bool Exp = true);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1�̃Z����ݒ肷��.
		@param[in]	RowNum	�ύX�s
		@param[in]	ColNum	�ύX��
		@param[in]	Cell,	�����ւ��镶����
		@param[in]	Exp = true	��O�������邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateCell(size_t RowNum, size_t ColNum, wstring& Cell, bool Exp = true);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s���𓾂�.
		@return	�s��
		*/
		//--------------------------------------------------------------------------------------
		size_t GetRowCount() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�s���̔z��𓾂�.<br />
		��RowNum���z��̏���𒴂����ꍇ�͗�O<br />
		���󂯎�镶����̔z�񂪋󔒂̏�Ԃ̂��Ƃ�����
		@param[in]	RowNum	�󂯎�肽���s
		@param[out]	Row	1�s���󂯎�镶����̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetRowVec(size_t RowNum, vector<wstring>& Row);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�̃Z���𓾂�.<br />
		��RowNum�܂���ColNum���z��̏���𒴂����ꍇ�͗�O<br/>
		���󂯎�镶���񂪋󔒂̏�Ԃ̂��Ƃ�����
		@param[in]	RowNum	�󂯎�肽���s
		@param[in]	ColNum	�󂯎�肽����
		@return	�Z���̒��g
		*/
		//--------------------------------------------------------------------------------------
		wstring GetCell(size_t RowNum, size_t ColNum);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������񂪏����L�[�Ɠ����s�𔲏o��������̔z��ɕԂ�.<br />
		��RetVec�̓N���A�����<br />
		���󂯎�镶����̔z�񂪋󔒂̏�Ԃ̂��Ƃ�����<br />
		���߂�l�̊e�s�́A�f���~�^�ɋ�؂�ꂽ������Ȃ̂�<br />
		Util::WStrToTokenVector�Ȃǂōď�������
		@param[out]	RetVec	���ʂ��󂯎�镶����̔z��
		@param[in]	ColNum	���������������
		@param[in]	Key	�����L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetSelect(vector< wstring >& RetVec, size_t ColNum, const wstring& Key);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R�[���o�b�N�֐����Ăяo���Atrue���Ԃ����s�𔲏o��������̔z��ɕԂ�.<br />
		��RetVec�̓N���A�����<br />
		���󂯎�镶����̔z�񂪋󔒂̏�Ԃ̂��Ƃ�����<br />
		���߂�l�̊e�s�́A�f���~�^�ɋ�؂�ꂽ������Ȃ̂�<br />
		Util::WStrToTokenVector�Ȃǂōď�������
		@param[out]	RetVec	���ʂ��󂯎�镶����̔z��
		@param[in]	(Func)(const wstring&)	��������������R�[���o�b�N�֐�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetSelect(vector< wstring >& RetVec,bool (Func)(const wstring&));
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����_�����Ăяo���Atrue���Ԃ����s�𔲏o��������̔z��ɕԂ�.<br />
		��RetVec�̓N���A�����<br />
		���󂯎�镶����̔z�񂪋󔒂̏�Ԃ̂��Ƃ�����<br />
		���߂�l�̊e�s�́A�f���~�^�ɋ�؂�ꂽ������Ȃ̂�<br />
		Util::WStrToTokenVector�Ȃǂōď�������
		@tparam	Fct	�����_���̌^
		@param[out]	RetVec	���ʂ��󂯎�镶����̔z��
		@param[in]	f	�������������郉���_��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename Fct>
		void GetSelect2(vector< wstring >& RetVec, Fct f){
			//�n���ꂽ�z��ɒl�������Ă�ꍇ������̂ŃN���A
			RetVec.clear();
			auto& CsvVec = GetCsvVec();
			size_t sz = CsvVec.size();
			for (size_t i = 0; i < sz; i++){
				//�R�[���o�b�N�֐����Ăяo��
				if (f(CsvVec[i])){
					//����������Βǉ�
					RetVec.push_back(CsvVec[i]);
				}
			}
		}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		CsvFile(const CsvFile&) = delete;
		CsvFile& operator=(const CsvFile&) = delete;
		//���[�u�֎~
		CsvFile(const CsvFile&&) = delete;
		CsvFile& operator=(const CsvFile&&) = delete;
	};


	//--------------------------------------------------------------------------------------
	/*!
	@brief	��r�̃G���[�̕�������쐬����.
	@tparam	T	��r����^
	@param[in]	key1	�L�[1��
	@param[in]	val1	�l�P
	@param[in]	key2	�L�[�Q��
	@param[in]	val2	�l2
	@return	�G���[������
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	wstring MakeRangeErr(const wchar_t* key1, T val1, const wchar_t* key2, T val2){
		//�Ԃ�������
		wstring str;
		//�����𐮂���X�g���[��
		wostringstream stream;
		stream << key1 << L"==" << val1 << L"," << key2 << L"==" << val2;
		str = stream.str();
		return str;
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	shared_ptr��void�^�ɕϊ�����.
	@tparam	T	shared_ptr�^
	@param[in]	SrcPtr	�ϊ���
	@return	void�^��shared_ptr
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<void> SharedToVoid(const shared_ptr<T>& SrcPtr){
		shared_ptr<void> RetPtr = static_pointer_cast<void>(SrcPtr);
		return RetPtr;
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	void�^��shared_ptr��T�^��shared_ptr�ɕϊ�����.
	@tparam	T	shared_ptr�^
	@param[in]	SrcPtr	�ϊ���
	@return	T�^��shared_ptr
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<T> VoidToShared(const shared_ptr<void>& SrcPtr){
		shared_ptr<T> RetPtr = static_pointer_cast<T>(SrcPtr);
		return RetPtr;
	}
	//--------------------------------------------------------------------------------------
	///	���̑��̃��[�e�B���e�B�Bstatic�Ăяo��������
	//--------------------------------------------------------------------------------------
	struct Util{
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�傫���������߂�.
		@tparam	T	��r����^
		@param[in]	v1	T�^�̒l1
		@param[in]	v2	T�^�̒l2
		@return	T�^�̑傫����
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T Maximum(const T& v1, const T& v2){
			return v1 > v2 ? v1 : v2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�������������߂�.
		@tparam	T	��r����^
		@param[in]	v1	T�^�̒l1
		@param[in]	v2	T�^�̒l2
		@return	T�^�̏�������
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T Minimum(const T& v1, const T& v2){
			return v1 < v2 ? v1 : v2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S��COM�C���^�[�t�F�C�X�̃|�C���^���擾����w���p�[�֐�.
		@tparam	T	�擾����^
		@tparam	TCreateFunc	�쐬�֐��̌^
		@param[in,out]	comPtr	COM�|�C���^
		@param[in]	mutex	�~���[�e�b�N�X
		@param[in]	createFunc	�쐬�֐�
		@return	COM�������̏ꍇ�͐V�����쐬���āA����ȊO��COM���瓾��COM�C���^�[�t�F�C�X�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename TCreateFunc>
		static T* DemandCreate(ComPtr<T>& comPtr, std::mutex& mutex, TCreateFunc createFunc)
		{
			T* result = comPtr.Get();
			//���b�N��Ԃ��`�F�b�N
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

		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S��COM�C���^�[�t�F�C�X�̃|�C���^���擾����w���p�[�֐�.<br />
		�����̏ꍇ�͗�O���o��
		@tparam	T	�擾����^
		@param[in]	comPtr	COM�|�C���^
		@return�@COM�������̏ꍇ�͗�O���o���āA����ȊO��COM���瓾��COM�C���^�[�t�F�C�X�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T* GetComPtr(ComPtr<T>& comPtr){
			T* result = comPtr.Get();
			//���b�N��Ԃ��`�F�b�N
			MemoryBarrier();
			if (!result)
			{
				//���s
				throw BaseException(
					L"����COM�C���^�[�t�F�C�X���擾�ł��܂���",
					Util::GetWSTypeName<T>(),
					L"Util::GetComPtr()"
					);
			}
			return result;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[����󔒂���菜��<br/>
		���ɂȂ镶���񂩂��菜��
		@param[in,out]	wstr	���ƂɂȂ镶����i���^�[���ɂ��g�p�����j
		@param[in]	trimCharacterList = L" \t\v\r\n"	��菜������
		@return�@�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void WStrTrim(wstring& wstr, const wchar_t* TrimCharList = L" \t\v\r\n"){
			wstring result(L"");
			if (wstr.size() <= 0){
				wstr = result;
				return;
			}
			wstring::size_type left = wstr.find_first_not_of(TrimCharList);
			if (left != wstring::npos){
				wstring::size_type right = wstr.find_last_not_of(TrimCharList);
				result = wstr.substr(left, right - left + 1);
			}
			wstr = result;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�h�����񂩂�}���`�o�C�g������ϊ�<br />
		���P�[���ˑ��̂��߁AWinMain()���ŁAsetlocale( LC_ALL, "JPN" );���K�v
		@param[in] src	�ϊ����镶����i���C�h�L�����j
		@param[out]	dest	�ϊ���̕�����i�}���`�o�C�g�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void WStoMB(const wstring& src, string& dest){
			size_t i;
			char *pMBstr = new char[src.length() * MB_CUR_MAX + 1];
			wcstombs_s(
				&i,
				pMBstr,
				src.length() * MB_CUR_MAX + 1,
				src.c_str(),
				_TRUNCATE	//���ׂĕϊ��ł��Ȃ�������؂�̂�
				);
			dest = pMBstr;
			delete[] pMBstr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�h�����񂩂�}���`�o�C�gUTF8������ϊ�<br />
		���P�[���ˑ��̂��߁AWinMain()���ŁAsetlocale( LC_ALL, "JPN" );���K�v
		@param[in]	src	�ϊ����镶����i���C�h�L�����j
		@param[out]	dest	�ϊ���̕�����i�}���`�o�C�g�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertWstringtoUtf8(const wstring& src, string& dest){
			INT bufsize = ::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
			char* Temp = new char[bufsize + 1];
			::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, Temp, bufsize, NULL, NULL);
			dest = Temp;
			delete[] Temp;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`�o�C�gUTF8�������烏�C�h������ϊ�<br />
		���P�[���ˑ��̂��߁AWinMain()���ŁAsetlocale( LC_ALL, "JPN" );���K�v
		@param[in]	src	�ϊ����镶����i�}���`�o�C�gUTF8�j
		@param[out]	dest	�ϊ��㕶����i���C�h�L�����j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertUtf8toWstring(const string& src, wstring& dest){
			INT bufsize = ::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, (wchar_t *)NULL, 0);
			wchar_t* Temp = (wchar_t*)new wchar_t[bufsize];
			::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, Temp, bufsize);
			dest = Temp;
			delete[] Temp;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`�o�C�g�����񂩂烏�C�h������ϊ�<br />
		���P�[���ˑ��̂��߁AWinMain()���ŁAsetlocale( LC_ALL, "JPN" );���K�v
		@param[in]	src	�ϊ����镶����i�}���`�o�C�g�j
		@param[out]	dest	�ϊ���̕�����i���C�h�L�����j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void MBtoWS(const string& src, wstring& dest){
			size_t i;
			wchar_t* WCstr = new wchar_t[src.length() + 1];
			mbstowcs_s(
				&i,
				WCstr,
				src.length() + 1,
				src.c_str(),
				_TRUNCATE //���ׂĕϊ��ł��Ȃ�������؂�̂�
				);
			dest = WCstr;
			delete[] WCstr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	��������f���~�^�����ɂ���؂��Ĕz��ɕۑ�
		@param[out]	wstrvec	�ۑ������z��
		@param[in]	line	���ƂȂ镶����
		@param[in]	delimiter	�f���~�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void WStrToTokenVector(vector<wstring>& wstrvec, const wstring& line, wchar_t delimiter){
			wstring::size_type i = 0;
			wstring wks(L"");
			for (i = 0; i < line.size(); i++){
				if (line[i] == delimiter){
					if (wks != L""){
						wstrvec.push_back(wks);
					}
					wks = L"";
				}
				else{
					wks += line[i];
				}
			}
			wstrvec.push_back(wks);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	T�^�̓����@��ʖ��𓾂�iwstring�j
		@tparam T	�^���𓾂�^
		@return	T�^�̓����@��ʖ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static wstring GetWSTypeName(){
			wstring clsname;
			MBtoWS(typeid(T).name(), clsname);
			return clsname;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T�^�̓����@��ʖ��𓾂�istring�j
		@tparam T	�^���𓾂�^
		@return	T�^�̓����@��ʖ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static string GetMBTypeName(){
			string clsname = typeid(T).name();
			return clsname;
		}

		//--------------------------------------------------------------------------------------
		/// float�^�𕶎���ɕϊ�����ꍇ�̌`��
		//--------------------------------------------------------------------------------------
		enum FloatModify{
			Default = 0,	///< �f�t�H���g�i���������_�j
			Fixed,	///< �������o��
			Scientific,	///< e+09�Ȃǂ̌`��
		};

		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������_�𕶎���ɕϊ�����iwstring�Łj
		@param[in]	Val	���������_�l
		@param[in]	Precision = 0	���x0�Ńf�t�H���g
		@param[in]	Modify = FloatModify::Default	�\���`��
		@return	�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static wstring FloatToWStr(float Val, streamsize Precision = 0,
			FloatModify Modify = FloatModify::Default){
			//�Ԃ�������
			wstring str;
			//�����𐮂���X�g���[��
			wostringstream stream;
			//���������_�̐��x�����߂�
			if (Precision > 0){
				stream.precision(Precision);
			}
			switch (Modify){
			case FloatModify::Fixed:
				stream << std::fixed << Val;
				break;
			case FloatModify::Scientific:
				stream << std::scientific << Val;
				break;
			case FloatModify::Default:
			default:
				stream << Val;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������_�𕶎���ɕϊ�����istring�Łj
		@param[in]	Val	���������_�l
		@param[in]	Precision = 0	���x0�Ńf�t�H���g
		@param[in]	Modify = FloatModify::Default	�\���`��
		@return	�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static string FloatToStr(float Val, streamsize Precision = 0,
			FloatModify Modify = FloatModify::Default){
			//�Ԃ�������
			string str;
			//�����𐮂���X�g���[��
			ostringstream stream;
			//���������_�̐��x�����߂�
			if (Precision > 0){
				stream.precision(Precision);
			}
			switch (Modify){
			case FloatModify::Fixed:
				stream << std::fixed << Val;
				break;
			case FloatModify::Scientific:
				stream << std::scientific << Val;
				break;
			case FloatModify::Default:
			default:
				stream << Val;
				break;
			}
			str = stream.str();
			return str;
		}


		//--------------------------------------------------------------------------------------
		/// �����^�𕶎���ɕϊ�����ꍇ�̌`��
		//--------------------------------------------------------------------------------------
		enum NumModify{
			Dec = 0,	///< 10�i��
			Hex,	///< 16�i��
			Oct,	///< 8�i��
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	unsigned���l�𕶎���ɕϊ�����iwstring�Łj
		@param[in]	num	unsigned���l
		@param[in]	Modify = NumModify::Dec	�i��
		@return	�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static wstring UintToWStr(UINT num, NumModify Modify = NumModify::Dec){
			//�Ԃ�������
			wstring str;
			//�����𐮂���X�g���[��
			wostringstream stream;
			//�\���`�������߂�
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	size_t���l�𕶎���ɕϊ�����iwstring�Łj(UINT)�ɃL���X�g���邾���Ȃ̂ŁA32�r�b�g�𒴂���ꍇ�͒��ӂ��K�v
		@param[in]	num	unsigned���l
		@param[in]	Modify = NumModify::Dec	�i��
		@return	�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static wstring SizeTToWStr(size_t num, NumModify Modify = NumModify::Dec) {
			//�Ԃ�������
			wstring str;
			//�����𐮂���X�g���[��
			wostringstream stream;
			//�\���`�������߂�
			switch (Modify) {
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	unsigned���l�𕶎���ɕϊ�����istring�Łj
		@param[in]	num	unsigned���l
		@param[in]	Modify = NumModify::Dec	�i��
		@return	�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static string UintToStr(UINT num, NumModify Modify = NumModify::Dec){
			//�Ԃ�������
			string str;
			//�����𐮂���X�g���[��
			ostringstream stream;
			//�\���`�������߂�
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	unsigned���l�𕶎���ɕϊ�����istring�Łj(UINT)�ɃL���X�g���邾���Ȃ̂ŁA32�r�b�g�𒴂���ꍇ�͒��ӂ��K�v
		@param[in]	num	unsigned���l
		@param[in]	Modify = NumModify::Dec	�i��
		@return	�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static string SizeTToStr(UINT num, NumModify Modify = NumModify::Dec) {
			return UintToStr((UINT)num,Modify);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	signed���l�𕶎���ɕϊ�����iwstring�Łj
		@param[in]	num	signed���l
		@param[in]	Modify = NumModify::Dec	�i��
		@return	�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static wstring IntToWStr(int num, NumModify Modify = NumModify::Dec){
			//�Ԃ�������
			wstring str;
			//�����𐮂���X�g���[��
			wostringstream stream;
			//�\���`�������߂�
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	signed���l�𕶎���ɕϊ�����istring�Łj
		@param[in]	num	signed���l
		@param[in]	Modify = NumModify::Dec	�i��
		@return	�ϊ�����������
		*/
		//--------------------------------------------------------------------------------------
		static string IntToStr(int num, NumModify Modify = NumModify::Dec){
			//�Ԃ�������
			string str;
			//�����𐮂���X�g���[��
			ostringstream stream;
			//�\���`�������߂�
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Probability����1�̊m���ŁAtrue��Ԃ�<br />
		Probability��0���傫���Ȃ���΂Ȃ�Ȃ��i0���Z�͂ł��Ȃ��j
		@param[in]	Probability	�m���B������1�����w�肷��
		@return	Probability����1�̊m���ɂȂ��true
		*/
		//--------------------------------------------------------------------------------------
		static bool DivProbability(UINT Probability){
			if (Probability <= 0){
				return false;
			}
			double Prob = (double)rand() / (double)RAND_MAX;
			double Seed = 1.0 / (double)Probability;
			if (Prob <= Seed){
				return true;
			}
			return false;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	0����1.0f�̊Ԃ̗�����Ԃ�<br />
		ZeroOK��false�̏ꍇ�A0���Ԃ�\���͂Ȃ��i0.00001f�ɂȂ�j
		@param[in]	ZeroOK = false	0���Ԃ��Ă������ǂ����̎w��
		@return	0����1.0f�̊Ԃ̗���
		*/
		//--------------------------------------------------------------------------------------
		static float RandZeroToOne(bool ZeroOK = false){
			double Prob = (double)rand() / (double)RAND_MAX;
			if (Prob <= 0){
				if (!ZeroOK){
					Prob = 0.00001;
				}
				else{
					Prob = 0.0;
				}
			}
			return (float)Prob;
		}

	};

	//--------------------------------------------------------------------------------------
	/*!
	@breaf �����^�C����O��throw����iwstring�Łj
	@param[in] m1  ��1���b�Z�[�W�i�x�����j
	@param[in] m2  ��2���b�Z�[�W�i���ƂȂ����f�[�^�j
	@param[in] m3  ��3���b�Z�[�W�i���������ʒu�j
	@return	�Ȃ�
	*/
	//--------------------------------------------------------------------------------------
	inline void ThrowBaseException(const wstring& m1, const wstring& m2, const wstring& m3) {
		wstring AllMess = m1;
		AllMess += L"\r\n";
		AllMess += m2;
		AllMess += L"\r\n";
		AllMess += m3;
		string Dest;
		Util::WStoMB(AllMess, Dest);
		throw runtime_error(Dest);
	}
	//--------------------------------------------------------------------------------------
	/*!
	@breaf �����^�C����O��throw����istring�Łj
	@param[in] m1  ��1���b�Z�[�W�i�x�����j
	@param[in] m2  ��2���b�Z�[�W�i���ƂȂ����f�[�^�j
	@param[in] m3  ��3���b�Z�[�W�i���������ʒu�j
	@return	�Ȃ�
	*/
	//--------------------------------------------------------------------------------------
	inline void ThrowBaseException(const string& m1, const string& m2, const string& m3) {
		string AllMess = m1;
		AllMess += "\r\n";
		AllMess += m2;
		AllMess += "\r\n";
		AllMess += m3;
		throw runtime_error(AllMess);
	}

	//--------------------------------------------------------------------------------------
	/*!
	@brief	T�^�̃|�C���^��null���������̃X���[�iwstring�Łj
	@tparam T	�^���𓾂�^
	@param[in]	ptr	���؂���|�C���^
	@param[in]	Str1	���b�Z�[�W1
	@param[in]	Str2	���b�Z�[�W2
	@param[in]	Str3	���b�Z�[�W3
	@return	T�^�̃|�C���^
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<T> ThrowIfNullFailed(const shared_ptr<T>& ptr, const wstring& Str1, const wstring& Str2, const wstring& Str3)
	{
		if (!ptr) {
			ThrowBaseException(
				Str1,
				Str2,
				Str3
			);
		}
		return ptr;
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	T�^�̃|�C���^��null���������̃X���[�istring�Łj
	@tparam T	�^���𓾂�^
	@param[in]	ptr	���؂���|�C���^
	@param[in]	Str1	���b�Z�[�W1
	@param[in]	Str2	���b�Z�[�W2
	@param[in]	Str3	���b�Z�[�W3
	@return	T�^�̃|�C���^
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<T> ThrowIfNullFailed(const shared_ptr<T>& ptr, const string& Str1, const string& Str2, const string& Str3)
	{
		if (!ptr) {
			ThrowBaseException(
				Str1,
				Str2,
				Str3
			);
		}
		return ptr;
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	T�^��weakptr���������������̃X���[�istring�Łj
	@tparam T	�^���𓾂�^
	@param[in]	SrcPtr	���؂���weakptr
	@return	T�^��shared_ptr
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<T> ThrowIfWeakToSharedFailed(const weak_ptr<T>& SrcPtr) {
		auto shptr = SrcPtr.lock();
		if (!shptr) {
			string Str = typeid(T).name();
			string Mess = Str + "�͕s��l�ɂȂ��Ă��܂��B";
			ThrowBaseException(
				"weak_ptr����shared_ptr�ւ̕ϊ��Ɏ��s���܂����B",
				Mess,
				"ThrowIfWeakToShared<T>()"
			);
		}
		return shptr;
	}





	typedef long long int64;
	typedef unsigned long long uint64;
	typedef unsigned int uint32;


	//--------------------------------------------------------------------------------------
	///	�X�e�b�v�^�C�}�[
	//--------------------------------------------------------------------------------------
	class StepTimer{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		StepTimer() :
			m_elapsedTicks(0),
			m_totalTicks(0),
			m_leftOverTicks(0),
			m_frameCount(0),
			m_framesPerSecond(0),
			m_framesThisSecond(0),
			m_qpcSecondCounter(0),
			m_isFixedTimeStep(false),
			m_targetElapsedTicks(TicksPerSecond / 60)
		{
			if (!QueryPerformanceFrequency(&m_qpcFrequency))
			{
				throw BaseException(
					L"�p�t�H�[�}���X�J�E���^�̎��g�����擾�ł��܂���B",
					L"if (!QueryPerformanceFrequency(&m_qpcFrequency))",
					L"StepTimer::StepTimer()"
					);
			}

			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw BaseException(
					L"�p�t�H�[�}���X�J�E���^���擾�ł��܂���B",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::StepTimer()"
					);
			}

			// �ő�f���^�� 1 �b�� 1/10 �ɏ��������܂��B
			m_qpcMaxDelta = m_qpcFrequency.QuadPart / 10;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�O�� Update �Ăяo������o�߂����J�E���^���擾.
		@return	�o�߃J�E���^
		*/
		//--------------------------------------------------------------------------------------
		uint64 GetElapsedTicks() const						{ return m_elapsedTicks; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief �O�� Update �Ăяo������o�߂������Ԃ��擾.
		@return	�o�ߎ���
		*/
		//--------------------------------------------------------------------------------------
		double GetElapsedSeconds() const					{ return TicksToSeconds(m_elapsedTicks); }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���O�����J�n����o�߂������v�J�E���^���擾.
		@return	���v�J�E���^
		*/
		//--------------------------------------------------------------------------------------
		uint64 GetTotalTicks() const						{ return m_totalTicks; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���O�����J�n����o�߂������v���Ԃ��擾.
		@return	���v����
		*/
		//--------------------------------------------------------------------------------------
		double GetTotalSeconds() const						{ return TicksToSeconds(m_totalTicks); }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���O�����J�n����̍��v�X�V�񐔂��擾.
		@return	���v�X�V��
		*/
		//--------------------------------------------------------------------------------------
		uint32 GetFrameCount() const						{ return m_frameCount; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃t���[�� ���[�g���擾.
		@return	���݂̃t���[�� ���[�g
		*/
		//--------------------------------------------------------------------------------------
		uint32 GetFramesPerSecond() const					{ return m_framesPerSecond; }

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Œ�܂��͉ς̂ǂ���̃^�C���X�e�b�v ���[�h���g�p���邩��ݒ�i�f�t�H���g�͉ρj.
		@param[in]	isFixedTimestep	�Œ胂�[�h�ɂ��邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetFixedTimeStep(bool isFixedTimestep)			{ m_isFixedTimeStep = isFixedTimestep; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Œ�^�C���X�e�b�v ���[�h�ŁAUpdate �̌Ăяo���p�x��ݒ�.
		@param[in]	targetElapsed	�Ăяo���p�x�i�J�E���^�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetElapsedTicks(uint64 targetElapsed)	{ m_targetElapsedTicks = targetElapsed; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Œ�^�C���X�e�b�v ���[�h�ŁAUpdate �̌Ăяo���p�x��ݒ�.
		@param[in]	targetElapsed	�Ăяo���p�x�i���ԁj
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetElapsedSeconds(double targetElapsed)	{ m_targetElapsedTicks = SecondsToTicks(targetElapsed); }
		// �����`���� 1 �b������ 10,000,000 �e�B�b�N���g�p���Ď��Ԃ�\���܂��B
		static const uint64 TicksPerSecond = 10000000;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�b�N�J�E���^�����ԂɊ��Z.
		@param[in]	ticks		�`�b�N�J�E���^
		@return	���Z���ꂽ����
		*/
		//--------------------------------------------------------------------------------------
		static double TicksToSeconds(uint64 ticks)			{ return static_cast<double>(ticks) / TicksPerSecond; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Ԃ��`�b�N�J�E���^�Ɋ��Z.
		@param[in]	seconds	����
		@return	���Z���ꂽ�`�b�N�J�E���^
		*/
		//--------------------------------------------------------------------------------------
		static uint64 SecondsToTicks(double seconds)		{ return static_cast<uint64>(seconds * TicksPerSecond); }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Ăяo���̍X�V<br />
		�Ӑ}�I�ȃ^�C�~���O�̕s�A�����̌� (�u���b�N IO ����Ȃ�)<br />
		������Ăяo���ƁA�Œ�^�C���X�e�b�v ���W�b�N�ɂ���Ĉ�A�̃L���b�`�A�b�v�����s�����̂�����ł���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ResetElapsedTime()
		{
			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw BaseException(
					L"�p�t�H�[�}���X�J�E���^���擾�ł��܂���B",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::ResetElapsedTime()"
					);
			}

			m_leftOverTicks = 0;
			m_framesPerSecond = 0;
			m_framesThisSecond = 0;
			m_qpcSecondCounter = 0;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�^�C�}�[��Ԃ��X�V���A�w��� Update �֐���K�؂ȉ񐔂����Ăяo��
		@tparam	�Ăяo���ׂ�Update�֐��^
		@param[in]	update	�Ăяo���ׂ�Update�֐�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			// ���݂̎������N�G�����܂��B
			LARGE_INTEGER currentTime;

			if (!QueryPerformanceCounter(&currentTime))
			{
				throw BaseException(
					L"�p�t�H�[�}���X�J�E���^���擾�ł��܂���B",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::ResetElapsedTime()"
					);
			}

			uint64 timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;

			m_qpcLastTime = currentTime;
			m_qpcSecondCounter += timeDelta;

			// �ɒ[�ɑ傫�Ȏ��ԍ� (�f�o�b�K�[�ňꎞ��~������Ȃ�)  ���N�����v���܂��B
			if (timeDelta > m_qpcMaxDelta)
			{
				timeDelta = m_qpcMaxDelta;
			}

			// QPC �P�ʂ�W���̖ڐ���`���ɕϊ����܂��B�O�̃N�����v�������������߁A���̕ϊ��ł̓I�[�o�[�t���[���s�\�ł��B
			timeDelta *= TicksPerSecond;
			timeDelta /= m_qpcFrequency.QuadPart;

			uint32 lastFrameCount = m_frameCount;

			if (m_isFixedTimeStep)
			{
				// �Œ�^�C���X�e�b�v�X�V���W�b�N

				// �N�����v�����^�[�Q�b�g�o�ߎ��� (1/4 �~���b�ȓ�) �ɐڋ߂��āA�A�v���P�[�V���������s����Ă���ꍇ
				// �^�[�Q�b�g�l�ɐ��m�Ɉ�v������N���b�N�B����ɂ��A���������֌W�ȃG���[�̔�����h���܂��B
				// �����ԓ��ɒ~�ς��ꂽ�B���̃N�����v�Ȃ��ŁA60 fps ��v�������Q�[����
				// �Œ肳�ꂽ�X�V (59.94 NTSC �f�B�X�v���C��ŗL���� vsync �Ŏ��s) �͍ŏI�I�ɂ́A
				// �t���[�����h���b�v����̂ɏ\���ȏ������G���[��~�ς��܂��B�ۂ߂邱�Ƃ������߂��܂�
				// �킸���ȕ΍����[���Ɏ����Ă����A�X���[�Y�ɐi�s����悤�ɂ��܂��B

				if (abs(static_cast<int64>(timeDelta - m_targetElapsedTicks)) < TicksPerSecond / 4000)
				{
					timeDelta = m_targetElapsedTicks;
				}

				m_leftOverTicks += timeDelta;

				while (m_leftOverTicks >= m_targetElapsedTicks)
				{
					m_elapsedTicks = m_targetElapsedTicks;
					m_totalTicks += m_targetElapsedTicks;
					m_leftOverTicks -= m_targetElapsedTicks;
					m_frameCount++;

					update();
				}
			}
			else
			{
				// �σ^�C���X�e�b�v�X�V���W�b�N�B
				m_elapsedTicks = timeDelta;
				m_totalTicks += timeDelta;
				m_leftOverTicks = 0;
				m_frameCount++;

				update();
			}

			// ���݂̃t���[�� ���[�g��ǐՂ��܂��B
			if (m_frameCount != lastFrameCount)
			{
				m_framesThisSecond++;
			}

			if (m_qpcSecondCounter >= static_cast<uint64>(m_qpcFrequency.QuadPart))
			{
				m_framesPerSecond = m_framesThisSecond;
				m_framesThisSecond = 0;
				m_qpcSecondCounter %= m_qpcFrequency.QuadPart;
			}
		}

	private:
		// �\�[�X �^�C�~���O �f�[�^�ł� QPC �P�ʂ��g�p���܂��B
		LARGE_INTEGER m_qpcFrequency;
		LARGE_INTEGER m_qpcLastTime;
		uint64 m_qpcMaxDelta;

		// �h���^�C�~���O �f�[�^�ł́A�W���̖ڐ���`�����g�p���܂��B
		uint64 m_elapsedTicks;
		uint64 m_totalTicks;
		uint64 m_leftOverTicks;

		// �t���[�� ���[�g�̒ǐ՗p�����o�[�B
		uint32 m_frameCount;
		uint32 m_framesPerSecond;
		uint32 m_framesThisSecond;
		uint64 m_qpcSecondCounter;

		// �Œ�^�C���X�e�b�v ���[�h�̍\���p�����o�[�B
		bool m_isFixedTimeStep;
		uint64 m_targetElapsedTicks;

		//�R�s�[�֎~
		StepTimer(const StepTimer&) = delete;
		StepTimer& operator=(const StepTimer&) = delete;
		//���[�u�֎~
		StepTimer(const StepTimer&&) = delete;
		StepTimer& operator=(const StepTimer&&) = delete;

	};


	//--------------------------------------------------------------------------------------
	///	2�����|�C���g
	/*!
	@tparam	T	�|�C���g����������^
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	struct Point2D{
		T x;
		T y;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Point2D() throw() :
			x{},
			y{}
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	initX	�����lX
		@param[in]	initY	�����lY
		*/
		//--------------------------------------------------------------------------------------
		Point2D(T initX, T initY) throw() :
			x{ initX },
			y{ initY }
		{
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���������ǂ�����ׂ�
		@param[in]	point	����
		@return	���������true
		*/
		//--------------------------------------------------------------------------------------
		bool operator==(Point2D<T> point) const throw(){
			return (x == point.x && y == point.y);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�������Ȃ����ǂ�����ׂ�
		@param[in]	point	����
		@return	�������Ȃ����true
		*/
		//--------------------------------------------------------------------------------------
		bool operator!=(Point2D<T> point) const throw(){
			return (x != point.x || y != point.y);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ǉ����Z
		@param[in]	point	����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void operator+=(Point2D<T> point) throw(){
			x += point.x; y += point.y;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ǉ����Z
		@param[in]	point	����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void operator-=(Point2D<T> point) throw(){
			x -= point.x; y -= point.y;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Z
		@param[in]	point	����
		@return	���Z��������
		*/
		//--------------------------------------------------------------------------------------
		Point2D operator+(Point2D<T> point) const throw(){
			return Point2D(x + point.x, y + point.y);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Z
		@param[in]	point	����
		@return	���Z��������
		*/
		//--------------------------------------------------------------------------------------
		Point2D operator-(Point2D<T> point) const throw(){
			return Point2DF(x - point.x, y - point.y);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�P���}�C�i�X
		@return	�}�C�i�X���|��������
		*/
		//--------------------------------------------------------------------------------------
		Point2D operator-() const throw(){
			return Point2DF(-x, -y);
		}
	};

	//--------------------------------------------------------------------------------------
	///	2������`
	/*!
	@tparam	T	��`����������^
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	struct Rect2D{
		T left;
		T top;
		T right;
		T bottom;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Rect2D() throw() :
			left{}, top{}, right{}, bottom{}
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	l	�����lLeft
		@param[in]	t	�����lTop
		@param[in]	r	�����lRight
		@param[in]	b	�����lBottom
		*/
		//--------------------------------------------------------------------------------------
		Rect2D(T l, T t, T r, T b)throw() :
			left{ l }, top{ t }, right{ r }, bottom{ b }
		{
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�󂩂ǂ������ׂ�
		@return	��ł����true
		*/
		//--------------------------------------------------------------------------------------
		bool IsRectEmpty()const throw(){
			if (left >= right || top >= bottom){
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���𓾂�
		@return	��
		*/
		//--------------------------------------------------------------------------------------
		T Width()const throw(){
			return abs(right - left);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����𓾂�
		@return	����
		*/
		//--------------------------------------------------------------------------------------
		T Height()const throw(){
			return abs(bottom - top);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Z�ړ�����
		@param[in]	point	�ړ����镪��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void operator+=(Point2D<T> point) throw(){
			left += point.x;
			right += point.x;
			top += point.y;
			bottom += point.y;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Z�ړ�����
		@param[in]	point	�ړ����镪��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void operator-=(Point2D<T> point) throw(){
			left -= point.x;
			right -= point.x;
			top -= point.y;
			bottom -= point.y;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Z����
		@param[in]	point	���Z���镪��
		@return	���Z��̋�`
		*/
		//--------------------------------------------------------------------------------------
		Rect2D<T> operator+(Point2D<T> point) const throw(){
			Rect2D<T> rect(*this);
			rect += point;
			return rect;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���Z����
		@param[in]	point	���Z���镪��
		@return	���Z��̋�`
		*/
		//--------------------------------------------------------------------------------------
		Rect2D<T> operator-(Point2D<T> point) const throw(){
			Rect2D<T> rect(*this);
			rect -= point;
			return rect;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�|�C���g���܂܂�邩�ǂ����𒲂ׂ�
		@param[in]	point	���ׂ�|�C���g
		@return	�܂܂�Ă����true
		*/
		//--------------------------------------------------------------------------------------
		bool PtInRect(Point2D<T> point) const throw(){
			if (point.x >= left && point.x < right
				&& point.y >= top && point.y < bottom){
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S�_�𓾂�
		@return	���S�̃|�C���g
		*/
		//--------------------------------------------------------------------------------------
		Point2D<T> CenterPoint() const throw(){
			return Point2D<T>((left + right) / 2.0f, (top + bottom) / 2.0f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���S�_�����ƂɃX�P�[�����O
		@param[in]	l	Left�ω��l
		@param[in]	t	Top�ω��l
		@param[in]	r	Right�ω��l
		@param[in]	b	Bottom�ω��l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void InflateRect(T l, T t, T r, T b){
			left -= l;			top -= t;
			right += r;			bottom += b;
		}
	};




	class ObjectFactory;
	class ObjectInterface;

	//--------------------------------------------------------------------------------------
	///	�C�x���g�\����
	//--------------------------------------------------------------------------------------
	struct Event {
		///	�x�����ԁiSendEvent�̏ꍇ�͏��0�j
		float m_DispatchTime;
		///	���̃��b�Z�[�W�𑗂����I�u�W�F�N�g
		weak_ptr<ObjectInterface> m_Sender;
		///	�󂯎��I�u�W�F�N�g�inullptr�̏ꍇ�̓A�N�e�B�u�X�e�[�W�����ׂĂ������̓L�[���[�h�Ŏ��ʂ���I�u�W�F�N�g�j
		weak_ptr<ObjectInterface> m_Receiver;
		///	���b�Z�[�W������
		wstring m_MsgStr;
		///	�ǉ��������I�u�W�F�N�g�̃|�C���^
		shared_ptr<void> m_Info;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	DispatchTime	�z���܂ł̎���
		@param[in]	Sender	���葤�I�u�W�F�N�g�inullptr�j
		@param[in]	Receiver	�󂯎葤�I�u�W�F�N�g
		@param[in]	MsgStr	���b�Z�[�W������
		@param[in]	Info	�ǉ����������[�U�[�f�[�^
		*/
		//--------------------------------------------------------------------------------------
		Event(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
			const wstring& MsgStr,const shared_ptr<void>& Info = shared_ptr<void>()) :
			m_DispatchTime(DispatchTime),
			m_Sender(Sender),
			m_Receiver(Receiver),
			m_MsgStr(MsgStr),
			m_Info(Info)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		~Event() {}
	};



	//--------------------------------------------------------------------------------------
	///	Create��PreCreate�������AThis�X�}�[�g�|�C���^���Ƃ��C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class ObjectInterface : public std::enable_shared_from_this<ObjectInterface> {
		friend class ObjectFactory;
		//�N���G�C�g�ς݂��ǂ���
		//Create�֐����Ăяo�����true�ɂȂ�
		bool m_Created{ false };
		void SetCreated(bool b) {
			m_Created = b;
		}
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �v���e�N�g�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		ObjectInterface(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �v���e�N�g�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ObjectInterface() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief this�|�C���^�擾
		@tparam T	this�|�C���^�̌^
		@return	this�|�C���^�ishared_ptr�j
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		std::shared_ptr<T> GetThis() {
			auto Ptr = dynamic_pointer_cast<T>(shared_from_this());
			if (Ptr) {
				return Ptr;
			}
			else {
				wstring str(L"this��");
				str += Util::GetWSTypeName<T>();
				str += L"�^�ɃL���X�g�ł��܂���";
				throw BaseException(
					str,
					L"if( ! dynamic_pointer_cast<T>(shared_from_this()) )",
					L"ObjectInterface::GetThis()"
					);
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �O���������s���i���z�֐��j<br />
		��this�|�C���^���K�v�ȃI�u�W�F�N�g�͂��̊֐��𑽏d��`���āA�擾�ł���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���������s���i���z�֐��j<br />
		��this�|�C���^���K�v�ȃI�u�W�F�N�g�͂��̊֐��𑽏d��`���āA�擾�ł���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �N���G�C�g�ς݂��ǂ���
		@return	�N���G�C�g�ς݂Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsCreated()const {
			return m_Created;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C�x���g��POST�i�L���[�ɓ����j
		@param[in]	DispatchTime	POST���鎞�ԁi0�Ŏ��̃^�[���j
		@param[in]	Sender	�C�x���g���M�ҁinullptr�j
		@param[in]	Receiver	�C�x���g��M�ҁinullptr�s�j
		@param[in]	MsgStr	���b�Z�[�W
		@param[in,out]	Info	�ǉ����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void PostEvent(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
			const wstring& MsgStr, const shared_ptr<void>& Info = shared_ptr<void>());
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C�x���g��POST�i�L���[�ɓ����j
		@param[in]	DispatchTime	POST���鎞�ԁi0�Ŏ��̃^�[���j
		@param[in]	Sender	�C�x���g���M�ҁinullptr�j
		@param[in]	ReceiverKey	�󂯎葤�I�u�W�F�N�g�𔻕ʂ���L�[
		@param[in]	MsgStr	���b�Z�[�W
		@param[in,out]	Info	�ǉ����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void PostEvent(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
			const wstring& MsgStr, const  shared_ptr<void>& Info = shared_ptr<void>());

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C�x���g��SEND�i�L���[�ɓ��ꂸ�ɂ��̂܂ܑ���j
		@param[in]	Sender	�C�x���g���M�ҁinullptr�j
		@param[in]	ReceiverKey	�󂯎葤�I�u�W�F�N�g�𔻕ʂ���L�[
		@param[in]	MsgStr	���b�Z�[�W
		@param[in,out]	Info	�ǉ����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SendEvent(const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
			const wstring& MsgStr, const shared_ptr<void>& Info = shared_ptr<void>());
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C�x���g��SEND�i�L���[�ɓ��ꂸ�ɂ��̂܂ܑ���j
		@param[in]	Sender	�C�x���g���M�ҁinullptr�j
		@param[in]	Receiver	�C�x���g��M�ҁinullptr�s�j
		@param[in]	MsgStr	���b�Z�[�W
		@param[in,out]	Info	�ǉ����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SendEvent(const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
			const wstring& MsgStr, const  shared_ptr<void>& Info = shared_ptr<void>());
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C�x���g���󂯎��
		@param[in]	event	�C�x���g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) {}
	private:
		//�R�s�[�֎~
		ObjectInterface(const ObjectInterface&) = delete;
		ObjectInterface& operator=(const ObjectInterface&) = delete;
		//���[�u�֎~
		ObjectInterface(const ObjectInterface&&) = delete;
		ObjectInterface& operator=(const ObjectInterface&&) = delete;
	};

	class Stage;
	//--------------------------------------------------------------------------------------
	///	Object�C���^�[�t�F�C�X�̔h���N���X���\�z����
	//--------------------------------------------------------------------------------------
	class ObjectFactory {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �I�u�W�F�N�g�쐬�istatic�֐��j
		@tparam T	�쐬����^
		@tparam Ts...	�ϒ��p�����[�^�^
		@param[in]	params	�ϒ��p�����[�^
		@return�@�쐬�����I�u�W�F�N�g��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		static shared_ptr<T> Create(Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T(params...));
			//���z�֐��Ăяo��
			Ptr->OnPreCreate();
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �I�u�W�F�N�g�쐬�istatic�֐��j<br/>
		�R���X�g���N�^�ł͂Ȃ��AOnCreateWithParam()�֐��Ƀp�����[�^��n���ꍇ
		@tparam T	�쐬����^
		@tparam Ts...	�ϒ��p�����[�^�^
		@param[in]	params	�ϒ��p�����[�^
		@return�@�쐬�����I�u�W�F�N�g��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		static shared_ptr<T> CreateWithParam(Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T());
			//���z�֐��Ăяo��
			Ptr->OnPreCreate();
			Ptr->OnCreateWithParam(params...);
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �Q�[���I�u�W�F�N�g�쐬�istatic�֐��j<br/>
		�R���X�g���N�^�ł͂Ȃ��AOnCreateWithParam()�֐��Ƀp�����[�^��n���ꍇ
		@tparam T	�쐬����^
		@tparam Ts...	�ϒ��p�����[�^�^
		@param[in]	StagePtr	�X�e�[�W��shared_ptr
		@param[in]	params	�ϒ��p�����[�^
		@return�@�쐬�����I�u�W�F�N�g��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		static shared_ptr<T> CreateGameObjectWithParam(const shared_ptr<Stage>& StagePtr, Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T(StagePtr));
			//���z�֐��Ăяo��
			Ptr->OnPreCreate();
			Ptr->OnCreateWithParam(params...);
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	///	Update��Draw�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class ShapeInterface {
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		ShapeInterface() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShapeInterface() {}
		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief OnUpdate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief OnUpdate2����.<br />
		���A�b�v�f�[�g�㏈��
		���������z�֐��ɂ��Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate2() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�j�������Ƃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDestroy() {}
	private:
		//�R�s�[�֎~
		ShapeInterface(const ShapeInterface&) = delete;
		ShapeInterface& operator=(const ShapeInterface&) = delete;
		//���[�u�֎~
		ShapeInterface(const ShapeInterface&&) = delete;
		ShapeInterface& operator=(const ShapeInterface&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	///	�V�[���C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class SceneInterface :public ObjectInterface, public ShapeInterface {
	protected:
		SceneInterface() {}
		virtual ~SceneInterface() {}
	};

	//--------------------------------------------------------------------------------------
	///	��������ɕێ����郊�\�[�X�̐e�N���X
	//--------------------------------------------------------------------------------------
	class BaseResource : public ObjectInterface {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�R���X���g�N�^
		*/
		//--------------------------------------------------------------------------------------
		BaseResource() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BaseResource() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �O���������s���i���z�֐��j<br />
		��this�|�C���^���K�v�ȃI�u�W�F�N�g�͂��̊֐��𑽏d��`���āA�擾�ł���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���������s���i���z�֐��j<br />
		��this�|�C���^���K�v�ȃI�u�W�F�N�g�͂��̊֐��𑽏d��`���āA�擾�ł���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
	};



	template <typename T>
	class StateMachine;


	//--------------------------------------------------------------------------------------
	///	�X�e�[�g�����e���v���[�g�N���X(���ۃN���X)
	/*!
	@tparam T	�I�[�i�[�̌^
	*/
	//--------------------------------------------------------------------------------------
	template <typename T>
	class ObjState{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		ObjState(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ObjState(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�ɓ������Ƃ��Ɏ��s�����
		@param[in]	Obj	�X�e�[�g��ێ�����I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Enter(const shared_ptr<T>& Obj) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Update�̂Ƃ��Ɏ��s�����
		@param[in]	Obj	�X�e�[�g��ێ�����I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Execute(const shared_ptr<T>& Obj) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g���o��Ƃ��Ɏ��s�����
		@param[in]	Obj	�X�e�[�g��ێ�����I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Exit(const shared_ptr<T>& Obj) = 0;
		//�ȉ��A�K�w���ɂ��ǉ����\�b�h�i�������z�֐��ɂ��Ȃ��j
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Update2�̂Ƃ��Ɏ��s�����
		@param[in]	Obj	�X�e�[�g��ێ�����I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Execute2(const shared_ptr<T>& Obj) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��ɃX�e�[�g��push���ꂽ�Ƃ��ɌĂ΂��
		@param[in]	Obj	�X�e�[�g��ێ�����I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Sleep(const shared_ptr<T>& Obj) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	��̃X�e�[�g��pop���ꂽ�Ƃ��ɌĂ΂��
		@param[in]	Obj	�X�e�[�g��ێ�����I�u�W�F�N�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void WakeUp(const shared_ptr<T>& Obj) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C�x���g�����s���ꂽ�Ƃ��ɌĂ�
		@param[in]	Message	���b�Z�[�W������
		@param[in]	pContext	�ėp�I�ȃ|�C���^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const wstring& Message,void* pContext) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g���𓾂�
		@param[in]	�X�e�[�g��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual wstring GetStateName()const { return L""; }
	};

	//--------------------------------------------------------------------------------------
	///	�X�e�[�g�}�V�������e���v���[�g�N���X(���ۃN���X)
	/*!
	@tparam T	�I�[�i�[�̌^
	*/
	//--------------------------------------------------------------------------------------
	template <typename T>
	class StateMachine
	{
	private:
		//���̃X�e�[�g�}�V�������I�[�i�[
		weak_ptr<T> m_Owner;
		//���݂̃X�e�[�g
		weak_ptr< ObjState<T> > m_CurrentState;
		//��O�̃X�e�[�g
		weak_ptr< ObjState<T> > m_PreviousState;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	owner	�I�[�i�[�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		explicit StateMachine(const shared_ptr<T>& owner) :
			m_Owner(owner)
			//m_CurrentState(nullptr), m_PreviousState(nullptr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~StateMachine(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�����g�X�e�[�g��ݒ肷��
		@param[in]	s	�ݒ肷��X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetCurrentState(const shared_ptr< ObjState<T> >& s){ m_CurrentState = s; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�O�̃X�e�[�g��ݒ肷��
		@param[in]	s	�ݒ肷��X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPreviousState(const shared_ptr< ObjState<T> >& s){ m_PreviousState = s; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�����g�X�e�[�g�𓾂�
		@return	�J�����g�X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< ObjState<T> >  GetCurrentState() const {
			auto shptr = m_CurrentState.lock();
			if (shptr){
				return shptr;
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�O�̃X�e�[�g�𓾂�
		@return	1�O�̃X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< ObjState<T> >  GetPreviousState()const {
			auto shptr = m_pPreviousState.lock();
			if (shptr){
				return shptr;
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g���X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Update() const{
			auto shptr = m_CurrentState.lock();
			auto ow_shptr = m_Owner.lock();
			if (shptr && ow_shptr){
				shptr->Execute(ow_shptr);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g���X�V2����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Update2() const {
			auto shptr = m_CurrentState.lock();
			auto ow_shptr = m_Owner.lock();
			if (shptr && ow_shptr) {
				shptr->Execute2(ow_shptr);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g��ύX����
		@param[in]	NewState	�ݒ肷��X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void  ChangeState(const shared_ptr< ObjState<T> >& NewState){
			//���̃X�e�[�g��ۑ�
			m_PreviousState = m_CurrentState;
			auto shptr = m_CurrentState.lock();
			auto ow_shptr = m_Owner.lock();
			if (shptr && ow_shptr){
				//���̃X�e�[�g�ɏI����m�点��
				shptr->Exit(ow_shptr);
			}
			//�V�����X�e�[�g���J�����g�ɐݒ�
			m_CurrentState = NewState;
			shptr = m_CurrentState.lock();
			if (shptr && ow_shptr){
				//���̃X�e�[�g�ɏI����m�点��
				shptr->Enter(ow_shptr);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g����O�̃X�e�[�g�ɖ߂�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void RevertToPreviousState(){
			ChangeState(m_pPreviousState);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�����g�X�e�[�g���w�肵���X�e�[�g�ɂȂ��Ă邩�ǂ������`�F�b�N
		@param[in]	st	���ׂ�X�e�[�g
		@return	�w�肵���X�e�[�g�ɂȂ��Ă��true
		*/
		//--------------------------------------------------------------------------------------
		bool IsInState(const shared_ptr< ObjState<T> >& st)const{
			auto shptr = m_CurrentState.lock();
			if (!shptr){
				return false;
			}
			return typeid(shptr) == typeid(st);
		}
	};




	//--------------------------------------------------------------------------------------
	///	�K�w���X�e�[�g�}�V�������e���v���[�g�N���X(���ۃN���X)
	/*!
	@tparam T	�I�[�i�[�̌^
	*/
	//--------------------------------------------------------------------------------------
	template <typename T>
	class LayeredStateMachine
	{
	private:
		//���̍s���}�V�������I�[�i�[
		weak_ptr<T> m_Owner;
		stack<shared_ptr< ObjState<T> >> m_StateStack;
		size_t m_MaxStack;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	owner	�I�[�i�[�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		explicit LayeredStateMachine(const shared_ptr<T>& owner) :
			m_Owner(owner),
			m_MaxStack(10)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~LayeredStateMachine() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃g�b�v�X�e�[�g�𓾂�
		@return	�g�b�v�X�e�[�g�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< ObjState<T> > GetTopState() const {
			if (m_StateStack.empty()) {
				return nullptr;
			}
			else {
				return m_StateStack.top();
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�^�b�N�̍ő�l�𓾂�
		@return	�X�^�b�N�̍ő吔
		*/
		//--------------------------------------------------------------------------------------
		size_t GetMaxStack() const {
			return m_MaxStack;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�^�b�N�̍ő�l��ݒ肷��
		@param[in]	s	�X�^�b�N�̍ő吔
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMaxStack(size_t s) {
			if (m_StateStack.size() >= s) {
				throw BaseException(
					L"�V�����ݒ�̃X�^�b�N�������łɒ����Ă��܂��B",
					L"�X�^�b�N�������炳�Ȃ��ł�������",
					L"LayeredStateMachine::SetMaxStack()"
				);
			}
			m_MaxStack = s;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g��push���ĊJ�n����
		@param[in]	Ptr	�V�����X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Push(const shared_ptr< ObjState<T> >& Ptr) {
			auto ow_shptr = m_Owner.lock();
			if (ow_shptr) {
				if (m_StateStack.size() >= m_MaxStack) {
					throw BaseException(
						L"����ȏ�s���X�^�b�N�ɒǉ��ł��܂���B",
						L"�ǉ�����ꍇ��SetMaxStack()�֐��ő��₵�Ă�������",
						L"LayeredStateMachine::Push()"
					);
				}
				if (!m_StateStack.empty()) {
					m_StateStack.top()->Sleep(ow_shptr);
				}
				m_StateStack.push(Ptr);
				Ptr->Enter(ow_shptr);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g��pop���đO�̃X�e�[�g�ɖ߂�Bpop�̌��ʁA�s������ɂȂ邱�Ƃ�����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Pop() {
			auto ow_shptr = m_Owner.lock();
			if (ow_shptr && !m_StateStack.empty()) {
				m_StateStack.top()->Exit(ow_shptr);
				m_StateStack.pop();
				if (!m_StateStack.empty()) {
					m_StateStack.top()->WakeUp(ow_shptr);
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�X�^�b�N���N���A����B���݂̃X�e�[�g�������Exit�𑗂肻�̂��ƂŃN���A����B
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Clear() {
			auto ow_shptr = m_Owner.lock();
			if (ow_shptr && !m_StateStack.empty()) {
				while (!m_StateStack.empty()) {
					m_StateStack.top()->Exit(ow_shptr);
					m_StateStack.pop();
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�X�^�b�N���N���A���A���炽�߂čŏ��̃X�e�[�g��o�^����
		@param[in]	Ptr	�o�^����X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Reset(const shared_ptr< ObjState<T> >& Ptr) {
			Clear();
			Push(Ptr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�e�[�g�����s����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Update() {
			auto ow_shptr = m_Owner.lock();
			if (ow_shptr && !m_StateStack.empty()) {
				m_StateStack.top()->Execute(ow_shptr);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s��2�����s����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Update2() {
			auto ow_shptr = m_Owner.lock();
			if (ow_shptr && !m_StateStack.empty()) {
				m_BehaviorStack.top()->Execute2(ow_shptr);
			}
		}
	};


	//�e�V���O���g���C���X�^���X�쐬�}�N��
#define DECLARE_SINGLETON_INSTANCE(InsType)	static shared_ptr<InsType> Instance();
#define IMPLEMENT_SINGLETON_INSTANCE(InsType)	shared_ptr<InsType> InsType::Instance() { \
	static shared_ptr<InsType> instance; \
	if(!instance) { instance = shared_ptr<InsType>(new InsType); }return instance;}



}

//end basecross
