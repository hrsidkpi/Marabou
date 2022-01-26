#ifndef __Polyhedron_h__
#define __Polyhedron_h__

#include "Halfspace.h"
#include "armadillo.h"
#include <list>

//using namespace arma;

namespace NLR {

class Polyhedron {

public:


    ~Polyhedron() {
        std::list<NLR::Halfspace*>::iterator it;
        for(it = halfspaces.begin(); it != halfspaces.end(); ++it) {
            delete *it;
        }
        std::cout << "deleted polyhedron halfspaces" << std::endl;
        halfspaces.clear();
    }

    Polyhedron(unsigned dim, std::list<NLR::Halfspace*> halfspaces) {
        this->dim = dim;
        this->halfspaces = halfspaces;
    }

    bool containsVertex(double *vertex) {
        (void) vertex;
        std::list<NLR::Halfspace*>::iterator it;
        for(it = halfspaces.begin(); it != halfspaces.end(); ++it) {
            if(!(*it)->containsVertex(vertex)) {
                return false;
            }
        }
        return true;
    }

    void applyAffineTransformation(arma::mat transformation_linear_mat, arma::mat transformation_translate_mat) {
        std::list<NLR::Halfspace*> new_halfspaces {};
        for(auto& h : halfspaces) {
            bool has_sol = h->applyAffineTransformation(transformation_linear_mat, transformation_translate_mat);
            if(has_sol) new_halfspaces.push_back(h);
            else delete h;
            //Insert code to add tight constraints for the transformation image.
        }

        dim = transformation_translate_mat.n_rows;
        halfspaces = new_halfspaces;
    }

    void applyRelu() {
        for(unsigned d = 0; d < dim; ++d) {
            double *weights = new double[dim];
            for(unsigned dd = 0; dd < dim; ++dd) {
                if(dd == d) weights[dd] = -1;
                else weights[dd] = 0;
            }
            NLR::Halfspace *h = new NLR::Halfspace(dim, weights, 0);
            halfspaces.push_back(h);
        }
    }

    void print() {
        std::cout << "Polyhedron of dim " << dim << ":" << std::endl;
        for(const auto& h : halfspaces) h->print();
    }

    unsigned dim;

private:


    std::list<NLR::Halfspace*> halfspaces;

};


}


#endif