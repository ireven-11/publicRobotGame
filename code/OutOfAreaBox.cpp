#include"DxLibForIreven.h"
#include<memory>
#include"MapObjectStatus.h"
#include"OutOfAreaBox.h"
#include"OutOfAreaBoxStatus.h"

OutOfAreaBox::OutOfAreaBox(const VECTOR initPosition, const float mapModelScale, const VECTOR standardSize)
	:status_(std::make_shared<OutOfAreaBoxStatus>(initPosition, mapModelScale, standardSize))
{
}

OutOfAreaBox::~OutOfAreaBox()
{
	status_ = nullptr;
}

void OutOfAreaBox::update()
{
	status_->update();
}

void OutOfAreaBox::draw()const
{
	//DrawCuboid3D(status_->getCenterPosition(), status_->getSizeXYZ(), status_->getObjectAxis(), GetColor(255, 25, 25));	//test
}