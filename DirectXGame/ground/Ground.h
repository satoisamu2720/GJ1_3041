#pragma once
#include <cassert>
#include "Model.h"
#include "WorldTransform.h"

class Ground {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& view);

public:
	WorldTransform worldTransform_[15];
	Vector3 groundPosition_[15];
	Model* model_ = nullptr;
};
