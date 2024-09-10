#include "Ground.h"

void Ground::Initialize(Model* model, Vector3 position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
}

void Ground::Update() {}

void Ground::Draw(ViewProjection& view) { model_->Draw(worldTransform_, view); }