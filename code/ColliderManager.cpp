#include"DxLibForIreven.h"
#include<vector>
#include<memory>
#include"IChara.h"
#include"IMapObject.h"
#include"ITargetableObject.h"
#include"Player.h"
#include"EnemyBase.h"
#include"ExplosionManager.h"
#include"ColliderManager.h"
#include"MapObjectStatus.h"
#include"PlayerStatus.h"
#include"IWeapon.h"
#include"WeaponStatus.h"
#include"IBullet.h"
#include"BulletStatus.h"
#include"ColliderEventHander.h"
#include"HitBulletEvent.h"
#include"ISphereCollider.h"
#include"ICuboidCollider.h"
#include"ICapsuleCollider.h"
#include"OnDamageEvent.h"
#include"Explosion.h"
#include"ExplosionBullet.h"
#include"ExplosionBulletStatus.h"

ColliderManager::ColliderManager()
{
}

ColliderManager::~ColliderManager()
{
}

void ColliderManager::update(const std::shared_ptr<Player>& player, const std::vector<std::shared_ptr<EnemyBase>>& enemies,
	const std::vector<std::shared_ptr<IMapObject>>& mapObjects, const std::vector<std::shared_ptr<IBullet>>& bullets,
	const std::shared_ptr<ExplosionManager>& explosionManager)
{
	//キャラクターは一括にして処理するようにする
	std::vector<std::shared_ptr<IChara>> charas;
	for (const auto& enemy : enemies)
	{
		charas.emplace_back(enemy);
	}
	charas.emplace_back(player);

	//当たり判定をする前にヒット状態をリセットしておく
	resetOnDamage(charas, mapObjects);

	//プレイヤー専用の判定
	playerCollider(player, mapObjects, enemies);

	//エネミー専用の判定
	enemyCollider(enemies, mapObjects, player);

	//弾の当たり判定
	for (const auto& bullet : bullets)
	{
		//マップオブジェクトと弾の当たり判定
		mapObjectWithBulletCollider(mapObjects, bullet, explosionManager);

		//キャラと弾の当たり判定
		charaWithBulletCollider(charas, bullet, explosionManager);
	}

	//敵と爆発の当たり判定
	explosionWithEnemeisCollider(enemies, explosionManager);

	//キャラとマップオブジェクトの押し戻し
	pushBackCharaWithMapObject(charas, mapObjects);

	//キャラ同士の押し戻し
	pushBackCharaWithChara(charas);
}

void ColliderManager::playerCollider(const std::shared_ptr<Player>& player, const std::vector<std::shared_ptr<IMapObject>>& mapObjects, const std::vector<std::shared_ptr<EnemyBase>>& enemies)
{
	const std::shared_ptr<PlayerStatus> playerStatus = std::static_pointer_cast<PlayerStatus>(player->getStatus());

	//ターゲットになることができるものをセット
	std::vector<std::shared_ptr<ITargetableObject>> targetableObjects;
	for (const auto& mapObject : mapObjects)
	{
		//マップオブジェクトのステータスがITargetableObjectを継承してるか
		const auto targetableObject = std::dynamic_pointer_cast<ITargetableObject>(mapObject->getStatus());
		if (!targetableObject) continue;

		targetableObjects.emplace_back(targetableObject);
	}
	for (const auto& enemy : enemies)
	{
		//敵のステータスがITargetableObjectを継承してるか
		const auto targetableObject = std::dynamic_pointer_cast<ITargetableObject>(enemy->getStatus());
		if (!targetableObject) continue;

		targetableObjects.emplace_back(targetableObject);
	}

	//ターゲッティング
	const std::shared_ptr<WeaponStatus> rightWeaponStatus	= playerStatus->getRightWeapon()->getWeaponStatus();
	const std::shared_ptr<WeaponStatus> leftWeaponStatus	= playerStatus->getLeftWeapon()->getWeaponStatus();
	rightWeaponStatus->startFindingTarget();
	leftWeaponStatus->startFindingTarget();
	for (const auto& targetableObject : targetableObjects)
	{
		rightWeaponStatus->setTargetingPosition(targetableObject->getTargetedPosition(),
			playerStatus->getScreenAttackRangeRadius(), targetableObject->getAdjustingAttackDistance());
		leftWeaponStatus->setTargetingPosition(targetableObject->getTargetedPosition(), 
			playerStatus->getScreenAttackRangeRadius(), targetableObject->getAdjustingAttackDistance());
	}
}

