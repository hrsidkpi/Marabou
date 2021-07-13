#ifndef __AbstractInterpretor_h__
#define __AbstractInterpretor_h__


#include "ap_global1.h"
#include "box.h"
#include "AbstractValue.h"
#include "AbstractEnviroment.h"


namespace NLR {

class AbstractInterpretor {

public:

    ~AbstractInterpretor() {
        delete _currentAV;
        delete _env;
    }

    void init(unsigned numberOfLayers, unsigned *layerSizes, double ***weights, double **bias) {
        std::cout << "number of layers: " << numberOfLayers << "\n";

        std::cout << "layer sizes: [";
        for(unsigned i = 0; i < numberOfLayers; i++) {
            std::cout << layerSizes[i] << ", ";
        }
        std::cout << "]\n";

        std::cout << "weights: [\n";
        for(unsigned i = 0; i < numberOfLayers-1; i++) {
            std::cout << "[\n";
            for(unsigned j = 0; j < layerSizes[i]; j++) {
                std::cout << "  [";
                for(unsigned k = 0; k < layerSizes[i+1]; k++) {
                    std::cout << weights[i][j][k] << ", ";
                }
                std::cout << "]\n";
            }
            std::cout << "]\n\n";
        }
        std::cout << "]\n";

        std::cout << "bias: [\n";
        for(unsigned i = 0; i < numberOfLayers-1; i++) {
            std::cout << "[";
            for(unsigned j = 0; j < layerSizes[i]; j++) {
                std::cout << bias[i][j] << ", ";
            }
            std::cout << "]\n";
        }
        std::cout << "]\n" << std::flush;

        void *p = (void*) new AbstractEnviroment(numberOfLayers, layerSizes, weights, bias);
        _env = (AbstractEnviroment *) p;
        std::cout <<"Done initializing AbstractInterpretor with " << _env->_numberOfLayers << " layers. " << std::endl;
    }

    void setInitialBounds(double **bounds) {
        std::cout << "Setting initial bounds " << std::endl;
        _currentAV = new AbstractValue(_env, 0);
        _currentAV->initAsFirstLayer(bounds);        
    }

    void propagate() {
        for(unsigned i = 0; i < _env->_numberOfLayers-1; i++) {
            std::cout << "Propagating... Layer " << i << " to " << i+1 << std::endl;


            AbstractValue *temp = _currentAV;
            _currentAV = _currentAV->performAffineTransformation(_env->_weights[i], _env->_bias[i]);
            delete temp;

            temp = _currentAV;
            _currentAV = _currentAV->performRelu();
            delete temp;

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
        std::cout << "ap_value->ap0 is: " << _currentAV->_ap_value->abstract0 << std::endl;
        ap_abstract1_fprint(stdout, _env->_manager, _currentAV->_ap_value);
    }

    AbstractValue *getCurrentAV() {
        return _currentAV;
    }

    AbstractEnviroment *getEnviroment() 
    {
        return _env;
    }



private:

    AbstractEnviroment *_env;
    AbstractValue *_currentAV;
};


}


#endif