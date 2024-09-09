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

class Player : public BaseCharacter {

private:
	WorldTransform worldTransform_;

	float move_ = 0.01f;

	// キャラクターの移動速度
	const float playerSpeed = 0.5f;

	Input* input_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

	// 左右のキーを押したときのフラグ
	bool leftFlag_ = false;
	bool rightFlag_ = false;

	bool selectSwitchFlag = true;
	float selectSwitchTimer = 120;

	// 回転の処理
	float degree;
	float rotf;

	// 移動を管理する変数
	int target_ = 0;

	// 初期ポジション
	float position_ = 90;
	// 　矢印を押したときの番号
	int stageCount_ = 0;

	enum class Parts {
		kBody,

		kRootLeftLeg,
		kRootRightLeg,
		kLeftLeg,
		kRightLeg,

		kRootLeftHand,
		kRootRightHand,
		kLeftHand,
		kRightHand,

		kMaxParts,
	};

	WorldTransform worldTransforms_[(int)Parts::kMaxParts];
	int32_t leftLegRotate_ = 0u;
	int32_t rightLegRotate_ = 0u;
	int32_t leftHandRotate_ = 0u;
	int32_t rightHandRotate_ = 0u;
	int32_t rotationSpeed_ = 1;

public:
	// プレイヤー初期化
	void Initialize(const std::vector<Model*>& models);

	// 更新処理
	void Update();

	// プレイヤー描画
	void Draw(ViewProjection& view);

	Vector3 GetWorldPosition();

	const WorldTransform& GetWorldTransform()
	{ return worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }


	~Player();


};
