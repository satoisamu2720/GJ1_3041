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

	
	modelGround_ = Model::CreateFromOBJ("ground", true);
	LoadGroundPopData();

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
	for (const std::unique_ptr<Ground>& ground_ : grounds_) {
		ground_->Update();
	}
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
	ImGui::Checkbox(" key Flag", &keyFlag_);
	ImGui::Checkbox(" next Flag", &nextFlag_[0]);
	ImGui::Checkbox(" next Flag", &nextFlag_[1]);
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

		keyFlag_ = key_->IsDead();

		keyBackZ_ = key_->GetWorldPosition().z - 1.0f;
		keyFlontZ_ = key_->GetWorldPosition().z + 1.0f;
		keyLeftX_ = key_->GetWorldPosition().x - 1.0f;
		keyRightX_ = key_->GetWorldPosition().x + 1.0f;

		if ((playerLeftX_ < keyRightX_ && keyRightX_ > keyLeftX_) && (keyFlontZ_ > playerBackZ_ && keyBackZ_ < playerFlontZ_)) {

			keyFlag_ = true;
			nextFlag_[i] = true;
			i += 1;

			if (keyFlag_) {
				key_->SetKeyFlag(keyFlag_);
			}
		}
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

	// デスフラグの立った敵を削除
	grounds_.remove_if([](std::unique_ptr<Ground>& item) {
		if (item->IsDead()) {
			item.release();
			return true;
		}
		return false;
	});

	// ボックスのCSVファイルの更新処理
	UpdateGroundPopCommands();

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

#pragma region 床 CSV

void GameScene::LoadGroundPopData() {
	groundPopCommands.clear();
	std::ifstream file;
	file.open("Resources/CSV/GroundPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	groundPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateGroundPopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(groundPopCommands, line)) {
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

			GroundGenerate({x, y, z});
		}
	}
}

void GameScene::GroundGenerate(Vector3 position) {

	// アイテムの生成と初期化処理
	Ground* ground_ = new Ground();
	ground_->Initialize(modelGround_, position);
	grounds_.push_back(static_cast<std::unique_ptr<Ground>>(ground_));
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
	for (const std::unique_ptr<Ground>& ground_ : grounds_) {
		ground_->Draw(viewProjection_);
	}
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
	
}