/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	
	class TriangleObject : public ObjectInterface, public ShapeInterface {
	public:
		TriangleObject();
		virtual ~TriangleObject();
		//初期化
		virtual void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override;

	private:
		//メッシュ
		shared_ptr<MeshResource> m_TriangleMesh;
	};

}
//end basecross
