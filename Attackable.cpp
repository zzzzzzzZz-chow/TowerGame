#include "Attackable.h"

Attackable::Attackable() noexcept : m_attackPower(Number{}) {}

Attackable::Attackable(ConstReference that) noexcept : m_attackPower(that.m_attackPower) {}

Attackable::Attackable(RightReference that) noexcept : m_attackPower(std::move(that.m_attackPower)) {}

auto Attackable::operator=(ConstReference that) noexcept ->Reference {
	m_attackPower = that.m_attackPower;
	return *this;
}

auto Attackable::operator=(RightReference that) noexcept->Reference {
	m_attackPower = std::move(that.m_attackPower);
	return *this;
}

Attackable::~Attackable() noexcept {}

auto Attackable::attackPower() noexcept ->Number& {
	return m_attackPower; 
}

auto Attackable::attackPower() const noexcept ->const Number& { 
	return m_attackPower; 
}

auto Attackable::setAttackPower(const Number& attackPower) noexcept ->Reference { 
	m_attackPower = attackPower; 
	return *this;
}