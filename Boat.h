#pragma once

#include "Object.h"
#include "RangeDefendable.h"
#include "Movable.h"

class Boat : public virtual Object, public virtual RangeDefendable, public virtual Movable {
	using Type = Boat;
	using Reference = Type & ;
	using ConstReference = const Type &;
	using RightReference = Type && ;
	using Pointer = Type * ;
	using ConstPointer = const Type *;
public:
	Boat() {
		m_isViewVirtualFunctionInitial.store(true);
	}
	void defend(const Number& attackPower) noexcept override;
	void view() noexcept override;
};