/*********************                                                        */
/*! \file Options.h
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

#ifndef __Options_h__
#define __Options_h__

#include "DivideStrategy.h"
#include "MString.h"
#include "Map.h"
#include "MILPSolverBoundTighteningType.h"
#include "OptionParser.h"
#include "SnCDivideStrategy.h"
#include "SymbolicBoundTighteningType.h"

#include "boost/program_options.hpp"

/*
  A singleton class that contains all the options and their values.
*/
class Options
{
public:
    enum BoolOptions {
        // Should the PL constraints add aux equations during preprocessing?
        PREPROCESSOR_PL_CONSTRAINTS_ADD_AUX_EQUATIONS = 0,

        // Should DNC mode be on or off
        DNC_MODE,

        // Restore tree states of the parent when handling children in DnC.
        RESTORE_TREE_STATES,

        // Dump the bounds of each variable after preprocessing
        DUMP_BOUNDS,

        // Help flag
        HELP,

        // Version flag
        VERSION,

        // Solve the input query with a MILP solver
        SOLVE_WITH_MILP
    };

    enum IntOptions {
        // DNC options
        NUM_WORKERS = 0,
        NUM_INITIAL_DIVIDES,
        NUM_ONLINE_DIVIDES,
        INITIAL_TIMEOUT,

        OUTPUT_LAYER_INDEX,

        // Engine verbosity
        VERBOSITY,

        // Global timeout
        TIMEOUT,

        CONSTRAINT_VIOLATION_THRESHOLD,

        // The number of simulations
        NUMBER_OF_SIMULATIONS,
    };

    enum FloatOptions{
        // DNC options
        TIMEOUT_FACTOR,

        // Gurobi options
        MILP_SOLVER_TIMEOUT,

        // Engine's Preprocessor options
        PREPROCESSOR_BOUND_TOLERANCE,
    };

    enum StringOptions {
        INPUT_FILE_PATH = 0,
        PROPERTY_FILE_PATH,
        INPUT_QUERY_FILE_PATH,
        OUTPUT_BOUNDS_FILE_PATH,
        SUMMARY_FILE,
        SPLITTING_STRATEGY,
        SNC_SPLITTING_STRATEGY,
        SYMBOLIC_BOUND_TIGHTENING_TYPE,
        MILP_SOLVER_BOUND_TIGHTENING_TYPE,
        QUERY_DUMP_FILE,
    };

    /*
      The singleton instance
    */
    static Options *get();

    /*
      Parse the command line arguments and extract the option values.
    */
    void parseOptions( int argc, char **argv );

    /*
      Print all command arguments
    */
    void printHelpMessage() const;

    /*
      Retrieve the value of the various options, by type
    */
    bool getBool( unsigned option ) const;
    int getInt( unsigned option ) const;
    float getFloat( unsigned option ) const;
    String getString( unsigned option ) const;
    DivideStrategy getDivideStrategy() const;
    SnCDivideStrategy getSnCDivideStrategy() const;
    SymbolicBoundTighteningType getSymbolicBoundTighteningType() const;
    MILPSolverBoundTighteningType getMILPSolverBoundTighteningType() const;

    /*
      Retrieve the value of the various options, by type
    */
    void setBool( unsigned option, bool );
    void setInt( unsigned option, int );
    void setFloat( unsigned option, float );
    void setString( unsigned option, std::string );

    /*
      Options that are determined at compile time
    */
    bool gurobiEnabled() const
    {
#ifdef ENABLE_GUROBI
        return true;
#else
        return false;
#endif
    }

private:
    /*
      Disable default constructor and copy constructor
    */
    Options();
    Options( const Options & );

    /*
      Initialize the default option values
    */
    void initializeDefaultValues();

    OptionParser _optionParser;

    /*
      The various option values
    */
    Map<unsigned, bool> _boolOptions;
    Map<unsigned, int> _intOptions;
    Map<unsigned, float> _floatOptions;
    Map<unsigned, std::string> _stringOptions;
};

#endif // __Options_h__

//
// Local Variables:
// compile-command: "make -C ../.. "
// tags-file-name: "../../TAGS"
// c-basic-offset: 4
// End:
//
