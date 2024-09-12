#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"


#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <memory>
#include <fstream>
#include <cassert>


#include "player/Player.h"
#include "camera/FollowCamera.h"
#include "ground/Ground.h"
#include "pillar/Pillar.h"
#include "key/KeyItem.h"

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

	void Reset();

#pragma region 鍵CSV関数

	// 鍵発生データを読み込み
	void LoadKeyPopData();

	// 鍵の発生コマンドの更新
	void UpdateKeyPopCommands();

	/// 鍵の生成
	void KeyGenerate(Vector3 position);

#pragma endregion



private: // メンバ変数


	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> modelPlayer_;
	std::list<Player*> players_;

	//床
	std::unique_ptr<Ground> ground_;
	std::unique_ptr<Model> modelGround_;
	std::list<Ground*> grounds_;

	float openTimer = 0;
	bool openTimerFlag = false;

	bool nextStageFlag = false;
	
	
	//柱
	std::unique_ptr<Pillar> pillar_;
	Model* modelPillar_ = nullptr;

	//鍵
	std::list<std::unique_ptr<KeyItem>> keys_;
	//発生コマンド
	std::stringstream keyPopCommands;
	Model* modelKey_ = nullptr;
	bool keyFlag_ = false;
	bool nextFlag_[10] = {false}; 
	int nextStageKey = 0;


	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<DebugCamera> debugCamera_;

	bool isDebugCameraActive_ = false;

	//当たり判定↓↓↓

	// プレイヤーの当たり判定
	float playerFlontZ_;
	float playerBackZ_;
	float playerRightX_;
	float playerLeftX_;

	// 鍵の当たり判定
	float keyFlontZ_;
	float keyBackZ_;
	float keyRightX_;
	float keyLeftX_;

	// 床の当たり判定
	float groundFlontZ_;
	float groundBackZ_;
	float groundRightX_;
	float groundLeftX_;

	float testFlontZ_;
	float testBackZ_;
	float testRightX_;
	float testLeftX_;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
