#ifndef __PolyAbstractApproximator_h__
#define __PolyAbstractApproximator_h__


#include "ap_global1.h"
//#include "oct.h"
#include "box.h"
#include "pk.h"
#include "Layer.h"
#include "pkeq.h"
#include "AbstractDomainEnum.h"
#include "Halfspace.h"
#include "armadillo.h"
#include <list>

using namespace arma;

namespace NLR {

class PolyAbstractApproximator {

public:

    ~PolyAbstractApproximator() {
        
    }

    PolyAbstractApproximator(std::list<Halfspace*> halfspaces) {
        this->halfspaces = halfspaces;
        dimension = halfspaces[0]->dim;
    }

    void applyAffineTransformation(mat linear_part, mat translation_part) {
        for(Halfspace *hs : halfspaces) {
            hs->applyAffineTransformation(linear_part, translation_part)
        }
        dimension = translation_part.n_rows;
    }

    void applyRelu() {
        for(unsigned d = 0; d < dimension; d++) {
            double *weights = new double[dimension];
            for(unsigned i = 0; i < dimension; i++) {
                if(i == d)
                    weights[i] = 1;
                else
                    weights[i] = 0;
            }
            halfspaces.push_front(new Halfsapce(dimension, weights, 0));
        }
    }

    mat getBounds() {
        return NULL;
    }

    bool containsPoint(VECTOR vertex) {
        bool found = 
    }

    double getHyperVolume() {
        return 0;
    }


private:

    std::list<Halfspace*> halfspaces;

    unsigned dimension;

};


}


#endif