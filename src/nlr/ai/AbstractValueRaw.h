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

    void initAsFirstLayer(double **firstLayerBounds) {
        if (_layerIndex != 0)
            std::cerr << "Called AbstractValue::initAsFirstLayer on non-first-layer" << std::endl;


        ap_lincons1_array_t constraintArray = ap_lincons1_array_make(_manager->_env, _manager->_layers[0]->getSize()*2);

        for(unsigned i = 0; i < _manager->_layers[0]->getSize(); i++) {
            

            char varName[20];
            sprintf(varName, "x_0_%d", i);


            ap_linexpr1_t exprLb = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, i+1);
            ap_lincons1_t consLb = ap_lincons1_make(AP_CONS_SUPEQ, &exprLb, NULL);
            ap_lincons1_set_list(&consLb,
                AP_COEFF_S_INT, 1, varName,
                AP_CST_S_DOUBLE, -firstLayerBounds[i][0],
                AP_END);
            ap_lincons1_array_set(&constraintArray, i*2, &consLb);


            ap_linexpr1_t exprUb = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, i+1);
            ap_lincons1_t consUb = ap_lincons1_make(AP_CONS_SUPEQ, &exprUb, NULL);
            ap_lincons1_set_list(&consUb,
                AP_COEFF_S_INT, -1, varName,
                AP_CST_S_DOUBLE, firstLayerBounds[i][1],
                AP_END);
            ap_lincons1_array_set(&constraintArray, i*2+1, &consUb);
        }

        _ap_value = new ap_abstract1_t();
        ap_abstract1_t t = ap_abstract1_of_lincons_array(_manager->_manager, _manager->_env, &constraintArray);
        *_ap_value = ap_abstract1_copy(_manager->_manager, &t);
        ap_abstract1_clear(_manager->_manager, &t);
        
        ap_lincons1_array_clear(&constraintArray);
        
    }


    AbstractValueRaw *performAffineTransformation(const double *weightsMatrix, double *bias) {

        unsigned currLayerSize = _manager->_layers[_layerIndex]->getSize();
        unsigned nextLayerSize = _manager->_layers[_layerIndex + 1]->getSize();

        ap_lincons1_array_t constraintArray = ap_lincons1_array_make(_manager->_env, nextLayerSize);
        
        char *varName = new char[20];
        
        for(unsigned i = 0; i < nextLayerSize; i++ ){
            ap_linexpr1_t expr = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, 1);
            ap_lincons1_t cons = ap_lincons1_make(AP_CONS_EQ, &expr, NULL);


            sprintf(varName, "x_%d_%d", _layerIndex+1, i);
            
            ap_lincons1_set_list( &cons,
                                  AP_COEFF_S_INT, -1, varName,
                                  AP_CST_S_DOUBLE, bias[i],
                                  AP_END);
            for(unsigned j = 0; j < currLayerSize; j++) {
                double weight = weightsMatrix[j*currLayerSize + i];
                char jVarName[20];
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
        return newVal;
    }


    AbstractValueRaw *performRelu() {
        
        char s1[20];
        char s2[20];

        unsigned layerSize = _manager->_layers[_layerIndex]->getSize();

        ap_abstract1_t currentAV = ap_abstract1_copy(_manager->_manager, _ap_value);

        for(unsigned i = 0; i < layerSize; ++i) {
            // Active case: neuron is positive and unchanged

            
            //Meet with condition
            ap_lincons1_array_t activeConstraintArray = ap_lincons1_array_make(_manager->_env, 1 );
            ap_linexpr1_t activeExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, i );
            ap_lincons1_t activeCons = ap_lincons1_make( AP_CONS_SUPEQ, &activeExpr, NULL );
            sprintf(s1, "x_%d_%d", _layerIndex, i);
            sprintf(s2, "x_%d_%d", _layerIndex+1, i);
            ap_lincons1_set_list( &activeCons,
                                    AP_COEFF_S_INT, 1, s1,
                                    AP_END );
            ap_lincons1_array_set( &activeConstraintArray, 0, &activeCons );

            ap_abstract1_t activeAV = ap_abstract1_meet_lincons_array(_manager->_manager, false, &currentAV, &activeConstraintArray);
            ap_lincons1_array_clear(&activeConstraintArray);


            //Apply transformation for active condition (x=x)
            activeConstraintArray = ap_lincons1_array_make( _manager->_env, 1 );
            activeExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, i);
            activeCons = ap_lincons1_make( AP_CONS_EQ, &activeExpr, NULL );

            ap_lincons1_set_list( &activeCons,
                                    AP_COEFF_S_INT, 1, s1,
                                    AP_COEFF_S_INT, -1, s2,
                                    AP_END );
            ap_lincons1_array_set( &activeConstraintArray, 0, &activeCons );

            ap_abstract1_t  activeAV1 = ap_abstract1_meet_lincons_array(_manager->_manager, false, &activeAV, &activeConstraintArray);
            ap_abstract1_clear(_manager->_manager, &activeAV);
            activeAV = activeAV1;
            ap_lincons1_array_clear(&activeConstraintArray);



            // Inactive case: neuron is negative and becomes zero
            sprintf(s1, "x_%d_%d", _layerIndex, i);
            sprintf(s2, "x_%d_%d", _layerIndex+1, i);
                
            //Meet with condition
            ap_lincons1_array_t inactiveConstraintArray = ap_lincons1_array_make( _manager->_env, 1 );
            ap_linexpr1_t inactiveExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, i );
            ap_lincons1_t inactiveCons = ap_lincons1_make( AP_CONS_SUPEQ, &inactiveExpr, NULL );
            ap_lincons1_set_list( &inactiveCons,
                                    AP_COEFF_S_INT, -1, s1,
                                    AP_END );
            ap_lincons1_array_set( &inactiveConstraintArray, 0, &inactiveCons );

            ap_abstract1_t inactiveAV = ap_abstract1_meet_lincons_array(_manager->_manager, false, &currentAV, &inactiveConstraintArray);
            ap_lincons1_array_clear(&inactiveConstraintArray);


            //Apply transformation for inactive condition (x=x)
            inactiveConstraintArray = ap_lincons1_array_make( _manager->_env, 1 );
            inactiveExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, i );
            inactiveCons = ap_lincons1_make( AP_CONS_EQ, &inactiveExpr, NULL );
            ap_lincons1_set_list( &inactiveCons,
                                    AP_COEFF_S_INT, 1, s2,
                                    AP_END );
            ap_lincons1_array_set( &inactiveConstraintArray, 0, &inactiveCons );

            ap_abstract1_t inactiveAV2;
            if(ap_abstract1_is_bottom(_manager->_manager, &inactiveAV)) {
                inactiveAV2 = ap_abstract1_bottom(_manager->_manager, _manager->_env);
            }
            else {
                inactiveAV2 = ap_abstract1_meet_lincons_array(_manager->_manager, false, &inactiveAV, &inactiveConstraintArray);
            }
            ap_abstract1_clear(_manager->_manager, &inactiveAV);
            ap_lincons1_array_clear(&inactiveConstraintArray);
            

            // Join the results
            ap_abstract1_clear(_manager->_manager, &currentAV);
            currentAV = ap_abstract1_join( _manager->_manager, false, &activeAV, &inactiveAV2);
            ap_abstract1_clear(_manager->_manager, &inactiveAV2);
            ap_abstract1_clear(_manager->_manager, &activeAV);
        }

        AbstractValueRaw *newVal = new AbstractValueRaw(_manager, _layerIndex+1);
        newVal->_ap_value = new ap_abstract1_t();
        *(newVal->_ap_value) = ap_abstract1_copy(_manager->_manager, &currentAV);
        ap_abstract1_clear(_manager->_manager, &currentAV);
        return newVal;
    }


private:
    AbstractEnvironmentRaw *_manager;
    

};

}

#endif