/*
 *  dtneuron.cpp
 *  nned
 *
 *  Created by w8r on 4/24/08.

 *
 */
#ifndef DTNEURON_CPP
#define DTNEURON_CPP

#include "dtneuron.h"

 DTNeuron::DTNeuron( const GreyscaleWindow& img ) : illuminationLevel( img.averageGreyLevel() ),
													upperThreshold( img.upperGreyLevel() ),
													lowerThreshold( img.lowerGreyLevel() )
 {
 }
 QVector<EdgeConfiguration> DTNeuron::detect( QVector<GreyscaleWindow>& env )
 {
	QVector<EdgeConfiguration> eVec;
	for( int i = 0; i < env.size(); i++ )
	{
		EdgeConfiguration tmp( env[i].matrix(), upperThreshold, lowerThreshold );
		if( B1( tmp ) && B2( env[i].upperGreyLevel(), env[i].lowerGreyLevel() ) )
		{
			updateIlluminationLevel( env[i].averageGreyLevel(), i, env.size() );
			if( A2( env[i].upperGreyLevel(), env[i].lowerGreyLevel(), upperThreshold, lowerThreshold ) )
				updateThresholds( env[i].upperGreyLevel(), env[i].lowerGreyLevel(), i, env.size() );
			eVec.push_back( tmp );
		}
		else
			eVec.push_back( blankEdge() );
	}
	return eVec;
 }
 
 bool DTNeuron::B1( EdgeConfiguration& edge )
 {
	if( validate( edge ) != blankEdge() )
		return true;
	else
		return false;
 }
 
 bool DTNeuron::B2( double mmax, double mmin )
 {
	if( ( illuminationLevel <= mmax ) && ( illuminationLevel >= mmin ) )
		return true;
	else
		return false;
 }
 
 bool DTNeuron::A2( double mmax, double mmin, double wmax, double wmin )
 {
	if( ( mmax - mmin ) < ( wmax - wmin ) )
		return false;
	else
		return true;	
 }
 
 void DTNeuron::updateIlluminationLevel( double ms, int i, int tf )
 {
	illuminationLevel += ( ms - illuminationLevel) * eta( i, tf );
 }
 
 void DTNeuron::updateThresholds( double mmax, double mmin, int i, int tf )
 {
	upperThreshold += ( upperThreshold - mmax ) * eta( i, tf );
	lowerThreshold += ( lowerThreshold - mmin ) * eta( i, tf );
 }


#endif
