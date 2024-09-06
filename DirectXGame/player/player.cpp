#include "Player.h"

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	input_ = Input::GetInstance();
	// 初期化
	worldTransform_.Initialize();

	// 初期ポジティブ
	//worldTransform_.translation_ = position;

	/*degree = position_;

	rotf = DirectX::XMConvertToRadians(degree);*/

	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
}

void Player::Update() {

	Vector3 move{0,0,0};

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

	if (input_->PushKey(DIK_I)) {
		move.z += playerSpeed;
	} else if (input_->PushKey(DIK_K)) {
		move.z -= playerSpeed;
	}

	/*if (input_->PushKey(DIK_LEFT) && leftFlag_ == false && rightFlag_ == false || input_->PushKey(DIK_A) ) {
		degree -= move_;
		rotf = DirectX::XMConvertToRadians(degree);
		leftFlag_ = true;
		worldTransform_.rotation_.y = 0.1f;
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

	worldTransform_.translation_.x = -cosf(rotf) * 10.0f;
	worldTransform_.translation_.z = -sinf(rotf) * 10.0f;*/
	

	move = TransformNormal(move, MakeRotateYmatrix(viewProjection_->rotation_.y));
	// Y軸周り角度
	worldTransform_.rotation_.y = std::atan2(worldTransform_.translation_.x, worldTransform_.translation_.z);
	// ベクターの加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move); 
	
	worldTransform_.UpdateMatrix();

#ifdef _DEBUG
	// GUI表示
	ImGui::Begin("Player");
	ImGui::DragFloat3("Player Position", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Player Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
#endif
}

void Player::Draw(ViewProjection& view) {

	models_[0]->Draw(worldTransform_, view);
}

Vector3 Player::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}