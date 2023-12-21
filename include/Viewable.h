#pragma once

#include "Object.h"
#include "Position.h"
#include <thread>
#include <mutex>
#include <map>
#include <atomic>
#include <functional>

class Viewable : public virtual Object, public virtual PositionType {
	using Type = Viewable;
	using Reference = Type & ;
	using ConstReference = const Type &;
	using RightReference = Type && ;
	using Pointer = Type * ;
	using ConstPointer = const Type *;
public:
	virtual void view() = 0;
protected:
	std::atomic_bool m_isViewVirtualFunctionInitial{ false };
public:
	Viewable() noexcept(false);
	Viewable(ConstReference that);
	Viewable(RightReference that) noexcept(false);
	virtual auto operator=(ConstReference that) noexcept ->Reference;
	virtual auto operator=(RightReference that) noexcept(false) ->Reference;
	~Viewable();

	auto position() noexcept ->Position2D&;
	auto position() const noexcept ->const Position2D&;
	auto setPosition(const Position2D& position) noexcept ->Reference;
private:
	auto viewThreadTask() ->void;
private:
	static auto viewMutex() noexcept ->std::mutex&;
	static auto viewThread() ->std::thread&;
	static auto viewThreadFunction() ->void;
	static auto viewThreadTasks() noexcept ->std::map<Object::ID, std::function<void()>>&;
private:
	Position2D m_position{0, 0};
};