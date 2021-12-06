#ifndef __AbstractInterpretorRaw_h__
#define __AbstractInterpretorRaw_h__


#include "ap_global1.h"
#include "oct.h"
#include "AbstractValueRaw.h"
#include "Layer.h"
#include "AbstractEnvironmentRaw.h"
#include "/home/yarden/Desktop/research/Marabou/src/nlr/ai/AbstractDomainEnum.h"


namespace NLR {

class AbstractInterpretorRaw {

public:

    ~AbstractInterpretorRaw() {
        delete _currentAV;
        delete _env;
    }

    void init(unsigned numberOfLayers, Layer** layerPointers, int domainType) {
        _env = new AbstractEnvironmentRaw(numberOfLayers, layerPointers, domainType);
    }

    void setInitialBounds(double **bounds) {
        _currentAV = new AbstractValueRaw(_env, 0);
        _currentAV->initAsFirstLayer(bounds);

        printCurrentAv();
        printCurrentBounds();
    }

    void propagate() {
        for(unsigned i = 0; i < _env->_numberOfLayers-1; i++) {
            Layer *layer = _env->_layers[i+1];

            std::cout << "AI: propagating layer " << i << std::endl;

            //Save the last value for deleting later
            AbstractValueRaw *temp = _currentAV;

            if(layer->getLayerType() == Layer::WEIGHTED_SUM) {
                std::cout << "Performing affine transformation" << std::endl;
                _currentAV = _currentAV->performAffineTransformation(layer->getWeightMatrix(i), layer->getBiases());
            }

            if(layer->getLayerType() == Layer::RELU) {
                _currentAV = _currentAV->performRelu();
            }

            //Delete the previous (now unused) value
            delete temp;

            //print the bounds
            printCurrentAv();
            printCurrentBounds();
        }        
    }

    void printCurrentBounds() {
        for(unsigned i = 0; i < _env->_numberOfLayers; i++) {
            Layer *layer = _env->_layers[i];
            for(unsigned neuron = 0; neuron < layer->getSize(); neuron++) {
                char varname[20];
                sprintf(varname, "x_%d_%d", i,  neuron);
                ap_interval_t *bounds = ap_abstract1_bound_variable(getEnvironment()->_manager, getCurrentAV()->_ap_value, const_cast<char *>(varname));
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
            std::cout << "Error: _env is NULL in AbstractInterpretor.h::printCurrentAv()" << std::endl;
        }
        if(_currentAV == NULL) {
            std::cout << "Error: _env is NULL in AbstractInterpretor.h::printCurrentAv()" << std::endl;
        }
        if(_currentAV->_ap_value == NULL) {
            std::cout << "Error: _env is NULL in AbstractInterpretor.h::printCurrentAv()" << std::endl;
        }
        ap_abstract1_fprint(stdout, _env->_manager, _currentAV->_ap_value);
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
};


}


#endif