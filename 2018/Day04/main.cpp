#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

#define TIMESTAMPSIZE 19
#define SLEEP "falls asleep"
#define WAKEUP "wakes up"

std::vector<std::string> V;
std::map<int, std::vector<int>> M;

int solvePart1() {
	int maxSleep = 0, maxId = 0;
	int maxMinute = 0;

	for (const auto& x : M) {
		int minutesSlept = 0;
		int overlaps = 0, minute = 0;

		for (int i = 0; i < 60; ++ i) {
			minutesSlept += x.second[i];

			if (overlaps < x.second[i]) {
				minute = i;
				overlaps = x.second[i];
			}
		}

		if (minutesSlept > maxSleep) {
			maxSleep = minutesSlept;
			maxId = x.first;
			maxMinute = minute;
		}
	}

	return maxId * maxMinute;
}

int solvePart2() {
	int maxSleepOnMinute = 0, maxId = 0, maxMinute = 0;

	for (const auto& x : M) {
		for (int i = 0; i < 60; ++ i) {
			if (x.second[i] > maxSleepOnMinute) {
				maxSleepOnMinute = x.second[i];
				maxId = x.first;
				maxMinute = i;
			}
		}
	}

	return maxId * maxMinute;
}


int main() {
	std::ifstream fin("data.in");
	std::string str;

	while (std::getline(fin, str)) {
		V.push_back(str);
	}

	std::sort(V.begin(), V.end());

	int currId = -1, lastSleepTime = 0;

	for (const auto& x : V) {
		int year, month, day, hour, minute, id;
		std::string action = x.substr(TIMESTAMPSIZE);

		sscanf(x.c_str(), "[%d-%d-%d %d:%d]", &year, &month, &day, &hour, &minute);

		if (action == SLEEP) {
			lastSleepTime = minute;
		} else if (action == WAKEUP) {
			for (int i = lastSleepTime; i < minute; ++ i) {
				M[currId][i] ++;
			}

			lastSleepTime = 0;
		} else {
			sscanf(action.c_str(), "Guard #%d begins shift", &id);
		
			if (M.find(id) == M.end()) {
				M[id] = std::vector<int> (60, 0);
			}
			
			currId = id;
			lastSleepTime = 0;
		}
	}

	std::cout << "The answer for part1 is: " << solvePart1() << "\n";
	std::cout << "The answer for part2 is: " << solvePart2() << "\n";

	fin.close();

	return 0;
}