    
    /*********************                                                        */
/*! \file Test_AbstractInterpretation.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Guy Katz
 ** This file is part of the Marabou project.
 ** Copyright (c) 2017-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved. See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** [[ Add lengthier description here ]]

**/

#include <cxxtest/TestSuite.h>

#include "../../engine/tests/MockTableau.h"
#include "FloatUtils.h"
#include "InputQuery.h"
#include "Layer.h"
#include "NetworkLevelReasoner.h"
#include "Tightening.h"
#include "/home/yarden/Desktop/research/Marabou/src/nlr/ai/AbstractDomainEnum.h"


class MockForNetworkLevelReasoner
{
public:
};


class AbstractInterpretationTestSuite : public CxxTest::TestSuite
{
public:

    void setUp()
    {
    }

    void tearDown()
    {
    }

    void populateNetwork( NLR::NetworkLevelReasoner &nlr )
    {
        /*
                a
          x           d    f
                b
          y           e    g
                c
        */

        // Create the layers
        nlr.addLayer( 0, NLR::Layer::INPUT, 2 );
        nlr.addLayer( 1, NLR::Layer::WEIGHTED_SUM, 3 );
        nlr.addLayer( 2, NLR::Layer::RELU, 3 );
        nlr.addLayer( 3, NLR::Layer::WEIGHTED_SUM, 2 );
        nlr.addLayer( 4, NLR::Layer::RELU, 2 );
        nlr.addLayer( 5, NLR::Layer::WEIGHTED_SUM, 2 );

        // Mark layer dependencies
        for ( unsigned i = 1; i <= 5; ++i )
            nlr.addLayerDependency( i - 1, i );

        // Set the weights and biases for the weighted sum layers
        nlr.setWeight( 0, 0, 1, 0, 1 );
        nlr.setWeight( 0, 0, 1, 1, 2 );
        nlr.setWeight( 0, 1, 1, 1, -3 );
        nlr.setWeight( 0, 1, 1, 2, 1 );

        nlr.setWeight( 2, 0, 3, 0, 1 );
        nlr.setWeight( 2, 0, 3, 1, -1 );
        nlr.setWeight( 2, 1, 3, 0, 1 );
        nlr.setWeight( 2, 1, 3, 1, 1 );
        nlr.setWeight( 2, 2, 3, 0, -1 );
        nlr.setWeight( 2, 2, 3, 1, -1 );

        nlr.setWeight( 4, 0, 5, 0, 1 );
        nlr.setWeight( 4, 0, 5, 1, 1 );
        nlr.setWeight( 4, 1, 5, 1, 3 );

        nlr.setBias( 1, 0, 1 );
        nlr.setBias( 3, 1, 2 );

        // Mark the ReLU sources
        nlr.addActivationSource( 1, 0, 2, 0 );
        nlr.addActivationSource( 1, 1, 2, 1 );
        nlr.addActivationSource( 1, 2, 2, 2 );

        nlr.addActivationSource( 3, 0, 4, 0 );
        nlr.addActivationSource( 3, 1, 4, 1 );

        // Variable indexing
        nlr.setNeuronVariable( NLR::NeuronIndex( 0, 0 ), 0 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 0, 1 ), 1 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 1, 0 ), 2 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 1, 1 ), 4 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 1, 2 ), 6 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 2, 0 ), 3 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 2, 1 ), 5 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 2, 2 ), 7 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 3, 0 ), 8 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 3, 1 ), 10 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 4, 0 ), 9 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 4, 1 ), 11 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 5, 0 ), 12 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 5, 1 ), 13 );
    }

    void populateNetworkSmall( NLR::NetworkLevelReasoner &nlr ) 
    {
        /*
          x_0_0    x_1_0    x_2_0
          x_0_1    x_1_1    x_2_1
        */

       /**
        * with a box:
        * [0,3]      [-6,3]      [0, 3]      [0, 14]      [0,14]
        *        =>          =>          =>           =>
        * [0,2]      [0,11]      [0,11]      [-3,11]      [0,11]
        * 
        * In total (tightest possible, found using desmos on the entire input domain):
        * [0,3]                  [0,12]
        *        =>   ...    =>   
        * [0,2]                  [0,11]

        *  Possible assignment:
        * 1     -2  0   4   4    
        * 1     4   4   4   4
        * x_0_0 = 1
        * x_0_1 = 1
        * x_1_0 = -2
        * x_1_1 = 4
        * x_2_0 = 0
        * x_2_1 = 4
        * x_3_0 = 4
        * x_3_1 = 4
        * x_4_0 = 4
        * x_4_1 = 4

        **/

        // Create the layers
        nlr.addLayer( 0, NLR::Layer::INPUT, 2 );

        nlr.addLayer( 1, NLR::Layer::WEIGHTED_SUM, 2 );
        nlr.addLayer( 2, NLR::Layer::RELU, 2 );
        nlr.addLayer( 3, NLR::Layer::WEIGHTED_SUM, 2 );
        nlr.addLayer( 4, NLR::Layer::RELU, 2 );

        nlr.addLayerDependency(0,1);
        nlr.addLayerDependency(1,2);
        nlr.addLayerDependency(2,3);
        nlr.addLayerDependency(3,4);

        // Set the weights and biases for the weighted sum layers
        //layer 0 to 1
        nlr.setWeight( 0, 0, 1, 0, 1 );
        nlr.setWeight( 0, 0, 1, 1, 3 );
        nlr.setWeight( 0, 1, 1, 0, -3 );
        nlr.setWeight( 0, 1, 1, 1, 1 );
        //layer 1 to 2
        nlr.setWeight( 2, 0, 3, 0, 1 );
        nlr.setWeight( 2, 0, 3, 1, -1 );
        nlr.setWeight( 2, 1, 3, 0, 1 );
        nlr.setWeight( 2, 1, 3, 1, 1 );

        //Set the RELU sources
        nlr.addActivationSource( 1, 0, 2, 0 );
        nlr.addActivationSource( 1, 1, 2, 1 );
        nlr.addActivationSource( 3, 0, 4, 0 );
        nlr.addActivationSource( 3, 1, 4, 1 );

        // Variable indexing
        nlr.setNeuronVariable( NLR::NeuronIndex( 0, 0 ), 0 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 0, 1 ), 1 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 1, 0 ), 2 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 1, 1 ), 3 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 2, 0 ), 4 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 2, 1 ), 5 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 3, 0 ), 6 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 3, 1 ), 7 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 4, 0 ), 8 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 4, 1 ), 9 );
    }

    void populateNetworkPureLinear( NLR::NetworkLevelReasoner &nlr ) 
    {
        /*
          x_0_0    x_1_0    x_2_0
          x_0_1    x_1_1    x_2_1
        */

       /**
        * with a box:
        * [0,3]      [-6,3]      [-6, 14] 
        *        =>          =>          
        * [0,2]      [0,11]      [-3,17] 
        * 
        **/

        // Create the layers
        nlr.addLayer( 0, NLR::Layer::INPUT, 2 );

        nlr.addLayer( 1, NLR::Layer::WEIGHTED_SUM, 2 );
        nlr.addLayer( 2, NLR::Layer::WEIGHTED_SUM, 2 );

        nlr.addLayerDependency(0,1);
        nlr.addLayerDependency(1,2);

        // Set the weights and biases for the weighted sum layers
        //layer 0 to 1
        nlr.setWeight( 0, 0, 1, 0, 1 );
        nlr.setWeight( 0, 0, 1, 1, 3 );
        nlr.setWeight( 0, 1, 1, 0, -3 );
        nlr.setWeight( 0, 1, 1, 1, 1 );
        //layer 1 to 2
        nlr.setWeight( 1, 0, 2, 0, 1 );
        nlr.setWeight( 1, 0, 2, 1, -1 );
        nlr.setWeight( 1, 1, 2, 0, 1 );
        nlr.setWeight( 1, 1, 2, 1, 1 );

        // Variable indexing
        nlr.setNeuronVariable( NLR::NeuronIndex( 0, 0 ), 0 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 0, 1 ), 1 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 1, 0 ), 2 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 1, 1 ), 3 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 2, 0 ), 4 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 2, 1 ), 5 );
    }

    void set_tableau(MockTableau &tableau, unsigned num_of_layers) {
        tableau.setLowerBound( 0, 0 );
        tableau.setUpperBound( 0, 3 );
        tableau.setLowerBound( 1, 0 );
        tableau.setUpperBound( 1, 2 );

        double large = 1000;
        for(unsigned neuron = 2; neuron < num_of_layers * 2; neuron++) {
            tableau.setLowerBound(neuron, -large ); tableau.setUpperBound(neuron, large );
        }
    }


    void test_perform_abstract_interpretation_pure_linear() 
    {

        return;

        std::cout << "!!!test_perform_abstract_interpretation_pure_linear()!!!" << std::endl;

        NLR::NetworkLevelReasoner nlr;
        populateNetworkPureLinear(nlr);

        MockTableau tableau;
        set_tableau(tableau, 3);
        nlr.setTableau( &tableau );

        TS_ASSERT_THROWS_NOTHING( nlr.obtainCurrentBounds() );
        TS_ASSERT_THROWS_NOTHING( nlr.startAbstractInterpretation(ABSTRACT_DOMAIN_POLY) );
        TS_ASSERT_THROWS_NOTHING( nlr.performAbstractInterpretation() );

        NLR::AbstractInterpretorRaw *ai = nlr.getCurrentAI();

        ap_abstract1_t *val = ai->getCurrentAV()->_ap_value;
        
        ap_interval_t *bounds1 = ap_abstract1_bound_variable(ai->getEnvironment()->_manager, val, const_cast<char *>("x_2_0"));
        double lb1 = bounds1->inf->val.dbl;
        double ub1 = bounds1->sup->val.dbl;
        ap_interval_free(bounds1);

        TS_ASSERT_EQUALS(lb1, -6);
        TS_ASSERT_EQUALS(ub1, 14);

        if(lb1 == 0) std::cout << "Lower bound 1 OK" << std::endl;
        if(ub1 == 14) std::cout << "Lower bound 1 OK" << std::endl;

        ap_interval_t *bounds2 = ap_abstract1_bound_variable(ai->getEnvironment()->_manager, val, const_cast<char *>("x_2_1"));
        double lb2 = bounds2->inf->val.dbl;
        double ub2 = bounds2->sup->val.dbl;
        ap_interval_free(bounds2);

        TS_ASSERT_EQUALS(lb2, -3);
        TS_ASSERT_EQUALS(ub2, 17);

        if(lb2 == 0) std::cout << "Lower bound 2 OK" << std::endl;
        if(ub2 == 11) std::cout << "Lower bound 2 OK" << std::endl;

        TS_ASSERT(false);
    }

    void test_perform_abstract_interpretation() 
    {
        std::cout << "!!!test_perform_abstract_interpretation()!!!" << std::endl;
        return;

        NLR::NetworkLevelReasoner nlr;
        populateNetworkSmall(nlr);

        MockTableau tableau;
        set_tableau(tableau, 5);
        nlr.setTableau( &tableau );

        TS_ASSERT_THROWS_NOTHING( nlr.obtainCurrentBounds() );
        TS_ASSERT_THROWS_NOTHING( nlr.startAbstractInterpretation(ABSTRACT_DOMAIN_POLY) );
        TS_ASSERT_THROWS_NOTHING( nlr.performAbstractInterpretation() );

        NLR::AbstractInterpretorRaw *ai = nlr.getCurrentAI();

        ap_abstract1_t *val = ai->getCurrentAV()->_ap_value;
        
        ap_interval_t *bounds1 = ap_abstract1_bound_variable(ai->getEnvironment()->_manager, val, const_cast<char *>("x_4_0"));
        double lb1 = bounds1->inf->val.dbl;
        double ub1 = bounds1->sup->val.dbl;
        
        ap_interval_free(bounds1);

        ai->printCurrentAv();
        TS_ASSERT_EQUALS(lb1, 0.0);
        TS_ASSERT_EQUALS(ub1, 14);

        if(lb1 == 0) std::cout << "Lower bound 1 OK" << std::endl;
        if(ub1 == 14) std::cout << "Lower bound 1 OK" << std::endl;


        ap_interval_t *bounds2 = ap_abstract1_bound_variable(ai->getEnvironment()->_manager, val, const_cast<char *>("x_4_1"));
        double lb2 = bounds2->inf->val.dbl;
        double ub2 = bounds2->sup->val.dbl;
        ap_interval_free(bounds2);

        TS_ASSERT_EQUALS(lb2, 0);
        TS_ASSERT_EQUALS(ub2, 11);

        if(lb2 == 0) std::cout << "Lower bound 2 OK" << std::endl;
        if(ub2 == 11) std::cout << "Lower bound 2 OK" << std::endl;

        TS_ASSERT(false);
    }
};