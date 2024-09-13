#include "Wall.h"

void Wall::Initialize(Model* model, Vector3 position, Vector3 rotation) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	
}

void Wall::Update() { 
#ifdef _DEBUG
	// GUI表示
	ImGui::Begin("Wall");
	ImGui::DragFloat3(" Position", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3(" Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
#endif
	worldTransform_.UpdateMatrix(); }

void Wall::Draw(ViewProjection& view) { model_->Draw(worldTransform_, view); }

Vector3 Wall::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};