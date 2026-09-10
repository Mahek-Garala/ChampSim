#include "optgen.h"

OPTgen::OPTgen(std::size_t num_sets , std::size_t associativity , std::size_t history_multiplier) 
{
    this->history_len = history_multiplier * associativity ;
    this->assoc = associativity ;

    addr.resize(num_sets);
    occVec.resize(num_sets);
}

bool OPTgen::access(std::size_t set_idx , std::uint64_t address) 
{
    if(set_idx >= addr.size())
    {
        return false;
    }
    if(addr[set_idx].size() >= history_len)
    {
        addr[set_idx].pop_front();
        occVec[set_idx].pop_front();
    }
    bool opt_hit = false;
    std::size_t last = findLast(set_idx,address);
    if(last != history_len) // we able to find last (else have to return false)
    {
        std::size_t max_occupancy = check_usageInterval(set_idx,last);
        if(max_occupancy < assoc)
        {
            incr_usageInterval(set_idx , last);
            opt_hit = true;
        }
    }
    addr[set_idx].push_back(address);
    occVec[set_idx].push_back(0);
    return opt_hit ;    
}
std::size_t OPTgen::findLast(std::size_t set_idx,std::uint64_t address)
{
    
    std::size_t i = addr[set_idx].size();
    std::size_t last = 0 ;
    while(i > 0)
    {
        last = i-1 ; 
        if(addr[set_idx][last] == address)
        {
            return last;
        }
        i--;
    }
    return history_len ;
}

std::size_t OPTgen::check_usageInterval(std::size_t set_idx , std::size_t last)
{
    std::size_t i = addr[set_idx].size() - 1;
    std::size_t max_occupancy = 0 ;
    while(true)// if i==last==0 then i-- become -ve but i is of type size_t , so i become too big number 
    {
        max_occupancy = std::max(max_occupancy ,static_cast<std::size_t>(occVec[set_idx][i])) ; 
        if(last == i)
            break;
        i--;
    }
    return max_occupancy ;
}
void OPTgen::incr_usageInterval(std::size_t set_idx  ,std::size_t last)
{
    std::size_t i = addr[set_idx].size() - 1 ;
    while(true)
    {
        occVec[set_idx][i]++ ;
        if(last == i)
            break ;
        i--;
    }

}