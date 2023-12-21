#pragma once

#include "Viewable.h"
#include <queue>

class Movable : public virtual Viewable {
	using Type = Movable;
	using Reference = Type & ;
	using ConstReference = const Type &;
	using RightReference = Type && ;
	using Pointer = Type * ;
	using ConstPointer = const Type *;
public:
	Movable();
	Movable(ConstReference that);
	Movable(RightReference that);
	virtual auto operator=(ConstReference that) noexcept ->Reference;
	virtual auto operator=(RightReference that) ->Reference;
	~Movable();

	auto addMoveDistination(const Position2D& position) ->Reference;
	auto clearMoveDistination() ->Reference;
	auto moveDistinationNumber() const ->Number;

	auto moveDistance() noexcept ->Number&;
	auto moveGap() noexcept ->Number&;
	auto moveDistance() const noexcept ->const Number&;
	auto moveGap() const noexcept ->const Number&;
	auto setMoveDistance(const Number& moveDistance) noexcept ->Reference;
	auto setMoveGap(const Number& moveGap) noexcept ->Reference;
private:
	void moveThreadTask();
private:
	static auto moveMutex() noexcept ->std::mutex&;
	static auto moveThread() ->std::thread&;
	static auto moveThreadFunction() ->void;
	static auto moveThreadTasks() noexcept ->std::map<Object::ID, std::function<void()>>&;
private:
	Number m_moveDistance;
	Number m_moveGap;
	Number m_lastMoveTime;
	std::queue<Position2D> m_moveDistinations;
};