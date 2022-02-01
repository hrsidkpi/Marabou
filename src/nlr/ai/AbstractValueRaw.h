#ifndef __AbstractValueRaw_h__
#define __AbstractValueRaw_h__


#include "ap_global1.h"
#include "box.h"
#include "AbstractEnvironmentRaw.h"
#include "RValueCopy.h"
#include <iostream>


namespace NLR {

class AbstractValueRaw {

public:

    ~AbstractValueRaw() {
        //ap_abstract1_clear(_manager->_manager, _ap_value);
        ap_abstract1_clear(_manager->_manager, _ap_value);
        delete _ap_value;
        _ap_value = NULL;
    }

    ap_abstract1_t *_ap_value;
    unsigned _layerIndex;

    AbstractValueRaw(NLR::AbstractEnvironmentRaw *manager, unsigned layerIndex) {
        _manager = manager;
        _layerIndex = layerIndex;
        _ap_value = NULL;
    }


    AbstractValueRaw *initLayer(unsigned layer, double **bounds)
    {
        unsigned layerSize = _manager->_layers[layer]->getSize();
        char **varNames = new char*[layerSize];

        ap_lincons1_array_t constraintArray = ap_lincons1_array_make(_manager->_env, layerSize*2);

        unsigned currentIndex = 0;
        for(unsigned i = 0; i < layerSize; i++) {


            varNames[i] = new char[30];
            sprintf(varNames[i], "x_%d_%d",layer, i);
            double lb = bounds[i][0];
            double ub = bounds[i][1];
            ap_linexpr1_t exprLb = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, i+1);
            ap_lincons1_t consLb = ap_lincons1_make(AP_CONS_SUPEQ, &exprLb, NULL);
            ap_lincons1_set_list(&consLb,
                AP_COEFF_S_INT, 1, varNames[i],
                AP_CST_S_DOUBLE, -lb,
                AP_END);
            ap_lincons1_array_set(&constraintArray, currentIndex, &consLb);
            currentIndex++;
            ap_linexpr1_t exprUb = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, i+1);
            ap_lincons1_t consUb = ap_lincons1_make(AP_CONS_SUPEQ, &exprUb, NULL);
            ap_lincons1_set_list(&consUb,
                AP_COEFF_S_INT, -1, varNames[i],
                AP_CST_S_DOUBLE, ub,
                AP_END);
            ap_lincons1_array_set(&constraintArray, currentIndex, &consUb);
            currentIndex++;
        }


        AbstractValueRaw *newVal = new AbstractValueRaw(_manager, 0);
        
        newVal->_ap_value = new ap_abstract1_t();

        ap_abstract1_t t = ap_abstract1_meet_lincons_array(_manager->_manager, false, _ap_value, &constraintArray);
        *(newVal->_ap_value) = t;

        ap_lincons1_array_clear(&constraintArray);

        for(unsigned i = 0; i < layerSize; i++)
            delete[] varNames[i];
        delete[] varNames;

