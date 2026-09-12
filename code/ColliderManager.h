#pragma once

class IChara;
class IMapObject;
class IBullet;
class EnemyBase;
class Player;
class ITargetableObject;
class ExplosionManager;

class ColliderManager
{
public:
	ColliderManager();
	~ColliderManager();

	void update(const std::shared_ptr<Player>& player, const std::vector<std::shared_ptr<EnemyBase>>& enemies,
		const std::vector<std::shared_ptr<IMapObject>>& mapObject, const std::vector<std::shared_ptr<IBullet>>& bullets,
		const std::shared_ptr<ExplosionManager>& explosionManager);

private:
	void playerCollider(const std::shared_ptr<Player>& player, const std::vector<std::shared_ptr<IMapObject>>& mapObjects,
		const std::vector<std::shared_ptr<EnemyBase>>& enemies);
	void mapObjectWithBulletCollider(const std::vector<std::shared_ptr<IMapObject>>& mapObjects, const std::shared_ptr<IBullet>& bullet,
		const std::shared_ptr<ExplosionManager>& explosionManager);
	void charaWithBulletCollider(const std::vector<std::shared_ptr<IChara>>& charas, const std::shared_ptr<IBullet>& bullet,
		const std::shared_ptr<ExplosionManager>& explosionManager);
	void pushBackCharaWithMapObject(const std::vector<std::shared_ptr<IChara>>& charas, const std::vector<std::shared_ptr<IMapObject>>& mapObjects);
	void pushBackCharaWithChara(const std::vector<std::shared_ptr<IChara>>& charas);
	void enemyCollider(const std::vector<std::shared_ptr<EnemyBase>>& enemies, const std::vector<std::shared_ptr<IMapObject>>& mapObjects,
		const std::shared_ptr<Player>& player);
	void resetOnDamage(const std::vector<std::shared_ptr<IChara>>& charas, const std::vector<std::shared_ptr<IMapObject>>& mapObjects);
	void explosionWithEnemeisCollider(const std::vector<std::shared_ptr<EnemyBase>>& enemies, const std::shared_ptr<ExplosionManager>& explosionManager);
	void explosion(const std::shared_ptr<IBullet>& bullet, const std::shared_ptr<ExplosionManager>& explosionManager);
	void damage(const std::shared_ptr<ITargetableObject> target, const float damageValue);
};