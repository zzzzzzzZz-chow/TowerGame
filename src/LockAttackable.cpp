#include "LockAttackable.h"
#include <execution>

auto LockAttackable::attackThread() ->std::thread& {
	static std::thread s_attackThread{Type::attackThreadFuntion};
	return s_attackThread;
}

auto LockAttackable::attackThreadFuntion() ->void {
	while (1) {
		{
			std::lock_guard<std::mutex> lock{ attackMutex() };
			std::for_each(std::execution::par, attackThreadTasks().begin(), attackThreadTasks().end(), [](const auto& taskPair) {
				taskPair.second();
			});
		}
		//std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

auto LockAttackable::attackMutex() noexcept ->std::mutex&{
	static std::mutex s_attackMutex{};
	return s_attackMutex;
}

auto LockAttackable::attackThreadTasks() noexcept -> std::map<ID, std::function<void()>>& {
	static std::map<ID, std::function<void()>> s_attackThreadTasks{};
	return s_attackThreadTasks;
}

void LockAttackable::attackThreadTask() {
	if (clock() - m_lastAttackTime > attackGap()) {
		m_lastAttackTime = clock();
		for (auto& p_enemy : m_attackObjects) {
			rangeAttack(*p_enemy);
		}
	}
}

LockAttackable::LockAttackable() noexcept(false) : m_attackGap(Number{ 0 }), m_lastAttackTime(std::clock_t{ 0 }), 
	m_attackObjects() {
	attackThread();
	std::lock_guard<std::mutex> lock{ attackMutex() };
	attackThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::attackThreadTask, this);
}

LockAttackable::LockAttackable(ConstReference that) : m_attackGap(that.m_attackGap), 
	m_lastAttackTime(std::clock_t{ 0 }), m_attackObjects() {
	std::lock_guard<std::mutex> lock{ attackMutex() };
	attackThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::attackThreadTask, this);
}

LockAttackable::LockAttackable(RightReference that) noexcept(false) : m_attackGap(std::move(that.m_attackGap)),
	m_lastAttackTime(std::move(that.m_lastAttackTime)), m_attackObjects(std::move(that.m_attackObjects)) {
	std::lock_guard<std::mutex> lock{ attackMutex() };
	attackThreadTasks().erase(static_cast<Object&&>(that).id());
	attackThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::attackThreadTask, this);
}

auto LockAttackable::operator=(ConstReference that) noexcept ->Reference {
	static_cast<RangeAttackable&>(*this) = static_cast<const RangeAttackable&>(that);
	m_attackGap = that.m_attackGap;
	m_lastAttackTime = that.m_lastAttackTime;
	return *this;
}

auto LockAttackable::operator=(RightReference that) noexcept(false) ->Reference {
	static_cast<RangeAttackable&>(*this) = static_cast<RangeAttackable&&>(that);
	m_attackGap = std::move(that.m_attackGap);
	m_lastAttackTime = std::move(that.m_lastAttackTime);
	m_attackObjects = std::move(that.m_attackObjects);
	std::lock_guard<std::mutex> lock{ attackMutex() };
	attackThreadTasks().erase(static_cast<Object&&>(that).id());
	attackThreadTasks()[static_cast<Object&>(*this).id()] = std::bind(&Type::attackThreadTask, this);
	return *this;
}

LockAttackable::~LockAttackable() noexcept {}

auto LockAttackable::lockAttack(RangeDefendable& enemy) ->void { 
	m_attackObjects.push_back(&enemy); 
}

auto LockAttackable::unlockAttack(RangeDefendable& enemy) ->void {
	auto it = std::find_if(m_attackObjects.begin(), m_attackObjects.end(), [&](RangeDefendable* object) {
		return object == &enemy;
	});
	m_attackObjects.erase(it);
}
auto LockAttackable::attackObjectNumber() const noexcept ->std::size_t {
	return m_attackObjects.size();
}

auto LockAttackable::attackGap() noexcept ->Number& { 
	return m_attackGap;
}

auto LockAttackable::attackGap() const noexcept ->const Number& {
	return m_attackGap;
}

auto LockAttackable::setAttackGap(const Number& attackGap) noexcept ->Reference { 
	m_attackGap = attackGap; 
	return *this;
}