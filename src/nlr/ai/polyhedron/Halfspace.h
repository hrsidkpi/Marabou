#ifndef __Halfspace_h__
#define __Halfspace_h__


#include "armadillo.h"
#include <algorithm>
#include <iterator>


//using namespace arma;

namespace NLR {

class Halfspace {

public:


    ~Halfspace() {
        delete[] weights;
    }

    Halfspace(unsigned dim, double *weights, double bias) {
        this->dim = dim;
        this->weights = weights;
        this->bias =  bias;
    }

    bool containsVertex(double *vertex) {
        double res = 0;
        for(unsigned i = 0; i < dim; i++) {
            res += vertex[i] * (this->weights[i]);
        }
        return res + this->bias < 0;
    }

    bool seperatorContainsVertex(double* vertex) {
        double res = 0;
        for(unsigned i = 0; i < dim; i++) {
            res += vertex[i] * this->weights[i];
        }
        return res + this->bias == 0;
    }

    bool applyAffineTransformation(arma::mat transformation_linear_mat, arma::mat transformation_translate_mat) {
        unsigned new_dim = transformation_linear_mat.n_rows;

        arma::mat weights_mat(&(this->weights[0]), dim, 1, false);

        arma::mat new_weights(new_dim, 1, arma::fill::zeros);
        
        bool has_sol = arma::solve(new_weights, transformation_linear_mat.t(), weights_mat, arma::solve_opts::no_approx);
        if(has_sol) {
            arma::mat new_offset_delta = new_weights.t() * transformation_translate_mat;
            
            double *new_weights_ptr = new_weights.memptr();

            delete[] this->weights;
            this->weights = new double[new_dim];

            for(unsigned i = 0; i < new_dim; ++i) {
                this->weights[i] = new_weights_ptr[i];
            }
            this->bias -= new_offset_delta(0,0);
            this->dim = new_dim;
            return true;
        }
        else {
            return false;
        }
    }

    void print() {
        std::cout << "halfpsace of dim " << dim << ":" << std::endl;

        bool added = false;

        for(unsigned i = 0; i < dim; i++) {
            if(!added) {
                if(weights[i] > 0) {
                    std::cout << weights[i] << "x" << i;
                    added = true;
                }
                if (weights[i] < 0) {
                    std::cout << "-" << -weights[i] << "x" << i; 
                    added = true;
                }
            }
            else {
                if(weights[i] > 0)
                    std::cout << " + " << weights[i] << "x" << i;
                if (weights[i] < 0)
                    std::cout << " - " << -weights[i] << "x" << i; 
            }
        }
        
        if(added && bias < 0)
            std::cout << " - " << -bias;
        else if(added) {
            std::cout << " + " << bias;
        }
        else
            std::cout << bias;
        std::cout << " < 0" << std::endl;
    }

    unsigned dim;

private:


    double *weights;
    double bias; 

};


}


#endif