void ColliderManager::mapObjectWithBulletCollider(const std::vector<std::shared_ptr<IMapObject>>& mapObjects, const std::shared_ptr<IBullet>& bullet,
	const std::shared_ptr<ExplosionManager>& explosionManager)
{
	const auto bulletStatus = bullet->getStatus();

	for (const auto& mapObject : mapObjects)
	{
		//弾が使われてないなら以降の処理をしない
		if (!bulletStatus->getIsUsing()) continue;

		const auto mapObjectStatus = mapObject->getStatus();
		bool isHit = false;

		//コライダーの形ごとに判定を変える
		const auto sphereCollider = std::dynamic_pointer_cast<ISphereCollider>(mapObjectStatus);
		if (sphereCollider)
		{
			isHit = HitCheck_Sphere_Capsule(sphereCollider->getSpherePosition(), sphereCollider->getRadius(),
				bulletStatus->getPosition(), bulletStatus->getPrevPosition_(), bullet_collision_radius);
		}
		const auto capsuleCollider = std::dynamic_pointer_cast<ICapsuleCollider>(mapObjectStatus);
		if (capsuleCollider)
		{
			isHit = HitCheck_Capsule_Capsule(capsuleCollider->getCapsuleBottomPosition(), capsuleCollider->getCapsuleBottomPosition(), capsuleCollider->getCapsuleRadius(),
				bulletStatus->getPosition(), bulletStatus->getPrevPosition_(), bullet_collision_radius);
		}
		const auto cuboidCollider = std::dynamic_pointer_cast<ICuboidCollider>(mapObjectStatus);
		if (cuboidCollider)
		{
			isHit = IsHitingColliderCapsuleWithOBB(bulletStatus->getPosition(), bulletStatus->getPrevPosition_(), bullet_collision_radius,
				cuboidCollider->getCenterPosition(), cuboidCollider->getSizeXYZ(), cuboidCollider->getObjectAxis());
		}

		//当たってなければ判定しない
		if (!isHit) continue;

		//弾によっては爆発する
		explosion(bullet, explosionManager);

		//弾が当たったのでを消す
		bulletStatus->init();

		//マップオブジェクトがITargetableObjectを継承してるか
		const auto targetableObject = std::dynamic_pointer_cast<ITargetableObject>(mapObjectStatus);
		if (!targetableObject) continue;

		//ダメージ処理
		damage(targetableObject, bulletStatus->attack_power);
	}
}

void ColliderManager::charaWithBulletCollider(const std::vector<std::shared_ptr<IChara>>& charas, const std::shared_ptr<IBullet>& bullet,
	const std::shared_ptr<ExplosionManager>& explosionManager)
{
	const auto bulletStatus = bullet->getStatus();

	for (const auto& chara : charas)
	{
		//弾の所有者の種類がキャラと同じだったら処理しない
		if (bullet->getBulletOwner() == BulletOwnerType::PLAYER)
		{
			if (std::dynamic_pointer_cast<Player>(chara)) continue;
		}
		else if (bullet->getBulletOwner() == BulletOwnerType::ENEMY)
		{
			if (std::dynamic_pointer_cast<EnemyBase>(chara)) continue;
		}

		//弾が使われてないなら以降の処理をしない
		if (!bulletStatus->getIsUsing()) continue;

		const auto charaStatus	= chara->getStatus();
		bool isHit				= false;

		//コライダーの形ごとに判定を変える
		auto sphereCollider = std::dynamic_pointer_cast<ISphereCollider>(charaStatus);
		if (sphereCollider)
		{
			isHit = HitCheck_Sphere_Capsule(sphereCollider->getSpherePosition(), sphereCollider->getRadius(),
				bulletStatus->getPosition(), bulletStatus->getPrevPosition_(), bullet_collision_radius);
		}
		else
		{
			auto capsuleCollider = std::dynamic_pointer_cast<ICapsuleCollider>(charaStatus);

			if (capsuleCollider)
			{
				isHit = HitCheck_Capsule_Capsule(capsuleCollider->getCapsuleBottomPosition(), capsuleCollider->getCapsuleTopPosition(), capsuleCollider->getCapsuleRadius(),
					bulletStatus->getPosition(), bulletStatus->getPrevPosition_(), bullet_collision_radius);
			}
		}

		//当たってなければ判定しない
		if (!isHit) continue;

		//弾によっては爆発する
		explosion(bullet, explosionManager);

		//弾が当たったのでを消す
		bulletStatus->init();

		//キャラがITargetableObjectを継承してるか
		const auto targetableObject = std::dynamic_pointer_cast<ITargetableObject>(charaStatus);
		if (!targetableObject) continue;

		//ダメージ処理
		damage(targetableObject, bulletStatus->attack_power);
		ColliderEventHander::instance().getOnDamageEvent()->occurStartingEvent();
	}
}

