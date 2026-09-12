#pragma once
#include"IAnimation.h"

class PlayerStatus;

class HoldUpRightGunAnim : public IAnimation<PlayerStatus>
{
public:
	HoldUpRightGunAnim();
	~HoldUpRightGunAnim();

	void init();
	void enter();
	void exit();
	void update(const std::shared_ptr<PlayerStatus>& status);
	
	const bool getOnUpdate()const noexcept { return onUpdate_; }

private:
	void downArm(const std::shared_ptr<PlayerStatus>& status);
	void faceArmToTargetPosition(const std::shared_ptr<PlayerStatus>& status);
	void reset();

	bool onUpdate_;

	bool	canRelease_;
	float	dropGunTime_;
	float	prevRaiseRotation_;
	float	prevDownRotation_;
	double	zBoneRotation_;

	const float init_rotation			= 0.0f;
	const float ready_gun_max_rota_z	= -DX_PI_F / 2;	//èeç\Ç¶ç≈ëÂÇöâÒì]
	const float max_lerp_time			= 1.0f;
	const float drop_gun_speed			= 0.2f;			//èeÇâ∫Ç∑ÉXÉsÅ[Éh
};