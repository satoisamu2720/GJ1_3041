#pragma once
#include "ImGuiManager.h"
#include "Model.h"
#include "WorldTransform.h"

class Magma {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	const Vector3& GetScale() const { return worldTransform_.scale_; };

	Vector3 GetWorldPosition();

private:
	Model* model_ = nullptr;

	WorldTransform worldTransform_;

	float fallSpeed_ = 0.01f;
};
