#include "Magma.h"

void Magma::Initialize(Model* model) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_.y = 20.0f;
	worldTransform_.scale_ = {50.0f, 1.0f, 50.0f};
}

void Magma::Update() {
	worldTransform_.translation_.y -= fallSpeed_;
	worldTransform_.UpdateMatrix();

	ImGui::Begin("Magma");
	ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();
}

void Magma::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection); 
}

Vector3 Magma::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}