#include "Movable.h"
#include <algorithm>
#include <execution>

auto Movable::moveMutex() noexcept ->std::mutex& {
	static std::mutex s_moveMutex{};
	return s_moveMutex;
}

auto Movable::moveThread() -> std::thread& {
	static std::thread s_moveThread{Type::moveThreadFunction};
	return s_moveThread;
}

auto Movable::moveThreadFunction() ->void {
	while (1) {
		{
			std::lock_guard<std::mutex> lock{ moveMutex() };
			std::for_each(std::execution::par, moveThreadTasks().begin(), moveThreadTasks().end(), [](const auto& taskPair) {
				taskPair.second();
			});
		}
		//std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

auto Movable::moveThreadTasks() noexcept ->std::map<Object::ID, std::function<void()>>& {
	static std::map<Object::ID, std::function<void()>> s_moveThreadTasks{};
	return s_moveThreadTasks;
}

#pragma warning(suppress: 26455)
Movable::Movable() : m_moveDistance(0), m_moveGap(0), m_lastMoveTime(clock()), m_moveDistinations() {
	moveThread();
	std::lock_guard<std::mutex> lock{ moveMutex() };
	moveThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::moveThreadTask, this);
}

Movable::Movable(ConstReference that) : m_moveDistance(that.m_moveDistance), m_moveGap(that.m_moveGap), m_lastMoveTime(0), m_moveDistinations() {
	std::lock_guard<std::mutex> lock{ moveMutex() };
	moveThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::moveThreadTask, this);
}

#pragma warning(suppress: 26439)
Movable::Movable(RightReference that) : m_moveDistance(std::move(that.m_moveDistance)), m_moveGap(std::move(that.m_moveGap)),
m_lastMoveTime(std::move(that.m_lastMoveTime)), m_moveDistinations(std::move(that.m_moveDistinations)) {
	std::lock_guard<std::mutex> lock{ moveMutex() };
	moveThreadTasks().erase(static_cast<Object&>(that).id());
	moveThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::moveThreadTask, this);
}

auto Movable::operator=(ConstReference that) noexcept ->Reference {
	m_moveDistance = that.m_moveDistance;
	m_moveGap = that.m_moveGap;
	return *this;
}

#pragma warning(suppress: 26439)
auto Movable::operator=(RightReference that) ->Reference {
	m_moveDistance = std::move(that.m_moveDistance);
	m_lastMoveTime = std::move(that.m_lastMoveTime);
	m_moveDistinations = std::move(that.m_moveDistinations);
	std::lock_guard<std::mutex> lock{ moveMutex() };
	moveThreadTasks().erase(static_cast<Object&>(that).id());
	moveThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::moveThreadTask, this);
	return *this;
}

Movable::~Movable() {
#pragma warning(suppress: 26447)
	std::lock_guard<std::mutex> lock{ moveMutex() };
#pragma warning(suppress: 26447)
	moveThreadTasks().erase(static_cast<Object&>(*this).id());
}

auto Movable::addMoveDistination(const Position2D& position)->Reference {
	std::lock_guard<std::mutex> lock{ moveMutex() };
	m_moveDistinations.push(position);
	return *this;
}

auto Movable::clearMoveDistination() ->Reference {
	std::lock_guard<std::mutex> lock{ moveMutex() };
	while (!m_moveDistinations.empty())
		m_moveDistinations.pop();
	return *this;
}

auto Movable::moveDistinationNumber() const ->Number {
	std::lock_guard<std::mutex> lock{ moveMutex() };
	return Number(m_moveDistinations.size());
}

auto Movable::moveDistance() noexcept ->Number& { 
	return m_moveDistance; 
}

auto Movable::moveGap() noexcept ->Number& { 
	return m_moveGap; 
}

auto Movable::moveDistance() const noexcept ->const Number& { 
	return m_moveDistance; 
}

auto Movable::moveGap() const noexcept ->const Number& { 
	return m_moveGap; 
}

auto Movable::setMoveDistance(const Number& moveDistance) noexcept ->Reference { 
	m_moveDistance = moveDistance; 
	return *this; 
}

auto Movable::setMoveGap(const Number& moveGap) noexcept ->Reference { 
	m_moveGap = moveGap; 
	return *this; 
}

void Movable::moveThreadTask() {
	if (clock() - this->m_lastMoveTime > this->moveGap()) {
		this->m_lastMoveTime += this->moveGap();/*clock()*/;
		if (this->m_moveDistinations.empty())
			return;

		const Position2D moveDistination = this->m_moveDistinations.front();
		if (this->position().distance(moveDistination) <= this->moveDistance()) {
			this->position() = moveDistination;
			this->m_moveDistinations.pop();
			return;
		}
		const Point2D moveForward = (moveDistination - this->position()).one(this->moveDistance());
#pragma warning(suppress: 26486)
		position().move(moveForward);
	}
}