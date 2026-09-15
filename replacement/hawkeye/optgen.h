#ifndef HAWKEYE_OPTGEN_H
#define HAWKEYE_OPTGEN_H

// #include<bits/stdc++.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <vector>

class OPTgen{
    public : 
    OPTgen(std::size_t num_sets , std::size_t associativity , std::size_t history_multiplier = 8);
    bool access(std::size_t set_idx , std::uint64_t address);

    private : 
    std::vector<std::deque<std::uint64_t>> addr;     // cuz ,per set
    std::vector<std::deque<int>> occVec;

    size_t history_len ;
    size_t assoc ;

    std::size_t findLast(std::size_t set_idx , std::uint64_t address);
    void incr_usageInterval(std::size_t set_idx  ,std::size_t last) ;
    std::size_t check_usageInterval(std::size_t set_idx ,std::size_t last ) ;

};

#endif