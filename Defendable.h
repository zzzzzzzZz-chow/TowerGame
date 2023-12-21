#pragma once

#include "Number.h"

class Defendable : public virtual NumberType {
	using Type = Defendable;
	using Reference = Type & ;
	using ConstReference = const Type &;
	using RightReference = Type && ;
	using Pointer = Type * ;
	using ConstPointer = const Type *;
public:
	virtual void defend(const Number& attackPower) = 0;

	Defendable() = default;
	Defendable(ConstReference that) = default;
	Defendable(RightReference that) = default;
	Defendable& operator=(ConstReference that) = default;
	Defendable& operator=(RightReference that) = default;
	virtual ~Defendable() noexcept = default;

	auto defendPower() noexcept ->Number&;
	auto health() noexcept ->Number&;

	auto defendPower() const noexcept ->const Number&;
	auto health() const noexcept ->const Number&;

	auto setDefendPower(const Number& defendPower) noexcept ->Reference;
	auto setHealth(const Number& health) noexcept ->Reference;
protected:
	Number m_defendPower{0}; // ·ÀÓùÁ¦ 
	Number m_health{0}; // ÉúÃüÖµ 
};