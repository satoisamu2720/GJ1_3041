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

	

	// キャラクターの移動速度
	const float playerSpeed = 0.58f;

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

	float g = 0.1f;
	bool wallRightFlag = false;
	bool wallLeftFlag_ = false;

public:
	// プレイヤー初期化
	void Initialize(const std::vector<Model*>& models);

	// 更新処理
	void Update();

	// プレイヤー描画
	void Draw(ViewProjection& view);


	void SetTranslate(Vector3 translate) { worldTransform_.translation_ = translate; }

	Vector3 GetWorldPosition();

	const WorldTransform& GetWorldTransform()
	{ return worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

	void SetRightWallFlag(bool Flag) { wallRightFlag = Flag; }
	void SetLeftWallFlag(bool Flag) { wallLeftFlag_ = Flag; }

	~Player();


};
