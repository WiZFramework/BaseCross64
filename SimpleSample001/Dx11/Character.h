/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	
	class TriangleObject : public ObjectInterface, public ShapeInterface {
	public:
		TriangleObject();
		virtual ~TriangleObject();
		//������
		virtual void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override;

	private:
		//���b�V��
		shared_ptr<MeshResource> m_TriangleMesh;
	};

}
//end basecross
