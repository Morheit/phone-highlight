// Copyright [2019] <Malinovsky Rodion> (rodionmalino@gmail.com)
#include "PhoneHighlightAlt1.h"
#include "PhoneHighlightAlt2.h"
#include "PhoneHighlightAlt3.h"
#include "PhoneHighlightAlt4.h"
#include "UtfUtils.h"
#include "UtfUtilsIcu.h"

#include <benchmark/benchmark.h>

namespace {

const std::vector<std::string> utf8Sstrings = {
    u8"ℵ",
    u8"zß水",
    "\u00A0",
    "(123) 59 35",
    "+49 (0) 56 56 5618",
    "1111111111",
    "+38\u00A0(0)\u00A097\u00A03545\u00A0123"
};

const std::vector<std::pair<std::u32string, std::u32string>> highlightInput = {
    { U"(123) 59 35", U"35" },
    { U"+49 (0) 56 56 5618", U"565618" },
    { U"1111111111", U"1" },
    { U"111 11111 11", U"1" },
    { U"8991891", U"891" },
    { U"09340932", U"0932" },
    { U"0934093257", U"0932" },
    { U"+38 (0) 97 3545 123", U"+380973545123" },
    { U"+38\u00A0(0)\u00A097\u00A03545\u00A0123", U"+380973545123" }
};

} // namespace

static void BM_UTF8ToUTF32(benchmark::State& state)
{
    for (auto _ : state)
        const auto result = rms::UTF8ToUTF32(utf8Sstrings[state.range(0)]);
}
BENCHMARK(BM_UTF8ToUTF32)->Arg(0)->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Complexity();

static void BM_UTF8ToUTF32ICU(benchmark::State& state)
{
    for (auto _ : state)
        const auto result = rms::UTF8ToUTF32ICU(utf8Sstrings[state.range(0)]);
}
BENCHMARK(BM_UTF8ToUTF32ICU)->Arg(0)->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Complexity();

static void BM_GetPhoneHighlightAlt1(benchmark::State& state)
{
    for (auto _ : state)
        const auto result = rms::GetPhoneHighlightAlt1(highlightInput[state.range(0)].first, highlightInput[state.range(0)].second);
}
BENCHMARK(BM_GetPhoneHighlightAlt1)->Arg(0)->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8)->Complexity();

static void BM_GetPhoneHighlightAlt2(benchmark::State& state)
{
    for (auto _ : state)
        const auto result = rms::GetPhoneHighlightAlt2(highlightInput[state.range(0)].first, highlightInput[state.range(0)].second);
}
BENCHMARK(BM_GetPhoneHighlightAlt2)->Arg(0)->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8)->Complexity();

static void BM_GetPhoneHighlightAlt3(benchmark::State& state)
{
    for (auto _ : state)
        const auto result = rms::GetPhoneHighlightAlt3(highlightInput[state.range(0)].first, highlightInput[state.range(0)].second);
}
BENCHMARK(BM_GetPhoneHighlightAlt3)->Arg(0)->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8)->Complexity();

static void BM_GetPhoneHighlightAlt4(benchmark::State& state)
{
    for (auto _ : state)
        const auto result = rms::GetPhoneHighlightAlt4(highlightInput[state.range(0)].first, highlightInput[state.range(0)].second);
}
BENCHMARK(BM_GetPhoneHighlightAlt4)->Arg(0)->Arg(1)->Arg(2)->Arg(3)->Arg(4)->Arg(5)->Arg(6)->Arg(7)->Arg(8)->Complexity();

BENCHMARK_MAIN();
