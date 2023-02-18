#include "../../Source Code//Chapter12/generator.h"
#include <fstream>
#include <ranges>
#include <vector>
#include <iostream>

Generator<std::uint8_t> vb_encode_num(int n) {
    for (std::uint8_t cont = 0; cont == 0; /*..*/ ) {
        std::uint8_t b = static_cast<std::uint8_t>(n % 128);
        n /= 128;
        cont = (!n ? 128 :0);
        co_yield (b + cont);
    }
}

template <typename Range>
Generator<std::uint8_t> vb_encode(Range &r) {
    for (const auto &n : r) {
        for (const auto &byte : vb_encode_num(n)) { co_yield byte; }
    }
}

template <typename Range>
Generator<int> vb_decode(Range &bytes) {
    int n = 0, weight = 1;
    for (auto byte : bytes) {
        if (byte < 128) {
            n += byte * weight;
            weight *= 128;
        } else {
            n += (byte - 128) * weight;
            co_yield n;
            n = 0;
            weight = 1;
        }
    }
}

template <typename Range>
Generator<int> gap_encode(Range &ids) {
    // bit of C++17 "init if" for pure shiggles
    for (int last_id = 0; const auto &id : ids) {
        const int gap = id - last_id;
        last_id = id;
        co_yield gap;
    }
}

template <typename Range>
Generator<int> gap_decode(Range &gaps) {
    // bit of C++17 "init if" for pure shiggles
    for (int last_id = 0; const auto &gap : gaps) {
        const int id = gap + last_id;
        co_yield id;
        last_id = id;
    }
}

template <typename Range>
Generator<int> compress(Range &ids) {
    auto gaps = gap_encode(ids);
    for (auto byte : vb_encode(gaps)) { co_yield byte; }
}

template <typename Range>
Generator<int> decompress(Range &bytes) {
    auto gaps = vb_decode(bytes);
    for (const auto &id : gap_decode(gaps)) { co_yield id; }
}

template <typename Range>
void write(const std::string& path, Range& bytes) {
    auto out = std::ofstream{path, std::ios::out | std::ofstream::binary};
    std::ranges::copy(bytes.begin(), bytes.end(), std::ostreambuf_iterator<char>(out));
}

auto read(std::string path) -> Generator<std::uint8_t> {
    auto in = std::ifstream {path, std::ios::in | std::ofstream::binary};
    auto it = std::istreambuf_iterator<char>{in};
    const auto end = std::istreambuf_iterator<char>{};
    for (   ; it != end; ++it) {
        co_yield *it; }
}

int main()
{
    {
        auto documents = std::vector{367, 438, 439, 440};
        auto bytes = compress(documents);
        write("values.bin", bytes);
    }
    
    {
        auto bytes = read("values.bin");
        auto documents = decompress(bytes);
        for (auto doc : documents) { std::cout << doc << ", "; }
    }
    
    return 0;
}

// i haven't tested if this code runs
// ranges, a million autos and necessary boilerplate for godbolt just didn't seem worth it
