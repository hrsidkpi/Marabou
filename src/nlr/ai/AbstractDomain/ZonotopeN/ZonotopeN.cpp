#include "Zonotope.h"




void AI::ZonotopeN::applyFullyConnectedLayer(arma::mat linear, arma::mat translate)
{
    for(auto &z : zonotopes) {
        z.applyFullyConnectedLayer(linear, translate);
    }
}

void AI::ZonotopeN::applyReLuOnDim(unsigned dim)
{
    std::vector<AI::Zonotope> newZonotopes;
    for(auto zPos : zonotopes) {
        AI::Zonotope zNeg(zPos);

        zPos.meetGt0(dim);

        zNeg.meetLt0(dim);
        zNeg.snapTo0(dim);

        new_zonotopes.push_back(zPos);
        new_zonotopes.push_back(zNeg);
    }
}

void AI::ZonotopeN::joinWith(AI::AbstractDomain& other)
{

}

AI::AbstractDomainDimBound AI::ZonotopeN::getBoundsForDim(unsigned dim) 
{

}

double** AI::ZonotopeN::getBounds()
{


}


unsigned AI::ZonotopeN::getDimension()
{
	
}

std::string AI::ZonotopeN::getSizeString()
{
	
}
