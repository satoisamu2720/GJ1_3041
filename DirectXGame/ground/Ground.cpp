#include "Ground.h"

void Ground:: Initialize(const std::vector<Model*>& models){
	BaseCharacter::Initialize(models);

	for (int i = 0; i < 5; i++) {
		worldTransform_[i].Initialize();
		worldTransform_[0].translation_ = {0, 8, 0};
		worldTransform_[1].translation_ = {0, 0, 0};
		worldTransform_[2].translation_ = {0, -8, 0};
		worldTransform_[3].translation_ = {0, -16, 0};
		worldTransform_[4].translation_ = {0, -24, 0};
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
		worldTransform_[i].rotation_ = {0.0f, 0.0f, 0.0f};

		worldTransformHole_[i].Initialize();
		worldTransformHole_[0].translation_ = {0, 8, 0};
		worldTransformHole_[1].translation_ = {0, 0, 0};
		worldTransformHole_[2].translation_ = {0, -8, 0};
		worldTransformHole_[3].translation_ = {0, -16, 0};
		worldTransformHole_[4].translation_ = {0, -24, 0};
		worldTransformHole_[i].scale_ = {1.0f, 1.0f, 1.0f};

		rotf = DirectX::XMConvertToRadians(degree);
		
	}

	input_ = Input::GetInstance();
	
}

void Ground::Update() {

	Vector3 move_ = {0, 0, 0};

	if (input_->PushKey(DIK_L) || nextStageFlag_) {
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
	for (int i = 0; i < 5; i++) {
		// move_ = TransformNormal(move_, MakeRotateYmatrix(viewProjection_->rotation_.y));
		//  Y軸周り角度
		worldTransformHole_[i].rotation_.y = std::atan2(worldTransformHole_[i].translation_.x, worldTransformHole_[i].translation_.z);
		// ベクターの加算
		worldTransformHole_[i].translation_ = Add(worldTransformHole_[i].translation_, move_);

		worldTransformHole_[nextStageKey_].translation_.x = -cosf(rotf) * 1.0f;
		worldTransformHole_[nextStageKey_].translation_.z = -sinf(rotf) * 1.0f;

		//  Y軸周り角度
		worldTransform_[i].rotation_.y = std::atan2(worldTransform_[i].translation_.x, worldTransform_[i].translation_.z);
		// ベクターの加算
		worldTransform_[i].translation_ = Add(worldTransform_[i].translation_, move_);

		worldTransform_[i].translation_.x = -cosf(ttt) * 1.0f;
		worldTransform_[i].translation_.z = -sinf(ttt) * 1.0f;

		worldTransformHole_[i].UpdateMatrix();

		worldTransform_[i].UpdateMatrix();
	}
#ifdef _DEBUG

	ImGui::Begin("Ground");
	ImGui::DragFloat3("Ground Rotation", &worldTransform_[1].rotation_.x, 0.01f);
	ImGui::DragFloat("Ground translation_", &ttt, 0.01f);
	ImGui::End();

#endif
}

void Ground::Draw(ViewProjection& view) { 
	for (int i = 0; i < 5; i++) {
		models_[0]->Draw(worldTransform_[i], view);
		models_[0]->Draw(worldTransformHole_[i], view);
	}
}

Vector3 Ground::GetWorldPositionOne() {
	Vector3 worldPos;

	worldPos.x = worldTransformHole_[0].matWorld_.m[3][0];
	worldPos.y = worldTransformHole_[0].matWorld_.m[3][1];
	worldPos.z = worldTransformHole_[0].matWorld_.m[3][2];

	return worldPos;
};