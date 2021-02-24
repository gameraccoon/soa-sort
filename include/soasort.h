#pragma once

#include <vector>
#include <numeric>
#include <algorithm>

namespace soasort
{
	template<typename ElementType>
	void getSortedPositions(std::vector<size_t>& inOutPositions, const std::vector<ElementType>& keys)
	{
		inOutPositions.resize(keys.size());
		std::iota(inOutPositions.begin(), inOutPositions.end(), 0);

		std::sort(
			inOutPositions.begin(),
			inOutPositions.end(),
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

	inline void generateSwaps(std::vector<Swap>& swaps, const std::vector<size_t>& sortedPositions)
	{
		// get reverse mapping
		std::vector<size_t> reverseMapping;
		reverseMapping.resize(sortedPositions.size());
		for (size_t i = 0; i < sortedPositions.size(); ++i)
		{
			reverseMapping[sortedPositions[i]] = i;
		}

		swaps.reserve(sortedPositions.size());

		for (size_t i = 0; i < reverseMapping.size();)
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

	template<typename KeyType, typename... ValueTypes>
	void sortWithSwaps(std::vector<KeyType>& keys, std::vector<ValueTypes>&... valueVectors)
	{
		std::vector<size_t> sortedPositions;
		getSortedPositions(sortedPositions, keys);

		std::vector<Swap> swaps;
		generateSwaps(swaps, sortedPositions);

		applySwaps(keys, swaps);

		(applySwaps(valueVectors, swaps), ...);
	}
}
