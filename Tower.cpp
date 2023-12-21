#include "Tower.h"
#include "Screen.h"

void Tower::attack(Defendable& enemy) {
	if (enemy.health() <= 0)
		return;
	enemy.defend(attackPower());
}

void Tower::view() {
	char buffer[100] = {};
	sprintf_s(buffer, "*");
	screen.show(id(), position(), std::vector<std::wstring>{
		L" 🗡",
		L"丨丨",
		L"⊥⊥"});
	std::memcpy(&screen.m_pBuffer[position().x() * 40 / 20000 + position().y() * 20 / 20000 * 40], buffer, sprintf_s(buffer, "塔", position().x(), position().y()));
}