void ColliderManager::pushBackCharaWithMapObject(const std::vector<std::shared_ptr<IChara>>& charas, const std::vector<std::shared_ptr<IMapObject>>& mapObjects)
{
	for (const auto& mapObject : mapObjects)
	{
		const auto mapObjectStatus = mapObject->getStatus();

		//キャラのコライダーはスフィアとカプセルのみの予定
		for (const auto& chara : charas)
		{
			const auto charaStatus	= chara->getStatus();
			VECTOR pushBackVector	= VGet(0.0f, 0.0f, 0.0f);

			//コライダーの形ごとに判定を変える
			const auto sphereCollider	= std::dynamic_pointer_cast<ISphereCollider>(charaStatus);
			bool isHit					= false;
			if (sphereCollider)
			{
				//スフィアとOBB当たり判定
				isHit = IsHitingColliderSphereWithOBB(sphereCollider->getSpherePosition(), sphereCollider->getRadius(),
					mapObjectStatus->getCenterPosition(), mapObjectStatus->getSizeXYZ(), mapObjectStatus->getObjectAxis());

				//当たってたら押し戻し
				if (isHit)
				{
					pushBackVector = CalculatePushBackVectorSphereWithOBB(sphereCollider->getSpherePosition(), sphereCollider->getRadius(),
						mapObjectStatus->getCenterPosition(), mapObjectStatus->getSizeXYZ(), mapObjectStatus->getObjectAxis());
				}
			}
			else
			{
				const auto capsuleCollider = std::dynamic_pointer_cast<ICapsuleCollider>(charaStatus);

				//カプセルとOBB当たり判定
				isHit = IsHitingColliderCapsuleWithOBB(capsuleCollider->getCapsuleTopPosition(), capsuleCollider->getCapsuleBottomPosition(), capsuleCollider->getCapsuleRadius(),
					mapObjectStatus->getCenterPosition(), mapObjectStatus->getSizeXYZ(), mapObjectStatus->getObjectAxis());

				//当たってたら押し戻し
				if (isHit)
				{
					pushBackVector = CalculatePushBackVectorCapsuleWithOBB(capsuleCollider->getCapsuleTopPosition(), capsuleCollider->getCapsuleBottomPosition(), capsuleCollider->getCapsuleRadius(),
						mapObjectStatus->getCenterPosition(), mapObjectStatus->getSizeXYZ(), mapObjectStatus->getObjectAxis());
				}
			}

			//押し戻し
			charaStatus->pushBack(pushBackVector);
		}
	}
}

