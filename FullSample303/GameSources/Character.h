/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};


	//--------------------------------------------------------------------------------------
	///	PC球
	//--------------------------------------------------------------------------------------
	class PcSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//構築と破棄
		PcSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PcSphere();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;

	};

	//--------------------------------------------------------------------------------------
	///	PN球
	//--------------------------------------------------------------------------------------
	class PnSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//構築と破棄
		PnSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PnSphere();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;

	};


	//--------------------------------------------------------------------------------------
	///	PT球
	//--------------------------------------------------------------------------------------
	class PtSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//構築と破棄
		PtSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PtSphere();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	PCT球
	//--------------------------------------------------------------------------------------
	class PctSphere : public GameObject {
		Vec3 m_StartPos;
	public:
		//構築と破棄
		PctSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PctSphere();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
	};




	//--------------------------------------------------------------------------------------
	///	Pnt球
	//--------------------------------------------------------------------------------------
	class PntSphere : public GameObject {
		Vec3 m_StartPos;
		bool m_TextureUse;
	public:
		//構築と破棄
		PntSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TextureUse);
		virtual ~PntSphere();
		//初期化
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Pntスペキュラー球
	//--------------------------------------------------------------------------------------
	class PntSpSphere : public GameObject {
		Vec3 m_StartPos;
		bool m_TextureUse;
	public:
		//構築と破棄
		PntSpSphere(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TextureUse);
		virtual ~PntSpSphere();
		//初期化
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	///	Staticキャラ
	//--------------------------------------------------------------------------------------
	class StaticChara : public GameObject {
		Vec3 m_StartPos;
		bool m_TamgentUse;
	public:
		//構築と破棄
		StaticChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse);
		virtual ~StaticChara();
		//初期化
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Staticキャラ(マルチメッシュ版)
	//--------------------------------------------------------------------------------------
	class StaticMultiMeshChara : public GameObject {
		Vec3 m_StartPos;
		bool m_TamgentUse;
	public:
		//構築と破棄
		StaticMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse);
		virtual ~StaticMultiMeshChara();
		//初期化
		virtual void OnCreate() override;
	};



	//--------------------------------------------------------------------------------------
	///	Boneキャラ
	//--------------------------------------------------------------------------------------
	class BoneChara : public GameObject {
		Vec3 m_StartPos;
		bool m_TamgentUse;
	public:
		//構築と破棄
		BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse);
		virtual ~BoneChara();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Boneキャラ(マルチメッシュ版)
	//--------------------------------------------------------------------------------------
	class BoneMultiMeshChara : public GameObject {
		Vec3 m_StartPos;
		bool m_TamgentUse;
		//入力ハンドラー
		InputHandler<BoneMultiMeshChara> m_InputHandler;
		//マルチメッシュの表示フラグ
		bool m_DrawFlg;
		//描画スイッチ
		void DrawSwitch();
	public:
		//構築と破棄
		BoneMultiMeshChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, bool TamgentUse);
		virtual ~BoneMultiMeshChara();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//Aボタン
		void OnPushA();
	};




}
//end basecross
