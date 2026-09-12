#pragma once

class ExpItem
{
public:
	ExpItem(const float expValue, const float moveSpeed, const VECTOR spawnPosition, const int modelHandle);
	~ExpItem();

	void update(const VECTOR playerPosition);
	void draw();
	void setCanDestroy(const bool canDestroy);

	const float getExpValue()const noexcept { return exp_value; }
	const VECTOR getPosition()const noexcept { return position_; }
	const bool getCanDestroy()const noexcept { return canDestroy_; }

private:
	void moveToPlayer(const VECTOR playerPosition);
	VECTOR position_;
	bool canDestroy_;
	int modelHandle_;
	
	const float exp_value;
	const float move_speed;

	const float model_scale = 3.0f;
};