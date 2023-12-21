#include "Defendable.h"

auto Defendable::defendPower() noexcept ->Number& { 
	return m_defendPower; 
}

auto Defendable::health() noexcept ->Number& { 
	return m_health; 
}

auto Defendable::defendPower() const noexcept ->const Number& { 
	return m_defendPower; 
}

auto Defendable::health() const noexcept ->const Number& { 
	return m_health; 
}

auto Defendable::setDefendPower(const Number& defendPower) noexcept ->Reference { 
	m_defendPower = defendPower; 
	return *this;
}

auto Defendable::setHealth(const Number& health) noexcept ->Reference { 
	m_health = health; 
	return *this; 
}