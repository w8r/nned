#include <QApplication>
#include "xnned.h"

int main( int argc, char *argv[] )
{
    QApplication app(argc, argv);
	QTranslator qtTranslator; 
	QString filename("xn_ru.qm"); 
	filename = filename.toLower(); 
	qtTranslator.load( filename ); 
	app.installTranslator( &qtTranslator ); 
    xnned nnedInterface;
    nnedInterface.show();
    return app.exec();
}
