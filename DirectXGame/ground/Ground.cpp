#include "Ground.h"

void Ground:: Initialize(const std::vector<Model*>& models){
	BaseCharacter::Initialize(models);

	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	/*worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;*/

	degree = position_;

	input_ = Input::GetInstance();
	rotf = DirectX::XMConvertToRadians(degree);
}

void Ground::Update() {

	Vector3 move_ = {0, 0, 0};

	if (input_->PushKey(DIK_L)) {
		degree -= 1.0f;
		rotf = DirectX::XMConvertToRadians(degree);
		
	} else {
	
	}

	if (input_->PushKey(DIK_K)) {
		degree += 1.0f;
		rotf = DirectX::XMConvertToRadians(degree);
		
	} 
	else {
	}

	//move_ = TransformNormal(move_, MakeRotateYmatrix(viewProjection_->rotation_.y));
	// Y軸周り角度
	worldTransform_.rotation_.y = std::atan2(worldTransform_.translation_.x, worldTransform_.translation_.z);
	// ベクターの加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move_); 
	
	worldTransform_.translation_.x = -cosf(rotf) * 1.0f;
	worldTransform_.translation_.z = -sinf(rotf) * 1.0f;

	worldTransform_.UpdateMatrix();
#ifdef _DEBUG

	ImGui::Begin("Ground");
	ImGui::DragFloat3("Ground Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();

#endif
}

void Ground::Draw(ViewProjection& view) { models_[0]->Draw(worldTransform_, view); }

Vector3 Ground::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};