void ColliderManager::pushBackCharaWithChara(const std::vector<std::shared_ptr<IChara>>& charas)
{
	for (int charaI = 0; charaI < charas.size(); charaI++)
	{
		const auto charaStatusI = charas[charaI]->getStatus();

		//コライダーの形ごとに判定を変える
		const auto sphereColliderI	= std::dynamic_pointer_cast<ISphereCollider>(charaStatusI);
		bool isHit					= false;

		//スフィアとの判定
		if (sphereColliderI)
		{
			for (int charaJ = charaI + 1; charaJ < charas.size(); charaJ++)	//同じペア同士を2回処理しないようにする
			{
				const auto charaStatusJ = charas[charaJ]->getStatus();
				VECTOR pushBackVector	= VGet(0.0f, 0.0f, 0.0f);

				//コライダーの形ごとに判定を変える
				const auto sphereColliderJ	= std::dynamic_pointer_cast<ISphereCollider>(charaStatusJ);
				bool isHit					= false;
				if (sphereColliderJ)
				{
					//スフィア同士の当たり判定
					isHit = HitCheck_Sphere_Sphere(sphereColliderI->getSpherePosition(), sphereColliderI->getRadius(), sphereColliderJ->getSpherePosition(), sphereColliderJ->getRadius());

					//当たってたら押し戻し
					if (isHit)
					{
						pushBackVector = CalculatePushBackVectorSphereWithSphere(sphereColliderI->getSpherePosition(), sphereColliderI->getRadius(), sphereColliderJ->getSpherePosition(), sphereColliderJ->getRadius());
					}
				}
				else
				{
					const auto capsuleCollider = std::dynamic_pointer_cast<ICapsuleCollider>(charaStatusJ);

					//カプセルとスフィアの当たり判定
					isHit = HitCheck_Sphere_Capsule(sphereColliderI->getSpherePosition(), sphereColliderI->getRadius(),
						capsuleCollider->getCapsuleBottomPosition(), capsuleCollider->getCapsuleTopPosition(), capsuleCollider->getCapsuleRadius());

					//当たってたら押し戻し
					if (isHit)
					{
						pushBackVector = CalculatePushBackVectorSphereWithCapsule(sphereColliderI->getSpherePosition(), sphereColliderI->getRadius(),
							capsuleCollider->getCapsuleBottomPosition(), capsuleCollider->getCapsuleTopPosition(), capsuleCollider->getCapsuleRadius());
					}
				}

				//押し戻し
				charaStatusI->pushBack(pushBackVector);
				charaStatusJ->pushBack(VScale(pushBackVector, -1.0f));	//関数の問題で一方向の押し戻ししかできないので-1をかけることで両者を押し戻しする
			}
		}
		else
		{
			//カプセルとの判定
			const auto capsuleColliderI = std::dynamic_pointer_cast<ICapsuleCollider>(charaStatusI);

			for (int charaJ = charaI + 1; charaJ < charas.size(); charaJ++)	//同じペア同士を2回処理しないようにする
			{
				const auto charaStatusJ = charas[charaJ]->getStatus();
				VECTOR pushBackVector	= VGet(0.0f, 0.0f, 0.0f);

				//コライダーの形ごとに判定を変える
				const auto sphereColliderJ	= std::dynamic_pointer_cast<ISphereCollider>(charaStatusJ);
				bool isHit					= false;
				if (sphereColliderJ)
				{
					//カプセルとスフィアの当たり判定
					isHit = HitCheck_Sphere_Capsule(sphereColliderJ->getSpherePosition(), sphereColliderJ->getRadius(),
						capsuleColliderI->getCapsuleTopPosition(), capsuleColliderI->getCapsuleBottomPosition(), capsuleColliderI->getCapsuleRadius());

					//当たってたら押し戻し
					if (isHit)
					{
						pushBackVector = CalculatePushBackVectorSphereWithCapsule(sphereColliderJ->getSpherePosition(), sphereColliderJ->getRadius(),
							capsuleColliderI->getCapsuleTopPosition(), capsuleColliderI->getCapsuleBottomPosition(), capsuleColliderI->getCapsuleRadius());
					}
				}
				else
				{
					const auto capsuleCollider = std::dynamic_pointer_cast<ICapsuleCollider>(charaStatusJ);

					//カプセル同士の当たり判定
					isHit = HitCheck_Capsule_Capsule(capsuleCollider->getCapsuleTopPosition(), capsuleCollider->getCapsuleBottomPosition(), capsuleCollider->getCapsuleRadius(),
						capsuleColliderI->getCapsuleTopPosition(), capsuleColliderI->getCapsuleBottomPosition(), capsuleColliderI->getCapsuleRadius());

					//当たってたら押し戻し
					if (isHit)
					{
						pushBackVector = CalculatePushBackVectorCapsuleWithCapsule(capsuleCollider->getCapsuleTopPosition(), capsuleCollider->getCapsuleBottomPosition(), capsuleCollider->getCapsuleRadius(),
							capsuleColliderI->getCapsuleTopPosition(), capsuleColliderI->getCapsuleBottomPosition(), capsuleColliderI->getCapsuleRadius());
					}
				}

				//押し戻し
				charaStatusI->pushBack(VScale(pushBackVector, -1.0f));	//関数の問題で一方向の押し戻ししかできないので-1をかけることで両者を押し戻しする
				charaStatusJ->pushBack(pushBackVector);
			}
		}
	}
}

