#include "GroundPiece.h"

void GroundPiece::Initialize(const std::vector<Model*>& models, Vector3 position, Vector3 rotation) {
	BaseCharacter::Initialize(models);

	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = rotation;
	worldTransform_.translation_ = position;
}

void GroundPiece::Update() {
	worldTransform_.UpdateMatrix();

#ifdef _DEBUG

	ImGui::Begin("Ground");
	// ImGui::Checkbox("Key Flag", &isDead_);
	ImGui::End();

#endif
}

void GroundPiece::Draw(ViewProjection& view) { models_[0]->Draw(worldTransform_, view); }

Vector3 GroundPiece::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
};
