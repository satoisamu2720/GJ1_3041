#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MT/MT.h"
#include "ImGuiManager.h"
#include "Input.h"
#include <memory>

class FollowCamera {

public:
	void Initialize(const Vector3& position, const Vector3& rotation);
	void Update();
	void SetTarget(const WorldTransform* target) { target_ = target; }

	//const Vector3& Getoffset() { return offset_; }
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const ViewProjection& GetEnemyViewProjection() { return enemyViewProjection_; }
	Matrix4x4 GetRotateYMatrix_() { return rotateYMatrix_; }

private:
	const WorldTransform* target_ = nullptr;
	ViewProjection viewProjection_;

	ViewProjection enemyViewProjection_;
	WorldTransform worldTransform_;
	Input* input_ = nullptr;
	Matrix4x4 rotateYMatrix_;
};
