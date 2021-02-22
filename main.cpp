#include <iostream>

#include <numeric>
#include <algorithm>
#include <vector>

using SomeData = float;
using SomeOtherData = float;
using SomeThirdData = float;

struct SoAExample {
	std::vector<int> keys;
	std::vector<SomeData> valuesOne;
	std::vector<SomeOtherData> valuesTwo;
	std::vector<SomeThirdData> valuesThree;
	//...
};

void sortSoa(SoAExample& soaExample)
{
	std::vector<size_t> sortedPositions;
	sortedPositions.resize(soaExample.keys.size());
	std::iota(sortedPositions.begin(), sortedPositions.end(), 0);

	std::sort(sortedPositions.begin(), sortedPositions.end(),
		[&soaExample](size_t i, size_t j) {
			return soaExample.keys[i] < soaExample.keys[j];
		}
	);

	// get reverse mapping
	std::vector<size_t> reverseMapping;
	reverseMapping.resize(sortedPositions.size());
	for (int i = 0; i < sortedPositions.size(); ++i)
	{
		reverseMapping[sortedPositions[i]] = i;
	}

	std::vector<std::pair<size_t, size_t>> swaps;
	swaps.reserve(sortedPositions.size());

	for (int i = 0; i < reverseMapping.size();)
	{
		size_t newI = reverseMapping[i];
		if (i != newI)
		{
			swaps.emplace_back(i, newI);
			reverseMapping[i] = reverseMapping[newI];
			reverseMapping[newI] = newI;
		}
		else
		{
			++i;
		}
	}

	for (auto [oldI, newI] : swaps)
	{
		std::swap(soaExample.keys[oldI], soaExample.keys[newI]);
	}

	for (auto [oldI, newI] : swaps)
	{
		std::swap(soaExample.valuesOne[oldI], soaExample.valuesOne[newI]);
	}

	for (auto [oldI, newI] : swaps)
	{
		std::swap(soaExample.valuesTwo[oldI], soaExample.valuesTwo[newI]);
	}

	for (auto [oldI, newI] : swaps)
	{
		std::swap(soaExample.valuesThree[oldI], soaExample.valuesThree[newI]);
	}
}

void fillSoa(SoAExample& soaExample)
{
	soaExample.keys = {3, 0, 1, 2};
	soaExample.valuesOne = {1.0f, 2.0f, 3.0f, 4.0f};
	soaExample.valuesTwo = {2.0f, 3.0f, 0.0f, 1.0f};
	soaExample.valuesThree = {-3.0f, -2.0f, -1.0f, 0.0f};
}

void printSoa(SoAExample& soaExample)
{
	for (size_t i = 0; i < soaExample.keys.size(); ++i)
	{
		std::cout << "{key=\"" << soaExample.keys[i];
		std::cout << "\", v1=\"" << soaExample.valuesOne[i];
		std::cout << "\", v2=\"" << soaExample.valuesTwo[i];
		std::cout << "\", v3=\"" << soaExample.valuesThree[i];
		std::cout << "\"}\n";
	}
}

int main() {
	SoAExample soaExample;
	fillSoa(soaExample);

	sortSoa(soaExample);

	printSoa(soaExample);

	return 0;
}
