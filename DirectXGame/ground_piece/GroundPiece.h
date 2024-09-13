#pragma once

#include "ImGuiManager.h"
#include "Model.h"
#include "baseCharacter/BaseCharacter.h"
#include "WorldTransform.h"
#include <cassert>
#include <list>
#include <optional>
#include <stdio.h>
class GroundPiece : public BaseCharacter {
public:
	void Initialize(const std::vector<Model*>& models, Vector3 position, Vector3 rotation);

	void Update();

	void Draw(ViewProjection& view);

	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

	void SetGroundPieceFlag(bool keyFlag) { isDead_ = keyFlag; }

	void SetNextStageKey(bool nextStageKey) { nextStageKey_ = nextStageKey; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	// デスフラグ
	bool isDead_ = false;
	bool nextStageKey_ = false;
};
