#pragma once

#include "RangeAttackable.h"
#include <ctime>
#include <vector>

class LockAttackable : public virtual RangeAttackable {
	using Type = LockAttackable;
	using Reference = Type & ;
	using ConstReference = const Type &;
	using RightReference = Type && ;
	using Pointer = Type * ;
	using ConstPointer = const Type *;
public:
	LockAttackable() noexcept(false);
	LockAttackable(ConstReference that);
	LockAttackable(RightReference that) noexcept(false);
	virtual auto operator=(ConstReference that) noexcept ->Reference;
	virtual auto operator=(RightReference that) noexcept(false) ->Reference;
	~LockAttackable() noexcept;

	auto lockAttack(RangeDefendable& enemy) ->void;
	auto unlockAttack(RangeDefendable& enemy) ->void;
	auto attackObjectNumber() const noexcept ->std::size_t;

	auto attackGap() noexcept ->Number&;
	auto attackGap() const noexcept ->const Number&;
	auto setAttackGap(const Number& attackGap) noexcept ->Reference;
private:
	static auto attackThread() ->std::thread&;
	static auto attackThreadFuntion() ->void;
	static auto attackMutex() noexcept ->std::mutex&;
	static auto attackThreadTasks() noexcept ->std::map<ID, std::function<void()>>&;
private:
	void attackThreadTask();
private:
	Number m_attackGap; // 攻击间隔 
	std::clock_t m_lastAttackTime; // 上次攻击时间 
	std::vector<RangeDefendable*> m_attackObjects; // 攻击目标 
};