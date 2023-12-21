#include "RangeAttackable.h"

RangeAttackable::RangeAttackable() noexcept : m_attackRange(Number{ 0 }) {}

RangeAttackable::RangeAttackable(ConstReference that) noexcept : m_attackRange(that.m_attackRange) {}

RangeAttackable::RangeAttackable(RightReference that) noexcept : m_attackRange(std::move(that.m_attackRange)) {}

auto RangeAttackable::operator=(ConstReference that) noexcept ->Reference {
	static_cast<Attackable&>(*this) = static_cast<const Attackable&>(that);
	static_cast<Viewable&>(*this) = static_cast<const Viewable&>(that);
	m_attackRange = that.m_attackRange;
	return *this;
}

auto RangeAttackable::operator=(RightReference that) noexcept ->Reference {
	static_cast<Attackable&>(*this) = static_cast<Attackable&&>(that);
	static_cast<Viewable&>(*this) = static_cast<Viewable&&>(that);
	m_attackRange = std::move(that.m_attackRange);
	return *this;
}

RangeAttackable::~RangeAttackable() noexcept {}

auto RangeAttackable::rangeAttack(RangeDefendable& enemy) ->bool {
	Viewable& viewableEnemy = static_cast<Viewable&>(enemy);
	Viewable& viewableSelf = static_cast<Viewable&>(*this);
	if ((viewableEnemy.position() - viewableSelf.position()).length() <= this->attackRange()) {
		attack(enemy);
		return true;
	}
	return false;
}

auto RangeAttackable::attackRange() noexcept ->Number& { 
	return m_attackRange; 
}

auto RangeAttackable::attackRange() const noexcept ->const Number& { 
	return m_attackRange; 
}

auto RangeAttackable::setAttackRange(const Number& attackRange) noexcept ->Reference { 
	m_attackRange = attackRange; 
	return *this;
}