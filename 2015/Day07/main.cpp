#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <functional>
#include <algorithm>
#include <regex>

std::map<std::string, unsigned short> wires;

bool isNumber(const std::string& n) {
    return std::all_of(n.begin(), n.end(), ::isdigit);
}

typedef struct Instruction {
	std::function<void(Instruction&)> f;
	std::string op1, op2, res;

	Instruction() : f(), op1(), op2(), res() {

	}

	Instruction(std::function<void(Instruction&)> _f, std::string _op1, std::string _res)
		: f(_f), op1(_op1), op2(), res(_res) {

	}

	Instruction(std::function<void(Instruction&)> _f, std::string _op1, std::string _op2, std::string _res)
		: f(_f), op1(_op1), op2(_op2), res(_res) {

	}

	bool isReady() {
		return (wires.find(res) == wires.end()) &&
			   (isNumber(op1) || wires.find(op1) != wires.end()) &&
			   (op2.empty() || isNumber(op2) || wires.find(op2) != wires.end());
	}
} Instruction; 

unsigned short getValue(std::string op) {
	unsigned short retVal = 0;

	if (isNumber(op)) {
		retVal = (unsigned short)(std::stoi(op));
	} else {
		retVal = wires[op];
	}

	return retVal;
}

std::map<std::string, std::function<void(Instruction&)>> functors {
	{"STORE",  [](Instruction& i)->void { wires[i.res] = getValue(i.op1); }},
	{"NOT",    [](Instruction& i)->void { wires[i.res] = ~(getValue(i.op1)); }},
	{"AND",    [](Instruction& i)->void { wires[i.res] = getValue(i.op1) & getValue(i.op2); }},
	{"OR",     [](Instruction& i)->void { wires[i.res] = getValue(i.op1) | getValue(i.op2); }},
	{"LSHIFT", [](Instruction& i)->void { wires[i.res] = getValue(i.op1) << getValue(i.op2); }},
	{"RSHIFT", [](Instruction& i)->void { wires[i.res] = getValue(i.op1) >> getValue(i.op2); }}
};

void update(std::list<Instruction> I) {
	bool updated = true;

	while (updated) {
		updated = false;

		for (auto it = I.begin(); it != I.end(); ) {
			if ((*it).isReady()) {
				(*it).f(*it);
				updated = true;
				it = I.erase(it);
			} else {
				++ it;
			}
		}
	}
}

int main() {
	std::ifstream fin("data.in");
	std::string str;
	std::list<Instruction> I;
	
	const std::regex assignOp("(\\w+) -> (\\w+)");
    const std::regex notOp("NOT (\\w+) -> (\\w+)");
    const std::regex binaryOp("(\\w+) (AND|OR|LSHIFT|RSHIFT) (\\w+) -> (\\w+)");

	while (std::getline(fin, str)) {
		std::smatch words;

		if (std::regex_match(str, words, assignOp)) {
      		I.push_back(Instruction(functors["STORE"], words[1], words[2]));
        } else if (std::regex_match(str, words, notOp)) {
    		I.push_back(Instruction(functors["NOT"], words[1], words[2]));
        } else if (std::regex_match(str, words, binaryOp)) {
            I.push_back(Instruction(functors[words[2]], words[1], words[3], words[4]));
        } else {
        	// do nothing
        }
	}

	update(I);
	std::cout << "The answer for part1 is: " << wires["a"] << "\n";

	unsigned short bWire = wires["a"];
	wires.clear();
	wires["b"] = bWire;

	update(I);
	std::cout << "The answer for part2 is: " << wires["a"] << "\n";	

	fin.close();

	return 0;
}