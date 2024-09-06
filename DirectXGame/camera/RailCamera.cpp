#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation) {

	viewProjection_.Initialize();
	viewProjection_.translation_ = position;
	viewProjection_.rotation_ = rotation;

	input_ = Input::GetInstance();

	degree = position_;

	rotf = DirectX::XMConvertToRadians(degree);

}

void RailCamera::Update() {
	
	// 追従対象からカメラまでのオフセット
	Vector3 offset_ = {0.0f, 2.0f, -30.0f};
	if (target_) {

		float kRotSpeed = 0.04f;

		if (input_->PushKey(DIK_LEFT) && leftFlag_ == false && rightFlag_ == false || input_->PushKey(DIK_A)) {
			degree -= move_;
			rotf = DirectX::XMConvertToRadians(degree);
			leftFlag_ = true;
			
		} else {
			leftFlag_ = false;
		}

		if (input_->PushKey(DIK_RIGHT) && leftFlag_ == false && rightFlag_ == false || input_->PushKey(DIK_D)) {
			degree += move_;
			rotf = DirectX::XMConvertToRadians(degree);
			rightFlag_ = true;
		} else {
			rightFlag_ = false;
		}

		viewProjection_.translation_.x = -cosf(rotf) * 10.0f;
		viewProjection_.translation_.z = -sinf(rotf) * 10.0f;
	

		// 押した方向で移動ベクトルを変更（上下）
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.rotation_.x += kRotSpeed;
		} else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.rotation_.x -= kRotSpeed;
		}
		/*rotateYMatrix_ = MakeRotateYmatrix(viewProjection_.rotation_.y);
		offset_ = TransformNormal(offset_, rotateYMatrix_);*/
		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset_);
	}

	viewProjection_.UpdateMatrix();

#ifdef _DEBUG
	// GUI表示
	ImGui::Begin("offset Camera");
	ImGui::DragFloat3("Camera Position", &viewProjection_.translation_.x, 0.01f);
	ImGui::DragFloat3("Camera Rotation", &viewProjection_.rotation_.x, 0.01f);
	ImGui::End();
#endif
}