#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

#pragma region プレイヤー初期

	// 自キャラモデル読み込み

	modelPlayer_.reset(Model::CreateFromOBJ("float_Body", true));
	
	// 自キャラモデル配列
	std::vector<Model*> playerModels = {
	    modelPlayer_.get(),
	};
	// プレイヤー初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);

#pragma endregion

#pragma region カメラ初期

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());

#pragma endregion

#pragma region 地面初期

	
		modelGround_.reset(Model::CreateFromOBJ("ground_piece", true));
		// 床モデル配列
		std::vector<Model*> groundModels = {
	        modelGround_.get(),
		};
	    /*std::vector<WorldTransform*> groundPos = {
	        
	    };*/
		ground_ = std::make_unique<Ground>();
	    ground_->Initialize(groundModels);
	
#pragma endregion


#pragma region 柱初期化

	modelPillar_ = Model::CreateFromOBJ("pillar", true);
	pillar_ = std::make_unique<Pillar>();
	pillar_->Initialize(modelPillar_, {0.0f, 0.0f, 0.0f});

#pragma endregion


#pragma region 鍵初期化

	modelKey_ = Model::CreateFromOBJ("key", true);
	// ボックスのCSVファイル読み込み
	LoadKeyPopData();

#pragma endregion

	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	

}

void GameScene::Update() {

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_LSHIFT) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	} else if (input_->TriggerKey(DIK_LSHIFT) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}
#endif

#pragma region カメラ処理
	// カメラ処理
	if (isDebugCameraActive_ == true) {
		debugCamera_->Update();
		followCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {

		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
#pragma endregion

#pragma region Update() アップデート
	// プレイヤー
	player_->Update();
	// 地面
	
	ground_->Update();
	// 柱
	pillar_->Update();
	// 鍵
	for (const std::unique_ptr<KeyItem>& key_ : keys_) {
		key_->Update();
	}

#pragma endregion

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_R)) {
		Reset();
	}

	ImGui::Begin("Flag");
	ImGui::Checkbox(" Key Flag", &keyFlag_);
	ImGui::Checkbox(" Stage 1 Flag", &nextFlag_[0]);
	ImGui::Checkbox(" Stage 2 Flag", &nextFlag_[1]);
	ImGui::Checkbox(" openTimerFlag", &openTimerFlag);
	ImGui::DragFloat("openTimer", &openTimer);
	
	ImGui::End();

#endif

#pragma region プレイヤーの当たり判定

	playerBackZ_ = player_->GetWorldPosition().z - 1.0f;
	playerFlontZ_ = player_->GetWorldPosition().z + 1.0f;
	playerLeftX_ = player_->GetWorldPosition().x - 1.0f;
	playerRightX_ = player_->GetWorldPosition().x + 1.0f;

#pragma endregion

#pragma region プレイヤーと鍵の当たり判定

	for (const std::unique_ptr<KeyItem>& key_ : keys_) {
		
		for (int i = 0; i < 10; i++) {

			keyFlag_ = key_->IsDead();

			keyBackZ_ = key_->GetWorldPosition().z - 1.0f;
			keyFlontZ_ = key_->GetWorldPosition().z + 1.0f;
			keyLeftX_ = key_->GetWorldPosition().x - 1.0f;
			keyRightX_ = key_->GetWorldPosition().x + 1.0f;

			if ((playerLeftX_ < keyRightX_ && playerRightX_ > keyLeftX_) && (keyFlontZ_ > playerBackZ_ && keyBackZ_ < playerFlontZ_)) {

			keyFlag_ = true;
			nextFlag_[keyCount] = true;
			keyCount += 1;

				if (openTimerFlag) {
				}
				if (keyFlag_) {

					Vector3 tmpTranslate = ground_->GetRotation();

					tmpTranslate.y += 0.01f;

					key_->SetKeyFlag(keyFlag_);
					nextFlag_[nextStageKey] = true;
					nextStageKey += 1;
					ground_->SetRotation(tmpTranslate);
				}
			}
		}
		
	}
	if (openTimerFlag) {
		openTimer++;
		
	}
	if (openTimer >= 60) {
		openTimer = 0;
		openTimerFlag = false;
	}
#pragma endregion

#pragma region プレイヤーと床の当たり判定

	
			groundBackZ_ = ground_->GetWorldPosition().z - 1.0f;
			groundFlontZ_ = ground_->GetWorldPosition().z + 1.0f;
	        groundLeftX_ = ground_->GetWorldPosition().x - 1.0f;
	        groundRightX_ = ground_->GetWorldPosition().x + 1.0f;

			if ((playerLeftX_ < groundRightX_ && playerRightX_ > groundLeftX_) && (groundFlontZ_ > playerBackZ_ && groundBackZ_ < playerFlontZ_)) {

			}
#pragma endregion

#pragma region CSV 更新処理, デスフラグ
	// デスフラグの立った敵を削除
	keys_.remove_if([](std::unique_ptr<KeyItem>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});

	// ボックスのCSVファイルの更新処理
	UpdateKeyPopCommands();

#pragma endregion
}

#pragma region 鍵 CSV

void GameScene::LoadKeyPopData() {
	keyPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/KeyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	keyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateKeyPopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(keyPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;
		// 　,区切りで行の先頭文字列を所得

		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			KeyGenerate({x, y, z});
		}
	}
}

void GameScene::KeyGenerate(Vector3 position) {

	// アイテムの生成と初期化処理
	KeyItem* key_ = new KeyItem();
	key_->Initialize(modelKey_, position);
	keys_.push_back(static_cast<std::unique_ptr<KeyItem>>(key_));
}

#pragma endregion




void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理

	//プレイヤー
	player_->Draw(viewProjection_);
	//地面
	
		ground_->Draw(viewProjection_);
	
	//柱
	pillar_->Draw(viewProjection_);
	//鍵
	for (const std::unique_ptr<KeyItem>& key_ : keys_) {
		key_->Draw(viewProjection_);
	}


	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}



void GameScene::Reset() {
	LoadKeyPopData();
		nextFlag_[0] = false;
		nextFlag_[1] = false;
		keyCount = 0;
}