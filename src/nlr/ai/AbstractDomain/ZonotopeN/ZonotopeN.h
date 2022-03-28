#pragma once

#include "armadillo.h"
#include "../AbstractDomain.h"
#include <stdlib.h>
#include "../Zonotope/Zonotope.h"
#include "../Zonotope/ZonotopeOperations.h"


namespace AI {

	enum SearchSign { POSITIVE, NEGATIVE };

	class ZonotopeN : public AbstractDomain {
	public:

		~Zonotope() {};

		Zonotope(Zonotope initialZonotope, unsigned N) : zonotopes(std::vector<Zonotope>() {initialZonotope}) {}
		
		virtual void applyFullyConnectedLayer(arma::mat linear, arma::mat translation) override;
		virtual void applyReLuOnDim(unsigned dim) override;

		virtual void joinWith(AbstractDomain& other) override;
		
		virtual double** getBounds() override;
		virtual AI::AbstractDomainDimBound getBoundsForDim(unsigned dim) override;
		
		virtual unsigned getDimension() override;

		virtual std::string getSizeString() override;

	private:
		std::vector<Zonotope> zonotopes;
		unsigned N;
	};



	

}