void ColliderManager::enemyCollider(const std::vector<std::shared_ptr<EnemyBase>>& enemies, const std::vector<std::shared_ptr<IMapObject>>& mapObjects,
	const std::shared_ptr<Player>& player)
{
	for (const auto& enemyI : enemies)
	{
		const auto statusI			= enemyI->getEnemyStatus();
		bool isOverlapWithTarget	= false;

		//射線の重なり判定を初期化
		statusI->setIsRayObstructed(false);
		statusI->setHasClearShotToTarget(false);
		statusI->resetTarget();

		//射線が敵同士でかぶっているか判定
		for (const auto& enemyJ : enemies)
		{
			//すでに射線にかぶってるものがある場合判定しない
			if (statusI->getIsRayObstructed()) break;

			//自分同氏は判定しない
			if (enemyI == enemyJ) continue;

			const auto statusJ = enemyJ->getStatus();

			//コライダーの形ごとに判定を変える
			const auto sphereCollider = std::dynamic_pointer_cast<ISphereCollider>(statusJ);
			if (sphereCollider)
			{
				//スフィアと線分の当たり判定
				const float distance = Segment_Point_MinLength(statusI->getMuzzlePosition(), statusI->getAimingPosition(), sphereCollider->getSpherePosition());

				//半径より最短距離が小さかったら
				if (distance < sphereCollider->getRadius())
				{
					//射線に被ってるものがある
					statusI->setIsRayObstructed(true);
					statusI->setHasClearShotToTarget(false);
					break;
				}
			}
			else
			{
				//カプセルと線分の当たり判定
				const auto capsuleCollider = std::dynamic_pointer_cast<ICapsuleCollider>(statusJ);

				float minimumDistance			= 0;
				const short divSegmentNumber	= 10;
				for (int i = 0; i < divSegmentNumber; i++)
				{
					//線分を分割した一点を求める
					const float		time = static_cast<float>(i) / divSegmentNumber;
					const VECTOR	divSegmentPosition = LerpVector(capsuleCollider->getCapsuleBottomPosition(), capsuleCollider->getCapsuleTopPosition(), time);

					//現在の距離を求める
					const float currentDistance = Segment_Point_MinLength(statusI->getMuzzlePosition(), statusI->getAimingPosition(), divSegmentPosition);

					//最初は必ず最短距離になる
					if (i == 0)
					{
						minimumDistance = currentDistance;
					}
					else
					{
						//最短距離を更新
						if (minimumDistance < currentDistance) continue;
						minimumDistance = currentDistance;
					}
				}

				//半径より最短距離が小さかったら
				if (minimumDistance < capsuleCollider->getCapsuleRadius())
				{
					//射線に被ってるものがある
					statusI->setIsRayObstructed(true);
					statusI->setHasClearShotToTarget(false);
					break;
				}
			}
		}

		//すでに遮蔽にかぶってるのでこの先の処理はしない
		if (statusI->getIsRayObstructed()) continue;

		for (const auto& mapObject : mapObjects)
		{
			const auto mapObjectStatus = mapObject->getStatus();

			//マップオブジェクトがターゲットになるか
			const auto target = std::dynamic_pointer_cast<ITargetableObject>(mapObjectStatus);

			//敵がターゲットを見つけるかどうか
			if (target)
			{
				statusI->findTarget(target);
			}

			if (!IsHitSegmentWithOBB(statusI->getMuzzlePosition(), statusI->getAimingPosition(),
				mapObjectStatus->getCenterPosition(), mapObjectStatus->getSizeXYZ(), mapObjectStatus->getObjectAxis())) continue;

			if (!target)
			{
				//OBBとの最短距離が0以下ならターゲットにならない奴と射線が被ってしまっている
				statusI->setIsRayObstructed(true);
				statusI->setHasClearShotToTarget(false);
				isOverlapWithTarget = false;
				break;
			}
			else
			{
				//ターゲットのみに射線が被ってる
				isOverlapWithTarget = true;
			}
		}

		//すでに遮蔽にかぶってるのでこの先の処理はしない
		if (statusI->getIsRayObstructed()) continue;

		//プレイヤーに射線が被ってるか判定
		const auto playerCapsule		= std::dynamic_pointer_cast<ICapsuleCollider>(player->getStatus());
		const float distanceToPlayer	= CalculateMinimumDistanceSegmentToCapsule(statusI->getMuzzlePosition(), statusI->getAimingPosition(),
			playerCapsule->getCapsuleTopPosition(), playerCapsule->getCapsuleBottomPosition(), playerCapsule->getCapsuleRadius());

		//敵がプレイヤーを発見するか
		const auto target = std::dynamic_pointer_cast<ITargetableObject>(player->getStatus());
		statusI->findTarget(target);

		//プレイヤーに射線が被ってる
		if (distanceToPlayer <= 0 && !statusI->getIsRayObstructed())
		{
			isOverlapWithTarget = true;
		}

		//ターゲットのみに射線が被ってる
		if (isOverlapWithTarget)
		{
			statusI->setHasClearShotToTarget(true);
			statusI->setIsRayObstructed(false);
		}
	}
}

