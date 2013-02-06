/*
 *  neoron.cpp
 *  nned
 *
 *  Created by w8r on 4/18/08.

 *
 */

#ifndef NEURON_CPP
#define NEURON_CPP

#include "neuron.h"

Neuron::Neuron( double w1, double w2 )
{
	if( w1 > w2 )
	{
		wmax = w1;
		wmin = w2;
	}
	else
	{
		wmax = w2;
		wmin = w1;
	}
}

EdgeConfiguration Neuron::QMapping( GreyscaleWindow& imgWindow )
{
	EdgeConfiguration edge( imgWindow.matrix(), wmin, wmax );
	return edge;
}

EdgeConfiguration Neuron::detect( GreyscaleWindow& win )
{
	wmin += ( eta() * ( win.lowerGreyLevel() - wmin ) );
	wmax += ( eta() * ( win.upperGreyLevel() - wmax ) );
	trainIteration++;
	return QMapping( win );
}

double Neuron::upperThreshold() const
{
	return wmax;
}

double Neuron::lowerThreshold() const
{
	return wmin;
}

#endif

