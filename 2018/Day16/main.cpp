#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <functional>

#define REG_NR     (int)(4)
#define OPCODE_MAX (int)(16)

typedef struct Instruction {
	int opcode, A, B, C;
} Instruction;

typedef struct Capture {
	int before[REG_NR];
	Instruction ins;
	int after[REG_NR];
} Capture;

std::vector<std::function<int(int*, Instruction)>> functors {
	// addr
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] + cpu[ins.B]; },
	// addi
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] + ins.B; },
	// mulr
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] * cpu[ins.B]; },
	// muli
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] * ins.B; },
	// banr
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] & cpu[ins.B]; },
	// bani
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] & ins.B; },
	// borr
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] | cpu[ins.B]; },
	// borri
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] | ins.B; },
	// setr
	[](int cpu[], Instruction ins)->int { return cpu[ins.A]; },
	// seti
	[](int cpu[], Instruction ins)->int { return ins.A; },
	// gtir
	[](int cpu[], Instruction ins)->int { return ins.A > cpu[ins.B]; },
	// gtri
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] > ins.B; },
	// gtrr
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] > cpu[ins.B]; },
	// eqir
	[](int cpu[], Instruction ins)->int { return ins.A == cpu[ins.B]; },
	// eqri
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] == ins.B; },
	// eqrr
	[](int cpu[], Instruction ins)->int { return cpu[ins.A] == cpu[ins.B]; }
};

std::vector<size_t> getOpcodeMapping(std::map<int, std::set<int>> adj) {
	std::vector<size_t> mapping(OPCODE_MAX);

	for (int i = 0; i < OPCODE_MAX; ++ i) {
		int opcode;

		// get an one to one mapping from adj
		for (const auto& k : adj) {
			if (k.second.size() == 1) {
				opcode = *(k.second.begin());
				mapping[opcode] = (size_t)k.first;
				break;
			}
		}

		// remove this opcode from other mappings
		for (auto& k : adj) {
			k.second.erase(opcode);
		}
	}

	return mapping;
}

int part1(std::istream& fin, std::map<int, std::set<int>>& adj) {
	std::string str;
	int ret = 0;

	while (std::getline(fin, str)) {
		Capture c;
		
		if (sscanf(str.c_str(), "Before: [%d, %d, %d, %d]", &c.before[0], &c.before[1], &c.before[2], &c.before[3]) > 0) {
			std::getline(fin, str);
			sscanf(str.c_str(), "%d %d %d %d", &c.ins.opcode, &c.ins.A, &c.ins.B, &c.ins.C);

			std::getline(fin, str);
			sscanf(str.c_str(), "After: [%d, %d, %d, %d]", &c.after[0], &c.after[1], &c.after[2], &c.after[3]);

			fin.ignore(1, '\n');

			int cnt = 0, ind = 0;
			for (const auto& f : functors) {
				if (c.after[c.ins.C] == f(c.before, c.ins)) {
					++ cnt;
					adj[ind].insert(c.ins.opcode);
				}

				++ ind;
			}

			ret += (cnt >= 3);
		} else {
			break;
		}
	}

	return ret;
}

int part2(std::istream& fin, const std::map<int, std::set<int>>& adj) {
	int cpu[REG_NR] = {0};
	std::string str;
	std::vector<size_t> mapping = getOpcodeMapping(adj);

	fin.ignore(2, '\n');

	while (std::getline(fin, str)) {
		Instruction ins;

		sscanf(str.c_str(), "%d %d %d %d", &ins.opcode, &ins.A, &ins.B, &ins.C);
		cpu[ins.C] = functors[mapping[ins.opcode]](cpu, ins);
	}

	return cpu[0];
}

int main() {
	std::ifstream fin("data.in");
	std::map<int, std::set<int>> adj;

	std::cout << "The answer for part1 is: " << part1(fin, adj) << "\n";
	std::cout << "The answer for part2 is: " << part2(fin, adj) << "\n";

	fin.close();

	return 0;
}
