#pragma once

class Dash;
class Walk;
class Descent;
class Raise;
class RightAttack;
class LeftAttack;
class ShoulderAttack;
class Interact;
class CharaStatus;

class PlayerAction
{
public:
	PlayerAction();
	~PlayerAction();

	void init();
	void dash(const std::shared_ptr<CharaStatus>& status, const bool canUpdate);
	void walk(const std::shared_ptr<CharaStatus>& status, const bool canUpdate);
	void descent(const std::shared_ptr<CharaStatus>& status, const bool canUpdate);
	void raise(const std::shared_ptr<CharaStatus>& status, const bool canUpdate);
	void rightAttack(const std::shared_ptr<CharaStatus>& status, const bool canUpdate);
	void leftAttack(const std::shared_ptr<CharaStatus>& status, const bool canUpdate);
	void shoulderAttack(const std::shared_ptr<CharaStatus>& status, const bool canUpdate);
	void interact(const std::shared_ptr<CharaStatus>& status, const bool canUpdate);

	const std::shared_ptr<Walk> getWalk()const noexcept { return walk_; }

private:
	bool isDash_;
	bool isWalk_;

	//ƒAƒNƒVƒ‡ƒ“
	std::shared_ptr<Dash> dash_;
	std::shared_ptr<Walk> walk_;
	std::shared_ptr<Descent> descent_;
	std::shared_ptr<Raise> raise_;
	std::shared_ptr<RightAttack> rightAttack_;
	std::shared_ptr<LeftAttack> leftAttack_;
	std::shared_ptr<ShoulderAttack> shoulderAttack_;
	std::shared_ptr<Interact> interact_;
};