#include "code/basic/gcd.hpp"
#include "code/basic/bit_enumeration.hpp"
#include "code/basic/fast_io.hpp"
#include "code/basic/mint.hpp"
#include "code/basic/binary_search.hpp"
#include "code/basic/stress.hpp"
#include "code/basic/int128_io.hpp"
#include "code/basic/rng.hpp"
#include "code/basic/custom_hash.hpp"
#include "code/basic/debug.hpp"
#include <algorithm>
#include <climits>
#include <cstdio>
#include <fstream>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

std::mt19937_64* other_rng();
std::uint64_t other_hash_seed();

void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

std::FILE* input_file(const std::string& data) {
    // The runner's working directory is temporary; Windows tmpfile may target C:\\.
    auto* file = std::fopen("fast_io.in", "w+b");
    check(file != nullptr, "input fixture open failed");
    check(std::fwrite(data.data(), 1, data.size(), file) == data.size(), "write failed");
    std::rewind(file);
    return file;
}

void test_gcd() {
    check(gcd(0LL, 0LL) == 0, "gcd zero");
    check(gcd(-42LL, 30LL) == 6, "gcd negative");
    check(gcd(LLONG_MAX, 0LL) == LLONG_MAX, "gcd maximum");
    check(gcd(1ULL << 63, 6ULL) == 2, "gcd unsigned high bit");
    for (int a = -50; a <= 50; ++a) {
        for (int b = -50; b <= 50; ++b) {
            int expected = 0;
            for (int d = 1; d <= std::max(std::abs(a), std::abs(b)); ++d) {
                if (a % d == 0 && b % d == 0) expected = d;
            }
            check(gcd(a, b) == expected, "gcd brute mismatch");
        }
    }
}

void test_bits() {
    for (int n = 0; n <= 8; ++n) {
        std::vector<std::uint64_t> masks;
        for_each_mask(n, [&](auto m) { masks.push_back(m); });
        check(masks.size() == (std::size_t{1} << n), "mask count");
        for (std::size_t i = 0; i < masks.size(); ++i) check(masks[i] == i, "mask order");
        for (auto m : masks) {
            std::vector<int> bits;
            for_each_set_bit(m, [&](int i) { bits.push_back(i); });
            std::vector<int> expected_bits;
            for (int i = 0; i < n; ++i) if ((m >> i) & 1) expected_bits.push_back(i);
            check(bits == expected_bits, "set bit enumeration");
            for (bool empty : {false, true}) {
                std::vector<std::uint64_t> subs, expected;
                for_each_submask(m, [&](auto s) { subs.push_back(s); }, empty);
                for (int s = int(m); s >= 0; --s) {
                    if ((std::uint64_t(s) & m) == std::uint64_t(s) && (s || empty)) {
                        expected.push_back(std::uint64_t(s));
                    }
                }
                check(subs == expected, "submask brute mismatch");
            }
        }
        std::uint64_t power3 = 1;
        for (int i = 0; i < n; ++i) power3 *= 3;
        for (bool empty : {false, true}) {
            std::uint64_t count = 0;
            for_each_mask_submask(n, [&](auto m, auto s) {
                check((m & s) == s && (s || empty), "mask-submask pair");
                ++count;
            }, empty);
            check(count == power3 - (empty ? 0 : (std::uint64_t{1} << n)), "pair count");
        }
    }
    std::vector<int> high_bits;
    for_each_set_bit((std::uint64_t{1} << 63) | 1, [&](int i) { high_bits.push_back(i); });
    check(high_bits == std::vector<int>({0, 63}), "bit 63");
    std::vector<std::uint64_t> subs;
    for_each_submask(std::uint64_t{1} << 63, [&](auto s) { subs.push_back(s); });
    check(subs == std::vector<std::uint64_t>({std::uint64_t{1} << 63, 0}), "high submask");
}

