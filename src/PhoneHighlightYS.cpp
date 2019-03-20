// Copyright [2019] <Stanislavyk Yaroslav> (stl.ros@outlook.com)

#include "PhoneHighlightYS.h"

#include <cstdint>
#include <tuple>

namespace {

constexpr uint8_t kPhoneNumber = 0u;
constexpr uint8_t kSearchString = 1u;
constexpr uint8_t kSearchStringPos = 2u;
constexpr uint8_t kMatchingStatus = 3u;
constexpr uint8_t kRange = 4u;
constexpr uint8_t kHighlightRanges = 5u;

template <typename Args>
void CollectRange(Args const& args)
{
    std::get<kMatchingStatus>(args) = false;
    std::get<kHighlightRanges>(args).push_back(std::get<kRange>(args));
}

template <typename Symbol, typename IgnoredSymbols>
bool IsSymbolIgnored(Symbol const symbol, IgnoredSymbols const& ignoredSymbols)
{
    return ignoredSymbols.find(symbol) != std::u32string::npos;
}

template <typename Args>
void ProcessCharacter(Args const& args, uint8_t const phoneNumberPos)
{
    auto const& phoneNumber = std::get<kPhoneNumber>(args);
    auto const& searchString = std::get<kSearchString>(args);
    auto& searchStringPos = std::get<kSearchStringPos>(args);
    auto& isMatchingStarted = std::get<kMatchingStatus>(args);
    auto& range = std::get<kRange>(args);

    if (!isMatchingStarted) {
        isMatchingStarted = true;
        range.first = phoneNumberPos;
    }

    range.second = phoneNumberPos;
    ++searchStringPos;

    if ((phoneNumberPos + 1u >= phoneNumber.size()) && (searchStringPos >= searchString.size())) {
        CollectRange(args);
    }
}

template <typename Args>
void ClearFoundRanges(Args const& args, uint8_t& phoneNumberPos)
{
    auto& searchStringPos = std::get<kSearchStringPos>(args);
    auto& isMatchingStarted = std::get<kMatchingStatus>(args);
    auto& range = std::get<kRange>(args);
    auto& highlightRanges = std::get<kHighlightRanges>(args);

    if (!highlightRanges.empty() && searchStringPos > 0) {
        phoneNumberPos = highlightRanges[0].first;
        highlightRanges.clear();
    }

    isMatchingStarted = false;
    searchStringPos = 0;
    range = {};
}

} // namespace

rms::HighlightRanges rms::GetPhoneHighlightYS(std::u32string const& phoneNumber, std::u32string const& searchString)
{
    if (phoneNumber.empty() || searchString.empty())
        return HighlightRanges();

    static std::u32string const ignoredChars = U" ()/#.\u00A0";

    HighlightRanges highlightRanges;
    std::pair<uint32_t, uint32_t> range;

    uint8_t searchStringPos = 0;
    bool isMatchingStarted = false;
    auto args = std::tie(phoneNumber, searchString, searchStringPos, isMatchingStarted, range, highlightRanges);

    for (uint8_t phoneNumberPos = 0; phoneNumberPos < phoneNumber.size(); ++phoneNumberPos) {
        if (searchStringPos >= searchString.size()) {
            CollectRange(args);
            searchStringPos = 0;
        }

        if (IsSymbolIgnored(phoneNumber[phoneNumberPos], ignoredChars)) {
            if (isMatchingStarted) {
                CollectRange(args);
            }
            continue;
        }

        const bool matchingFound = (phoneNumber[phoneNumberPos] == searchString[searchStringPos]);
        if (matchingFound) {
            ProcessCharacter(args, phoneNumberPos);
            continue;
        }

        ClearFoundRanges(args, phoneNumberPos);
    }

    return highlightRanges;
}
