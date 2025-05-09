#pragma once

#include <complexities/complexity_analyzer.h>
#include <libds/adt/table.h>
#include <random>

namespace ds::utils
{
	template<typename TableType>
	class TableRemoveAnalyser :
		public ComplexityAnalyzer<TableType>
		
	{
	public:
		TableRemoveAnalyser() :
			ComplexityAnalyzer(name),
			rng_(144)
		{}

		void growToSize(TableType& structure, size_t size) override
		{
			std::uniform_int_distribution<int> dist(0);

			while (structure.size() < size)
			{
				const int key = dist(rng_);
				structure.insert(key, key);
				try
				{
					structure.insert(key, key);
					keys.push_back(key);
				} catch (...)
				{
					
				}
			}
		}

		
		void executeOperation(TableType& structure) override
		{
			structure.remove();
		}
	private:
		std::default_random_engine rng_;
		std::vector<int> keys;
	};

	class SSTRemoveAnalyzer : public TableRemoveAnalyser<ds::adt::SortedSequenceTable<int, int>>
	{
	public: SSTRemoveAnalyzer() : TableRemoveAnalyser("SSTRemoveAnalyzer") {}
	};
}

