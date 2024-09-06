#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "player/Player.h"
#include "camera/FollowCamera.h"
#include "ground/Ground.h"
#include "pillar/Pillar.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数


	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> modelPlayer_;
	std::list<Player*> players_;

	//地面
	std::unique_ptr<Ground> ground_;
	Model* modelGround_ = nullptr;

	//柱
	std::unique_ptr<Pillar> pillar_;
	Model* modelPillar_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<DebugCamera> debugCamera_;

	bool isDebugCameraActive_ = false;




	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
