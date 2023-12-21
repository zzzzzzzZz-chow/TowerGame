#pragma once

#include "Number.h"
#include "RangeDefendable.h"

class Attackable : public virtual NumberType {
	using Type = Attackable;
	using Reference = Type & ;
	using ConstReference = const Type &;
	using RightReference = Type && ;
	using Pointer = Type * ;
	using ConstPointer = const Type *;
public:
	virtual void attack(Defendable& enemy) = 0;

	Attackable() noexcept;
	Attackable(ConstReference that) noexcept;
	Attackable(RightReference that) noexcept;
	virtual auto operator=(ConstReference that) noexcept ->Reference;
	virtual auto operator=(RightReference that) noexcept ->Reference;
	~Attackable() noexcept;

	auto attackPower() noexcept ->Number&;
	auto attackPower() const noexcept ->const Number&;
	auto setAttackPower(const Number& attackPower) noexcept ->Reference;
private:
	Number m_attackPower; // ¹¥»÷Á¦ 
};