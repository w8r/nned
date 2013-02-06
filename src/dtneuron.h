 #ifndef DTNEURON_H
 #define DTNEURON_H
 
 #include <QVector>
 #include "globals.h" 
 #include "ednetworkmember.h"
 #include "greyscalewindow.h"
 #include "edgeconfiguration.h"
 
 class DTNeuron: public EDNetworkMember
 {
	public:
		DTNeuron( const GreyscaleWindow& img );
		QVector<EdgeConfiguration> detect( QVector<GreyscaleWindow> & env );
	private:
		/* набор условий для вторичных участков контура */
		bool B1( EdgeConfiguration& edge );
		bool B2( double mmax, double mmin );
		bool A2( double mmax, double mmin, double wmax, double wmin );
		
		void updateIlluminationLevel( double ms, int i, int tf );
		void updateThresholds( double mmax, double mmin, int i, int tf );
		
		/* прототип уровня освещённости */
		double illuminationLevel;
		/* верхнее пороговое значение */
		double upperThreshold;
		/* нижнее пороговое значение */
		double lowerThreshold;
 };
 
 #endif
 

