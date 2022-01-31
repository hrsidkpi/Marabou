    
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
#include "/home/yarden/Desktop/research/Marabou/src/nlr/ai/Halfspace.h"
#include "/home/yarden/Desktop/research/Marabou/src/nlr/ai/Polyhedron.h"
#include "Tightening.h"
#include "/home/yarden/Desktop/research/Marabou/src/nlr/ai/AbstractDomainEnum.h"
#include "armadillo.h"


#define RUN_PURE_LINEAR false
#define RUN_BASIC_TEST false
#define RUN_HALFSPACE_TEST false
#define RUN_POLYHEDRON_TEST false
#define RUN_MULTI_TEST false
#define RUN_LARGE_TEST false


using namespace arma;


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

    void populateNetworkTiny(NLR::NetworkLevelReasoner &nlr)
    {
        // Create the layers
        nlr.addLayer( 0, NLR::Layer::INPUT, 2 );
        nlr.addLayer( 1, NLR::Layer::WEIGHTED_SUM, 2 );


        nlr.addLayerDependency(0,1);

        // Set the weights and biases for the weighted sum layers
        //layer 0 to 1
        nlr.setWeight( 0, 0, 1, 0, 1 );
        nlr.setWeight( 0, 0, 1, 1, 0 );
        nlr.setWeight( 0, 1, 1, 0, 1 );
        nlr.setWeight( 0, 1, 1, 1, 1 );

        // Variable indexing
        nlr.setNeuronVariable( NLR::NeuronIndex( 0, 0 ), 0 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 0, 1 ), 1 );

        nlr.setNeuronVariable( NLR::NeuronIndex( 1, 0 ), 2 );
        nlr.setNeuronVariable( NLR::NeuronIndex( 1, 1 ), 3 );
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
        nlr.setWeight( 0, 0, 1, 0, 3.4 );
        nlr.setWeight( 0, 0, 1, 1, 0.1 );
        nlr.setWeight( 0, 1, 1, 0, 1.4 );
        nlr.setWeight( 0, 1, 1, 1, 1 );
        //layer 1 to 2
        nlr.setWeight( 2, 0, 3, 0, 1 );
        nlr.setWeight( 2, 0, 3, 1, 0 );
        nlr.setWeight( 2, 1, 3, 0, -1 );
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
        * [0,3]      [0,5]      [-2, 5] 
        *        =>          =>          
        * [0,2]      [0,2]      [0,2] 
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
        nlr.setWeight( 0, 0, 1, 1, 0 );
        nlr.setWeight( 0, 1, 1, 0, 1 );
        nlr.setWeight( 0, 1, 1, 1, 1 );
        //layer 1 to 2
        nlr.setWeight( 1, 0, 2, 0, 1 );
        nlr.setWeight( 1, 0, 2, 1, 0 );
        nlr.setWeight( 1, 1, 2, 0, -1 );
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
        tableau.setLowerBound( 0, -0.3);
        tableau.setUpperBound( 0, -0.2 );
        tableau.setLowerBound( 1, -1.2 );
        tableau.setUpperBound( 1, 1.1 );

        double large = 1000;
        for(unsigned neuron = 2; neuron < num_of_layers * 2; neuron++) {
            tableau.setLowerBound(neuron, -large ); tableau.setUpperBound(neuron, large );
        }
    }

    void set_tableau_variable_layer_size(MockTableau &tableau, unsigned num_of_neurons) {
        tableau.setLowerBound( 0, 0 );
        tableau.setUpperBound( 0, 3 );
        tableau.setLowerBound( 1, 0 );
        tableau.setUpperBound( 1, 2 );

        double large = 1000;
        for(unsigned neuron = 2; neuron < num_of_neurons; neuron++) {
            tableau.setLowerBound(neuron, -large ); tableau.setUpperBound(neuron, large );
        }
    }

                                
    void test_perform_abstract_interpretation_pure_linear() 
    {
        if(!RUN_PURE_LINEAR) return;
        std::cout << "!!!test_perform_abstract_interpretation_pure_linear()!!!" << std::endl;

        NLR::NetworkLevelReasoner nlr;
        populateNetworkPureLinear(nlr);

        MockTableau tableau;
        set_tableau(tableau, 3);
        nlr.setTableau( &tableau );

        TS_ASSERT_THROWS_NOTHING( nlr.obtainCurrentBounds() );
        TS_ASSERT_THROWS_NOTHING( nlr.startAbstractInterpretation(ABSTRACT_DOMAIN_BOX) );
        TS_ASSERT_THROWS_NOTHING( nlr.performAbstractInterpretation() );


        NLR::AbstractInterpretorRaw *ai = nlr.getCurrentAI();
        ap_abstract1_t *val = ai->getCurrentAV()->_ap_value;
        
        ai->printCurrentAv();

        ap_interval_t *bounds1 = ap_abstract1_bound_variable(ai->getEnvironment()->_manager, val, const_cast<char *>("x_2_0"));
        double lb1 = bounds1->inf->val.dbl;
        double ub1 = bounds1->sup->val.dbl;
        ap_interval_free(bounds1);

        TS_ASSERT_EQUALS(lb1, -2);
        TS_ASSERT_EQUALS(ub1, 5);

        if(lb1 == 0) std::cout << "Lower bound 1 OK" << std::endl;
        if(ub1 == 14) std::cout << "Lower bound 1 OK" << std::endl;

        ap_interval_t *bounds2 = ap_abstract1_bound_variable(ai->getEnvironment()->_manager, val, const_cast<char *>("x_2_1"));
        double lb2 = bounds2->inf->val.dbl;
        double ub2 = bounds2->sup->val.dbl;
        ap_interval_free(bounds2);

        TS_ASSERT_EQUALS(lb2, 0);
        TS_ASSERT_EQUALS(ub2, 2);

        if(lb2 == 0) std::cout << "Lower bound 2 OK" << std::endl;
        if(ub2 == 11) std::cout << "Lower bound 2 OK" << std::endl;
    }

    void test_perform_abstract_interpretation() 
    {
        if(!RUN_BASIC_TEST) return;
        std::cout << "!!!test_perform_abstract_interpretation()!!!" << std::endl;

        NLR::NetworkLevelReasoner nlr;
        populateNetworkSmall(nlr);

        MockTableau tableau;
        set_tableau(tableau, 5);
        nlr.setTableau( &tableau );

        TS_ASSERT_THROWS_NOTHING( nlr.obtainCurrentBounds() );
        TS_ASSERT_THROWS_NOTHING( nlr.startAbstractInterpretation(ABSTRACT_DOMAIN_BOX) );
        TS_ASSERT_THROWS_NOTHING( nlr.performAbstractInterpretation() );

        throw(0);

        NLR::AbstractInterpretorRaw *ai = nlr.getCurrentAI();

        ap_abstract1_t *val = ai->getCurrentAV()->_ap_value;
                                                                                                                                                                                                 
        ap_interval_t *bounds1 = ap_abstract1_bound_variable(ai->getEnvironment()->_manager, val, const_cast<char *>("x_1_0"));
        double lb1 = bounds1->inf->val.dbl;
        double ub1 = bounds1->sup->val.dbl;
        
        ap_interval_free(bounds1);

        ai->printCurrentAv();
        TS_ASSERT_EQUALS(lb1, 0.0);
        TS_ASSERT_EQUALS(ub1, 5);

        if(lb1 == 0) std::cout << "Lower bound 1 OK" << std::endl;
        if(ub1 == 14) std::cout << "Lower bound 1 OK" << std::endl;


        ap_interval_t *bounds2 = ap_abstract1_bound_variable(ai->getEnvironment()->_manager, val, const_cast<char *>("x_1_1"));
        double lb2 = bounds2->inf->val.dbl;
        double ub2 = bounds2->sup->val.dbl;
        ap_interval_free(bounds2);

        TS_ASSERT_EQUALS(lb2, 0);
        TS_ASSERT_EQUALS(ub2, 2);

        if(lb2 == 0) std::cout << "Lower bound 2 OK" << std::endl;
        if(ub2 == 11) std::cout << "Lower bound 2 OK" << std::endl;
    }

    void test_halfspaces() 
    {
        if(!RUN_HALFSPACE_TEST) return;
        double *w = new double[2];
        w[0] = 1;
        w[1] = -1;
        NLR::Halfspace *a = new NLR::Halfspace(2, w, 1);

        a->print();


        double linear_transformation[4];
        linear_transformation[0] = 1;
        linear_transformation[1] = 0;
        linear_transformation[2] = 1;
        linear_transformation[3] = 1;
        mat linear_trans_mat(linear_transformation, 2, 2);

        double translation[2];
        translation[0] = 0;
        translation[1] = 0;
        mat translate_trans_mat(translation, 2, 1);

        bool sol = a->applyAffineTransformation(linear_trans_mat, translate_trans_mat);
        if(sol) std::cout << "invertible!" << std::endl;
        if(!sol) std::cout << "not invertible!" << std::endl;
        a->print();

        delete a;
    }

    void test_poly_under_approximation() 
    {

        if(!RUN_POLYHEDRON_TEST) return;

        double *w1 = new double[2] {1, 0};
        double *w2 = new double[2] {-1, 0};
        double *w3 = new double[2] {0, 1};
        double *w4 = new double[2] {0, -1};

        NLR::Halfspace *left = new NLR::Halfspace(2, w1, -1);
        NLR::Halfspace *right = new NLR::Halfspace(2, w2, 0);
        NLR::Halfspace *top = new NLR::Halfspace(2, w3, -1);
        NLR::Halfspace *bottom = new NLR::Halfspace(2, w4, 0);

        std::list<NLR::Halfspace*> halfspaces = {left, right, top, bottom };
        NLR::Polyhedron *poly = new NLR::Polyhedron(2, halfspaces);

        double *v1 = new double[2] {-2, 1};
        double *v2 = new double[2] {0.5, 0.7};
        TS_ASSERT(!poly->containsVertex(v1));
        TS_ASSERT(poly->containsVertex(v2));

        std::cout << "before: " << std::endl;
        poly->print();

        double linear_transformation[4];
        linear_transformation[0] = 1;
        linear_transformation[1] = 0;
        linear_transformation[2] = 1;
        linear_transformation[3] = 1;
        mat linear_trans_mat(linear_transformation, 2, 2);

        double translation[2];
        translation[0] = -0.5;
        translation[1] = 0;
        mat translate_trans_mat(translation, 2, 1);

        poly->applyAffineTransformation(linear_trans_mat, translate_trans_mat);
        poly->applyRelu();

        std::cout << "\n\nafter: " << std::endl;
        poly->print();

        delete[] v1;
        delete[] v2;
        delete poly;
    }

    void test_multi_abstract_interpretation()
    {
        if(!RUN_MULTI_TEST) return;

        NLR::NetworkLevelReasoner nlr;
        populateNetworkSmall(nlr);

        MockTableau tableau;
        set_tableau(tableau, 5);
        nlr.setTableau( &tableau );

        for(unsigned iter = 0; iter < 10; ++iter)
        {
            std::cout << "!!!test_perform_abstract_interpretation() iteration!!!" << std::endl;

            TS_ASSERT_THROWS_NOTHING( nlr.obtainCurrentBounds() );
            TS_ASSERT_THROWS_NOTHING( nlr.startAbstractInterpretation(ABSTRACT_DOMAIN_ZONOTOPE) );
            TS_ASSERT_THROWS_NOTHING( nlr.performAbstractInterpretation() );
        }
    }


    void populateNetworkLarge( NLR::NetworkLevelReasoner &nlr, unsigned hidednLayerCount, unsigned neuronPerHiddenLayer) 
    {

        if(!RUN_LARGE_TEST) return;

        // Create the layers
        nlr.addLayer( 0, NLR::Layer::INPUT, 2 );
        for(unsigned layer = 1; layer < hidednLayerCount * 2 + 1; layer+=2) {
            nlr.addLayer( layer, NLR::Layer::WEIGHTED_SUM, neuronPerHiddenLayer );
            nlr.addLayer( layer+1, NLR::Layer::RELU, neuronPerHiddenLayer );
        }

        for(unsigned layer = 0; layer < hidednLayerCount * 2; layer++) {
            nlr.addLayerDependency(layer,layer+1);
        }

        // Set the weights and biases for the weighted sum layers
        for(unsigned layer = 0; layer < hidednLayerCount * 2; layer+=2) 
        {
            unsigned layerSize = neuronPerHiddenLayer;
            if(layer == 0) layerSize = 2;
            for(unsigned n1 = 0; n1 < layerSize; n1++) 
            {
                for(unsigned n2 = 0; n2 < neuronPerHiddenLayer; n2++) 
                {
                    nlr.setWeight(layer, n1, layer+1, n2, 1);
                }
            }
        }

        // Set the weights and biases for the weighted sum layers
        for(unsigned layer = 1; layer < hidednLayerCount * 2; layer+=2) 
        {
            for(unsigned n1 = 0; n1 < neuronPerHiddenLayer; n1++) 
            {
                nlr.addActivationSource(layer, n1, layer+1, n1);
            }
        }

        unsigned index = 0;
        for(unsigned layer = 0; layer <= hidednLayerCount * 2; layer++) 
        {
            unsigned layerSize = neuronPerHiddenLayer;
            if(layer == 0) layerSize = 2;
            for(unsigned n1 = 0; n1 < layerSize; n1++) 
            {
                nlr.setNeuronVariable( NLR::NeuronIndex( layer, n1 ), index );
                index++;
            }
        }
    }

    void test_abstract_interpretation_large()
    {

        if(!RUN_LARGE_TEST) return;

        NLR::NetworkLevelReasoner nlr;

        unsigned layerCount = 20;
        unsigned neuronPerHiddenLayer = 100;
        populateNetworkLarge(nlr, layerCount, neuronPerHiddenLayer);

        MockTableau tableau;
        set_tableau_variable_layer_size(tableau, layerCount*2 * neuronPerHiddenLayer + 2);
        nlr.setTableau( &tableau );

        TS_ASSERT_THROWS_NOTHING( nlr.obtainCurrentBounds() );
        TS_ASSERT_THROWS_NOTHING( nlr.startAbstractInterpretation(ABSTRACT_DOMAIN_ZONOTOPE) );
        
        TS_ASSERT_THROWS_NOTHING( nlr.performAbstractInterpretation() );
    }

};