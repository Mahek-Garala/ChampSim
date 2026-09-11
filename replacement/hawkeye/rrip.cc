#include "rrip.h"

void update_rrpv(std::vector<int>& rrpv, std::size_t way, Classification cls, bool is_hit)
{
    if(cls == Classification::CACHE_FRIENDLY && is_hit)
    {
        rrpv[way] = 0;
    }
    else if(cls == Classification::CACHE_AVERSE && !is_hit)
    {
        rrpv[way] = 7;
    }
    else if(cls == Classification::CACHE_FRIENDLY && !is_hit)
    {
        rrpv[way] = 0 ;
        // aging to other
        for(std::size_t i=0 ; i<rrpv.size() ;i++)
        {
            if(i!=way && rrpv[i] < 6)
                rrpv[i]++;
        }
    }
    else if(cls == Classification::CACHE_AVERSE && is_hit)
    {
        rrpv[way] = 7 ;
    }


}

std::size_t find_victim(std::vector<int>& rrpv)
{
    std::size_t n = rrpv.size();
    if(n==0)// no valid victim if set has no ways.
        return 0;
    std::size_t toBeEvicted = n ; 
    while(true)
    {
    
        for(std::size_t i=0 ; i<n ; i++)
        {
            if(rrpv[i] == 7)
            {
                return i;
            }
        }
       // find no victim yet so aging all
        for(std::size_t i=0;i<n;i++)
        {
            rrpv[i]++ ;
        }
    
    }
    return toBeEvicted ;

}
