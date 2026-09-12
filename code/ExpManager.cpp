#include"DxLibForIreven.h"
#include"PlayerStatus.h"
#include"EnemyBase.h"
#include"ExpItem.h"
#include"ExpManager.h"

//スタティックメンバーを初期化
int ExpManager::expModel_ = 0;

ExpManager::ExpManager()
{
	expModel_ = MV1LoadModel("3dmodel/enchanted_crystal_08/crystal.mv1");
}

ExpManager::~ExpManager()
{
	MV1DeleteModel(expModel_);
}

void ExpManager::init()
{
	expItems_.clear();
}

void ExpManager::update(const std::shared_ptr<PlayerStatus>& playerStatus, const std::vector<std::shared_ptr<EnemyBase>>& enemies)
{
	expItemFactory(enemies);

	for (const auto& expItem : expItems_)
	{
		expItem->update(playerStatus->getPosition());

		bool isHiting = CalculateDistance<float>(playerStatus->getPosition(), expItem->getPosition()) < playerStatus->getCapsuleRadius() * 2.0f;
		if (!isHiting) continue;

		playerStatus->addExp(expItem->getExpValue());
		expItem->setCanDestroy(true);
	}

	eraceAndRemoveFromVector();
}

void ExpManager::draw()
{
	for (const auto& expItem : expItems_)
	{
		expItem->draw();
	}
}

void ExpManager::expItemFactory(const std::vector<std::shared_ptr<EnemyBase>>& enemies)
{
	for (const auto& enemy : enemies)
	{
		const auto enemyStatus = enemy->getEnemyStatus();
		if (!enemyStatus->getCanDestroy()) continue;

		expItems_.emplace_back(std::make_shared<ExpItem>(enemyStatus->getDropingExp(), exp_move_speed, enemyStatus->getPosition(), expModel_));
	}
}

void ExpManager::eraceAndRemoveFromVector()
{
	//破壊
	expItems_.erase(//erace-removeイディオムで死亡フラグが立っているキャラをまとめて削除
		std::remove_if(	//削除したい領域を一旦後ろに送る
			expItems_.begin(),
			expItems_.end(),
			[](const std::shared_ptr<ExpItem>& expItem)	//削除したい条件をラムダ式で書く
			{
				return expItem->getCanDestroy();
			}
		),
		expItems_.end());	//後ろに送ったいらないやつの最初から最後までeraseで削除
}