void test_fast_io() {
    auto* file = input_file(" \r\n0 -0 +7 -2147483648 2147483647 "
                           "-9223372036854775808 9223372036854775807 "
                           "18446744073709551615 hello world");
    auto io = std::make_unique<FastIO>(file);
    int value = 9;
    check(io->read(value) && value == 0, "read zero");
    check(io->read(value) && value == 0, "read negative zero");
    check(io->read(value) && value == 7, "read plus");
    check(io->read(value) && value == INT_MIN, "read int min");
    check(io->read(value) && value == INT_MAX, "read int max");
    long long wide = 0;
    check(io->read(wide) && wide == LLONG_MIN, "read long long min");
    check(io->read(wide) && wide == LLONG_MAX, "read long long max");
    unsigned long long unsigned_value = 0;
    check(io->read(unsigned_value) && unsigned_value == ULLONG_MAX, "read unsigned max");
    std::string token = "old";
    check(io->read(token) && token == "hello", "read first token");
    check(io->read(token) && token == "world", "token at EOF");
    check(!io->read(token) && token == "world", "EOF unchanged string");
    check(!io->read(value) && value == INT_MAX, "repeated EOF");
    std::fclose(file);

    file = input_file("2147483648 -2147483649 12x + - 8 -1 "
                      "18446744073709551616 9 -128 127 128 255 256");
    io = std::make_unique<FastIO>(file);
    value = 42;
    for (int i = 0; i < 5; ++i) check(!io->read(value) && value == 42, "bad int unchanged");
    check(io->read(value) && value == 8, "recover after bad token");
    unsigned_value = 17;
    check(!io->read(unsigned_value) && unsigned_value == 17, "negative unsigned");
    check(!io->read(unsigned_value) && unsigned_value == 17, "unsigned overflow");
    check(io->read(unsigned_value) && unsigned_value == 9, "unsigned recovery");
    signed char small = 0;
    check(io->read(small) && small == -128, "small min");
    check(io->read(small) && small == 127, "small max");
    check(!io->read(small) && small == 127, "small overflow");
    unsigned char byte = 0;
    check(io->read(byte) && byte == 255, "byte max");
    check(!io->read(byte) && byte == 255, "byte overflow");
    std::fclose(file);

    std::string data(FastIO::BUFFER_SIZE - 5, ' ');
    data += "-9223372036854775808 ";
    std::string long_token(FastIO::BUFFER_SIZE + 17, 'z');
    data += long_token + " 23";
    file = input_file(data);
    io = std::make_unique<FastIO>(file);
    check(io->read(wide) && wide == LLONG_MIN, "integer crosses buffer");
    check(io->read(token) && token == long_token, "string crosses buffer");
    check(io->read(value) && value == 23 && !io->read(value), "final number without space");
    std::fclose(file);

    std::mt19937_64 engine(721);
    std::vector<long long> numbers;
    data.clear();
    for (int i = 0; i < 10000; ++i) {
        auto x = std::uniform_int_distribution<long long>(LLONG_MIN, LLONG_MAX)(engine);
        numbers.push_back(x);
        data += std::to_string(x) + (i % 2 ? "\n" : " ");
    }
    file = input_file(data);
    io = std::make_unique<FastIO>(file);
    for (auto x : numbers) check(io->read(wide) && wide == x, "fast IO randomized roundtrip");
    std::fclose(file);

    for (const auto& empty : {std::string(), std::string(" \t\r\n"), std::string("-")}) {
        file = input_file(empty);
        io = std::make_unique<FastIO>(file);
        value = 31;
        check(!io->read(value) && value == 31, "empty or sign-only input");
        std::fclose(file);
    }
}

template <int MOD>
void test_mint_mod(std::mt19937_64& engine, bool prime) {
    using M = Mint<MOD>;
    auto normalized = [](i128 x) {
        x %= MOD;
        return int(x < 0 ? x + MOD : x);
    };
    check(M().v == 0, "mint default");
    for (auto x : {LLONG_MIN, -1LL, 0LL, 1LL, LLONG_MAX}) {
        check(M(x).v == normalized(x), "mint constructor extreme");
    }
    for (int i = 0; i < 2000; ++i) {
        long long x = std::uniform_int_distribution<long long>(LLONG_MIN, LLONG_MAX)(engine);
        long long y = std::uniform_int_distribution<long long>(LLONG_MIN, LLONG_MAX)(engine);
        M a(x), b(y), original = a;
        check((a + b).v == normalized(i128(a.v) + b.v), "mint addition");
        check((a - b).v == normalized(i128(a.v) - b.v), "mint subtraction");
        check((a * b).v == normalized(i128(a.v) * b.v), "mint multiplication");
        check((-a).v == normalized(-i128(a.v)), "mint unary minus");
        int exponent = int(engine() % 50);
        i128 expected = 1;
        for (int k = 0; k < exponent; ++k) expected = expected * a.v % MOD;
        check(a.pow(exponent).v == expected && a == original, "mint pow reference and nonmutation");
        if (prime && b.v) {
            check((b * b.inv()).v == 1, "mint inverse");
            check(a / b * b == a, "mint division");
        }
        M compound = a;
        compound += b; compound -= b;
        check(compound == a, "mint compound reuse");
    }
    check(M(0).pow(0).v == 1, "mint zero exponent");
    check(M(1).pow(LLONG_MAX).v == 1, "mint maximum exponent");
    check(M(MOD - 1).pow(LLONG_MAX) == M(MOD - 1), "mint maximum odd exponent");
    check(M(MOD - 1) + M(MOD - 1) == M(MOD - 2), "mint large addition");
    check(M(MOD - 1) * M(MOD - 1) == M(1), "mint large product");
}

