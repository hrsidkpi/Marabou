#pragma once
#include "armadillo.h"
#include "../AbstractDomain.h"
#include "ZonotopeVertex.h"
#include <stdlib.h>
#include "../../utilities/LPSolver/LPSolver.h"
#include "ZonotopeOperations.h"


namespace AI {

	enum SearchSign { POSITIVE, NEGATIVE };

	class Zonotope : public AbstractDomain {
	public:

		~Zonotope() {};

		Zonotope(std::vector<arma::mat>& generators, arma::mat& bias) : generators(generators), bias(bias) {}
		Zonotope(const AI::Zonotope& other) : generators(other.generators), bias(other.bias) {}
		
		virtual void applyFullyConnectedLayer(arma::mat linear, arma::mat translation) override;
		virtual void applyReLuOnDim(unsigned dim) override;

		virtual void joinWith(AbstractDomain& other) override;
		
		virtual double** getBounds() override;
		virtual AI::AbstractDomainDimBound getBoundsForDim(unsigned dim) override;
		
		virtual unsigned getDimension() override;

		std::vector<ZonotopeVertex> getVertices();

		virtual std::string getSizeString() override;

		//void print() override;

	private:
		std::vector<arma::mat> generators;
		arma::mat bias;


		void changeEpsilonBounds(unsigned generatorIndex, double lower, double upper);

		bool getEdgiestPoint(unsigned dim, unsigned generatorIndex, AI::BoundType boundType, AI::SearchSign sign, AI::ZonotopeVertex* dest);
				
		void meetGt0(unsigned dim);
		void meetLt0(unsigned dim);
		void snapTo0(unsigned dim);

		bool isVertex(arma::mat vert);

		std::vector<arma::mat> getAllPossibleEpsilonValues(unsigned dim);

	};



	

}