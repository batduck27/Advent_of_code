#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <fstream>

const int PREAMBLE = 25;

bool find_elems_to_sum(const std::unordered_map<long long, int> &M, long target) {
	for (const auto &[key, app] : M) {
		if (target - key != key && M.find(target - key) != M.end()) {
			return true;
		}
	}

	return false;
}

int main() {
	std::ifstream fin("../input/day09.in");
	std::vector<long long> V;
	long long val;
	std::queue<long long> Q;
	std::unordered_map<long long, int> M;

	while (fin >> val) {
		V.push_back(val);
	}

	for (int i = 0; i < PREAMBLE; ++ i) {
		Q.push(V[i]);
		M[V[i]] ++;
	}

	long long part1 = 0;

	for (int i = PREAMBLE; i < V.size(); ++ i) {
		if (find_elems_to_sum(M, V[i]) == false) {
			part1 = V[i];
			std::cout << V[i] << "\n";
			break;
		}

		int front = Q.front();
		Q.pop();
		M[V[i]] ++;
		Q.push(V[i]);
		M[front] --;
		if (M[front] == 0) {
			M.erase(front);
		}
	}

	std::unordered_map<int, long long> part;

	part[-1] = 0;
	for (int i = 0; i < V.size(); ++ i) {
		part[i] = part[i - 1] + V[i];
	}

	for (int i = 0; i < V.size(); ++ i) {
		int j = i - 1;
		while (part[i] - part[j - 1] <= part1 && j > -1) {
			--j;
		}

		// std::cout << j << " " << i << "   " << part[i] - part[j] << "\n";

		if (part[i] - part[j] == part1 && j != i - 1) {
			std::cout << "found it ! " << j + 1 << " -> " << i << "\n";

			long long min_elem = V[i];
			long long max_elem = V[i];
			for (int k = j + 1; k <= i; ++ k) {
				min_elem = std::min(min_elem, V[k]);
				max_elem = std::max(max_elem, V[k]);
			}
			std::cout << min_elem + max_elem << "\n";

			break;
		}
	}

	std::cout << "\n";


	return 0;
}
