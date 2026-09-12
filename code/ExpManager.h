#pragma once
#include<vector>
#include<memory>

class ExpItem;
class EnemyBase;

class ExpManager
{
public:
	ExpManager();
	~ExpManager();

	void init();
	void update(const std::shared_ptr<PlayerStatus>& playerStatus, const std::vector<std::shared_ptr<EnemyBase>>& enemies);
	void draw();

private:
	void expItemFactory(const std::vector<std::shared_ptr<EnemyBase>>& enemies);
	void eraceAndRemoveFromVector();
	
	std::vector<std::shared_ptr<ExpItem>> expItems_;
	static int expModel_;

	const float exp_move_speed = 10.0f;
};