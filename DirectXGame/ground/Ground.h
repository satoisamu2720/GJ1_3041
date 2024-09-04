#pragma once
#include <cassert>
#include "Model.h"
#include "WorldTransform.h"

class Ground {
public:
	void Initialize(Model* model, Vector3 position);

	void Update();

	void Draw(ViewProjection& view);

public:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};
