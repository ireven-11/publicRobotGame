#pragma once
#include<vector>
#include<memory>

class IBullet;

class BulletManager
{
public:
	BulletManager();
	~BulletManager();

	void init();
	void update();
	void draw();
	void addActiveBullet(const std::shared_ptr<IBullet>& bullet);

	const std::vector<std::shared_ptr<IBullet>>& getActiveBullets()const noexcept { return activeBullets_; }

private:
	std::vector<std::shared_ptr<IBullet>> activeBullets_;
};