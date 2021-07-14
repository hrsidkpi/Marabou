#ifndef __AbstractEnvironmentRaw_h__
#define __AbstractEnvironmentRaw_h__


#include "ap_global1.h"
#include "oct.h"
#include "t1p.h"
#include "Layer.h"

namespace NLR {

class AbstractEnvironmentRaw {

public:


    ~AbstractEnvironmentRaw() {
        delete[] _layers;
        
        ap_environment_free2(_env);
        ap_manager_free(_manager);
    }

    unsigned _numberOfLayers;
    Layer* *_layers;

    ap_manager_t *_manager;
    ap_environment_t *_env;

    AbstractEnvironmentRaw(unsigned numberOfLayers, Layer **layers) {
        _numberOfLayers = numberOfLayers;
        _layers = layers;

        allocate();
    }

private:


    void allocate() {
        _manager = t1p_manager_alloc();

        // Count the total number of variables
        unsigned totalNumberOfVariables = 0;
        for ( unsigned i = 0; i < _numberOfLayers; ++i )
            totalNumberOfVariables += _layers[i]->getSize();


        // Allocate the array
        char **variableNames = new char *[totalNumberOfVariables];

        // Populate the array
        unsigned counter = 0;
        for ( unsigned i = 0; i < _numberOfLayers; ++i )
        {
            for ( unsigned j = 0; j < _layers[i]->getSize(); ++j )
            {
                variableNames[counter] = &((new char[20])[0]);
                sprintf( variableNames[counter], "x_%d_%d", i, j);
                ++counter;
            }
            
        }

        
        _env = ap_environment_alloc(NULL, 0, (void **)&variableNames[0], totalNumberOfVariables );
        
        for(unsigned i = 0; i < counter; ++i) 
        {
            delete[] variableNames[i];
        }
        delete[] variableNames;

    }

};


}


#endif