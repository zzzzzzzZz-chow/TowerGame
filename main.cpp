#include <iostream>
#pragma warning(disable : 4996)
 
#include "Screen.h"
#include "Tower.h"
#include "Boat.h"

int main(int argc, char* argv[]) {
	try {
		Tower a;
		a.setPosition({ 10000, 10000 });
		a.setAttackGap(50);
		a.setAttackPower(100);
		a.setAttackRange(100000);
		Boat b;
		b.setMoveDistance(100).setMoveGap(100);
		b.setHealth(10000);
		Boat b1;
		b1.setHealth(10000);
		b1.setMoveDistance(100).setMoveGap(100);
		b1.addMoveDistination({ 19999, 0 }).addMoveDistination({ 19999, 19999 }).addMoveDistination({ 0, 0 });
		b.addMoveDistination({ 9999, 9999 }).addMoveDistination({ 0, 19999 }).addMoveDistination({ 0 ,0 }).addMoveDistination({ 19999, 0 });

		b.position().setX(0).setY(0);
		screen.show(10000, { 15000, 15000 }, std::vector<std::wstring>{L"end"});
		a.lockAttack(b1);
		std::this_thread::sleep_for(std::chrono::seconds(5));
		Boat c[100];
		for (int i = 0; i < 100; ++i) {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			c[i].setMoveDistance(100).setMoveGap(100);
			c[i].setHealth(10000);
			c[i].addMoveDistination({ 9999, 9999 }).addMoveDistination({ 0, 19999 }).addMoveDistination({ 0 ,0 }).addMoveDistination({ 19999, 0 });
		}
		a.lockAttack(b);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		Boat d{ b1 };
		a.unlockAttack(b);
		a.unlockAttack(b1);
		getchar();
		
	}
	catch (const std::exception& e) {
		//screen.show({ 15000, 10000 }, e.what());
	}
	std::cout << "end\n";
	getchar();
	return 0;
}
