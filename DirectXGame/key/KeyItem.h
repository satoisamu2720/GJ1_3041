#pragma once

#include "ImGuiManager.h"
#include "Model.h"
#include "WorldTransform.h"
#include <list>
#include <cassert>
#include <optional>
#include <stdio.h>

class KeyItem {
public:
	void Initialize(Model* model, Vector3 position);

	void Update();

	void Draw(ViewProjection& view);

	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

	void SetKeyFlag(bool keyFlag) { 
	
			isDead_ = keyFlag;
	}
	//void SetTranslate(Vector3 translate) { worldTransform_.translation_ = translate; }

public:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	// デスフラグ
	bool isDead_ = false;
};