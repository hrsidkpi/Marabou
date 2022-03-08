#pragma once
#include "armadillo.h"

namespace AI {

	struct ZonotopeVertex {
		arma::mat vertex;
		arma::mat generatorCoefs;
	
		ZonotopeVertex(arma::mat vertex, arma::mat coefs) : vertex(vertex), generatorCoefs(coefs) {};
	
	};

}