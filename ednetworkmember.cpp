/*
 *  ednetworkmember.cpp
 *  nned
 *
 *  Created by w8r on 4/28/08.

 *
 */
#ifndef EDNETWORKMEMBER_CPP
#define EDNETWORKMEMBER_CPP

#include "ednetworkmember.h"

QVector<EdgeConfiguration> EDNetworkMember::validEdges = QVector<EdgeConfiguration>();
EdgeConfiguration          EDNetworkMember::BlankEdge  = EdgeConfiguration(); 

long EDNetworkMember::trainIteration       = 0;
long EDNetworkMember::totalTrainIterations = 0; 

 
 EDNetworkMember::EDNetworkMember()
 {
 }
 
 EdgeConfiguration& EDNetworkMember::validate( EdgeConfiguration& edge ) const
 {
	/* если идёт обучение */
	if( totalTrainIterations != 0 )
	{	
		/* проверяемого участка нет в наборе */
		if( !isValid( edge ) )
			/* он добавляется в набор */
			validEdges.push_back( edge );
		/* поворачивается на 45 градусов 7 раз и 
		   добавляется в набор, если такие конфигурации 
		   не присутствовали в нём до этого */
		EdgeConfiguration tmp = edge.rotate45();
		for( int j = 0; j < 6; j++ )
		{
			if( !isValid( tmp ) )
				validEdges.push_back( tmp );
			tmp = tmp.rotate45();
		}
	}
	/* из набора выбирается соответствующая конфигурация
	   на неё возвращается ссылка */
	for( int i = 0; i < validEdges.size(); i++ )
	{
		if( edge == validEdges[i] )
			return validEdges[i];
	}
	/* если конфигурации в наборе не было, что может быть в режиме поиска,
	   но не обучения, то возвращается ссылка на пустую конфигурацию */
	return BlankEdge; 
 }
 
 bool EDNetworkMember::isValid( EdgeConfiguration& edge ) const
 {
	for( int i = 0; i < validEdges.size(); i++ )
		if( edge == validEdges[i] )
				return true;
	return false;	
 }
 
 EdgeConfiguration& EDNetworkMember::blankEdge()
 {
	return BlankEdge;
 }
 
 double EDNetworkMember::eta( int i, int tf )
 {
	if( i == 0 )
		return 1.0;
	else
		return ( eta( 0, tf ) * (1 - ( i / tf ) ) );
 }
 double EDNetworkMember::eta()
 {
	if( totalTrainIterations != 0 )
	{
	 	return ( eta( 0, totalTrainIterations ) * ( 1 - trainIteration / totalTrainIterations ) );
	}
	else
		return 0;
 }
int EDNetworkMember::memorySize()
{
	return validEdges.size();
}

EdgeConfiguration& EDNetworkMember::prototype( int num )
{
	if( num < validEdges.size() )
		return validEdges[num];
	else
		return BlankEdge;
}

#endif
