//
//  dkf_helpers.cpp
//  dkf
//
//  Created by nlogn on 3/18/17.
//  Copyright © 2017 nesl. All rights reserved.
//

#include "dkf_helpers.hpp"

#define LTHRESHOLD 2.220446049250313e-030

std::complex<double> zbound(std::complex<double> cnum)
{
    double real = cnum.real();
    double imag = cnum.imag();
    
    if (std::abs(real) < LTHRESHOLD) real = 0;
    if (std::abs(imag) < LTHRESHOLD) imag = 0;
    
    return std::complex<double>(real, imag);
}

double zbound(double num)
{
//    if (std::abs(num) < LTHRESHOLD) num = 0;
    
    return num;
}