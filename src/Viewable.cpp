#include "Viewable.h"
#include <algorithm>
#include <execution>
#include <iostream>

auto Viewable::viewMutex() noexcept ->std::mutex&{
	static std::mutex s_viewMutex{};
	return s_viewMutex;
}

auto Viewable::viewThread() ->std::thread& {
	static std::thread s_viewThread{Type::viewThreadFunction};
	return s_viewThread;
}

auto Viewable::viewThreadFunction() ->void {
	while (1) {
		{
			std::lock_guard<std::mutex> lock{ viewMutex() };
			std::for_each(std::execution::par, viewThreadTasks().begin(), viewThreadTasks().end(), [](const auto& taskPair) {
				taskPair.second();
			});
		}
		//std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

auto Viewable::viewThreadTasks() noexcept ->std::map<Object::ID, std::function<void()>>& {
	static std::map<Object::ID, std::function<void()>> s_viewThreadTasks{};
	return s_viewThreadTasks;
}

Viewable::Viewable() noexcept(false) {
	viewThread();
	std::unique_lock<std::mutex> lock{ viewMutex() };
	viewThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::viewThreadTask, this);
}

Viewable::Viewable(ConstReference that) : m_position(that.m_position) {
	std::lock_guard<std::mutex> lock{ viewMutex() };
	viewThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::viewThreadTask, this);
}

Viewable::Viewable(RightReference that) noexcept(false) : m_position(std::move(that.m_position)) {
	std::lock_guard<std::mutex> lock{ viewMutex() };
	viewThreadTasks().erase(static_cast<Object&>(that).id());
	viewThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::viewThreadTask, this);
}

auto Viewable::operator=(ConstReference that) noexcept ->Reference {
	m_position = that.m_position;
	return *this;
}

auto Viewable::operator=(RightReference that) noexcept(false) ->Reference {
	m_position = std::move(that.m_position);
	std::lock_guard<std::mutex> lock{ viewMutex() };
	viewThreadTasks().erase(static_cast<Object&>(that).id());
	viewThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::viewThreadTask, this);
	return *this;
}

Viewable::~Viewable() {
#pragma warning(suppress: 26447)
	std::lock_guard<std::mutex> lock{ viewMutex() };
#pragma warning(suppress: 26447)
	viewThreadTasks().erase(static_cast<Object&>(*this).id());
}

auto Viewable::position() noexcept ->Position2D& { 
	return m_position; 
}

auto Viewable::position() const noexcept ->const Position2D& { 
	return m_position; 
}

auto Viewable::setPosition(const Position2D& position) noexcept ->Reference {
	m_position = position;
	return *this;
}

auto Viewable::viewThreadTask() ->void {
	if(!m_isViewVirtualFunctionInitial.load())
		return;
	view();
}