#ifndef __AbstractEnviroment_h__
#define __AbstractEnviroment_h__


#include "ap_global1.h"
#include "box.h"

namespace NLR {

class AbstractEnviroment {

public:


    ~AbstractEnviroment() {

        std::cout << "Deleting weights...\n";
        for(unsigned i = 0; i < _numberOfLayers-1; i++) {
            for(unsigned j = 0; j < _layerSizes[i]; j++) {
                delete[] _weights[i][j];
            }
            delete[] _weights[i];
        }
        delete[] _weights;

        std::cout << "Deleting biases...\n";
        for(unsigned i = 0; i < _numberOfLayers-1; i++) {
            delete[] _bias[i];
        }
        delete[] _bias;

        delete[] _layerSizes;
        delete _manager;
        delete _env;
        
    }

    unsigned _numberOfLayers;
    unsigned *_layerSizes;

    ap_manager_t *_manager;
    ap_environment_t *_env;

    double ***_weights;
    double **_bias;

    AbstractEnviroment(unsigned numberOfLayers, unsigned *layerSizes, double ***weights, double **bias) {
        _numberOfLayers = numberOfLayers;
        _layerSizes = layerSizes;
        _weights = weights;
        _bias = bias;

        std::cout << "Allocating abstract enviroment " << std::endl;
        allocate();
        std::cout << "Done allocating abstract enviroment " << std::endl;
    }

private:


    void allocate() {
        _manager = box_manager_alloc();

        // Count the total number of variables
        unsigned totalNumberOfVariables = _layerSizes[0];
        for ( unsigned i = 1; i < _numberOfLayers; ++i )
            totalNumberOfVariables += _layerSizes[i] * 2;


        // Allocate the array
        char **variableNames = new char *[totalNumberOfVariables];

        // Populate the array
        unsigned counter = 0;
        for ( unsigned i = 0; i < _numberOfLayers; ++i )
        {
            for ( unsigned j = 0; j < _layerSizes[i]; ++j )
            {
                variableNames[counter] = &((new char[20])[0]);
                sprintf(variableNames[counter], "x_%d_a_%d", i, j);
                ++counter;
                if(i>0) {
                    variableNames[counter] = &((new char[20])[0]);
                    sprintf( variableNames[counter], "x_%d_%d", i, j);
                    ++counter;
                }
            }
        }

        
        _env = ap_environment_alloc(NULL, 0, (void **)&variableNames[0], totalNumberOfVariables );
        
    }

};


}


#endif