#pragma once

template <typename Chara>
class IAnimation
{
public:
	virtual ~IAnimation() = default;

	virtual void init()											= 0;
	virtual void enter()										= 0;
	virtual void exit()											= 0;
	virtual void update(const std::shared_ptr<Chara>& actor)	= 0;
	virtual const bool getOnUpdate()const noexcept				= 0;
};