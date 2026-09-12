// hawkeye.cc
#include "hawkeye.h"
#include <algorithm>

// TODO: implement find_victim / replacement_cache_fill /
// update_replacement_state here, delegating to optgen / predictor / rrip.h as
// described in hawkeye.h above.
hawkeye::hawkeye(CACHE* cache)
    : hawkeye(cache, cache->NUM_SET, cache->NUM_WAY)
{
}
hawkeye::hawkeye(CACHE* cache , long sets , long ways)
        : replacement(cache), optgen(sets, ways), predictor(), 
        rrpv(sets, std::vector<int>(ways, 0)), lastPC(sets)
{
}

long hawkeye::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set, const champsim::cache_block* current_set, champsim::address ip,champsim::address full_addr, access_type type)
{
    return ::find_victim(rrpv[set]);
}

void hawkeye::replacement_cache_fill(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip, champsim::address victim_addr,access_type type)
{
    bool is_friendly = predictor.predict(ip.to<uint64_t>());
    Classification cls = is_friendly ? Classification::CACHE_FRIENDLY : Classification::CACHE_AVERSE;
    update_rrpv(rrpv[set],way,cls,false); // miss + cls

}

void hawkeye::update_replacement_state(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip, champsim::address victim_addr,access_type type, uint8_t hit)
{
    uint64_t block_addr = full_addr.to<uint64_t>() >> 6;// 64B cache block address from full Byte address

    bool opt_hit = optgen.access(set,block_addr);
    if(lastPC[set].find(block_addr) != lastPC[set].end() )
    {
        uint64_t prevPC = lastPC[set][block_addr] ;
        predictor.train(prevPC,opt_hit);
    }
    bool is_friendly = predictor.predict(ip.to<uint64_t>());
    Classification cls = is_friendly ? Classification::CACHE_FRIENDLY : Classification::CACHE_AVERSE;
    if(hit) // cuz, miss will handle at replacement time
    {
        update_rrpv(rrpv[set],way,cls,true); // hit + cls
    }
    
    lastPC[set][block_addr] = ip.to<uint64_t>() ; // now update last pc to current as curr become last now for next same pc access
}