#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <set>
#include <map>
#include <tuple>

typedef std::pair<int, int> II;
typedef std::tuple<int, int, int> III;
typedef std::tuple<int, int, int, int> IIII;

const int IMMUNE = 1, INFECTION = -1;
const std::regex groupReg("(\\d+) .* (\\d+) hit points (\\(.*\\))? ?.* (\\d+) (\\w+) .* (\\d+)");
const std::regex weaknessesReg("weak to ((?:\\w+(?:, )?)+)");
const std::regex immunitiesReg("immune to ((?:\\w+(?:, )?)+)");

std::vector<std::string> tokenize(const std::string str) {
    static const std::string delim = " ,";
    std::vector<std::string> words;
    size_t start = 0, end = 0;

    while (start != std::string::npos) {
        end = str.find_first_of(delim, start);
        words.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(delim, end);
    }

    return words;
}

typedef struct Group {
	int units, hitPoints, damage, initiative;
	std::string damageType;
	std::set<std::string> immunities, weaknesses;

	Group() : units(), hitPoints(), damage(), initiative(), damageType(), immunities(), weaknesses() {

	}

	void setFromMatch(const std::smatch& words) {
		units = std::stoi(words[1]);
		hitPoints = std::stoi(words[2]);
		damage = std::stoi(words[4]);
		initiative = std::stoi(words[6]);
		damageType = words[5];

		std::string wkAndIm(words[3]);

		{
			std::smatch im;

			if (std::regex_search(wkAndIm, im, immunitiesReg)) {
				std::vector<std::string> tmp = tokenize(im[1]);
				immunities = std::set<std::string>(tmp.begin(), tmp.end());
			}
		}

		{
			std::smatch wk;

			if (std::regex_search(wkAndIm, wk, weaknessesReg)) {
				std::vector<std::string> tmp = tokenize(wk[1]);
				weaknesses = std::set<std::string>(tmp.begin(), tmp.end());
			}
		}
	}

	int getEffPower() const {
		return units * damage;
	}

	bool isWeakAgainst(const std::string& weakness) const {
		return weaknesses.find(weakness) != weaknesses.end();
	}

	bool isImmuneAgainst(const std::string& immunity) const {
		return immunities.find(immunity) != immunities.end();
	}

	int getPowerAgainstGroup(const Group& other) const {
		int power = getEffPower();

		if (other.isImmuneAgainst(damageType)) {
			power = 0;
		} else if (other.isWeakAgainst(damageType)) {
			power *= 2;
		}

		return power;
	}
} Group;

typedef std::vector<Group> System;

void readSystem(std::istream& is, System& sys) {
	std::string str;

	while (std::getline(is, str) && !str.empty()) {
		Group g;
		std::smatch words;

		if (std::regex_match(str, words, groupReg)) {
			g.setFromMatch(words);
			sys.push_back(g);
		} else {
			std::cerr << "Invalid group definition! " << str << "\n";
		}
	}
}

std::vector<int> targetPhase(const System& attack, const System& defend) {
	std::set<III, std::greater<III>> attackOrder;

	for (size_t i = 0; i < attack.size(); ++ i) {
		if (attack[i].units > 0) {
			attackOrder.insert(III(attack[i].getEffPower(), attack[i].initiative, i));
		}
	}

	std::vector<int> targets(attack.size(), -1);

	for (const auto& k : attackOrder) {
		std::set<IIII, std::greater<IIII>> targetOrder;
		int grpInd = std::get<2>(k);

		for (size_t i = 0; i < defend.size(); ++ i) {
			if (defend[i].units > 0 &&
				std::find(targets.begin(), targets.end(), (int)i) == targets.end() &&
				attack[grpInd].getPowerAgainstGroup(defend[i]) > 0)
			{
				targetOrder.insert(IIII(attack[grpInd].getPowerAgainstGroup(defend[i]),
									    defend[i].getEffPower(),
									    defend[i].initiative,
									    i));
			}
		}

		
		if (targetOrder.size() > 0) {
			targets[grpInd] = std::get<3>(*(targetOrder.begin()));
		}
	}

	return targets;
}

void attackGroup(const Group& attacker, Group& defender) {
	if (attacker.units > 0) {
		defender.units -= std::min(defender.units,
								   attacker.getPowerAgainstGroup(defender) / defender.hitPoints);
	}
}

bool attackPhase(System& immune, System& infection) {
	std::vector<int> immuneTargets = targetPhase(immune, infection);
	std::vector<int> infectionTargets = targetPhase(infection, immune);
	std::set<III, std::greater<III>> attackOrder;

	for (size_t i = 0; i < immune.size(); ++ i) {
		if (immuneTargets[i] != -1) {
			attackOrder.insert(III(immune[i].initiative, i, IMMUNE));
		}
	}

	for (size_t i = 0; i < infection.size(); ++ i) {
		if (infectionTargets[i] != -1) {
			attackOrder.insert(III(infection[i].initiative, i, INFECTION));
		}
	}

	bool status = false;

	for (const auto& k : attackOrder) {
		int grpInd = std::get<1>(k);
		int grpType = std::get<2>(k);
		int before = 0, after = 0;

		if (grpType == INFECTION) {
			before = immune[infectionTargets[grpInd]].units;
			attackGroup(infection[grpInd], immune[infectionTargets[grpInd]]);
			after = immune[infectionTargets[grpInd]].units;
		} else {
			before = infection[immuneTargets[grpInd]].units;
			attackGroup(immune[grpInd], infection[immuneTargets[grpInd]]);
			after = infection[immuneTargets[grpInd]].units;
		}

		if (before - after > 0) {
			status = true;
		}
	}

	return status;
}

bool systemStillAlive(const System& sys) {
	return std::any_of(sys.begin(), sys.end(), [](const Group& g) { return g.units > 0; });
}

int countUnitsAlive(const System& sys) {
	int unitsAlive = 0;

	for (const auto& i : sys) {
		unitsAlive += i.units;
	}

	return unitsAlive;
}

II battle(System immune, System infection, int boost) {
	for (auto& grp : immune) {
		grp.damage += boost;
	}

	while (attackPhase(immune, infection));

	if (systemStillAlive(infection)) {
		return II(countUnitsAlive(infection), INFECTION);
	} 

	return II(countUnitsAlive(immune), IMMUNE);
}

int findBoost(const System& immune, const System& infection) {
	int winner = 0, unitsAlive = 0;
	int boost = 35; // TODO: CHANGE THIS TO 0...

	do {
		std::tie(unitsAlive, winner) = battle(immune, infection, ++ boost);
	} while (winner != IMMUNE);

	return unitsAlive;
}

int main() {
	std::ifstream fin("data.in");
	std::string str;
	System immune, infection;

	while (std::getline(fin, str)) {
		if (str == "Immune System:") {
			readSystem(fin, immune);
		} else if (str == "Infection:") {
			readSystem(fin, infection);
		} else {
			// do nothing
		}
	}

	std::cout << "The answer for part1 is: " << battle(immune, infection, 0).first << "\n";
	std::cout << "The answer for part2 is: " << findBoost(immune, infection) << "\n";

	fin.close();

	return 0;
}