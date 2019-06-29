/*!
@file App.cpp
@brief �A�v���P�[�V�����N���X�B���͋@�퓙����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	//	XAudio2Manager::Impl�N���X
	//--------------------------------------------------------------------------------------
	struct XAudio2Manager::Impl {
		bool m_IsAudioActive;
		ComPtr<IXAudio2>    m_IXAudio2;
		IXAudio2MasteringVoice* m_MasteringVoice;
		vector<shared_ptr<SoundItem>> m_SoundItemVec;
		Impl() :
			m_IsAudioActive(false),
			m_IXAudio2(nullptr),
			m_MasteringVoice(nullptr)
		{}
		~Impl() {
			XAUDIO2_VOICE_STATE state;
			bool isRunning;
			for (auto v : m_SoundItemVec) {
				if (v->m_SourceVoice) {
					v->m_SourceVoice->GetState(&state);
					isRunning = (state.BuffersQueued > 0) != 0;
					if (isRunning) {
			//			v->m_SourceVoice->Stop();
					}
					v->m_SourceVoice->DestroyVoice();
					v->m_SourceVoice = nullptr;
				}
			}
			m_IXAudio2.Reset();
			m_IXAudio2 = nullptr;
		}
		//�A�C�e���z����`�F�b�N���Ȃ���K�v�ł���Βǉ�����
		shared_ptr<SoundItem> ChkAndPushBackItem(const SoundItem& Item) {
			for (auto v : m_SoundItemVec) {
				if (!v->m_SourceVoice) {
					v->m_SourceVoice = Item.m_SourceVoice;
					v->m_AudioResource = Item.m_AudioResource;
					return v;
				}
			}
			auto Ptr = make_shared<SoundItem>();
			Ptr->m_SourceVoice = Item.m_SourceVoice;
			Ptr->m_AudioResource = Item.m_AudioResource;
			m_SoundItemVec.push_back(Ptr);
			return Ptr;
		}
		void OnUpdate() {
			XAUDIO2_VOICE_STATE state;
			bool isRunning;
			for (auto v : m_SoundItemVec) {
				if (v->m_SourceVoice) {
					v->m_SourceVoice->GetState(&state);
					isRunning = (state.BuffersQueued > 0) != 0;
					if (!isRunning) {
						v->m_AudioResource.reset();
						v->m_SourceVoice->DestroyVoice();
						v->m_SourceVoice = nullptr;
					}
				}
			}
		}
		bool IsItemActive(const shared_ptr<SoundItem>& ChkItem) {
			for (auto v : m_SoundItemVec) {
				if (v == ChkItem && v->m_SourceVoice) {
					return true;
				}
			}
			return false;
		}
	};



	//--------------------------------------------------------------------------------------
	/// XAudio2�}�l�[�W���N���X
	//--------------------------------------------------------------------------------------
	XAudio2Manager::XAudio2Manager():
		pImpl(new Impl())
	{}

	XAudio2Manager::~XAudio2Manager() {}

	bool XAudio2Manager::IsAudioActive() const {
		return IsCreated() && pImpl->m_IsAudioActive;
	}

	ComPtr<IXAudio2> XAudio2Manager::GetXAudio2() const {
		return pImpl->m_IXAudio2;
	}

	IXAudio2MasteringVoice* XAudio2Manager::GetMasteringVoice() const {
		return pImpl->m_MasteringVoice;
	}

	vector<shared_ptr<SoundItem>>& XAudio2Manager::GetSoundItemVec() {
		return pImpl->m_SoundItemVec;
	}
	const vector<shared_ptr<SoundItem>>& XAudio2Manager::GetSoundItemVec() const {
		return pImpl->m_SoundItemVec;
	}




	void XAudio2Manager::OnCreate() {
		UINT32 flags = 0;
		HRESULT hr = XAudio2Create(&pImpl->m_IXAudio2, flags);
		if (FAILED(hr)) {
			pImpl->m_IsAudioActive = false;
			return;
		}

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
		// To see the trace output, you need to view ETW logs for this application:
		//    Go to Control Panel, Administrative Tools, Event Viewer.
		//    View->Show Analytic and Debug Logs.
		//    Applications and Services Logs / Microsoft / Windows / XAudio2. 
		//    Right click on Microsoft Windows XAudio2 debug logging, Properties, then Enable Logging, and hit OK 
		XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
		debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
		debug.BreakMask = XAUDIO2_LOG_ERRORS;
		pImpl->m_IXAudio2->SetDebugConfiguration(&debug, 0);
#endif
		//�}�X�^�����O�{�C�X
		if (FAILED(hr = pImpl->m_IXAudio2->CreateMasteringVoice(&pImpl->m_MasteringVoice)))
		{
			pImpl->m_IXAudio2.Reset();
			pImpl->m_IsAudioActive = false;
			return;
		}
		pImpl->m_IsAudioActive = true;

	}

	void XAudio2Manager::OnUpdate() {
		if (!IsAudioActive()) {
			return;
		}
		pImpl->OnUpdate();
	}


	shared_ptr<SoundItem>  XAudio2Manager::Start(const wstring& ResKey, size_t LoopCount, float Volume) {
		if (!IsAudioActive()) {
			return nullptr;
		}
		auto SoundRes = App::GetApp()->GetResource<AudioResource>(ResKey);
		//�\�[�X�{�C�X�̍쐬
		SoundItem Item;
		HRESULT hr = pImpl->m_IXAudio2->CreateSourceVoice(&Item.m_SourceVoice, SoundRes->GetOutputWaveFormatEx());
		if (FAILED(hr)) {
			pImpl->m_IsAudioActive = false;
			return nullptr;
		}
		Item.m_AudioResource = SoundRes;
		auto Ptr = pImpl->ChkAndPushBackItem(Item);
		XAUDIO2_BUFFER buffer = { 0 };
		buffer.AudioBytes = (UINT32)SoundRes->GetSoundData().size();
		buffer.LoopCount = (UINT32)LoopCount;
		buffer.pAudioData = &SoundRes->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		Ptr->m_SourceVoice->SetVolume(Volume);
		Ptr->m_SourceVoice->SubmitSourceBuffer(&buffer);
		Ptr->m_SourceVoice->Start();
		return Ptr;
	}

	void XAudio2Manager::Stop(const shared_ptr<SoundItem>& Item) {
		if (!IsAudioActive()) {
			return;
		}
		if (!Item) {
			return;
		}
		if (Item->m_SourceVoice) {
			XAUDIO2_VOICE_STATE state;
			bool isRunning;
			Item->m_SourceVoice->GetState(&state);
			isRunning = (state.BuffersQueued > 0) != 0;
			if (isRunning) {
				Item->m_SourceVoice->Stop();
			}
			Item->m_SourceVoice->DestroyVoice();
			Item->m_SourceVoice = nullptr;
		}
	}



	//--------------------------------------------------------------------------------------
	//	struct AudioResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct AudioResource::Impl {
		WAVEFORMATEX m_WaveFormat;	//�E�F�u�t�H�[�}�b�g
		vector<byte> m_SoundData;	//�f�[�^
		wstring m_FileName;		//�t�@�C���ւ̃p�X
		Impl(const wstring& FileName) :
			m_FileName(FileName) {}
		~Impl() {}
		//�~���[�e�b�N�X
		std::mutex Mutex;

	};



	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	//	�p�r: �I�[�f�B�I���\�[�X�iwav�Ȃǁj�̃��b�s���O�N���X
	//--------------------------------------------------------------------------------------
	AudioResource::AudioResource(const wstring& FileName) :
		BaseResource(),
		pImpl(new Impl(FileName))
	{
		try {
			
			if (!App::GetApp()->GetXAudio2Manager()->IsAudioActive()) {
				//�}�l�[�W���������Ȃ烊�^�[��
				return;
			}


			ThrowIfFailed(
				MFStartup(MF_VERSION),
				L"MediaFoundation�̏������Ɏ��s���܂���",
				L"MFStartup(MF_VERSION)",
				L"AudioResource::AudioResource()"
			);

			ComPtr<IMFSourceReader> reader;

			ThrowIfFailed(
				MFCreateSourceReaderFromURL(FileName.c_str(), nullptr, &reader),
				L"�T�E���h���[�_�[�̍쐬�Ɏ��s���܂���",
				L"MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader)",
				L"AudioResource::AudioResource()"
			);

			// Set the decoded output format as PCM.
			// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
			// When using MediaFoundation, this sample always decodes into PCM.
			Microsoft::WRL::ComPtr<IMFMediaType> mediaType;

			ThrowIfFailed(
				MFCreateMediaType(&mediaType),
				L"���f�B�A�^�C�v�̍쐬�Ɏ��s���܂���",
				L"MFCreateMediaType(&mediaType)",
				L"AudioResource::AudioResource()"
			);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio),
				L"���f�B�AGUID�̐ݒ�Ɏ��s���܂���",
				L"mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)",
				L"AudioResource::AudioResource()"
			);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM),
				L"���f�B�A�T�uGUID�̐ݒ�Ɏ��s���܂���",
				L"mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)",
				L"AudioResource::AudioResource()"
			);

			ThrowIfFailed(
				reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get()),
				L"���[�_�[�ւ̃��f�B�A�^�C�v�̐ݒ�Ɏ��s���܂���",
				L"reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get())",
				L"AudioResource::AudioResource()"
			);


			// Get the complete WAVEFORMAT from the Media Type.
			Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;

			ThrowIfFailed(
				reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType),
				L"�o�͗p�̃��f�B�A�^�C�v�̐ݒ�Ɏ��s���܂���",
				L"reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType)",
				L"AudioResource::AudioResource()"
			);

			UINT32 size = 0;
			WAVEFORMATEX* waveFormat;

			ThrowIfFailed(
				MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size),
				L"�E�F�u�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���",
				L"MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size)",
				L"AudioResource::AudioResource()"
			);


			CopyMemory(&pImpl->m_WaveFormat, waveFormat, sizeof(pImpl->m_WaveFormat));
			CoTaskMemFree(waveFormat);

			PROPVARIANT propVariant;

			ThrowIfFailed(
				reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant),
				L"�A�g���r���[�g�̐ݒ�Ɏ��s���܂���",
				L"reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant)",
				L"AudioResource::AudioResource()"
			);

			// 'duration' is in 100ns units; convert to seconds, and round up
			// to the nearest whole byte.
			LONGLONG duration = propVariant.uhVal.QuadPart;
			unsigned int maxStreamLengthInBytes =
				static_cast<unsigned int>(
				((duration * static_cast<ULONGLONG>(pImpl->m_WaveFormat.nAvgBytesPerSec)) + 10000000) /
					10000000
					);

			pImpl->m_SoundData.resize(maxStreamLengthInBytes);

			ComPtr<IMFSample> sample;
			ComPtr<IMFMediaBuffer> mediaBuffer;
			DWORD flags = 0;

			int positionInData = 0;
			bool done = false;
			while (!done)
			{

				ThrowIfFailed(
					reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample),
					L"�T���v���[�̓ǂݍ��݂Ɏ��s���܂���",
					L"reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample)",
					L"AudioResource::AudioResource()"
				);

				if (sample != nullptr)
				{
					ThrowIfFailed(
						sample->ConvertToContiguousBuffer(&mediaBuffer),
						L"�T���v���[�̃R���o�[�g�Ɏ��s���܂���",
						L"sample->ConvertToContiguousBuffer(&mediaBuffer)",
						L"AudioResource::AudioResource()"
					);

					BYTE *audioData = nullptr;
					DWORD sampleBufferLength = 0;

					ThrowIfFailed(
						mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength),
						L"�o�b�t�@��Lock�Ɏ��s���܂���",
						L"mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength)",
						L"AudioResource::AudioResource()"
					);

					for (DWORD i = 0; i < sampleBufferLength; i++)
					{
						pImpl->m_SoundData[positionInData++] = audioData[i];
					}
				}
				if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
				{
					done = true;
				}
			}
		}
		catch (...) {
			throw;
		}
	}
	AudioResource::~AudioResource() {}

	const vector<byte>& AudioResource::GetSoundData()const {
		return pImpl->m_SoundData;
	}

	WAVEFORMATEX*  AudioResource::GetOutputWaveFormatEx()const {
		return &pImpl->m_WaveFormat;
	}



	//--------------------------------------------------------------------------------------
	//	struct EventDispatcher::Impl;
	//	�p�r: Impl�\����
	//--------------------------------------------------------------------------------------
	struct EventDispatcher::Impl {
		//�C�x���g�̃L���[
		list< shared_ptr<Event> > m_PriorityQ;
		map<wstring, vector<weak_ptr<ObjectInterface>>> m_EventInterfaceGroupMap;
		//
		//--------------------------------------------------------------------------------------
		//	void Discharge(
		//	const Event& event	//�C�x���g
		//	);
		//�p�r: �C�x���g�̑��M
		//�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void Discharge(const shared_ptr<Event>& event);
		Impl() {}
		~Impl() {}
	};

	void EventDispatcher::Impl::Discharge(const shared_ptr<Event>& event) {
		auto shptr = event->m_Receiver.lock();
		if (shptr) {
			//�󂯎肪�L��
			shptr->OnEvent(event);
		}
	}



	//--------------------------------------------------------------------------------------
	///	�C�x���g�z���N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	EventDispatcher::EventDispatcher():
		pImpl(new Impl())
	{}
	EventDispatcher::~EventDispatcher() {}

	void EventDispatcher::AddEventReceiverGroup(const wstring& GroupKey, const shared_ptr<ObjectInterface>& Receiver) {
		auto it = pImpl->m_EventInterfaceGroupMap.find(GroupKey);
		if (it != pImpl->m_EventInterfaceGroupMap.end()) {
			//�L�[��������
			it->second.push_back(Receiver);
		}
		else {
			//�O���[�v���Ȃ�
			vector<weak_ptr<ObjectInterface>> vec;
			pImpl->m_EventInterfaceGroupMap[GroupKey] = vec;
			pImpl->m_EventInterfaceGroupMap[GroupKey].push_back(Receiver);
		}
	}


	//�C�x���g��POST�i�L���[�ɓ����j
	void EventDispatcher::PostEvent(float Delay, const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
		const wstring& MsgStr, const  shared_ptr<void>& Info) {
		//�C�x���g�̍쐬 
		auto Ptr = make_shared<Event>(Delay, Sender, Receiver, MsgStr, Info);
		//�L���[�ɂ��߂�
		pImpl->m_PriorityQ.push_back(Ptr);
	}

	void EventDispatcher::PostEvent(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
		const wstring& MsgStr, const  shared_ptr<void>& Info) {
		//ReceiverKey�ɂ�鑊��̓���
		//�d���L�[�̌���
		auto it = pImpl->m_EventInterfaceGroupMap.find(ReceiverKey);
		if (it != pImpl->m_EventInterfaceGroupMap.end()) {
			//�L�[��������
			for (auto v : it->second) {
				auto shptr = v.lock();
				if (shptr) {
					//�C�x���g�̍쐬 
					auto Ptr = make_shared<Event>(0.0f, Sender, shptr, MsgStr, Info);
					//�L���[�ɂ��߂�
					pImpl->m_PriorityQ.push_back(Ptr);
				}
			}
		}
		//�L�[��������Ȃ��Ă��������Ȃ�
	}


	//�C�x���g��SEND�i�L���[�ɓ��ꂸ�ɂ��̂܂ܑ���j
	void EventDispatcher::SendEvent(const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
		const wstring& MsgStr, const  shared_ptr<void>& Info) {
		//�C�x���g�̍쐬 
		auto Ptr = make_shared<Event>(0.0f, Sender, Receiver, MsgStr, Info);
		//���M
		pImpl->Discharge(Ptr);
	}

	void EventDispatcher::SendEvent(const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
		const wstring& MsgStr, const  shared_ptr<void>& Info) {
		//ReceiverKey�ɂ�鑊��̓���
		//�d���L�[�̌���
		auto it = pImpl->m_EventInterfaceGroupMap.find(ReceiverKey);
		if (it != pImpl->m_EventInterfaceGroupMap.end()) {
			//�L�[��������
			for (auto v : it->second) {
				auto shptr = v.lock();
				if (shptr) {
					//�C�x���g�̍쐬 
					auto Ptr = make_shared<Event>(0.0f, Sender, shptr, MsgStr, Info);
					//�C�x���g�̑��o
					pImpl->Discharge(Ptr);
				}
			}
		}
		//�L�[��������Ȃ��Ă��������Ȃ�
	}


	void EventDispatcher::DispatchDelayedEvwnt() {
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto it = pImpl->m_PriorityQ.begin();
		while (it != pImpl->m_PriorityQ.end()) {
			(*it)->m_DispatchTime -= ElapsedTime;
			if ((*it)->m_DispatchTime <= 0.0f) {
				(*it)->m_DispatchTime = 0.0f;
				//���b�Z�[�W�̑��M
				pImpl->Discharge(*it);
				//�L���[����폜
				it = pImpl->m_PriorityQ.erase(it);
				//�폜��̃C�e���[�^���u�Ō�v��
				//�Ƃ��̓��[�v�𔲂���
				if (it == pImpl->m_PriorityQ.end()) {
					break;
				}
			}
			else {
				it++;
			}
		}
	}

	void EventDispatcher::ClearEventQ() {
		pImpl->m_PriorityQ.clear();
	}




#include <mfplay.h>
#include <mferror.h>
#pragma comment( lib, "mfplay.lib" )


	//--------------------------------------------------------------------------------------
	///	���[�r�[�Ŏg�p����l�[���X�y�[�X
	//--------------------------------------------------------------------------------------
	namespace movie {

		//--------------------------------------------------------------------------------------
		///	���[�r�[�p�R�[���o�b�N�N���X
		//--------------------------------------------------------------------------------------
		class MediaPlayerCallback : public IMFPMediaPlayerCallback
		{
			long m_cRef; // Reference count

		public:

			MediaPlayerCallback() : m_cRef(1)
			{
			}

			STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
			{
				static const QITAB qit[] =
				{
					QITABENT(MediaPlayerCallback, IMFPMediaPlayerCallback),
					{ 0 },
				};
				return QISearch(this, qit, riid, ppv);
			}
			STDMETHODIMP_(ULONG) AddRef()
			{
				return InterlockedIncrement(&m_cRef);
			}
			STDMETHODIMP_(ULONG) Release()
			{
				ULONG count = InterlockedDecrement(&m_cRef);
				if (count == 0)
				{
					delete this;
					return 0;
				}
				return count;
			}

			// IMFPMediaPlayerCallback methods
			void STDMETHODCALLTYPE OnMediaPlayerEvent(MFP_EVENT_HEADER *pEventHeader);
		};
		//���[�r�[�p�f�[�^
		namespace data {
			ComPtr<IMFPMediaPlayer> Player;
			ComPtr<MediaPlayerCallback> PlayerCB;
			BOOL                    bHasVideo = FALSE;
			HWND ChildHWnd = 0;
			bool bAutoRepeat = true;
		}

		//--------------------------------------------------------------------------------------
		//	���[�r�[�p�`���C���h�E�C���h�E�N���X�o�^
		//--------------------------------------------------------------------------------------
		BOOL RegisterChild(HINSTANCE hInst, WNDPROC WndProc, const wchar_t* szClassName)
		{
			WNDCLASS wc = {};
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
			wc.lpfnWndProc = WndProc;    //�v���V�[�W����
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInst;        //�C���X�^���X
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wc.lpszMenuName = NULL;    //���j���[��
			wc.lpszClassName = (LPCWSTR)szClassName;
			return (RegisterClass(&wc));
		}

		LRESULT CALLBACK ChildProc(HWND hChdWnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			switch (msg) {
			case WM_LBUTTONDBLCLK:
				if (data::Player)
				{
					MFP_MEDIAPLAYER_STATE state = MFP_MEDIAPLAYER_STATE_EMPTY;

					auto hr = data::Player->GetState(&state);

					if (SUCCEEDED(hr))
					{
						if (state == MFP_MEDIAPLAYER_STATE_PAUSED || state == MFP_MEDIAPLAYER_STATE_STOPPED)
						{
							hr = data::Player->Play();
						}
						else if (state == MFP_MEDIAPLAYER_STATE_PLAYING)
						{
							hr = data::Player->Pause();
						}
					}
				}
				break;
			case WM_CLOSE:
				DestroyWindow(hChdWnd);
				break;
			default:
				return (DefWindowProc(hChdWnd, msg, wp, lp));
			}
			return 0L;
		}


		HWND CleateMobieWnd() {
			auto hInst = App::GetApp()->GetHInstance();
			movie::RegisterChild(App::GetApp()->GetHInstance(),
				movie::ChildProc,
				L"BaseCrossChildClass");
			auto ParHWnd = App::GetApp()->GetHWnd();
			RECT rectPar;
			GetClientRect(ParHWnd, &rectPar);
			data::ChildHWnd = CreateWindowExW(
				WS_EX_TOPMOST,
				L"BaseCrossChildClass",
				L"",//�^�C�g���o�[�ɂ��̖��O���\������܂�
				WS_CHILD,    //�E�B���h�E�̎��
				0,    //�w���W
				0,    //�x���W
				rectPar.right,    //��
				rectPar.bottom,    //����
				ParHWnd,            //�e�E�B���h�E�̃n���h���A�e�����Ƃ���NULL
				0, //���j���[�n���h���A�q����ID
				hInst,            //�C���X�^���X�n���h��
				NULL);
			ShowWindow(data::ChildHWnd, SW_SHOW);
			UpdateWindow(data::ChildHWnd);
			return data::ChildHWnd;
		}

		void OnMediaItemCreated(MFP_MEDIAITEM_CREATED_EVENT *pEvent)
		{
			HRESULT hr = S_OK;

			if (data::Player)
			{
				BOOL bHasVideo = FALSE, bIsSelected = FALSE;

				// Check if the media item contains video.
				hr = pEvent->pMediaItem->HasVideo(&bHasVideo, &bIsSelected);

				if (FAILED(hr)) { 
					throw BaseException(
						L"���[�r�[�t�@�C���̓o�^�Ɏ��s���܂���",
						L"if (FAILED(pEvent->pMediaItem->HasVideo))",
						L"OnMediaItemCreated()"
					);
				}

				data::bHasVideo = bHasVideo && bIsSelected;

				hr = data::Player->SetMediaItem(pEvent->pMediaItem);
			}

		}

		void OnMediaItemSet(MFP_MEDIAITEM_SET_EVENT * /*pEvent*/)
		{
			HRESULT hr = S_OK;

			hr = data::Player->Play();

			if (FAILED(hr))
			{
				throw BaseException(
					L"���[�r�[�t�@�C���̍Đ��Ɏ��s���܂���",
					L"if (FAILED(movie::g_Player->Play))",
					L"OnMediaItemSet()"
				);
			}
		}

		void OnMediaPlayEnd() {
			if (data::bAutoRepeat) {
				HRESULT hr = S_OK;
				hr = data::Player->Play();

				if (FAILED(hr))
				{
					throw BaseException(
						L"���[�r�[�t�@�C���̘A���Đ��Ɏ��s���܂���",
						L"if (FAILED(movie::g_Player->Play))",
						L"OnMediaPlayEnd()"
					);
				}
			}
		}

		void MediaPlayerCallback::OnMediaPlayerEvent(MFP_EVENT_HEADER * pEventHeader)
		{

			if (FAILED(pEventHeader->hrEvent))
			{
				throw BaseException(
					L"���[�r�[�̃R�[���o�b�N�֐��̃C�x���g�擾�Ɏ��s���܂���",
					L"if (FAILED(pEventHeader->hrEvent))",
					L" MediaPlayerCallback::OnMediaPlayerEvent()"
				);
			}

			switch (pEventHeader->eEventType)
			{
			case MFP_EVENT_TYPE_MEDIAITEM_CREATED:
				OnMediaItemCreated(MFP_GET_MEDIAITEM_CREATED_EVENT(pEventHeader));
				break;

			case MFP_EVENT_TYPE_MEDIAITEM_SET:
				OnMediaItemSet(MFP_GET_MEDIAITEM_SET_EVENT(pEventHeader));
				break;
			case MFP_EVENT_TYPE_PLAYBACK_ENDED:
				OnMediaPlayEnd();
				break;
			}
		}


		void UpdateMovie() {
			if (data::Player) {
				data::Player->UpdateVideo();
			}
		}

		bool IsMovieActive() {
			return data::bHasVideo;
		}

		void ClearMovie() {
			if (data::Player)
			{
				data::Player->Shutdown();
				data::Player.Reset();
			}

			if (data::PlayerCB)
			{
				data::PlayerCB.Reset();
			}
			data::bHasVideo = false;
			//�E�C���h�E�̍폜
			if (data::ChildHWnd) {
				DestroyWindow(data::ChildHWnd);
			}
			data::ChildHWnd = NULL;
		}

		void PlayMovie(const wstring& MovieFileName) {

			auto hwnd = CleateMobieWnd();

			HRESULT hr = S_OK;

			// Create the MFPlayer object.
			if (data::Player == nullptr)
			{
				data::PlayerCB = new (std::nothrow) movie::MediaPlayerCallback();

				if (data::PlayerCB == nullptr)
				{
					throw BaseException(
						L"���[�r�[�̃R�[���o�b�N�֐��̐ݒ�Ɏ��s���܂���",
						L"if (g_PlayerCB == nullptr)",
						L"App::PlayMovie()"
					);
				}

				hr = MFPCreateMediaPlayer(
					NULL,
					FALSE,          // Start playback automatically?
					0,              // Flags
					data::PlayerCB.Get(),    // Callback pointer
					hwnd,           // Video window
					&data::Player
				);

				if (FAILED(hr)) {
					throw BaseException(
						L"���[�r�[�̃v���C���[�̍쐬�Ɏ��s���܂���",
						L"FAILED MFPCreateMediaPlayer",
						L"App::PlayMovie()"
					);
				}
			}
			hr = data::Player->CreateMediaItemFromURL(MovieFileName.c_str(), FALSE, 0, NULL);
		}

		void OnSize()
		{
			if (data::ChildHWnd) {
				auto ParHWnd = App::GetApp()->GetHWnd();
				RECT rectPar;
				GetClientRect(ParHWnd, &rectPar);
				MoveWindow(
					data::ChildHWnd,
					0,
					0,
					rectPar.right,
					rectPar.bottom,
					true);
			}
			if (data::Player)
			{
				data::Player->UpdateVideo();
			}
		}

		bool IsMovieAutoRepeat() {
			return data::bAutoRepeat;
		}
		void SetMovieAutoRepeat(bool b) {
			data::bAutoRepeat = b;
		}

	}//end namespace movie

	//--------------------------------------------------------------------------------------
	//	class App;
	//	�p�r: �A�v���P�[�V�����N���X
	//--------------------------------------------------------------------------------------
	//static�ϐ�����
	unique_ptr<App, App::AppDeleter> App::m_App;
	//�\�z
	App::App(HINSTANCE hInstance, HWND hWnd, bool FullScreen, UINT Width, UINT Height) :
		m_hInstance{ hInstance },
		m_hWnd{ hWnd },
		m_FullScreen{ FullScreen },
		m_GameWidth{ Width },
		m_GameHeight{ Height },
		m_Timer(),
		m_ScriptsDirActive(false)
	{
		try {
			//��f�B���N�g���̐ݒ�
			//���΃p�X�ɂ���ƁA�t�@�C���_�C�A���O�ŃJ�����g�p�X�������̂�
			//��΃p�X�w��
			wchar_t Modulebuff[MAX_PATH];
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];

			::ZeroMemory(Modulebuff, sizeof(Modulebuff));
			::ZeroMemory(Drivebuff, sizeof(Drivebuff));
			::ZeroMemory(Dirbuff, sizeof(Dirbuff));
			::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
			::ZeroMemory(Extbuff, sizeof(Extbuff));

			//���W���[�����i�v���O�����t�@�C�����j�𓾂�
			if (!::GetModuleFileName(nullptr, Modulebuff, sizeof(Modulebuff))) {
				throw BaseException(
					L"���W���[�����擾�ł��܂���B",
					L"if(!::GetModuleFileName())",
					L"App::App()"
				);
			}
			m_wstrModulePath = Modulebuff;
			//���W���[��������A�e�u���b�N�ɕ�����
			_wsplitpath_s(Modulebuff,
				Drivebuff, _MAX_DRIVE,
				Dirbuff, _MAX_DIR,
				FileNamebuff, _MAX_FNAME,
				Extbuff, _MAX_EXT);

			//�h���C�u���̎擾
			m_wstrDir = Drivebuff;
			//�f�B���N�g�����̎擾
			m_wstrDir += Dirbuff;
			//media�f�B���N�g����T��
			m_wstrDataPath = m_wstrDir;
			m_wstrDataPath += L"media";
			//�܂��A���s�t�@�C���Ɠ����f�B���N�g����T��
			DWORD RetCode;
			RetCode = GetFileAttributes(m_wstrDataPath.c_str());
			if (RetCode == 0xFFFFFFFF) {
				//���s����
				m_wstrDataPath = m_wstrDir;
				m_wstrDataPath += L"..\\media";
				RetCode = GetFileAttributes(m_wstrDataPath.c_str());
				if (RetCode == 0xFFFFFFFF) {
					//�Ăю��s����
					throw BaseException(
						L"media�f�B���N�g�����m�F�ł��܂���B",
						L"if(RetCode == 0xFFFFFFFF)",
						L"App::App()"
					);
				}
				else {
					m_wstrDataPath += L"\\";
					//���΃p�X�̐ݒ�
					m_wstrRelativeDataPath = L"..\\media\\";
				}
			}
			else {
				m_wstrDataPath += L"\\";
				//���΃p�X�̐ݒ�
				m_wstrRelativeDataPath = L"media\\";
			}
			m_wstrShadersPath = m_wstrDataPath + L"Shaders\\";
			m_wstrRelativeShadersPath = m_wstrRelativeDataPath + L"Shaders\\";

			//Scripts�f�B���N�g����T��
			m_wstrScriptsPath = m_wstrDir;
			m_wstrScriptsPath += L"scripts";
			//�܂��A���s�t�@�C���Ɠ����f�B���N�g����T��
			RetCode = GetFileAttributes(m_wstrScriptsPath.c_str());
			if (RetCode == 0xFFFFFFFF) {
				//���s����
				m_wstrScriptsPath = m_wstrDir;
				m_wstrScriptsPath += L"..\\scripts";
				RetCode = GetFileAttributes(m_wstrDataPath.c_str());
				if (RetCode == 0xFFFFFFFF) {
					//�Ăю��s����
					//Scripts�f�B���N�g���͕K�{�ł͂Ȃ��̂ōĂ�
					//���s�t�@�C���Ɠ����f�B���N�g���ɐݒ�
					m_ScriptsDirActive = false;
					m_wstrScriptsPath = L"";
					m_wstrScriptsPath += L"";
				}
				else {
					m_ScriptsDirActive = true;
					m_wstrScriptsPath += L"\\";
					//���΃p�X�̐ݒ�
					m_wstrRelativeScriptsPath = L"..\\scripts\\";
				}
			}
			else {
				m_ScriptsDirActive = true;
				m_wstrScriptsPath += L"\\";
				//���΃p�X�̐ݒ�
				m_wstrRelativeScriptsPath = L"scripts\\";
			}

			//Assets�f�B���N�g����T��
			m_wstrRelativeAssetsPath = m_wstrDir;
			m_wstrRelativeAssetsPath += L"..\\..\\Assets";
			//���΃f�B���N�g����T��
			RetCode = GetFileAttributes(m_wstrRelativeAssetsPath.c_str());
			if (RetCode == 0xFFFFFFFF) {
				//���s����
				//�A�Z�b�g�f�B���N�g�������f�B�A�f�B���N�g���ɂ���
				m_wstrRelativeAssetsPath = m_wstrRelativeDataPath;
			}
			else {
				//��������
				m_wstrRelativeAssetsPath += L"\\";
			}
			////�f�o�C�X���\�[�X�̍\�z
			m_DeviceResources = shared_ptr<DeviceResources>(new DeviceResources(hWnd, FullScreen, Width, Height));
			//�I�[�f�B�I�}�l�[�W���̎擾
			GetXAudio2Manager();
			//�C�x���g�z���N���X
			m_EventDispatcher = make_shared<EventDispatcher>();
			//�����̏�����
			srand((unsigned)time(nullptr));

		}
		catch (...) {
			throw;
		}
	}

	//�V���O���g���\�z
	unique_ptr<App, App::AppDeleter>& App::CreateApp(HINSTANCE hInstance, HWND hWnd,
		bool FullScreen, UINT Width, UINT Height, bool ShadowActive) {
		try {
			if (m_App.get() == 0) {
				//�������g�̍\�z
				m_App.reset(new App(hInstance, hWnd, FullScreen, Width, Height));
				m_App->AfterInitContents(ShadowActive);

			}
			return m_App;
		}
		catch (...) {
			throw;
		}
	}

	//�V���O���g���A�N�Z�T
	unique_ptr<App, App::AppDeleter>& App::GetApp() {
		try {
			if (m_App.get() == 0) {
				throw BaseException(
					L"�A�v���P�[�V�������܂��쐬����Ă܂���",
					L"if (m_App.get() == 0)",
					L"App::GetApp()"
				);
			}
			return m_App;
		}
		catch (...) {
			throw;
		}

	}

	bool App::AppCheck() {
		if (m_App.get() == 0) {
			return false;
		}
		return true;
	}

	//�����j��
	void App::DeleteApp() {
		if (m_App.get()) {
			m_App->GetSceneInterface()->OnDestroy();
			m_App->OnDestroy();
			m_App.reset();
		}
	}


	shared_ptr<XAudio2Manager>& App::GetXAudio2Manager() {
		try {
			if (!m_XAudio2Manager) {
				m_XAudio2Manager = ObjectFactory::Create<XAudio2Manager>();
			}
			return m_XAudio2Manager;
		}
		catch (...) {
			throw;
		}

	}


	void App::AfterInitContents(bool ShadowActive) {
		if (!m_DeviceResources) {
			throw BaseException(
				L"�f�o�C�X������������Ă��܂���",
				L"if (!m_DeviceResources)",
				L"App::AfterInitContents()"
			);
		}
		m_ShadowActive = ShadowActive;
		m_DeviceResources->AfterInitContents(ShadowActive);
	}


	void App::RegisterResource(const wstring& Key, const shared_ptr<BaseResource>& ResObj) {
		try {
			if (Key == L"") {
				throw BaseException(
					L"�L�[���󔒂ł�",
					L"if(Key == L\"\")",
					L"App::RegisterResource()"
				);
			}
			if (!ResObj) {
				throw BaseException(
					L"���\�[�X���s��ł�",
					L"if(!pResObj)",
					L"App::RegisterResource()"
				);
			}
			map<wstring, shared_ptr<BaseResource> >::iterator it;
			//�d���|�C���^�̌���
			for (it = m_ResMap.begin(); it != m_ResMap.end(); it++) {
				if (it->second == ResObj) {
					//�|�C���^���d�����Ă��Ă��A�L�[�������Ȃ�
					//���łɓo�^����Ă���̂Ń��^�[��
					if (it->first == Key) {
						return;
					}
					wstring keyerr = Key;
					throw BaseException(
						L"���łɂ��̃��\�[�X�͓o�^����Ă��܂�",
						keyerr,
						L"App::RegisterResource()"
					);
				}
			}
			//�d���L�[�̌���
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end()) {
				//�w��̖��O����������
				//��O����
				wstring keyerr = Key;
				throw BaseException(
					L"���łɂ��̃L�[�͓o�^����Ă��܂�",
					keyerr,
					L"App::RegisterResource()"
				);
			}
			else {
				//������Ȃ�
				//���\�[�X�y�A�̒ǉ�
				m_ResMap[Key] = ResObj;
			}
		}
		catch (...) {
			throw;
		}
	}

	//�e�N�X�`���̓o�^(�����L�[�̃e�N�X�`�����Ȃ���΃t�@�C�����ō쐬���A�o�^)
	//�������O�̃e�N�X�`��������΂��̃|�C���^��Ԃ�
	shared_ptr<TextureResource> App::RegisterTexture(const wstring& Key, const wstring& TextureFileName, const wstring& TexType) {
		if (CheckResource<TextureResource>(Key)) {
			return GetResource<TextureResource>(Key);
		}
		//
		auto PtrTexture = TextureResource::CreateTextureResource(TextureFileName, TexType);
		RegisterResource(Key, PtrTexture);
		return PtrTexture;
	}


	//Wav�̓o�^(�����L�[��Wav���Ȃ���΃t�@�C�����ō쐬���A�o�^)
	//�������O��Wav������΂��̃|�C���^��Ԃ�
	shared_ptr<AudioResource> App::RegisterWav(const wstring& Key, const wstring& WavFileName) {
		if (CheckResource<AudioResource>(Key)) {
			return GetResource<AudioResource>(Key);
		}
		//
		auto PtrWav = ObjectFactory::Create<AudioResource>(WavFileName);
		RegisterResource(Key, PtrWav);
		return PtrWav;
	}

	void App::OnMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	}

	void App::OnSize() {
		movie::OnSize();
	}


	void App::SetFullScreenMode() {
		if (IsFullScreen()) {
			return;
		}
		//�{�[�_�[���X�E�C���h�E���g�p
		auto iClientWidth = GetSystemMetrics(SM_CXSCREEN);
		auto iClientHeight = GetSystemMetrics(SM_CYSCREEN);
		auto HWnd = GetHWnd();
		//�E�B���h�E�̃X�^�C����ς���
		SetWindowLong(HWnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(
			HWnd, HWND_TOP,
			0, 0, iClientWidth, iClientHeight,
			SWP_SHOWWINDOW
		);
		m_FullScreen = true;
	}

	void App::SetWindowMode(const RECT& r) {
		auto HWnd = GetHWnd();
		if (IsFullScreen()) {
			//�t���X�N���[���Ȃ�E�C���h�E�X�^�C����ς���
			SetWindowLong(HWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		}
		SetWindowPos(
			HWnd, HWND_TOP,
			r.left, r.top, r.right - r.left, r.bottom - r.top,
			SWP_SHOWWINDOW
		);
		m_FullScreen = false;
	}

	void App::UpdateDraw(unsigned int SyncInterval) {
		if (!m_SceneInterface) {
			//�V�[�����������Ȃ�
			throw BaseException(
				L"�V�[��������܂���",
				L"if(!m_SceneInterface)",
				L"App::UpdateDraw()"
			);
		}
		//�I�[�f�B�I�̍X�V
		GetXAudio2Manager()->OnUpdate();
		// �V�[�� �I�u�W�F�N�g���X�V���܂��B
		m_InputDevice.ResetControlerState();
		m_Timer.Tick([&]()
			{
				//�C�x���g�L���[�̑��M
				m_EventDispatcher->DispatchDelayedEvwnt();
				m_SceneInterface->OnUpdate();
			});
		// ����X�V�O�Ƀ����_�����O�͍s��Ȃ��B
		if (GetFrameCount() == 0)
		{
			return;
		}
		if (!movie::IsMovieActive()) {
			//���[�r�[������
			m_SceneInterface->OnDraw();
			// �o�b�N�o�b�t�@����t�����g�o�b�t�@�ɓ]��
			m_DeviceResources->Present(SyncInterval, 0);
		}
	}

	bool App::IsMovieActive() {
		return movie::IsMovieActive();
	}

	void App::ClearMovie() {
		movie::ClearMovie();
	}

	void App::OnDestroy() {
		movie::ClearMovie();
	}

	void App::PlayMovie(const wstring& MovieFileName) {
		movie::PlayMovie(MovieFileName);
	}

	void App::UpdateMovie() {
		movie::UpdateMovie();
	}

	bool App::IsMovieAutoRepeat() const {
		return movie::IsMovieAutoRepeat();
	}
	void App::SetMovieAutoRepeat(bool b) {
		movie::SetMovieAutoRepeat(b);
	}


}
//end basecross
