#pragma once


class PlayerModel {
public:
	~PlayerModel();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
						 
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


private:
	enum class Pats {
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

	



};
