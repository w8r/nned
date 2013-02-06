/*
 *  neoron.h
 *  nned
 *
 *  Created by w8r on 4/18/08.

 *
 */
 
#ifndef NEURON_H
#define NEURON_H

#include <QImage>
#include <QBitmap>
#include <QColor>

#include "globals.h"
#include "greyscalewindow.h"
#include "edgeconfiguration.h"
#include "ednetworkmember.h"

class Neuron : public EDNetworkMember
{	
	public:
		/* конструктор с пороговыми значениями */
		Neuron( double w1, double w2 );
		/* выделение контура */
		EdgeConfiguration detect( GreyscaleWindow& imgWindow );
		/* пороговые параметры нейрона */
		double upperThreshold() const;
		double lowerThreshold() const;
		
	private:		
		double            wmax, wmin;
		GMatrix           edgeMap;
		EdgeConfiguration QMapping( GreyscaleWindow& imgWindow );
};

#endif


