#pragma once

#include "ImGuiManager.h"
#include <cassert>
#include "Model.h"
#include "WorldTransform.h"
class Wall {
public:
	void Initialize(Model* model, Vector3 position, Vector3 rotation);

	void Update();

	void Draw(ViewProjection& view);
	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

	void SetGroundFlag(bool keyFlag) { isDead_ = keyFlag; }

public:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	bool isDead_ = false;
};
