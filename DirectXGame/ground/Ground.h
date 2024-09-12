#pragma once
#define _USE_MATH_DEFINES
#include "ImGuiManager.h"
#include "Input.h"
#include "MT/MT.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "baseCharacter/BaseCharacter.h"
#include "math.h"
#include <cassert>
#include <list>
#include <optional>
#include <stdio.h>
#include <DirectXMath.h>

class Ground : public BaseCharacter {
public:
	void Initialize(const std::vector<Model*>& models);

	void Update();

	void Draw(ViewProjection& view);

	Vector3 GetWorldPositionOne();

	bool IsDead() const { return isDead_; }

	void SetGroundFlag(bool keyFlag) { isDead_ = keyFlag; }

	void SetNextStageFlag(bool nextStageFlag) { nextStageFlag_ = nextStageFlag; }

	void SetNextStageKey(int nextStageKey) { nextStageKey_ = nextStageKey; }

private:
	WorldTransform worldTransform_[5];

	WorldTransform worldTransformHole_[5];

	const ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;
	Input* input_ = nullptr;
	// 回転の処理
	float degree;
	float rotf;

	// 移動を管理する変数
	int target_ = 0;

	// 初期ポジション
	float position_ = 90;
	// 　矢印を押したときの番号
	int stageCount_ = 0;

	// デスフラグ
	bool isDead_ = false;

	bool nextStageFlag_ = false;

	int nextStageKey_ = 0;

	float ttt = 0.52f;
};
