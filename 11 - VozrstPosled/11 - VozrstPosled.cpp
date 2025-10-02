#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> findVozrs(const std::vector<int>& a) {
	int n = a.size();
	if (n == 0) {
		return {};
	}

	std::vector<int> d(n);

	std::vector<int> prev(n, -1);

	for (int i = 0; i < n; ++i) {
		d[i] = 1;
		for (int j = 0; j < i; ++j) {
			if (a[j] < a[i] && d[j] + 1 > d[i]) {
				d[i] = d[j] + 1;
				prev[i] = j;
			}
		}
	}

	int maxLenght = 0;
	int endIndex = -1;

	for (int i = 0; i < n; i++) {
		if (d[i] > maxLenght) {
			maxLenght = d[i];
			endIndex = i;
		}
	}

	std::vector<int> vp;
	if (endIndex != -1) {
		int current = endIndex;
		while (current != -1) {
			vp.push_back(a[current]);
			current = prev[current];
		}
		std::reverse(vp.begin(), vp.end());
	}

	return vp;
}

int main() {
	setlocale(0, "");

	std::vector<int> a = { 1, 5, 3, 7, 1, 4, 10, 15 };

	std::vector<int> vp = findVozrs(a);

	std::cout << "Исход. послед: ";
	for (int x : a) {
		std::cout << x << " ";
	}

	std::cout << "\nНаибольшая возрст послед: ";
	if (vp.empty()) {
		std::cout << "Пусто" << std::endl;
	}
	else {
		for (int x : vp) {
			std::cout << x << " ";
		}

		std::cout << "\nДлинна НВП: " << vp.size();
	}
}