        return newVal;
    }

    AbstractValueRaw *initAllLayers(double ***bounds) 
    {
        AbstractValueRaw *val = this;

        for(unsigned layer = 1; layer < _manager->_numberOfLayers; layer++) {
            AbstractValueRaw *tempVal = val;
            val = val->initLayer(layer, bounds[layer]);
            if(tempVal != nullptr && tempVal != this)
                delete tempVal;
        }

        return val;
    }

    void initFirstLayer(double **firstLayerBounds) {
        unsigned layerSize = _manager->_layers[0]->getSize();

        ap_lincons1_array_t constraintArray = ap_lincons1_array_make(_manager->_env, layerSize*2);

        char **varNames = new char*[layerSize];
        
        unsigned currentIndex = 0;
        for(unsigned i = 0; i < layerSize; i++) {
            varNames[i] = new char[30];
            sprintf(varNames[i], "x_%d_%d",0, i);
            double lb = firstLayerBounds[i][0];
            double ub = firstLayerBounds[i][1];

            ap_linexpr1_t exprLb = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, i+1);
            ap_lincons1_t consLb = ap_lincons1_make(AP_CONS_SUPEQ, &exprLb, NULL);
            ap_lincons1_set_list(&consLb,
                AP_COEFF_S_INT, 1, varNames[i],
                AP_CST_S_DOUBLE, -lb,
                AP_END);
            ap_lincons1_array_set(&constraintArray, currentIndex, &consLb);
            currentIndex++;
            ap_linexpr1_t exprUb = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, i+1);
            ap_lincons1_t consUb = ap_lincons1_make(AP_CONS_SUPEQ, &exprUb, NULL);
            ap_lincons1_set_list(&consUb,
                AP_COEFF_S_INT, -1, varNames[i],
                AP_CST_S_DOUBLE, ub,
                AP_END);
            ap_lincons1_array_set(&constraintArray, currentIndex, &consUb);
            currentIndex++;
        }
        

        _ap_value = new ap_abstract1_t();

        ap_abstract1_t t = ap_abstract1_of_lincons_array(_manager->_manager, _manager->_env, &constraintArray);

        *_ap_value = ap_abstract1_copy(_manager->_manager, &t);

        ap_abstract1_clear(_manager->_manager, &t);
        
        ap_lincons1_array_clear(&constraintArray);

        for(unsigned i = 0; i < layerSize; i++)
            delete[] varNames[i];
        delete[] varNames;
        
    }

    void setLayerIndex(unsigned layerIndex) {
        _layerIndex = layerIndex;
    }


    AbstractValueRaw *performAffineTransformation(const double *weightsMatrix, double *bias) {

        unsigned currLayerSize = _manager->_layers[_layerIndex]->getSize();
        unsigned nextLayerSize = _manager->_layers[_layerIndex + 1]->getSize();

        ap_lincons1_array_t constraintArray = ap_lincons1_array_make(_manager->_env, nextLayerSize);
        
        char *varName = new char[20];
        char *jVarName = new char[20];
        
        for(unsigned i = 0; i < nextLayerSize; i++ ){
            ap_linexpr1_t expr = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, currLayerSize+1);
            ap_lincons1_t cons = ap_lincons1_make(AP_CONS_EQ, &expr, NULL);

            sprintf(varName, "x_%d_%d", _layerIndex+1, i);            

            ap_lincons1_set_list( &cons,
                                  AP_COEFF_S_INT, -1, varName,
                                  AP_CST_S_DOUBLE, bias[i],
                                  AP_END);
            for(unsigned j = 0; j < currLayerSize; j++) {
                //double weight = weightsMatrix[j*currLayerSize + i];
                double weight = weightsMatrix[j * nextLayerSize + i];
                sprintf(jVarName, "x_%d_%d", _layerIndex, j);

                ap_lincons1_set_list( &cons,
                                      AP_COEFF_S_DOUBLE, weight, jVarName,
                                      AP_END);
            }            
            ap_lincons1_array_set(&constraintArray, i, &cons);
        }

        AbstractValueRaw *newVal = new AbstractValueRaw(_manager, _layerIndex + 1);
        
        newVal->_ap_value = new ap_abstract1_t();

        ap_abstract1_t t = ap_abstract1_meet_lincons_array(_manager->_manager, false, _ap_value, &constraintArray);
        *(newVal->_ap_value) = t;

        ap_lincons1_array_clear(&constraintArray);


        delete[] varName;
        delete[] jVarName;
        return newVal;
    }


    AbstractValueRaw *performReluOnNeuron(unsigned neuron) {
        char n1[20];
        char n2[20];
        sprintf(n1, "x_%d_%d", _layerIndex, neuron);
        sprintf(n2, "x_%d_%d", _layerIndex+1, neuron);

        // === Active case: neuron is positive and unchanged ===
        //Meet with condition
        ap_lincons1_array_t activeConstraintArray = ap_lincons1_array_make(_manager->_env, 1 );
        ap_linexpr1_t activeExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, 1 );
        ap_lincons1_t activeCons = ap_lincons1_make( AP_CONS_SUPEQ, &activeExpr, NULL );
        ap_lincons1_set_list( &activeCons,
                                AP_COEFF_S_INT, 1, n1,
                                AP_END );
        ap_lincons1_array_set( &activeConstraintArray, 0, &activeCons );        
        ap_abstract1_t activeAV = ap_abstract1_meet_lincons_array(_manager->_manager, false, _ap_value, &activeConstraintArray);
        ap_lincons1_array_clear(&activeConstraintArray);   

        //Apply transformation for active condition (x=x)
        activeConstraintArray = ap_lincons1_array_make( _manager->_env, 1 );
        activeExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, 1);
        activeCons = ap_lincons1_make( AP_CONS_EQ, &activeExpr, NULL );     
        ap_lincons1_set_list( &activeCons,
                                AP_COEFF_S_INT, 1, n1,
                                AP_COEFF_S_INT, -1, n2,
                                AP_END );
        ap_lincons1_array_set( &activeConstraintArray, 0, &activeCons );        
        ap_abstract1_t  activeAV_trans = ap_abstract1_meet_lincons_array(_manager->_manager, false, &activeAV, &activeConstraintArray);
        ap_abstract1_clear(_manager->_manager, &activeAV);
        ap_lincons1_array_clear(&activeConstraintArray);      


        // === Inactive case: neuron is negative and becomes zero ===
        //Meet with condition
        ap_lincons1_array_t inactiveConstraintArray = ap_lincons1_array_make( _manager->_env, 1 );
        ap_linexpr1_t inactiveExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, 1 );
        ap_lincons1_t inactiveCons = ap_lincons1_make( AP_CONS_SUPEQ, &inactiveExpr, NULL );
        ap_lincons1_set_list( &inactiveCons,
                                AP_COEFF_S_INT, -1, n1,
                                AP_END );
        ap_lincons1_array_set( &inactiveConstraintArray, 0, &inactiveCons );        
        ap_abstract1_t inactiveAV = ap_abstract1_meet_lincons_array(_manager->_manager, false, _ap_value, &inactiveConstraintArray);
        ap_lincons1_array_clear(&inactiveConstraintArray);     

        //Apply transformation for inactive condition (x=x)
        inactiveConstraintArray = ap_lincons1_array_make( _manager->_env, 1 );
        inactiveExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, 1 );
        inactiveCons = ap_lincons1_make( AP_CONS_EQ, &inactiveExpr, NULL );
        ap_lincons1_set_list( &inactiveCons,
                                AP_COEFF_S_INT, 1, n2,
                                AP_END );
        ap_lincons1_array_set( &inactiveConstraintArray, 0, &inactiveCons );        
        ap_abstract1_t inactiveAV_trans;
        if(ap_abstract1_is_bottom(_manager->_manager, &inactiveAV)) {
            inactiveAV_trans = ap_abstract1_bottom(_manager->_manager, _manager->_env);
        }
        else {
            inactiveAV_trans = ap_abstract1_meet_lincons_array(_manager->_manager, false, &inactiveAV, &inactiveConstraintArray);
        }
        ap_abstract1_clear(_manager->_manager, &inactiveAV);
        ap_lincons1_array_clear(&inactiveConstraintArray);

        // Join the results
        AbstractValueRaw *newVal = new AbstractValueRaw(_manager, _layerIndex);
        newVal->_ap_value = new ap_abstract1_t();
        *(newVal->_ap_value) = ap_abstract1_join( _manager->_manager, false, &activeAV_trans, &inactiveAV_trans);

        //cleanup and return
        ap_abstract1_clear(_manager->_manager, &inactiveAV_trans);
        ap_abstract1_clear(_manager->_manager, &activeAV_trans);
        return newVal;
    }

    AbstractValueRaw *performRelu() {
        unsigned layerSize = _manager->_layers[_layerIndex]->getSize();

        AbstractValueRaw *val = this;

        for(unsigned n = 0; n < layerSize; n++) {
            AbstractValueRaw *tempVal = val;
            val = val->performReluOnNeuron(n);
            if(tempVal != nullptr && tempVal != this)
                delete tempVal;
        }

        val->setLayerIndex(_layerIndex+1);
        return val;
    }


private:
    AbstractEnvironmentRaw *_manager;
    

};

}

#endif
