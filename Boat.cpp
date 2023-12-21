#include "Boat.h"
#include "Screen.h"

void Boat::defend(const Number& attackPower) noexcept {
	Number realAttackPower = attackPower - defendPower();
	if (realAttackPower < 0)
		realAttackPower = 0;
	setHealth(health() - realAttackPower);
}
void Boat::view() noexcept {
	char buffer[100] = {};
	sprintf_s(buffer, "*");
	screen.show(id(), position(), std::vector<std::wstring>{
		L"_◢ __",
	   L"\\___/"});
	//std::memcpy(&screen.m_pBuffer[position().x()*40/20000 + position().y() * 20/20000*40], buffer, sprintf_s(buffer, "*%d", health()));
}