void ColliderManager::resetOnDamage(const std::vector<std::shared_ptr<IChara>>& charas, const std::vector<std::shared_ptr<IMapObject>>& mapObjects)
{
	//ターゲットになることができるものをセット
	std::vector<std::shared_ptr<ITargetableObject>> targetableObjects;
	for (const auto& mapObject : mapObjects)
	{
		//マップオブジェクトのステータスがITargetableObjectを継承してるか
		const auto targetableObject = std::dynamic_pointer_cast<ITargetableObject>(mapObject->getStatus());
		if (!targetableObject) continue;

		targetableObjects.emplace_back(targetableObject);
	}
	for (const auto& chara : charas)
	{
		//敵のステータスがITargetableObjectを継承してるか
		const auto targetableObject = std::dynamic_pointer_cast<ITargetableObject>(chara->getStatus());
		if (!targetableObject) continue;

		targetableObjects.emplace_back(targetableObject);
	}

	//ダメージ状態をリセット
	for (const auto& targetbleObject : targetableObjects)
	{
		targetbleObject->setOnDamage(false);
	}
}

void ColliderManager::explosionWithEnemeisCollider(const std::vector<std::shared_ptr<EnemyBase>>& enemies, const std::shared_ptr<ExplosionManager>& explosionManager)
{
	for (const auto& explosion : explosionManager->getActiveExplosion())
	{
		//アクティブじゃないなら処理しない
		if (!explosion->getIsActive()) continue;
		
		for (const auto& enemy : enemies)
		{
			bool isHiting = false;

			//敵のコライダーの形によって判定を変える
			const auto sphereCollider = std::dynamic_pointer_cast<ISphereCollider>(enemy->getStatus());
			if (sphereCollider)
			{
				isHiting = HitCheck_Sphere_Sphere(sphereCollider->getSpherePosition(), sphereCollider->getRadius(), explosion->getPosition(), explosion->getRadius());

				if (!isHiting) continue;

				//ダメージ処理
				damage(enemy->getEnemyStatus(), explosion->getAttackPower());
			}
			else
			{
				const auto capsuleCollider = std::dynamic_pointer_cast<ICapsuleCollider>(enemy->getStatus());
				isHiting = HitCheck_Sphere_Capsule(explosion->getPosition(), explosion->getRadius(),
					capsuleCollider->getCapsuleTopPosition(), capsuleCollider->getCapsuleBottomPosition(), capsuleCollider->getCapsuleRadius());

				if (!isHiting) continue;

				//ダメージ処理
				damage(enemy->getEnemyStatus(), explosion->getAttackPower());
			}
		}
	}
}

void ColliderManager::explosion(const std::shared_ptr<IBullet>& bullet, const std::shared_ptr<ExplosionManager>& explosionManager)
{
	const auto explosionBullet = std::dynamic_pointer_cast<ExplosionBullet>(bullet);
	if (!explosionBullet) return;
	
	const auto explosion = explosionBullet->getExplosion();
	explosion->start(explosionBullet->getStatus()->getPosition());
	explosionManager->addActiveExplosion(explosion);
}

void ColliderManager::damage(const std::shared_ptr<ITargetableObject> target, const float damageValue)
{
	target->setOnDamage(true);
	target->decreaseHp(damageValue);
	target->destroy();
}