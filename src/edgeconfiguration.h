 #ifndef EDGECONFIGURATION_H
 #define EDGECONFIGURATION_H
 
 #include <QVector>
 #include <QImage>
 #include <QColor> 
 
 #include "globals.h"  
 
 class EdgeConfiguration
 {
	public:
		EdgeConfiguration();
		/* создаёт конфигурацию контура по матрице уровней яркости и пороговым значениям */
		EdgeConfiguration( const GMatrix& matrix, const double& mmax, const double& mmin );
		/* копирование */
		EdgeConfiguration( const EdgeConfiguration& other );
		
		/* присваивание и сравнение - для проверки контура и создания библиотеки образцов */
		EdgeConfiguration& operator = ( const EdgeConfiguration& other );
		bool               operator == ( const EdgeConfiguration& other ) const;
		bool               operator != ( const EdgeConfiguration& other ) const;
		
		/* доступ к матрице контура */
		const GMatrix&     edgemap() const;
		void               set( int x, int y, uint value );
		uint			   pixel( int x, int y );
		/* контурная карта в виде изображения */
		QImage             toImage();
		/* поворот на 45 градусов */
		EdgeConfiguration  rotate45();
	private:
		/* матрица контура */
		GMatrix edge;
 };
 
#endif

