#include "extraFunctions.hpp"

double mymax(double a, double b)
{
    if(a >= b) return a;
    else return b;
}

double mymin(double a, double b)
{
    if(a <= b) return a;
    else return b;
}

int checkArray(int m_gametype, int arrIndex, int triangleIndex){
    if(m_gametype == 1){
        if(1000 == arrIndex - 4)
            return false;
    }

    else if(m_gametype == 2){
    	if(1500 == arrIndex)
            return false;
    }

    return true;
}
