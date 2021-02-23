#include <iostream>

#include <numeric>
#include <algorithm>
#include <vector>

using SomeData = float;
using SomeOtherData = float;
using SomeThirdData = float;

template <typename... ValueTypes>
struct SoAExample {
	std::vector<int> keys;
	std::tuple<std::vector<ValueTypes>...> values;
};

template<typename ElementType>
void getSortedPositions(std::vector<size_t>& inOutPositions, const std::vector<ElementType>& keys)
{
	inOutPositions.resize(keys.size());
	std::iota(inOutPositions.begin(), inOutPositions.end(), 0);

	std::sort(inOutPositions.begin(), inOutPositions.end(),
		[&keys](size_t i, size_t j) {
			return keys[i] < keys[j];
		}
	);
}

struct Swap
{
	Swap(size_t pos1, size_t pos2) : pos1(pos1), pos2(pos2) {}
	size_t pos1;
	size_t pos2;
};

static void generateSwaps(std::vector<Swap>& swaps, const std::vector<size_t>& sortedPositions)
{
	// get reverse mapping
	std::vector<size_t> reverseMapping;
	reverseMapping.resize(sortedPositions.size());
	for (int i = 0; i < sortedPositions.size(); ++i)
	{
		reverseMapping[sortedPositions[i]] = i;
	}

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
}

template<typename VectorType>
void applySwaps(std::vector<VectorType>& valuesVector, const std::vector<Swap>& swaps)
{
	for (auto [oldI, newI] : swaps)
	{
		std::swap(valuesVector[oldI], valuesVector[newI]);
	}
}

template<typename... ValueTypes>
void sortSoaVariant1(SoAExample<ValueTypes...>& inOutStruct)
{
	std::vector<size_t> sortedPositions;
	getSortedPositions(sortedPositions, inOutStruct.keys);

	std::vector<Swap> swaps;
	generateSwaps(swaps, sortedPositions);

	applySwaps(inOutStruct.keys, swaps);

	std::apply(
		[&swaps](auto&... values) {	(applySwaps(values, swaps), ...); },
		inOutStruct.values
	);
}

void fillSoa(SoAExample<float, float, float>& outSoaExample)
{
	outSoaExample.keys = {1, 2, 3, 0};
	std::get<0>(outSoaExample.values) = {1.0f, 2.0f, 3.0f, 4.0f};
	std::get<1>(outSoaExample.values) = {2.0f, 3.0f, 0.0f, 1.0f};
	std::get<2>(outSoaExample.values) = {-3.0f, -2.0f, -1.0f, 0.0f};
}

void printSoa(const SoAExample<float, float, float>& soaExample)
{
	for (size_t i = 0; i < soaExample.keys.size(); ++i)
	{
		std::cout << "{key=\"" << soaExample.keys[i];
		std::cout << "\", v1=\"" << std::get<0>(soaExample.values)[i];
		std::cout << "\", v2=\"" << std::get<1>(soaExample.values)[i];
		std::cout << "\", v3=\"" << std::get<2>(soaExample.values)[i];
		std::cout << "\"}\n";
	}
}

int main() {
	SoAExample<float, float, float> soaExample;
	fillSoa(soaExample);

	sortSoaVariant1(soaExample);

	printSoa(soaExample);

	return 0;
}
