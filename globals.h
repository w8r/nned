/*
 *  globals.h
 *  nned
 *
 *  Created by w8r on 4/22/08.

 *
 */
 #ifndef GLOBALS_H
 #define GLOBALS_H
 
 #include <QVector>
 #include <QFile>
 #include <QTextStream>
 #include <QtXml>

 
 #define N 3
 #define WHITE Qt::color1
 #define BLACK Qt::color0
 
 typedef QVector<uint>    GVector;
 typedef QVector<GVector> GMatrix;
 
 
 
inline double distance( const double a, const double b )
{
	return ( ( a > b ) ? ( a - b ) : ( b - a) );
}


 
 #endif

