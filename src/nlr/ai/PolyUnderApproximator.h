#ifndef __PolyAbstractApproximator_h__
#define __PolyAbstractApproximator_h__


#include "ap_global1.h"
//#include "oct.h"
#include "box.h"
#include "pk.h"
#include "t1p.h"
#include "Layer.h"
#include "pkeq.h"
#include "AbstractDomainEnum.h"
#include "Halfspace.h"

namespace NLR {

class PolyAbstractApproximator {

public:

    ~PolyAbstractApproximator() {
        
    }

    PolyAbstractApproximator(Halfspace *halfspaces) {
        this->halfspaces = halfspaces;
        dimension = 0; //len(halfspaces)
    }

    void applyAffineTransformation(MATRIX linear_part, VECTOR translation_part) {

    }

    void applyRelu() {

    }

    MATRIX getBounds() {

    }

    bool containsPoint(VECTOR vertex) {
        return true;
    }

    double getHyperVolume() {
        return 0;
    }


private:

    Halfspace *halfspaces;
    unsigned dimension;

};


}


#endif