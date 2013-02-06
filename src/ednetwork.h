#ifndef EDNETWORK_H
#define EDNETWORK_H

#include "globals.h"
#include "greyscalewindow.h"
#include "ednetworkmember.h"
#include "edsubnetwork.h"
#include "neuron.h"
#include "dtneuron.h"

class EDNetwork : public EDNetworkMember
{
	public:
		EDNetwork( int subnetworksCount );
		EDNetwork( const QString& fileName );		
		EDSubnetwork*            subnetworkCompetition( GreyscaleWindow& win );
		GreyscaleWindow          detect( GreyscaleWindow& img );
		QVector<GreyscaleWindow> train( QVector<GreyscaleWindow>& imgs );
		void					 savePreferences( const QString& fileName );
		void					 loadPreferences( const QString& fileName );
		int						 subnetworksCount();
		bool					 isTrained();
		const EDSubnetwork&      subnetwork( int i );
	private:
		/* функции для преобразования размеров изображения под разбиение на участки NxN */
		GreyscaleWindow			   resize( GreyscaleWindow& img );
		GreyscaleWindow			   crop( GreyscaleWindow& img );
		/* выявление первичных участков контура */
		void					   detectPrimaryEdgePoints( GreyscaleWindow& img );
		/* выявление вторичных участков контура */
		void					   detectSecondaryEdgePoints();
		QVector<GreyscaleWindow>&  getPrimaryPointEnvironment( int number, QVector<GreyscaleWindow>& env );
		void					   setEnvironmentPointsNumbers( int number, QVector<int>& nums );
		void					   setPrimaryPointEnvironment( int number, QVector<EdgeConfiguration> env );
		void					   drawEdges( GreyscaleWindow& img );		
		
		int						   widthIncrement;
		int						   heightIncrement;
		int						   edgeTableRows;
		int						   edgeTableCols;
		QVector<EDSubnetwork>	   subnetworks;
		DTNeuron*				   dynamicNeuron;
		QSize			           resizedImageSize;		
		QVector<EdgeConfiguration> edgeMaps;
		QVector<GreyscaleWindow>   greyscaleWindows;
		GVector					   primaryEdgePoints;
};


#endif

