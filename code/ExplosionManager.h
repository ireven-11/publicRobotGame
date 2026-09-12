#pragma once
#include<vector>
#include<memory>
#include<functional>

class Explosion;
class ExplosionEffect;

class ExplosionManager
{
public:
	ExplosionManager(const std::shared_ptr<ExplosionEffect>& explosionEffect, const std::function<void()>& soundPlayer);
	~ExplosionManager();

	void init();
	void update();
	void addActiveExplosion(const std::shared_ptr<Explosion>& explosion);

	const std::vector<std::shared_ptr<Explosion>> getActiveExplosion()const noexcept { return activeExplosion_; }

private:
	std::vector<std::shared_ptr<Explosion>> activeExplosion_;
	std::shared_ptr<ExplosionEffect> effect_;
	std::function<void()> playExplosionSound_;
};