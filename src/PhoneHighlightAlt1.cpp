// Copyright [2019] <Malinovsky Rodion> (rodionmalino@gmail.com)
#include "PhoneHighlightAlt1.h"

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

rms::HighlightRanges rms::GetPhoneHighlightAlt1(std::u32string const& phoneNumber, std::u32string const& searchString)
{
    using boost::adaptors::filtered;

    static std::u32string const ignoredChars = U" ()/#.\u00A0";

    auto const IsValidChar = [](char32_t const charToCheck) {
        return ignoredChars.find(charToCheck) == std::u32string::npos;
    };

    HighlightRanges highlightRanges;

    if (phoneNumber.empty() || searchString.empty())
        return highlightRanges;

    auto filteredRange = boost::make_iterator_range(phoneNumber | filtered(IsValidChar));
    auto itFoundPattern = boost::range::search(filteredRange, searchString);
    auto const endOfFilteredRange = filteredRange.end();

    while (itFoundPattern != endOfFilteredRange) {
        // Found match which might contain ignored chars
        // Calculate indexes of [begin; end) and prepare for next iteration
        auto tokenBeginIdx = itFoundPattern.base() - phoneNumber.begin();
        auto const searchStringUtf32Size = searchString.size();
        // Advance begin iterator for next search loop. Iteration is done within match range [begin, end]
        // At the same time split matched range into sub-ranges: if distance between two indexes
        // in is greater than 1 then some ignored chars are in-between.
        for (size_t idx = 1u; idx < searchStringUtf32Size; ++idx) {
            auto const prevItFoundPattern = itFoundPattern++;
            auto const nextItemIdx = itFoundPattern.base() - phoneNumber.begin();
            auto const prevItemIdx = prevItFoundPattern.base() - phoneNumber.begin();

            if (nextItemIdx - prevItemIdx > 1) {
                // Found sub-range
                highlightRanges.push_back(std::make_pair(tokenBeginIdx, prevItemIdx));
                tokenBeginIdx = nextItemIdx;
            }
        }
        auto tokenEndIdx = itFoundPattern.base() - phoneNumber.begin();
        highlightRanges.push_back(std::make_pair(tokenBeginIdx, tokenEndIdx));
        // position iterator on the element next after last element in the match range
        ++itFoundPattern;
        filteredRange = boost::make_iterator_range(itFoundPattern, filteredRange.end());

        itFoundPattern = boost::range::search(filteredRange, searchString);
    }
    return highlightRanges;
}
