#pragma once

#include "Audio.h"
#include "ImGuiManager.h"
#include "MT/MT.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "input.h"
#include <memory>
#include <DirectXMath.h>


class RailCamera {

public:
	void Initialize(const Vector3& position, const Vector3& rotation);
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }
	void SetParent(const WorldTransform* parent) {
		worldTransform_.parent_ = parent;
	}
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	const WorldTransform* target_ = nullptr;
	Input* input_ = nullptr;

	// キャラクターの移動速度
	float move_ = 0.04f;

	const float kCharacterSpeed = 2.0f;
	const float cameraSpeed = 0.05f;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Audio* audio_ = nullptr;

	// 左右のキーを押したときのフラグ
	bool leftFlag_ = false;
	bool rightFlag_ = false;

	// 回転の処理
	float degree;
	float rotf;

	// 初期ポジション
	float position_ = 90;

	bool isSpeedUp = false;
	bool isSpeedDown = false;
	
};

