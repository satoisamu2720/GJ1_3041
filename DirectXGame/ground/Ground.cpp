#include "Ground.h"

void Ground::Initialize(Model* model) {
	assert(model);
	model_ = model;


	groundPosition_[0] = {0.0f, 8.0f, 0.0f};
	groundPosition_[1] = {0.0f, 0.0f, 0.0f};

	for (int i = 0; i < 15; i++) {
		worldTransform_[i].Initialize();
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
		worldTransform_[i].translation_ = groundPosition_[i];
		worldTransform_[i].UpdateMatrix();
	}
}

void Ground::Update() {}

void Ground::Draw(ViewProjection& view) {
	for (int i = 0; i < 15; i++) {
		model_->Draw(worldTransform_[0], view);
		model_->Draw(worldTransform_[1], view);
	}
}