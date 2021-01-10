#include "environmentoptions.h"

EnvironmentOptions::EnvironmentOptions()
{
    metrictype = CN_SP_MT_EUCL;
    allowsqueeze = false;
    allowdiagonal = true;
    cutcorners = false;
}

EnvironmentOptions::EnvironmentOptions(int AL, double HW, int BT, bool AS, bool AD, bool CC, int MT)
{
    algorithm = AL;
    hweight = HW;
    breakingties = BT;
    metrictype = MT;
    allowsqueeze = AS;
    allowdiagonal = AD;
    cutcorners = CC;
}

