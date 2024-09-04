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

class Player : public BaseCharacter {

private:
	WorldTransform worldTransform_;

	Vector3 position = {0.0f, 0.0f, 0.0f};

	Vector3 move_ = {0, 0, 0};

	// キャラクターの移動速度
	const float playerSpeed = 0.5f;

	Input* input_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;


public:
	// プレイヤー初期化
	void Initialize(const std::vector<Model*>& models);

	// 更新処理
	void Update();

	// プレイヤー描画
	void Draw(ViewProjection& view);

	Vector3 GetWorldPosition();

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

	~Player();
};
