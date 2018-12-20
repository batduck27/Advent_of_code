#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <queue>

typedef std::pair<int, int> Pos;

const int PART2_LIMIT = 1000;
const int INF = (1 << 30);
const int UNEXPLORED = -1;
const char START = '^', END = '$', OR = '|', O_BRACE = '(', C_BRACE = ')';
const Pos START_POS = Pos(0, 0);

std::map<char, Pos> adv {
	{'N', Pos(0, -1)},
	{'E', Pos(+1, 0)},
	{'S', Pos(0, +1)},
	{'W', Pos(-1, 0)}
};

Pos operator+(const Pos& a, const Pos& b)
{
    return Pos(a.first + b.first, a.second + b.second);
}

Pos operator*(const Pos& p, const int& f) {
	return Pos(p.first * f, p.second * f);
}

Pos& operator+=(Pos& a, const Pos& b) {
	a.first += b.first;
	a.second += b.second;

	return a;
}

std::map<Pos, int> rooms;
std::set<Pos> doors;

void advance(std::vector<Pos>& currRooms, const std::string& str, size_t& ind) {
	static const std::string DIR = "NSEW";

	while (DIR.find(str[ind]) != std::string::npos) {
		for (auto& i : currRooms) {
			rooms[i + adv[str[ind]] * 2] = UNEXPLORED;
			doors.insert(i + adv[str[ind]]);

			i += adv[str[ind]] * 2;
		}

		++ ind;
	}

	-- ind;
}

std::vector<Pos> explore(std::vector<Pos> currRooms, const std::string& str, size_t& ind) {
	std::vector<Pos> save(currRooms);
	std::vector<Pos> newRooms;

	if (str[ind] == START) {
		++ ind;
	}

	while (str[ind] != END) {
		switch (str[ind]) {
			case O_BRACE:
				currRooms = explore(currRooms, str, ++ ind);
				break;
			case C_BRACE:
				newRooms.insert(newRooms.end(), currRooms.begin(), currRooms.end());
				
				std::sort(newRooms.begin(), newRooms.end());
				newRooms.erase(std::unique(newRooms.begin(), newRooms.end()), newRooms.end());
				
				return newRooms;
			case OR:
				newRooms.insert(newRooms.end(), currRooms.begin(), currRooms.end());
				currRooms = save;
				break;
			default:
				advance(currRooms, str, ind);
				break;
		}

		++ ind;
	}

	return currRooms;
}

int main() {
	std::ifstream fin("data.in");
	std::string str;

	std::getline(fin, str);

	std::vector<Pos> currRooms(1, START_POS);
	size_t ind = 0;
	explore(currRooms, str, ind);

	int maxSteps = -INF, part2Cnt = 0;
	std::queue<Pos> Q;
	Q.push(START_POS);
	rooms[START_POS] = 0;

	while (!Q.empty()) {
		Pos currRoom = Q.front();
		Q.pop();

		maxSteps = std::max(maxSteps, rooms[currRoom]);

		if (rooms[currRoom] >= PART2_LIMIT) {
			++ part2Cnt;
		}

		for (const auto& i : adv) {
			Pos nextRoom = currRoom + i.second * 2;
			Pos doorBtwn = currRoom + i.second;

			if (rooms.find(nextRoom) != rooms.end() &&
			    doors.find(doorBtwn) != doors.end() &&
			    rooms[nextRoom] == UNEXPLORED) {

				rooms[nextRoom] = rooms[currRoom] + 1;
				Q.push(nextRoom);
			}
		}
	}

	std::cout << "The answer for part1 is: " << maxSteps << "\n";
	std::cout << "The answer for part2 is: " << part2Cnt << "\n"; 

	fin.close();

	return 0;
}

void writeMap() {
	int minX = INF, maxX = -INF, minY = INF, maxY = -INF;

	for (const auto& x : rooms) {
		minX = std::min(minX, x.first.first);
		maxX = std::max(maxX, x.first.first);
		minY = std::min(minY, x.first.second);
		maxY = std::max(maxY, x.first.second);
	}

	for (int i = minY - 1; i <= maxY + 1; ++ i) {
		for (int j = minX - 1; j <= maxX + 1; ++ j) {
			if (i == 0 && j == 0) {
				std::cout << 'X';
			} else if (rooms.find(Pos(j, i)) != rooms.end()) {
				std::cout << '.';
			} else if (doors.find(Pos(j, i)) != doors.end()) {
				if (i & 1) {
					std::cout << '-';
				} else {
					std::cout << '|';
				}
			} else {
				std::cout << '#';
			}
		}

		std::cout << "\n";
	}
}