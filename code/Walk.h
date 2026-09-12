#pragma once
#include"ActionBase.h"

class Walk : public ActionBase
{
public:
	Walk();
	~Walk();

	void update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate);

	const VECTOR getMoveDirection()const noexcept { return moveDirection_; }

protected:
	static VECTOR	moveDirection_;
	static VECTOR	prevMoveDirection_;
	static bool		isTurn_;

	const VECTOR left_direction	= VGet(-1.0f, 0.0f, 0.0f);
};