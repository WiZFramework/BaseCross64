/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrLookAtCamera = ObjectFactory::Create<MyCamera>();
		PtrView->SetCamera(PtrLookAtCamera);
		PtrLookAtCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	//�{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		auto BoxNodes = m_XmlDocReader->GetSelectNodes(L"GameStage/GameObjects/TilingFixedBox");
		if (!BoxNodes) {
			throw BaseException(
				L"GameStage/GameObjects/TilingFixedBox��������܂���",
				L"if (!BoxNodes)",
				L"GameStage::CreateFixedBox()"
			);
		}
		long CountNode = XmlDocReader::GetLength(BoxNodes);
		for (long i = 0; i < CountNode; i++) {
			auto Node = XmlDocReader::GetItem(BoxNodes, i);
			auto ScaleNode = XmlDocReader::GetSelectSingleNode(Node, L"Scale");
			auto RotNode = XmlDocReader::GetSelectSingleNode(Node, L"Rot");
			auto PosNode = XmlDocReader::GetSelectSingleNode(Node, L"Pos");
			auto TexNode = XmlDocReader::GetSelectSingleNode(Node, L"Tex");
			wstring ScaleStr = XmlDocReader::GetText(ScaleNode);
			wstring RotStr = XmlDocReader::GetText(RotNode);
			wstring PosStr = XmlDocReader::GetText(PosNode);
			wstring TexStr = XmlDocReader::GetText(TexNode);
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, ScaleStr, L',');
			//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
			Vec3 Scale(
				(float)_wtof(Tokens[0].c_str()),
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str())
			);
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, RotStr, L',');
			Vec3 Rot;
			//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
			Rot.x = (Tokens[0] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[0].c_str());
			Rot.y = (Tokens[1] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[1].c_str());
			Rot.z = (Tokens[2] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[2].c_str());
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, PosStr, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[0].c_str()),
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str())
			);
			//�e�l����������̂ŃI�u�W�F�N�g�쐬
			AddGameObject<TilingFixedBox>(Scale, Rot, Pos, 1.0f, 1.0f, TexStr);
		}




		////CSV�̍s�P�ʂ̔z��
		//vector<wstring> LineVec;
		////0�Ԗڂ̃J������L"TilingFixedBox"�ł���s�𔲂��o��
		//m_GameStageCsvB.GetSelect(LineVec, 0, L"TilingFixedBox");
		//for (auto& v : LineVec) {
		//	//�g�[�N���i�J�����j�̔z��
		//	vector<wstring> Tokens;
		//	//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
		//	Util::WStrToTokenVector(Tokens, v, L',');
		//	//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
		//	Vec3 Scale(
		//		(float)_wtof(Tokens[1].c_str()),
		//		(float)_wtof(Tokens[2].c_str()),
		//		(float)_wtof(Tokens[3].c_str())
		//	);
		//	Vec3 Rot;
		//	//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
		//	Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
		//	Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
		//	Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());
		//	Vec3 Pos(
		//		(float)_wtof(Tokens[7].c_str()),
		//		(float)_wtof(Tokens[8].c_str()),
		//		(float)_wtof(Tokens[9].c_str())
		//	);

		//	//�e�l����������̂ŃI�u�W�F�N�g�쐬
		//	AddGameObject<TilingFixedBox>(Scale, Rot, Pos, 1.0f, 1.0f, Tokens[10]);
		//}
	}


	//CSV��A�I�u�W�F�N�g�̍쐬
	void GameStage::CreateXmlObjects() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto group = CreateSharedObjectGroup(L"SeekGroup");
		//�Z���}�b�v�̃m�[�h���擾
		auto CellmapNode = m_XmlDocReader->GetSelectSingleNode(L"GameStage/CellMap");
		if (!CellmapNode) {
			throw BaseException(
				L"GameStage/CellMap��������܂���",
				L"if (!CellmapNode)",
				L"GameStage::CreateEnemy()"
			);
		}
		//���e�̕�������擾
		wstring MapStr = XmlDocReader::GetText(CellmapNode);
		vector<wstring> LineVec;
		//�ŏ��Ɂu���s�v���f���~�^�Ƃ���������̔z��ɂ���
		Util::WStrToTokenVector(LineVec, MapStr, L'\n');
		for (size_t i = 0; i < LineVec.size(); i++) {
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, LineVec[i], L',');
			for (size_t j = 0; j < Tokens.size(); j++) {
				//X��Z�̈ʒu���v�Z
				float XPos = (float)((int)j - 19);
				float ZPos = (float)(19 - (int)i);
				if (Tokens[j] == L"1") {
					AddGameObject<SeekObject>(Vec3(XPos, 0.125f, ZPos));

				}
				else if (Tokens[j] == L"2") {
					AddGameObject<MoveBox>(
						Vec3(1.0f, 1.0f, 1.0f),
						Vec3(0.0f, 0.0f, 0.0f),
						Vec3(XPos, 1.0f, ZPos));

				}
			}
		}

/*

		//CSV�̑S�̂̔z��
		//CSV���炷�ׂĂ̍s�𔲂��o��
		auto& LineVec = m_GameStageCsvA.GetCsvVec();
		for (size_t i = 0; i < LineVec.size(); i++) {
			//�g�[�N���i�J�����j�̔z��
			vector<wstring> Tokens;
			//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
			Util::WStrToTokenVector(Tokens, LineVec[i], L',');
			for (size_t j = 0; j < Tokens.size(); j++) {
				//X��Z�̈ʒu���v�Z
				float XPos = (float)((int)j - 19);
				float ZPos = (float)(19 - (int)i);
				if (Tokens[j] == L"1") {
					AddGameObject<SeekObject>(Vec3(XPos, 0.125f, ZPos));
				}
				if (Tokens[j] == L"2") {
					AddGameObject<MoveBox>(
						Vec3(1.0f, 1.0f, 1.0f),
						Vec3(0.0f, 0.0f, 0.0f),
						Vec3(XPos, 1.0f, ZPos));
				}
			}
		}
*/

	}


	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	void GameStage::OnCreate() {
		try {
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			//XML�̓ǂݍ���
			m_XmlDocReader.reset(new XmlDocReader(DataDir + L"GameStage.xml"));
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�{�b�N�X�̍쐬
			CreateFixedBox();
			//XML�̃I�u�W�F�N�g�̍쐬
			CreateXmlObjects();
			//�v���[���[�̍쐬
			CreatePlayer();

		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
