#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(const Vector3& v1, const float speed);
// 正規化
Vector3 Normalize(const Vector3& v);

// 長さ
float Length(const Vector3& v);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// プロトタイプ宣言
void Add(Vector3& v1, Vector3& v2);

Vector3 Add(const Vector3& v1, const Vector3& v2);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// アフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

void Move(Vector3& transform, Vector3& move);

Matrix4x4 Inverse(const Matrix4x4& m);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

Vector3 Add2(const Vector3& v1, const Vector3& v2);

float ToRadian(int degree);