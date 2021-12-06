#ifndef __AbstractEnvironmentRaw_h__
#define __AbstractEnvironmentRaw_h__


#include "ap_global1.h"
//#include "oct.h"
#include "box.h"
#include "pk.h"
#include "t1p.h"
#include "Layer.h"
#include "pkeq.h"
#include "AbstractDomainEnum.h"

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

    AbstractEnvironmentRaw(unsigned numberOfLayers, Layer **layers, int domainType) {
        _numberOfLayers = numberOfLayers;
        _layers = layers;

        _manager = NULL;

        allocate(domainType);
    }

private:


    void allocate(int domainType) {
        if (domainType == ABSTRACT_DOMAIN_BOX) {
            std::cout << "using box (domain type 0)" << std::endl;
            _manager = box_manager_alloc();
        }
        else if(domainType == ABSTRACT_DOMAIN_OCT) {
            std::cout << "using oct (domain type 1)" << std::endl;
            //_manager = oct_manager_alloc();
            std::cout << "domain type oct used but not implemented, use another domain" << std::endl;
        }
        else if(domainType == ABSTRACT_DOMAIN_POLY) {
            std::cout << "using pk/t1p (domain type 2)" << std::endl;
            _manager = pk_manager_alloc(1);
        }
        else if(domainType == ABSTRACT_DOMAIN_ZONOTOPE) {
            std::cout << "using pk/t1p (domain type 2)" << std::endl;
            _manager = t1p_manager_alloc();
        }
        else {
            std::cout << "domain type " << domainType << " not found, defaulting to box." << std::endl;
            _manager = box_manager_alloc();
        }

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