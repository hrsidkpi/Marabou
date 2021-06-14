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
        free(_env);
        free(_currentAV);
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

        _env = new AbstractEnviroment(numberOfLayers, layerSizes, weights, bias);
        std::cout <<"Done initializing AbstractInterpretor with " << _env->_numberOfLayers << " layers. " << std::endl;
    }

    void setInitialBounds(double **bounds) {
        std::cout << "Setting initial bounds " << std::endl;
        _currentAV = new AbstractValue(_env, 0);
        _currentAV->initAsFirstLayer(bounds);
        printCurrentAv();
        
    }

    void propagate() {
        for(unsigned i = 0; i < _env->_numberOfLayers-1; i++) {
            std::cout << "Propagating... Layer " << i << " to " << i+1 << std::endl;
            _currentAV = _currentAV->performAffineTransformation(_env->_weights[i], _env->_bias[i]);
            std::cout << "After affine: " << std::endl;
            printCurrentAv();
            _currentAV = _currentAV->performRelu();
            std::cout << "After relu: " << std::endl;
            printCurrentAv();
            std::cout << "\n\n" << std::endl;
        }
        
    }

    void printCurrentAv() {
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