void test_binary_search() {
    std::mt19937_64 engine(937);
    for (int tc = 0; tc < 2000; ++tc) {
        std::vector<int> a(std::size_t(engine() % 30));
        for (auto& value : a) value = int(engine() % 15) - 7;
        std::sort(a.begin(), a.end());
        for (int x = -9; x <= 9; ++x) {
            int lower = 0, upper = 0;
            for (auto value : a) { lower += value < x; upper += value <= x; }
            check(lower_index(a, x) == lower, "lower linear reference");
            check(upper_index(a, x) == upper, "upper linear reference");
            check(last_less_index(a, x) == lower - 1, "less predecessor");
            check(last_less_equal_index(a, x) == upper - 1, "less equal predecessor");
        }
    }
}

void test_int128() {
    const i128 maximum = i128((u128{1} << 127) - 1);
    const i128 minimum = -maximum - 1;
    for (const auto& item : std::vector<std::pair<std::string, i128>>{
        {"0", 0}, {"-0", 0}, {"+17", 17},
        {"170141183460469231731687303715884105727", maximum},
        {"-170141183460469231731687303715884105728", minimum}}) {
        std::istringstream in(item.first);
        i128 value = 99;
        check(bool(in >> value) && value == item.second, "int128 boundary input");
        std::ostringstream out;
        out << value;
        if (value == minimum || value == maximum) check(out.str() == item.first, "int128 boundary output");
        if (value == 0) check(out.str() == "0", "int128 zero output");
    }
    for (const auto& token : {"", " ", "+", "-", "12x", "--1",
                            "170141183460469231731687303715884105728",
                            "-170141183460469231731687303715884105729"}) {
        std::istringstream in(token);
        i128 value = 37;
        check(!(in >> value) && value == 37, "int128 fail unchanged");
    }
    std::istringstream in("bad 42");
    i128 value = 9;
    check(!(in >> value), "int128 malformed");
    in.clear();
    check(bool(in >> value) && value == 42, "int128 recovery");
    check(!(in >> value) && value == 42, "int128 repeated EOF");
    std::mt19937_64 engine(612);
    for (int i = 0; i < 10000; ++i) {
        u128 magnitude = (u128(engine() & (UINT64_MAX >> 1)) << 64) | engine();
        i128 original = i128(magnitude);
        if (engine() & 1) original = -original;
        std::ostringstream out;
        out << original;
        std::istringstream back(out.str());
        i128 parsed = 0;
        check(bool(back >> parsed) && parsed == original, "int128 random roundtrip");
    }
}

void test_rng_hash() {
    check(other_rng() == &rng, "inline RNG shared across translation units");
    check(other_hash_seed() == CustomHash::seed(), "hash seed shared across translation units");
    rng.seed(12345);
    std::vector<long long> first;
    for (int i = 0; i < 100; ++i) first.push_back(rnd(-100, 100));
    rng.seed(12345);
    for (auto x : first) check(rnd(-100, 100) == x, "RNG seed reproducibility");
    for (int i = 0; i < 10000; ++i) {
        auto x = rnd(-13, 19);
        check(-13 <= x && x <= 19, "RNG closed interval");
        check(rnd(LLONG_MIN, LLONG_MIN) == LLONG_MIN, "RNG minimum singleton");
        check(rnd(LLONG_MAX, LLONG_MAX) == LLONG_MAX, "RNG maximum singleton");
        (void)rnd(LLONG_MIN, LLONG_MAX);
    }
    std::vector<int> values(100);
    std::iota(values.begin(), values.end(), 0);
    std::shuffle(values.begin(), values.end(), rng);
    std::sort(values.begin(), values.end());
    for (int i = 0; i < 100; ++i) check(values[i] == i, "shuffle preserves permutation");
    CustomHash hash;
    auto initial = hash(std::pair<int, int>{-1, 2});
    std::unordered_map<long long, int, CustomHash> integers;
    std::unordered_map<std::pair<int, int>, int, CustomHash> pairs;
    for (int i = -2000; i <= 2000; ++i) {
        integers[i] = i + 2000;
        pairs[{i, -i}] = i;
    }
    integers.rehash(20000); pairs.rehash(20000);
    for (int i = -2000; i <= 2000; ++i) {
        check(integers.at(i) == i + 2000, "integer hash lookup after rehash");
        check(pairs.at({i, -i}) == i, "pair hash lookup after rehash");
    }
    integers[LLONG_MIN] = 1; integers[LLONG_MAX] = 2;
    check(integers.at(LLONG_MIN) == 1 && integers.at(LLONG_MAX) == 2, "hash signed extremes");
    check(initial == hash(std::pair<int, int>{-1, 2}), "stable pair hash");
    integers.clear(); integers[-1] = 9;
    check(integers.at(-1) == 9, "hash table reuse");
}

