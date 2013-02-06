/*
 *  greyscalewindow.cpp
 *  nned
 *
 *  Created by w8r on 4/23/08.

 *
 */
 
#ifndef GREYSCALEWINDOW_CPP
#define GREYSCALEWINDOW_CPP

#include "greyscalewindow.h"

GreyscaleWindow::GreyscaleWindow()
{
}

GreyscaleWindow::GreyscaleWindow( const GMatrix& img )
{
	GreyscaleMatrix =  img;
		
	if( ( img.size() == N ) && ( img[0].size() == N ) )
		calculateLevels();
	else
	{
		agl = ugl = lgl = 0;
	}
}

GreyscaleWindow::GreyscaleWindow( const QImage& img )
{
	/*
	QFile log( "greyscale.txt" );
	log.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append );
	QTextStream out( &log );
	out << "\t **** Constructing greyscale map from image (" << img.width()
		<< "x" << img.height() << "): \n";
	*/
	for( int i = 0; i < img.height(); i++ )
	{
		//out << "\n line " << i << ":\n";
		GVector tmp;
		for( int j = 0; j < img.width(); j++ )
		{
			tmp += qGray( img.pixel( j, i ) );
			//out << tmp.last() << ",";
		}
		GreyscaleMatrix.push_back( tmp );
	}
	if( ( img.height() == N ) && ( img.width() == N ) )
		calculateLevels();
	else
	{
		agl = ugl = lgl = 0;
	}
	/*
	out<<"successful. GreyscaleWindow created (" << width() << "x" << height() << ")\n";
	log.close();
	*/
}
GreyscaleWindow::GreyscaleWindow( const GreyscaleWindow& other )
{
	if( this != &other )
	{
		agl = other.averageGreyLevel();
		ugl = other.upperGreyLevel();
		lgl = other.lowerGreyLevel();
		GreyscaleMatrix = other.matrix();
	}
}

double GreyscaleWindow::upperGreyLevel() const
{
	return ugl;
}

double GreyscaleWindow::lowerGreyLevel() const
{
	return lgl;
}

double GreyscaleWindow::averageGreyLevel() const
{
	return agl;
}

int GreyscaleWindow::width() const
{
	if( !GreyscaleMatrix.empty() )
		return GreyscaleMatrix[0].size();
	else
		return 0;
}
int GreyscaleWindow::height() const
{
	return GreyscaleMatrix.size();
}

uint GreyscaleWindow::pixel( int x, int y )
{
	return GreyscaleMatrix[x][y];
}

void GreyscaleWindow::setPixel( int x, int y, uint value )
{
	GreyscaleMatrix[x][y] = value;
}


const GMatrix& GreyscaleWindow::matrix() const
{
	return GreyscaleMatrix;
}

GreyscaleWindow& GreyscaleWindow::operator = ( const GreyscaleWindow& other )
{
	if( this != &other )
	{
		agl = other.averageGreyLevel();
		ugl = other.upperGreyLevel();
		lgl = other.lowerGreyLevel();
		GreyscaleMatrix = other.matrix();
	}
	return *this;
}

QImage GreyscaleWindow::toImage()
{
	QImage res( GreyscaleMatrix[0].size(), GreyscaleMatrix.size(), QImage::Format_RGB32 );
	for( int i = 0; i < res.height(); i++ )
		for( int j = 0; j < res.width(); j++ )
		{
			QColor c;
			c = c.fromHsv( 20, 0, GreyscaleMatrix[i][j] );
			res.setPixel( j, i, c.toRgb().rgb() );
		}
	return res;
}

QImage GreyscaleWindow::toMonochromeImage()
{
	QImage res( GreyscaleMatrix[0].size(), GreyscaleMatrix.size(), QImage::Format_Mono );
	for( int i = 0; i < res.height(); i++ )
		for( int j = 0; j < res.width(); j++ )
		{
			res.setPixel( j, i, GreyscaleMatrix[i][j] );
		}
	return res;
}

void GreyscaleWindow::calculateLevels()
{
	long double sum1 = 0;
	long double sum2 = 0;
	int i, j;
	int upperCount = 0;
	int lowerCount = 0;

	for( i = 0; i < N; i++ )
		for( j = 0; j < N; j++ )
			sum1 += GreyscaleMatrix[i][j];
			
	agl = sum1 / ( N * N );
	
	sum1 = 0;

	for( i = 0; i < N; i++ )
		for( j = 0; j < N; j++ )
		{
			if( GreyscaleMatrix[i][j] < agl )
			{
				sum2 += GreyscaleMatrix[i][j];
				lowerCount++;
			}
			else
			{
				sum1 += GreyscaleMatrix[i][j];
				upperCount++;
			}
		}
	ugl = ( upperCount > 0 ) ? ( sum1 / upperCount ) : agl;
	lgl = ( lowerCount > 0 ) ? ( sum2 / lowerCount ) : agl;
}


#endif

