#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

#include <math.h>
#include <stdio.h>

enum ItemType {WEAPON, ARMOR, RING};
enum Steps {WEAPON_CHOOSING, ARMOR_CHOOSING, RING1_CHOOSING, RING2_CHOOSING};

typedef struct Item {
	int type;
	std::string name;
	int cost, damage, armor;

	Item() : type(), name(), cost(), damage(), armor() {

	}

	Item(int type, std::string name, int cost, int damage, int armor)
		: type(type), name(name), cost(cost), damage(damage), armor(armor) {

	}

	bool operator==(const Item& otherItem) const {
		return name == otherItem.name;
	}
} Item;

const int INF = (1 << 30);
const int INIT_HIT_POINTS = 100;

const std::vector<Item> shop {
	Item(WEAPON, "Dagger", 8, 4, 0),
	Item(WEAPON, "Shortsword", 10, 5, 0),
	Item(WEAPON, "Warhammer", 25, 6, 0),
	Item(WEAPON, "Longsword", 40, 7, 0),
	Item(WEAPON, "Greataxe", 74, 8, 0),
	Item(ARMOR, "Leather", 13, 0, 1),
	Item(ARMOR, "Chainmail", 31, 0, 2),
	Item(ARMOR, "Splintmail", 53, 0, 3),
	Item(ARMOR, "Bandedmail", 75, 0, 4),
	Item(ARMOR, "Platemail", 102, 0, 5),
	Item(RING, "Damage +1", 25, 1, 0),
	Item(RING, "Damage +2", 50, 2, 0),
	Item(RING, "Damage +3", 100, 3, 0),
	Item(RING, "Defense +1", 20, 0, 1),
	Item(RING, "Defense +2", 40, 0, 2),
	Item(RING, "Defense +3", 80, 0, 3)
};

typedef struct Player {
	int hitPoints, armor, damage;
	std::vector<Item> inventory;

	Player() : hitPoints(INIT_HIT_POINTS), armor(0), damage(0), inventory() {

	}

	bool hasItem(const Item& shopItem) {
		return std::find(inventory.begin(), inventory.end(), shopItem) != inventory.end();
	}

	bool hasSameItemType(const Item& shopItem) {
		int cnt = 0;

		for (const auto& i : inventory) {
			if (i.type == shopItem.type) {
				++ cnt;
			}
		}

		return !((cnt < 2 && shopItem.type == RING) || cnt < 1);
	}

	void addItem(const Item& item) {
		armor += item.armor;
		damage += item.damage;
		inventory.push_back(item);
	}

	void eraseLastItem() {
		armor -= inventory.back().armor;
		damage -= inventory.back().damage;
		inventory.pop_back();
	}

	bool winAgainstPlayer(const Player& otherPlayer) {
		int damageDealtPerTurn = std::max(1, damage - otherPlayer.armor);
		int damageTakenPerTurn = std::max(1, otherPlayer.damage - armor);
		int turnsToKill = ceil((float)otherPlayer.hitPoints / (float)damageDealtPerTurn);

		return damageTakenPerTurn * (turnsToKill - 1) < hitPoints;
	}
} Player;

int smallestPrice = INF;
int biggestPrice = -INF;

void chooseSecondaries(Player& player, const Player& boss, int ind, int currPrice) {
	if (player.winAgainstPlayer(boss)) {
		smallestPrice = std::min(smallestPrice, currPrice);
	} else {
		biggestPrice = std::max(biggestPrice, currPrice);
	}

	for (size_t i = ind; i < shop.size(); ++ i) {
		if (!player.hasSameItemType(shop[i])) {
			player.addItem(shop[i]);
			chooseSecondaries(player, boss, i + 1, currPrice + shop[i].cost);
			player.eraseLastItem();
		}
	}
}

int main() {
	FILE* fin = fopen("data.in", "r");
	Player player, boss;
	auto armorIt = std::find_if(shop.begin(), shop.end(), [](Item i)->bool { return i.type == ARMOR; });

	fscanf(fin, "Hit Points: %d\nDamage: %d\nArmor: %d", &boss.hitPoints, &boss.damage, &boss.armor);

	for (auto i = shop.begin(); i < armorIt; ++ i) {
		player.addItem(*i);
		chooseSecondaries(player, boss, armorIt - shop.begin(), i->cost);
		player.eraseLastItem();
	}

	std::cout << "The answer for part1 is: " << smallestPrice << "\n";
	std::cout << "The answer for part2 is: " << biggestPrice << "\n";

	fclose(fin);

	return 0;
}