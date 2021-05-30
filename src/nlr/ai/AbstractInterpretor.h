#ifndef __AbstractInterpretor_h__
#define __AbstractInterpretor_h__


#include "ap_global1.h"
#include "box.h"
#include "AbstractValue.h"
#include "AbstractEnviroment.h"


namespace NLR {

class AbstractInterpretor {

public:


    void init(unsigned numberOfLayers, unsigned *layerSizes, double ***weights, double **bias) {
        _env = new AbstractEnviroment(numberOfLayers, layerSizes, weights, bias);
    }

    void setInitialBounds(double **bounds) {
        _currentAV = new AbstractValue(_env, 0);
        _currentAV->initAsFirstLayer(bounds);
    }

    void propagate() {
        for(unsigned i = 1; i < _env->_numberOfLayers; i++) {
            _currentAV = _currentAV->performAffineTransformation(_env->_weights[i], _env->_bias[i]);
            _currentAV = _currentAV->performRelu();
        }
    }

    void printCurrentAv() {
        ap_abstract1_fprint(stdout, _env->_manager, _currentAV->_ap_value);
    }


private:

    AbstractEnviroment *_env;
    AbstractValue *_currentAV;

};


}


#endif