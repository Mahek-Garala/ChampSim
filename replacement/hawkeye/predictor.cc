#include "predictor.h" 

HawkeyePredictor::HawkeyePredictor(std::size_t entries, int bits)
{
    counter_bits = bits;
    num_entries = entries;
    pred_counter = std::vector<int>(num_entries, 4); // give it a size and assigned with default value 4
}
 // cache access give PC also with address and set index.
void HawkeyePredictor::train(uint64_t pc, bool opt_hit)
{
    uint64_t hashed_pc = get_hashedIndex(pc);
    if(opt_hit)
    {
        if(pred_counter[hashed_pc] < 7)
            pred_counter[hashed_pc]++;
    }
    else
    {
        if(pred_counter[hashed_pc] > 0)
            pred_counter[hashed_pc]--;
    }

}
bool HawkeyePredictor::predict(uint64_t pc) const
{
    std::size_t hashed_pc = get_hashedIndex(pc);
    return (pred_counter[hashed_pc] & (1 << (counter_bits - 1))) != 0; // look at the MSB

}
int HawkeyePredictor::get_counter(uint64_t pc) const
{
    std::size_t hashed_pc = get_hashedIndex(pc);
    return pred_counter[hashed_pc];

}
std::size_t HawkeyePredictor::get_hashedIndex(uint64_t pc) const 
{
    uint64_t hashed_pc = pc ^ (pc >> 12);
    return hashed_pc & ((1ULL << 13) - 1); // cuz , send only lower 13 bits. -1 : cuz range is [0,8191]
}
