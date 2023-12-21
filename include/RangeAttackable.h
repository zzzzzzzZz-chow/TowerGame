#pragma once

#include "Attackable.h"
#include "Viewable.h"

class RangeAttackable : public virtual Attackable, public virtual Viewable {
	using Type = RangeAttackable;
	using Reference = Type & ;
	using ConstReference = const Type &;
	using RightReference = Type && ;
	using Pointer = Type * ;
	using ConstPointer = const Type *;
public:
	RangeAttackable() noexcept;
	RangeAttackable(ConstReference that) noexcept;
	RangeAttackable(RightReference that) noexcept;
	virtual auto operator=(ConstReference that) noexcept ->Reference;
	virtual auto operator=(RightReference that) noexcept ->Reference;
	~RangeAttackable() noexcept;

	auto rangeAttack(RangeDefendable& enemy) ->bool;

	auto attackRange() noexcept ->Number&;
	auto attackRange() const noexcept ->const Number&;
	auto setAttackRange(const Number& attackRange) noexcept ->Reference;
protected:
	Number m_attackRange; // ¹¥»÷·¶Î§ 
};