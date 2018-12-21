#include <iostream>
#include <fstream>
#include <vector>

#include <stdio.h>

const std::string frmt = "%s capacity %d, durability %d, flavor %d, texture %d, calories %d";
const int MAX_INGS = 100;
const int INF = (1 << 30);
const int MAX_CALORIES = 500;

typedef struct Ingredient {
	int capacity, durability, flavor, texture, calories;

	Ingredient operator+(const Ingredient& i) {
		Ingredient tmp;

		tmp.capacity = this->capacity + i.capacity;
		tmp.durability = this->durability + i.durability;
		tmp.flavor = this->flavor + i.flavor;
		tmp.texture = this->texture + i.texture;
		tmp.calories = this->calories + i.calories;

		return tmp;
	}

	int getScore() {
		return std::max(0, capacity) * std::max(0, durability) *
			   std::max(0, flavor) * std::max(0, texture);
	}

	Ingredient() : capacity(0), durability(0), flavor(0), texture(0), calories(0) {

	}
} Ingredient;

int res1 = -INF;
int res2 = -INF;

void mixIngredients(const std::vector<Ingredient>& ings, Ingredient mix, int ind, int cnt) {
	if (cnt == MAX_INGS) {
		res1 = std::max(mix.getScore(), res1);

		if (mix.calories == MAX_CALORIES) {
			res2 = std::max(mix.getScore(), res2);
		}
	} else {
		for (size_t i = ind; i < ings.size(); ++ i) {
			mixIngredients(ings, mix + ings[i], i, cnt + 1);
		}
	}
}

int main() {
	std::ifstream fin("data.in");
	std::string str;
	std::vector<Ingredient> ings;

	while (std::getline(fin, str)) {
		Ingredient i;
		char name[20];

		sscanf(str.c_str(), frmt.c_str(), name, &i.capacity, &i.durability, &i.flavor, &i.texture, &i.calories);

		ings.push_back(i);
	}

	Ingredient tmp;
	mixIngredients(ings, tmp, 0, 0);

	std::cout << "The answer for part1 is: " << res1 << "\n";
	std::cout << "The answer for part2 is: " << res2 << "\n";

	fin.close();

	return 0;
}