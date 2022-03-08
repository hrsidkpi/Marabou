#ifndef __AbstractValueRaw_h__
#define __AbstractValueRaw_h__


#include "ap_global1.h"
#include "box.h"
#include "AbstractEnvironmentRaw.h"
#include <iostream>

#define _CALL_DEBUG

namespace NLR {

class AbstractValueRaw {

public:

    ~AbstractValueRaw() {
        //ap_abstract1_clear(_manager->_manager, _ap_value);
        ap_abstract1_clear(_manager->_manager, &_ap_value);
    }

    ap_abstract1_t _ap_value;
    bool initialized;
    unsigned _layerIndex;

    AbstractValueRaw(NLR::AbstractEnvironmentRaw *manager, unsigned layerIndex) {
        _manager = manager;
        _layerIndex = layerIndex;
        initialized = false;
    }


    void initLayer(unsigned layer, double **bounds)
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
            std::cout << "variable " << varNames[i] << " bounded by " << lb <<", " << ub << std::endl;
            ap_linexpr1_t exprLb = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, 2);
            ap_lincons1_t consLb = ap_lincons1_make(AP_CONS_SUPEQ, &exprLb, NULL);
            ap_lincons1_set_list(&consLb,
                AP_COEFF_S_INT, 1, varNames[i],
                AP_CST_S_DOUBLE, -lb,
                AP_END);
            ap_lincons1_array_set(&constraintArray, currentIndex, &consLb);
            currentIndex++;
            ap_linexpr1_t exprUb = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, 2);
            ap_lincons1_t consUb = ap_lincons1_make(AP_CONS_SUPEQ, &exprUb, NULL);
            ap_lincons1_set_list(&consUb,
                AP_COEFF_S_INT, -1, varNames[i],
                AP_CST_S_DOUBLE, ub,
                AP_END);
            ap_lincons1_array_set(&constraintArray, currentIndex, &consUb);
            currentIndex++;
        }                                                  
        
        //newVal->_ap_value = new ap_abstract1_t();

        if(!initialized) {
            std::cout << "Creating ap_value for env " << _manager->_env << std::endl;
            _ap_value = ap_abstract1_of_lincons_array(_manager->_manager, _manager->_env, &constraintArray);
            initialized = true;
        }else{
            std::cout << "Updating ap_value" << std::endl;
            ap_abstract1_t t = ap_abstract1_meet_lincons_array(_manager->_manager, false, &_ap_value, &constraintArray);
            ap_abstract1_clear(_manager->_manager, &_ap_value);
            _ap_value = t;
        }
        std::cout << "Clearing constraint array" << std::endl;

        ap_lincons1_array_clear(&constraintArray);

        for(unsigned i = 0; i < layerSize; i++)
            delete[] varNames[i];
        delete[] varNames;
    }

    void setLayerIndex(unsigned layerIndex) {
        _layerIndex = layerIndex;
    }

    void updateToEnv() {
        ap_abstract1_t t = ap_abstract1_change_environment(_manager->_manager, false, &_ap_value, _manager->_env, false);
        ap_abstract1_clear(_manager->_manager,&_ap_value);
        _ap_value = t;
    }

    void performAffineTransformation(const double *weightsMatrix, double *bias) {

        unsigned currLayerSize = _manager->_layers[_layerIndex]->getSize();
        unsigned nextLayerSize = _manager->_layers[_layerIndex + 1]->getSize();

        ap_lincons1_array_t constraintArray = ap_lincons1_array_make(_manager->_env, nextLayerSize);
        
        char *varName = new char[20];
        char *jVarName = new char[20];
        
        for(unsigned i = 0; i < nextLayerSize; i++ ){
            ap_linexpr1_t expr = ap_linexpr1_make(_manager->_env, AP_LINEXPR_SPARSE, currLayerSize+2);
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
        
        ap_abstract1_t t = ap_abstract1_meet_lincons_array(_manager->_manager, false, &_ap_value, &constraintArray);
        ap_abstract1_clear(_manager->_manager,&_ap_value);
        _ap_value = t;

        ap_lincons1_array_clear(&constraintArray);


        delete[] varName;
        delete[] jVarName;
    }


    void performReluOnNeuron(unsigned neuron) {

        

        char n1[20] = { 0 };
        char n2[20] = { 0 };
        sprintf(n1, "x_%d_%d", _layerIndex, neuron);
        sprintf(n2, "x_%d_%d", _layerIndex+1, neuron);

        std::cout << "performing relu from " << n1 << " to " << n2 << std::endl;

        if(neuron == 44) ap_abstract1_fprint(stdout, _manager->_manager, &_ap_value);

        // === Active case: neuron is positive and unchanged ===
        //Meet with condition
        std::cout << "step 1..." << std::endl;
        ap_lincons1_array_t activeConstraintArray = ap_lincons1_array_make(_manager->_env, 1 );
        ap_linexpr1_t activeExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, 1 );
        ap_lincons1_t activeCons = ap_lincons1_make( AP_CONS_SUPEQ, &activeExpr, NULL );
        ASSERT(!ap_lincons1_set_list( &activeCons,
                                AP_COEFF_S_INT, 1, n1,
                                AP_END ));
        ASSERT(!ap_lincons1_array_set( &activeConstraintArray, 0, &activeCons ));        
        ap_abstract1_t activeAV = ap_abstract1_meet_lincons_array(_manager->_manager, false, &_ap_value, &activeConstraintArray);
        ap_lincons1_array_clear(&activeConstraintArray);   

        //Apply transformation for active condition (x=x)
        std::cout << "2..." << std::endl;
        ap_lincons1_array_t activeConstraintArrayTrans = ap_lincons1_array_make( _manager->_env, 1 );
        std::cout << "3..." << std::endl;
        ap_linexpr1_t activeExprTrans = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, 3);
        std::cout << "4..." << std::endl;
        ap_lincons1_t activeConsTrans = ap_lincons1_make( AP_CONS_EQ, &activeExprTrans, NULL );     
        std::cout << "5...";
        std::cout.flush();
        ASSERT(!ap_lincons1_set_list( &activeConsTrans,
                                AP_COEFF_S_INT, 1, n1,
                                AP_COEFF_S_INT, -1, n2,
                                AP_END ));
        ASSERT(!ap_lincons1_array_set( &activeConstraintArrayTrans, 0, &activeConsTrans ));    
        std::cout << "6...";
        std::cout.flush();
        ap_linexpr1_fprint(stdout, &activeExprTrans);
        ap_abstract1_t activeAV_trans = ap_abstract1_meet_lincons_array(_manager->_manager, false, &activeAV, &activeConstraintArrayTrans);
        std::cout << "7...";
        std::cout.flush();
        ap_abstract1_clear(_manager->_manager, &activeAV);
        std::cout << "8...";
        std::cout.flush();
        ap_lincons1_array_clear(&activeConstraintArrayTrans);      


        // === Inactive case: neuron is negative and becomes zero ===
        //Meet with condition
        std::cout << "9...";
        std::cout.flush();
        ap_lincons1_array_t inactiveConstraintArray = ap_lincons1_array_make( _manager->_env, 1 );
        ap_linexpr1_t inactiveExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, 1 );
        ap_lincons1_t inactiveCons = ap_lincons1_make( AP_CONS_SUPEQ, &inactiveExpr, NULL );
        ASSERT(!ap_lincons1_set_list( &inactiveCons,
                                AP_COEFF_S_INT, -1, n1,
                                AP_END ));
        ASSERT(!ap_lincons1_array_set( &inactiveConstraintArray, 0, &inactiveCons ));        
        ap_abstract1_t inactiveAV = ap_abstract1_meet_lincons_array(_manager->_manager, false, &_ap_value, &inactiveConstraintArray);
        ap_lincons1_array_clear(&inactiveConstraintArray);     

        //Apply transformation for inactive condition (x=0)
        std::cout << "10...";
        std::cout.flush();
        inactiveConstraintArray = ap_lincons1_array_make( _manager->_env, 1 );
        inactiveExpr = ap_linexpr1_make( _manager->_env, AP_LINEXPR_SPARSE, 1 );
        inactiveCons = ap_lincons1_make( AP_CONS_EQ, &inactiveExpr, NULL );
        ASSERT(!ap_lincons1_set_list( &inactiveCons,
                                AP_COEFF_S_INT, 1, n2,
                                AP_END ));
        ASSERT(!ap_lincons1_array_set( &inactiveConstraintArray, 0, &inactiveCons ));        
        ap_abstract1_t inactiveAV_trans;
        if(ap_abstract1_is_bottom(_manager->_manager, &inactiveAV)) {
            inactiveAV_trans = ap_abstract1_bottom(_manager->_manager, _manager->_env);
        }
        else {
            inactiveAV_trans = ap_abstract1_meet_lincons_array(_manager->_manager, false, &inactiveAV, &inactiveConstraintArray);
        }
        std::cout << "11...";
        std::cout.flush();
        ap_abstract1_clear(_manager->_manager, &inactiveAV);
        std::cout << "11.5...";
        std::cout.flush();
        ap_lincons1_array_clear(&inactiveConstraintArray);

        // Join the results
        std::cout << "12...";
        std::cout.flush();
        ap_abstract1_clear(_manager->_manager,&_ap_value);
        _ap_value = ap_abstract1_join( _manager->_manager, false, &activeAV_trans, &inactiveAV_trans);

        //cleanup and return
        std::cout << "13...";
        std::cout.flush();
        ap_abstract1_clear(_manager->_manager, &inactiveAV_trans);
        std::cout << "14...";
        std::cout.flush();
        ap_abstract1_clear(_manager->_manager, &activeAV_trans);
    }

    void performRelu() {
        unsigned layerSize = _manager->_layers[_layerIndex]->getSize();

        for(unsigned n = 0; n < layerSize; n++) {
            performReluOnNeuron(n);
        }

        _layerIndex++;
    }


private:
    AbstractEnvironmentRaw *_manager;
    

};

}

#endif
