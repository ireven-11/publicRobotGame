#pragma once
#include"IAnimation.h"

class PlayerStatus;

class SlideMoveAnim : public IAnimation<PlayerStatus>
{
public:
	SlideMoveAnim();
	~SlideMoveAnim();

	void init();
	void enter();
	void exit();
	void update(const std::shared_ptr<PlayerStatus>& status);

	const bool getOnUpdate()const noexcept { return onUpdate_; }

private:
	void attachFront();
	void attachBack();
	void attachRight();
	void attachLeft();
	void blend();
	void playingAnimation();
	void attach();
	void stickZeroGrace();
	void setStickInfo();
	void detachPrevAnim();
	void detachNonOneAnim();
	
	int		modelHandle_;
	bool	onUpdate_;
	bool	canRelease_;
	float	currentTotalTime_;
	int		frontAnimAttach_;
	int		backAnimAttach_;
	int		rightAnimAttach_;
	int		leftAnimAttach_;
	float	currentTime_;
	float	blendRate_;
	float	curretnSitckAngle_;
	float	returnIdleCounter_;
	bool	canReturnIdle_;

	int		prevAnimAttachFrontBack_;
	int		prevAnimAttachRightLeft_;
	float	prevBlendCounter_;
	float	prevBlendRate_;
	VECTOR	prevStickPostion_;
	VECTOR	currentStickPostion_;

	int prevAttachFront_;
	int prevAttachBack_;
	int prevAttachRight_;
	int prevAttachLeft_;
	
	const float right_pi				= 0.0f;
	const float left_pi					= DX_PI_F;
	const float front_pi				= -DX_PI_F / 2;
	const float back_pi					= DX_PI_F / 2;
	const char	back_anim_index			= 3;
	const char	front_anim_index		= 0;
	const char	left_anim_index			= 1;
	const char	right_anim_index		= 2;
	const float max_slide_time			= 10.0f;
	const float anim_speed				= 0.1f;
	const float add_counter_value		= 0.02f;
	const int	max_return_idle_count	= 60;
};