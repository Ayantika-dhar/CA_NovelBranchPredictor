#include <cmath>
#include <cstdint>
#include <map>
#include <array>
#include <bitset>
#include "ooo_cpu.h"
#include "msl/fwcounter.h"

namespace {
    constexpr std::size_t PHT_SIZE = 512;
    constexpr std::size_t GHR_SIZE = 32;
    constexpr std::size_t DATA_PRECISION = 8;

    // Define theta for perceptron
    constexpr int PERCEPTRON_THETA = std::floor(1.93 * GHR_SIZE + 14);

    std::map<O3_CPU*, std::array<champsim::msl::fwcounter<DATA_PRECISION>, PHT_SIZE>> perceptron_table;
    std::map<O3_CPU*, std::bitset<GHR_SIZE>> global_history;
}

void O3_CPU::initialize_branch_predictor() {}

uint8_t O3_CPU::predict_branch(uint64_t ip)
{
    auto hash = ip % PHT_SIZE;
    auto value = perceptron_table[this][hash];
    return value.value() >= (value.maximum / 2);
}

void O3_CPU::last_branch_result(uint64_t ip, uint64_t branch_target, uint8_t taken, uint8_t branch_type)
{
    auto hash = ip % PHT_SIZE;
    perceptron_table[this][hash] += taken ? 1 : -1;

    // Update global history
    global_history[this] <<= 1;
    global_history[this][0] = taken;
}


