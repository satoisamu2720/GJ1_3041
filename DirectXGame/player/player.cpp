#include "Player.h"

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	input_ = Input::GetInstance();
	// 初期化
	worldTransform_.Initialize();

	// 初期ポジティブ
	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
	worldTransform_.UpdateMatrix();
}

void Player::Update() {

	move_ = {0, 0, 0};

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 押した方向で移動ベクトルを変更（左右）
		if (joyState.Gamepad.sThumbLX < -100) {
			move_.x -= playerSpeed;

		} else if (joyState.Gamepad.sThumbLX > 100) {
			move_.x += playerSpeed;
		}
	}

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)) {
		move_.x -= playerSpeed;

	} else if (input_->PushKey(DIK_D)) {
		move_.x += playerSpeed;
	}


	worldTransform_.translation_ = Add(worldTransform_.translation_, move_);
	move_ = TransformNormal(move_, MakeRotateYmatrix(viewProjection_->rotation_.y));
	
	worldTransform_.UpdateMatrix();


#ifdef _DEBUG
	//GUI表示

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
};