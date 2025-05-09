#pragma once

#include <libds/heap_monitor.h>

namespace Algoritmus
{
	template<typename Iterator, typename Predikat, typename Operacia>
    void algoritmus(const Iterator& begin, const Iterator& end, Predikat predikat, Operacia operacia)
	{
		for (auto i = begin; i != end; ++i)
		{
			if (predikat(*i))
			{
                operacia(*i);
			}
		}

	}
}
