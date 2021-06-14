#ifndef __AbstractValue_h__
#define __AbstractValue_h__


#include "ap_global1.h"
#include "box.h"
#include "AbstractEnviroment.h"
#include "RValueCopy.h"
#include <iostream>


namespace NLR {

class AbstractValue {

public:


    ap_abstract1_t *_ap_value;
    unsigned _layerIndex;

    AbstractValue(NLR::AbstractEnviroment *manager, unsigned layerIndex) {
        _manager = manager;
        _layerIndex = layerIndex;
        _ap_value = NULL;
    }

    void initAsFirstLayer(double **firstLayerBounds) {
        if (_layerIndex != 0)
            std::cerr << "Called AbstractValue::initAsFirstLayer on non-first-layer" << std::endl;


        ap_lincons1_array_t constraintArray = ap_lincons1_array_make(_manager->_env, _manager->_layerSizes[0]*2);

        std::cout << "[" << firstLayerBounds[0][0] << ", " << firstLayerBounds[0][1] << "]" << std::endl;
        std::cout << "[" << firstLayerBounds[1][0] << ", " << firstLayerBounds[1][1] << "]" << std::endl;


        for(unsigned i = 0; i < _manager->_layerSizes[0]; i++) {
            

            char * varName = new char[20];
            sprintf(varName, "x_0_a_%d", i);

            std::cout << "Setting bounds for neuron " << varName << "(" << i  << ")" << std::endl; 

            std::cout << "Setting upper bound: " << std::flush;
            std::cout << -firstLayerBounds[i][0] << std::endl;
            ap_linexpr1_t exprLb = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, i+1);
            ap_lincons1_t consLb = ap_lincons1_make(AP_CONS_SUPEQ, &exprLb, NULL);
            ap_lincons1_set_list(&consLb,
                AP_COEFF_S_INT, 1, varName,
                AP_CST_S_DOUBLE, -firstLayerBounds[i][0],
                AP_END);
            ap_lincons1_array_set(&constraintArray, i*2, &consLb);


            std::cout << "Setting lower bound: " << std::flush;
            std::cout << firstLayerBounds[i][1] << std::endl;
            ap_linexpr1_t exprUb = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, i+1);
            ap_lincons1_t consUb = ap_lincons1_make(AP_CONS_SUPEQ, &exprUb, NULL);
            ap_lincons1_set_list(&consUb,
                AP_COEFF_S_INT, -1, varName,
                AP_CST_S_DOUBLE, firstLayerBounds[i][1],
                AP_END);
            ap_lincons1_array_set(&constraintArray, i*2+1, &consUb);
        }

