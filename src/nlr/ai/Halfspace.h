#ifndef __Halfspace_h__
#define __Halfspace_h__


#include "ap_global1.h"
//#include "oct.h"
#include "box.h"
#include "pk.h"
#include "t1p.h"
#include "Layer.h"
#include "pkeq.h"
#include "AbstractDomainEnum.h"

namespace NLR {

class Halfspace {

public:


    ~Halfspace() {
        
    }

    Halfspace() {
        
    }

    bool containsVertex(std::list<double> vertex) {
        return true;
    }

    bool seperatorContainsVertex(std::list<double> vertex) {
        return true;
    }

    void applyAffineTransformation(MATRIX transformation_linear_part, VECTOR transformation_translate_part) {

    }


private:


    void *weights; // a 2 dimentional weights matrix
    double bias; 

};


}


#endif