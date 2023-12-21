#pragma once

#include "Object.h"
#include "LockAttackable.h"
#include "Viewable.h"

class Tower : public virtual Object, public virtual LockAttackable {
	using Type = Tower;
	using Reference = Type & ;
	using ConstReference = const Type &;
	using RightReference = Type && ;
	using Pointer = Type * ;
	using ConstPointer = const Type *;
public:
	Tower() : Object(), LockAttackable() {
		m_isViewVirtualFunctionInitial.store(true);
	}
	void attack(Defendable& enemy) override;
	void view() override;
protected:
};