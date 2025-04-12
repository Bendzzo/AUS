#pragma once
#include <vector>
#include <functional>

template <typename Iterator, typename OutputContainer>
class FilterAlgorithm {
public:
    void filter(Iterator begin, Iterator end,
        OutputContainer& vysledok,
        std::function<bool(const typename std::iterator_traits<Iterator>::value_type&)> predikat) {
        vysledok.clear();
        for (Iterator it = begin; it != end; ++it) {
            if (predikat(*it)) {
                vysledok.push_back(*it);
            }
        }
    }
};
