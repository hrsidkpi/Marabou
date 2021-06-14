#ifndef __RValueCopy_h__
#define __RValueCopy_h__


#include "ap_global1.h"
#include "box.h"
#include <iostream>


namespace NLR {


static ap_abstract1_t *AV_addr(ap_abstract1_t val) {
    ap_abstract1_t *res = new ap_abstract1_t();
    *res = val;
    return res;
}

}


#endif