void test_debug()
{
    std::ostringstream captured;
    auto* old = std::cerr.rdbuf(captured.rdbuf());
    int n = 7, cnt = 0;
    debug(n);
    if (true) debug("if");
    else debug("else");
    i128 wide = i128(1000000000000LL) * 1000000000000LL;
    debug(wide);
    debug(++cnt);
    std::cerr.rdbuf(old);
    check(cnt == 1, "debug evaluates once, with and without LOCAL");
    check(captured.str().find("n: 7\n") != std::string::npos, "debug name and scalar");
    check(captured.str().find("n: 7\n\"if\": if\n") == 0, "debug if/else and string");
    check(captured.str().find("1000000000000000000000000") != std::string::npos, "int128 stream debug");
}

std::string read_text(const std::string& path)
{
    std::ifstream in(path);
    check(bool(in), "fixture file read");
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

void test_stress(const std::string& command)
{
    std::ostringstream log;
    auto* old = std::cerr.rdbuf(log.rdbuf());
    bool ac1 = stress(command + " --gen", command + " --sol", command + " --brute", 3);
    bool ac2 = stress(command + " --gen", command + " --sol", command + " --brute", 2);
    bool wa = stress(command + " --gen", command + " --sol", command + " --wrong", 1);
    std::string saved = read_text("data.in");
    check(std::system((command + " --gen 1 > replay.in").c_str()) == 0, "replay generator");
    check(saved == read_text("replay.in"), "seed reproduces failing input");
    check(std::system((command + " --gen 2 > replay.in").c_str()) == 0, "next seed generator");
    check(saved != read_text("replay.in"), "different rounds generate different inputs");
    bool tokens = stress(command + " --gen", command + " --split", command + " --joined", 1);
    bool empty = stress(command + " --gen", command + " --empty", command + " --empty-lines", 1);
    bool gen_re = stress(command + " --fail", command + " --sol", command + " --brute", 1);
    bool sol_re = stress(command + " --gen", command + " --fail", command + " --brute", 1);
    bool brute_re = stress(command + " --gen", command + " --sol", command + " --fail", 1);
    bool stopped = stress(command + " --gen-stop", command + " --sol", command + " --brute");
    std::cerr.rdbuf(old);
    check(ac1 && ac2 && !wa && !tokens && empty && !gen_re && !sol_re && !brute_re && !stopped,
          "stress AC, WA, empty output and process error branches");
    check(log.str().find("AC 3") != std::string::npos, "finite rounds completed");
    check(log.str().find("FAIL seed=1") != std::string::npos, "failure seed diagnostic");
    check(log.str().find("FAIL seed=3") != std::string::npos, "unlimited rounds stop on failure");
}

int main(int argc, char** argv) {
    if (argc > 1) {
        std::string mode = argv[1];
        if (mode == "--fail") return 3;
        if (mode == "--gen" || mode == "--gen-stop")
        {
            if (argc != 3) return 4;
            auto seed = std::stoull(argv[2]);
            if (mode == "--gen-stop" && seed == 3) return 3;
            rng.seed(seed);
            std::cout << "5\n";
            for (int i = 1; i <= 5; i++) std::cout << rnd(-20, 20) << " ";
            std::cout << '\n';
            return 0;
        }
        if (mode == "--wrong") { std::cout << "wrong\n"; return 0; }
        if (mode == "--split") { std::cout << "1 2\n"; return 0; }
        if (mode == "--joined") { std::cout << "12\n"; return 0; }
        if (mode == "--empty") return 0;
        if (mode == "--empty-lines") { std::cout << "\n\n"; return 0; }
        int n; std::cin >> n;
        std::vector<long long> a(n);
        for (auto& x : a) std::cin >> x;
        if (mode == "--sol") std::cout << std::accumulate(a.begin(), a.end(), 0LL) << '\n';
        else if (mode == "--brute") {
            long long sum = 0;
            for (auto x : a) sum += x;
            std::cout << sum << "   \n\n";
        } else return 4;
        return 0;
    }
    try {
        test_gcd(); test_bits(); test_fast_io();
        std::mt19937_64 engine(456);
        test_mint_mod<2>(engine, true);
        test_mint_mod<998244353>(engine, true);
        test_mint_mod<2147483647>(engine, true);
        test_mint_mod<12>(engine, false);
        test_binary_search(); test_int128(); test_rng_hash(); test_debug();
#ifdef _WIN32
        test_stress(".\\tools_test.exe");
#else
        test_stress("./tools_test");
#endif
        std::cout << "all 10 basic tools passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
