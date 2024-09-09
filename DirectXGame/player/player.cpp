#include "Player.h"

#define LEG_ROTATE_MAX 30

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	input_ = Input::GetInstance();
	// 初期化
	worldTransform_.Initialize();

	// 初期ポジティブ
	//worldTransform_.translation_ = position;

	degree = position_;

	rotf = DirectX::XMConvertToRadians(degree);

	worldTransform_.scale_ = {0.2f, 0.2f, 0.2f};

	worldTransforms_[static_cast<int>(Parts::kBody)].parent_ = &worldTransform_;
	worldTransforms_[static_cast<int>(Parts::kRootLeftLeg)].parent_ = &worldTransforms_[static_cast<int>(Parts::kBody)];
	worldTransforms_[static_cast<int>(Parts::kRootRightLeg)].parent_ = &worldTransforms_[static_cast<int>(Parts::kBody)];

	worldTransforms_[static_cast<int>(Parts::kLeftLeg)].parent_ = &worldTransforms_[static_cast<int>(Parts::kRootLeftLeg)];
	worldTransforms_[static_cast<int>(Parts::kRightLeg)].parent_ = &worldTransforms_[static_cast<int>(Parts::kRootRightLeg)];

	// 手
	worldTransforms_[static_cast<int>(Parts::kRootLeftHand)].parent_ = &worldTransforms_[static_cast<int>(Parts::kBody)];
	worldTransforms_[static_cast<int>(Parts::kRootRightHand)].parent_ = &worldTransforms_[static_cast<int>(Parts::kBody)];
	worldTransforms_[static_cast<int>(Parts::kLeftHand)].parent_ = &worldTransforms_[static_cast<int>(Parts::kRootLeftHand)];
	worldTransforms_[static_cast<int>(Parts::kRightHand)].parent_ = &worldTransforms_[static_cast<int>(Parts::kRootRightHand)];

	for (int i = 0; i < (int)Parts::kMaxParts; i++) {
		// 定数バッファ作成、これ呼ばないとTranferMatrixでエラーが起きる
		worldTransforms_[i].Initialize();
	}

	worldTransforms_[static_cast<int>(Parts::kBody)].translation_ = {0, 7.7f, 0};
	
	worldTransforms_[static_cast<int>(Parts::kLeftLeg)].translation_ = {0.0f, -3.0f, 0.0f};
	worldTransforms_[static_cast<int>(Parts::kRightLeg)].translation_ = {0.0f, -3.0f, 0.0f};
	worldTransforms_[static_cast<int>(Parts::kRootLeftLeg)].translation_ = {2.0f, -3.0f, 0.0f};
	worldTransforms_[static_cast<int>(Parts::kRootRightLeg)].translation_ = {-2.0f, -3.0f, 0.0f};

	worldTransforms_[static_cast<int>(Parts::kLeftHand)].translation_ = {1.0f, -0.5f, 0.0f};
	worldTransforms_[static_cast<int>(Parts::kRightHand)].translation_ = {-1.0f, -0.5f, 0.0f};
	worldTransforms_[static_cast<int>(Parts::kRootLeftHand)].translation_ = {2.0f, -0.5f, 0.0f};
	worldTransforms_[static_cast<int>(Parts::kRootRightHand)].translation_ = {-2.0f, -0.5f, 0.0f};

	worldTransforms_[static_cast<int>(Parts::kRootRightHand)].rotation_ = {0.0f, 0.0f, ToRadian(-20)};
	worldTransforms_[static_cast<int>(Parts::kRootLeftHand)].rotation_ = {0.0f, 0.0f, ToRadian(20)};
}

void Player::Update() {

	

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 押した方向で移動ベクトルを変更（左右）
		degree -= move_;
		rotf = DirectX::XMConvertToRadians(degree);
		leftFlag_ = true;
		worldTransform_.rotation_.y = 0.1f;
	} else {
		leftFlag_ = false;
	}
	
	if (joyState.Gamepad.sThumbLX > 100) {
		degree += move_;
		rotf = DirectX::XMConvertToRadians(degree);
		rightFlag_ = true;
	} else {
		rightFlag_ = false;
	}

	if (input_->PushKey(DIK_LEFT) && leftFlag_ == false && rightFlag_ == false || input_->PushKey(DIK_A) ) {
		degree -= move_;
		rotf = DirectX::XMConvertToRadians(degree);
		leftFlag_ = true;
		
	} 
	else {
		leftFlag_ = false;
	}

	if (input_->PushKey(DIK_RIGHT) && leftFlag_ == false && rightFlag_ == false || input_->PushKey(DIK_D)) {
		degree += move_;
		rotf = DirectX::XMConvertToRadians(degree);
		rightFlag_ = true;
	} else {
		rightFlag_ = false;
	}

	if (input_->PushKey(DIK_UP) || input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += 0.1f;
		
	} 

	worldTransform_.translation_.x = -cosf(rotf) * 10.0f;
	worldTransform_.translation_.z = -sinf(rotf) * 10.0f;
	

	worldTransform_.translation_ = TransformNormal(worldTransform_.translation_, MakeRotateYmatrix(viewProjection_->rotation_.y));
	// Y軸周り角度
	worldTransform_.rotation_.y = std::atan2(worldTransform_.translation_.x, worldTransform_.translation_.z);
	// ベクターの加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, worldTransform_.translation_); 
	
	
	worldTransform_.UpdateMatrix();

	worldTransforms_[static_cast<int>(Parts::kRootLeftLeg)].rotation_.x = ToRadian(leftLegRotate_);
	worldTransforms_[static_cast<int>(Parts::kRootRightLeg)].rotation_.x = ToRadian(-leftLegRotate_);

	worldTransforms_[static_cast<int>(Parts::kRootLeftHand)].rotation_.x = ToRadian(-leftHandRotate_);
	worldTransforms_[static_cast<int>(Parts::kRootRightHand)].rotation_.x = ToRadian(leftHandRotate_);

	// 足
	leftLegRotate_ += rotationSpeed_; // leftLegRotate_ = leftLegRotate_ + rotationSpeed_
	if (leftLegRotate_ >= LEG_ROTATE_MAX || leftLegRotate_ <= -LEG_ROTATE_MAX) {
		rotationSpeed_ = -rotationSpeed_;
	}

	// 手
	leftHandRotate_ += rotationSpeed_; // leftHandRotate_ = leftHandRotate_ + rotationSpeed_
	
	for (int i = 0; i < (int)Parts::kMaxParts; i++) {
		// アフィン変換 -> 回転や拡大縮小、平行移動行うための行列
		// worldTransformのrotation,translation,scale
		// アフィン変換してワールド行列計算、ワールド行列を転送
		worldTransforms_[i].UpdateMatrix();
	}

#ifdef _DEBUG
	// GUI表示
	ImGui::Begin("Player");
	ImGui::DragFloat3("Player Position", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Player Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
#endif
}

void Player::Draw(ViewProjection& view) {

	models_[0]->Draw(worldTransforms_[static_cast<int>(Parts::kBody)], view);
	models_[1]->Draw(worldTransforms_[static_cast<int>(Parts::kRightLeg)], view);
	models_[1]->Draw(worldTransforms_[static_cast<int>(Parts::kLeftLeg)], view);
	models_[2]->Draw(worldTransforms_[static_cast<int>(Parts::kRightHand)], view);
	models_[2]->Draw(worldTransforms_[static_cast<int>(Parts::kLeftHand)], view);
}

Vector3 Player::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}