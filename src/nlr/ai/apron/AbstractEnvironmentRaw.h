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
        
        std::cout << "deleting abstract environment" << std::endl;

        ap_environment_free(_env);
        ap_manager_free(_manager);
    }

    unsigned currentLayer;
    unsigned _numberOfLayers;
    Layer* *_layers;

    ap_manager_t *_manager;
    ap_environment_t *_env;

    AbstractEnvironmentRaw(unsigned numberOfLayers, Layer **layers, int domainType) {
        _numberOfLayers = numberOfLayers;
        _layers = layers;

        _manager = NULL;

        allocateManager(domainType);
        allocateFirstLayer();
        currentLayer = 0;
    }

    void prepareForNextLayer() {

        if(currentLayer != 0) {
            removeLayer(currentLayer-1);
        }
        addLayer(currentLayer+1);
    }

    void finalize() {
        ASSERT(currentLayer == _numberOfLayers-1);
        removeLayer(currentLayer-1);
    }

private:


    void allocateManager(int domainType) {
        if (domainType == ABSTRACT_DOMAIN_BOX) {
            std::cout << "using box" << std::endl;
            _manager = box_manager_alloc();
        }
        else if(domainType == ABSTRACT_DOMAIN_OCT) {
            std::cout << "using oct" << std::endl;
            //_manager = oct_manager_alloc();
            std::cout << "domain type oct used but not implemented, use another domain" << std::endl;
        }
        else if(domainType == ABSTRACT_DOMAIN_POLY) {
            std::cout << "using poly (pk)" << std::endl;
            _manager = pk_manager_alloc(1);
        }
        else if(domainType == ABSTRACT_DOMAIN_ZONOTOPE) {
            std::cout << "using t1p" << std::endl;
            _manager = t1p_manager_alloc();
        }
        else {
            std::cout << "domain type " << domainType << " not found, defaulting to box." << std::endl;
            _manager = box_manager_alloc();
        }
    }

    void allocateFirstLayer() {
        unsigned numberOfVariables = _layers[0]->getSize();


        char **variableNames = new char *[numberOfVariables];
        for ( unsigned j = 0; j < _layers[0]->getSize(); ++j )
        {
            variableNames[j] = new char[20];
            sprintf(variableNames[j], "x_%d_%d", 0, j);
            std::cout << "adding variable " << variableNames[j] << std::endl;
        }
        
        _env = ap_environment_alloc(NULL, 0, (void **)&variableNames[0], numberOfVariables );
        
        for(unsigned i = 0; i < _layers[0]->getSize(); ++i) 
        {
            delete[] variableNames[i];
        }
        delete[] variableNames;

    }

    void removeLayer(unsigned layerIndex) {
        unsigned numberOfVariables = _layers[layerIndex]->getSize();

        char **variableNames = new char *[numberOfVariables];

        for ( unsigned j = 0; j < numberOfVariables; ++j )
        {
            variableNames[j] = new char[20];
            sprintf(variableNames[j], "x_%d_%d", layerIndex, j);
        }

        ap_environment_t *temp = _env;
        _env = ap_environment_remove(_env, (void **)&variableNames[0], numberOfVariables);
        ap_environment_free(temp);

        for(unsigned i = 0; i < numberOfVariables; ++i) 
        {
            delete[] variableNames[i];
        }
        delete[] variableNames;
    }

    void addLayer(unsigned layerIndex) {
        unsigned numberOfVariables = _layers[layerIndex]->getSize();

        char **variableNames = new char *[numberOfVariables];

        for ( unsigned j = 0; j < numberOfVariables; ++j )
        {
            variableNames[j] = new char[20];
            sprintf(variableNames[j], "x_%d_%d", layerIndex, j);
        }

        ap_environment_t *temp = _env;
        _env = ap_environment_add(_env, NULL, 0, (void **)&variableNames[0], numberOfVariables);
        ap_environment_free(temp);

        for(unsigned i = 0; i < numberOfVariables; ++i) 
        {
            delete[] variableNames[i];
        }
        delete[] variableNames;
    }


    

};


}


#endif