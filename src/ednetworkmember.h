 #ifndef EDNETWORKMEMBER_H
 #define EDNETWORKMEMBER_H
 
 #include <QVector>
 #include "globals.h"
 #include "edgeconfiguration.h"
 
 class EDNetworkMember
 {
	public:
		EDNetworkMember();
		bool               isValid( EdgeConfiguration& edge ) const;
		EdgeConfiguration& validate( EdgeConfiguration& edge ) const;
		EdgeConfiguration& blankEdge();
		double			   eta( int i, int tf );
		/* для элементов сети, не имеющих доступа к информации 
		   об итерациях обучения */
		double			   eta();
		int				   memorySize();
		EdgeConfiguration& prototype( int num );
				
	protected:
		/* память правильных конфигураций контура */
		static QVector<EdgeConfiguration> validEdges;
		/* пустая контурная конфигурация */
		static EdgeConfiguration		  BlankEdge;
		
		/* текущий цикл обучения
		   инкрементируется нейроном, обрабатывающим участок, подсети
		   при коррекции прототипа освещённости не изменяют номера цикла,
		   так как это бы сбило прогрессию корректирующего множителя */
		static long   trainIteration;
		/* общее число итераций обучения */
		static long   totalTrainIterations;
		
	private:
		
 };
 
 #endif
 
 