        _ap_value = new ap_abstract1_t();
        *_ap_value = ap_abstract1_of_lincons_array(_manager->_manager, _manager->_env, &constraintArray);
        //ap_lincons1_array_clear(&constraintArray);
        
    }


    AbstractValue *performAffineTransformation(double **weights, double *bias) {
        (void) weights;
        (void) bias;
        
        std::cout << "affine on layer index " << _layerIndex << std::endl;
        unsigned currLayerSize = _manager->_layerSizes[_layerIndex];
        unsigned nextLayerSize = _manager->_layerSizes[_layerIndex + 1];

        ap_lincons1_array_t constraintArray = ap_lincons1_array_make(_manager->_env, nextLayerSize);
        
        char *varName = new char[20];
        
        for(unsigned i = 0; i < nextLayerSize; i++ ){
            ap_linexpr1_t expr = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, 1);
            ap_lincons1_t cons = ap_lincons1_make(AP_CONS_EQ, &expr, NULL);


            sprintf(varName, "x_%d_%d", _layerIndex+1, i);

            std::cout << "Adding constraint: -" << varName << " + " << bias[i];
            
            ap_lincons1_set_list( &cons,
                                  AP_COEFF_S_INT, -1, varName,
                                  AP_CST_S_DOUBLE, bias[i],
                                  AP_END);
            for(unsigned j = 0; j < currLayerSize; j++) {
                double weight = weights[j][i];
                char *jVarName = new char[20];
                sprintf(jVarName, "x_%d_a_%d", _layerIndex, j);

                std::cout << " + " << weight << "*" << jVarName;

                ap_lincons1_set_list( &cons,
                                      AP_COEFF_S_DOUBLE, weight, jVarName,
                                      AP_END);
            }
            
            std::cout << std::endl;

            ap_lincons1_array_set(&constraintArray, i, &cons);
        }

        AbstractValue *newVal = new AbstractValue(_manager, _layerIndex + 1);
        newVal->_ap_value = new ap_abstract1_t();
        *(newVal->_ap_value) = ap_abstract1_meet_lincons_array(_manager->_manager, false, _ap_value, &constraintArray);

        ap_lincons1_array_clear(&constraintArray);

        return newVal;
    }


    AbstractValue *performRelu() {
        
        char s1[20];
        char s2[20];

        unsigned layerSize = _manager->_layerSizes[_layerIndex];

        ap_abstract1_t *currentAV = _ap_value;

        for(unsigned i = 0; i < layerSize; ++i) {
            // Active case: neuron is positive and unchanged

            
            //Meet with condition
            ap_lincons1_array_t activeConstraintArray = ap_lincons1_array_make(_manager->_env, 1 );
            ap_linexpr1_t activeExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, i );
            ap_lincons1_t activeCons = ap_lincons1_make( AP_CONS_SUPEQ, &activeExpr, NULL );
            sprintf(s1, "x_%d_%d", _layerIndex, i);
            sprintf(s2, "x_%d_a_%d", _layerIndex, i);
            ap_lincons1_set_list( &activeCons,
                                    AP_COEFF_S_INT, 1, s1,
                                    AP_END );
            ap_lincons1_array_set( &activeConstraintArray, 0, &activeCons );

            ap_abstract1_t activeAV = ap_abstract1_meet_lincons_array(_manager->_manager, false, currentAV, &activeConstraintArray);


            //Apply transformation for active condition (x=x)
            activeConstraintArray = ap_lincons1_array_make( _manager->_env, 1 );
            activeExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, i);
            activeCons = ap_lincons1_make( AP_CONS_EQ, &activeExpr, NULL );

            ap_lincons1_set_list( &activeCons,
                                    AP_COEFF_S_INT, 1, s1,
                                    AP_COEFF_S_INT, -1, s2,
                                    AP_END );
            ap_lincons1_array_set( &activeConstraintArray, 0, &activeCons );

            activeAV = ap_abstract1_meet_lincons_array(_manager->_manager, false, &activeAV, &activeConstraintArray);



            // Inactive case: neuron is negative and becomes zero
            sprintf(s1, "x_%d_%d", _layerIndex, i);
            sprintf(s2, "x_%d_a_%d", _layerIndex, i);
                
            //Meet with condition
            ap_lincons1_array_t inactiveConstraintArray = ap_lincons1_array_make( _manager->_env, 1 );
            ap_linexpr1_t inactiveExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, i );
            ap_lincons1_t inactiveCons = ap_lincons1_make( AP_CONS_SUPEQ, &inactiveExpr, NULL );
            ap_lincons1_set_list( &inactiveCons,
                                    AP_COEFF_S_INT, -1, s1,
                                    AP_END );
            ap_lincons1_array_set( &inactiveConstraintArray, 0, &inactiveCons );

            ap_abstract1_t *inactiveAV = AV_addr(ap_abstract1_meet_lincons_array(_manager->_manager, false, currentAV, &inactiveConstraintArray));

            //Apply transformation for inactive condition (x=x)
            inactiveConstraintArray = ap_lincons1_array_make( _manager->_env, 1 );
            inactiveExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, i );
            inactiveCons = ap_lincons1_make( AP_CONS_EQ, &inactiveExpr, NULL );
            ap_lincons1_set_list( &inactiveCons,
                                    AP_COEFF_S_INT, 1, s2,
                                    AP_END );
            ap_lincons1_array_set( &inactiveConstraintArray, 0, &inactiveCons );

            if(ap_abstract1_is_bottom(_manager->_manager, inactiveAV)) {
                inactiveAV = AV_addr(ap_abstract1_bottom(_manager->_manager, _manager->_env));
            }
            else {
                inactiveAV = AV_addr(ap_abstract1_meet_lincons_array(_manager->_manager, false, inactiveAV, &inactiveConstraintArray));
            }
            
            printf( "\n inactive AV:\n" );
            ap_abstract1_fprint( stdout, _manager->_manager, inactiveAV );


            // Join the results
            currentAV = AV_addr(ap_abstract1_join( _manager->_manager, false, &activeAV, inactiveAV));

            ap_lincons1_array_clear(&inactiveConstraintArray);
            ap_lincons1_array_clear(&activeConstraintArray);

        }

        AbstractValue *newVal = new AbstractValue(_manager, _layerIndex);
        newVal->_ap_value = currentAV;
        return newVal;
    }


private:
    AbstractEnviroment *_manager;
    

};

}

#endif