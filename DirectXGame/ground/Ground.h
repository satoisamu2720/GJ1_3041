#pragma once
#include "ImGuiManager.h"
#include "Model.h"
#include "WorldTransform.h"
#include <list>
#include <cassert>
#include <optional>
#include <stdio.h>

class Ground {
public:
	void Initialize(Model* model, Vector3 position);

	void Update();

	void Draw(ViewProjection& view);

	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

	void SetGroundFlag(bool keyFlag) { isDead_ = keyFlag; }

public:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	// デスフラグ
	bool isDead_ = false;
};
