/*
 *  edsubnetwork.cpp
 *  nned
 *
 *  Created by w8r on 4/23/08.

 *
 */
 
#ifndef EDSUBNETWORK_CPP
#define EDSUBNETWORK_CPP

#include "edsubnetwork.h"
#include <cmath>

double quad( double x )
{
	return x * x;
}

EDSubnetwork::EDSubnetwork()
{
}

EDSubnetwork::EDSubnetwork( double illuminationLevelPrototype, 
							 double neuron1UpperThreshold,
							 double neuron1LowerThreshold,
							 double neuron2UpperThreshold,
							 double neuron2LowerThreshold ) : pr( illuminationLevelPrototype )
{
	if( distance( neuron1UpperThreshold, neuron1LowerThreshold ) > distance( neuron2UpperThreshold, neuron2LowerThreshold ) )
	{
		strong = new Neuron( neuron1UpperThreshold, neuron1LowerThreshold );
		weak =   new Neuron( neuron2UpperThreshold, neuron2LowerThreshold );
	}
	else
	{
		strong = new Neuron( neuron2UpperThreshold, neuron2LowerThreshold );
		weak   = new Neuron( neuron1UpperThreshold, neuron1LowerThreshold );
	}
}
EDSubnetwork::~EDSubnetwork()
{
	if( weak != NULL )
		delete weak;
	if( strong != NULL )
		delete strong;
}

EDSubnetwork& EDSubnetwork::operator = ( const EDSubnetwork& other )
{
	if( this != &other )
	{
		pr     = other.illuminationPrototype();
		weak   = new Neuron( other.weakNeuron().upperThreshold(),   other.weakNeuron().lowerThreshold() );
		strong = new Neuron( other.strongNeuron().upperThreshold(), other.strongNeuron().lowerThreshold() );
	}
	return *this;
}

EDSubnetwork::EDSubnetwork( const EDSubnetwork& other ) : EDNetworkMember()
{
	EDNetworkMember();
	if( this != &other )
	{
		pr     = other.illuminationPrototype();
		weak   = new Neuron( other.weakNeuron().upperThreshold(),   other.weakNeuron().lowerThreshold() );
		strong = new Neuron( other.strongNeuron().upperThreshold(), other.strongNeuron().lowerThreshold() );
	}
}


Neuron& EDSubnetwork::neuronCompetition( GreyscaleWindow& img )
{
	if ( sqrt( quad( img.upperGreyLevel() - weak->upperThreshold() ) + 
	           quad( img.lowerGreyLevel() - weak->lowerThreshold() ) ) < 
		 sqrt( quad( img.upperGreyLevel() - strong->upperThreshold() ) + 
	           quad( img.lowerGreyLevel() - strong->lowerThreshold() ) ) )
		return *weak;
	else 
		return *strong;
}

double EDSubnetwork::illuminationPrototype() const
{
	return pr;
}
Neuron& EDSubnetwork::weakNeuron() const
{
	return *weak;
}
Neuron& EDSubnetwork::strongNeuron() const
{
	return *strong;
}
bool EDSubnetwork::A2( double mmax, double mmin )
{
	/* при обучении это условие не действует, так как когда оно применяется, веса нейронов
	   могут быть уже изменены. если поставить это условие до коррекции весов нейронов,
	   нейрон слабого контура не будет обучаться */
	return ( ( mmax - mmin ) >= ( weak->upperThreshold() - weak->lowerThreshold() ) );
}

bool EDSubnetwork::A3( EdgeConfiguration& edge )
{
	if( validate( edge ) != blankEdge() )
		return true;
	else
		return true;
}

EdgeConfiguration& EDSubnetwork::detect( GreyscaleWindow& win )
{
	pr += ( eta() * ( win.averageGreyLevel() - pr ) );
	Neuron& Winner = neuronCompetition( win );
	/* иначе нейрон слабо видимого контура не будет обучаться */
	EdgeConfiguration detectionResult = Winner.detect( win );
	if( A2( win.upperGreyLevel(), win.lowerGreyLevel() ) )
	{
		if( A3( detectionResult ) )
			return validate( detectionResult );
		else
			return blankEdge();
	}
	else
		return blankEdge();
}


#endif

