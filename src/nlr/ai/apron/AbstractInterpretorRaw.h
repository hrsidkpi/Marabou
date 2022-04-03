#ifndef __AbstractInterpretorRaw_h__
#define __AbstractInterpretorRaw_h__


#include "ap_global1.h"
#include "oct.h"
#include "AbstractValueRaw.h"
#include "Layer.h"
#include "AbstractEnvironmentRaw.h"
#include "AbstractDomainEnum.h"
#include "polyhedron/Polyhedron.h"
//#include "armadillo.h"

//using namespace arma;

namespace NLR {

class AbstractInterpretorRaw {

public:

    ~AbstractInterpretorRaw() {
        delete _currentAV;
        if(_useUnderApprox)
            delete _currentUnderAV;
        delete _env;
    }

    void init(unsigned numberOfLayers, Layer** layerPointers, int domainType) {
        _env = new AbstractEnvironmentRaw(numberOfLayers, layerPointers, domainType);
    }

    void setUseUnderApproximation(bool useUnderApprox) {
        _useUnderApprox = useUnderApprox;
    }

    void setInitialBounds(double ***bounds, bool initAllLayers) {

        (void) initAllLayers;

        _currentAV = new AbstractValueRaw(_env, 0);

        
        std::cout << "initing" << std::endl;

        _currentAV->initLayer(0, bounds[0]);

        std::cout << "init worked" << std::endl;

        /**
        if(initAllLayers) {
            AbstractValueRaw *tempVal = _currentAV;
            _currentAV = _currentAV->initAllLayers(bounds);
            delete tempVal;
        }
        **/

        if(_useUnderApprox) {
            unsigned dim = _env->_layers[0]->getSize();
            std::list<NLR::Halfspace*> halfspaces = {};
            for(unsigned i = 0; i < dim; i++) {
                double *w1 = new double[dim];
                double *w2 = new double[dim];
                for(unsigned d = 0; d < dim; d++) {
                    if(d == i) {
                        w1[i] = 1;
                        w2[i] = -1;
                    }
                    else {
                        w1[i] = 0;
                        w2[i] = 0;
                    }
                }
                halfspaces.push_back(new Halfspace(dim, w1, -bounds[0][i][1])); // x - UB < 0     ==>      x<UB
                halfspaces.push_back(new Halfspace(dim, w2, bounds[0][i][0])); // -x + LB < 0     ==>      x>LB
            }         
            std::cout << "creating the new polyhedron" << std::endl;
            _currentUnderAV = new NLR::Polyhedron(dim, halfspaces);
        }
    }

    void propagate() {
        _currentAV->setLayerIndex(0);
        for(unsigned i = 0; i < _env->_numberOfLayers-1; i++) {
            Layer *layer = _env->_layers[i+1];

            _env->currentLayer = i;
            _env->prepareForNextLayer();
            _currentAV->setLayerIndex(i);
            _currentAV->updateToEnv();

            if(layer->getLayerType() == Layer::WEIGHTED_SUM) {
                const double *weights = layer->getWeightMatrix(i);
                double *biases = layer->getBiases();

                _currentAV->performAffineTransformation(weights, biases);

                if(_useUnderApprox) {
                    arma::mat linear_trans_mat(weights, _env->_layers[i]->getSize(), _env->_layers[i+1]->getSize());
                    arma::mat translate_trans_mat(biases, _env->_layers[i+1]->getSize(), 1);

                    linear_trans_mat = linear_trans_mat.t(); //Because armadillo matrices are transposed to the way marabou saves them

                    _currentUnderAV->applyAffineTransformation(linear_trans_mat, translate_trans_mat);
                }
            }

            if(layer->getLayerType() == Layer::RELU) {
                _currentAV->performRelu();

                if(_useUnderApprox)
                    _currentUnderAV->applyRelu();
            }


        }    
        
        _env->currentLayer = _env->_numberOfLayers-1;
        _env->finalize();  
    }

    void printCurrentBounds() {
        for(unsigned i = 0; i < _env->_numberOfLayers; i++) {
            Layer *layer = _env->_layers[i];
            for(unsigned neuron = 0; neuron < layer->getSize(); neuron++) {
                char varname[20];
                sprintf(varname, "x_%d_%d", i,  neuron);
                ap_interval_t *bounds = ap_abstract1_bound_variable(getEnvironment()->_manager, &(getCurrentAV()->_ap_value), const_cast<char *>(varname));
                double lb = bounds->inf->val.dbl;
                double ub = bounds->sup->val.dbl;
                std::cout<<"======================== " << varname << " (" << i << ", " << neuron << "): " << lb <<", " << ub << " ============================" << std::endl;
                ap_interval_free(bounds);
            }
        }
    }

    void printCurrentAv() {
        if(_env == NULL) {
            std::cout << "Error: _env is NULL in AbstractInterpretor.h::printCurrentAv()" << std::endl;
        }
        if(_env->_manager == NULL) {
            std::cout << "Error: _env->_manager is NULL in AbstractInterpretor.h::printCurrentAv()" << std::endl;
        }
        if(_currentAV == NULL) {
            std::cout << "Error: _curretntAV is NULL in AbstractInterpretor.h::printCurrentAv()" << std::endl;
        }
        ap_abstract1_fprint(stdout, _env->_manager, &(_currentAV->_ap_value));

        if(_useUnderApprox)
            _currentUnderAV->print();
        std::cout << std::endl;
    }

    AbstractValueRaw *getCurrentAV() {
        return _currentAV;
    }

    AbstractEnvironmentRaw *getEnvironment() 
    {
        return _env;
    }



private:

    AbstractEnvironmentRaw *_env;
    AbstractValueRaw *_currentAV;
    NLR::Polyhedron *_currentUnderAV;

    bool _useUnderApprox;


};


}


#endif