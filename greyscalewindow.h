/*
 *  greyscalewindow.h
 *  nned
 *
 *  Created by w8r on 4/23/08.

 *
 */
#ifndef GREYSCALEWINDOW_H
#define GREYSCALEWINDOW_H

#include "globals.h"
#include <QImage>
#include <QColor>


class GreyscaleWindow
{
	public:
		GreyscaleWindow();
		GreyscaleWindow( const QImage& img );
		GreyscaleWindow( const GMatrix& img );
		GreyscaleWindow( const GreyscaleWindow& other );
		uint           pixel( int x, int y );
		void		   setPixel( int x, int y, uint value );
		double         upperGreyLevel() const;
		double         lowerGreyLevel() const;
		double         averageGreyLevel() const;
		int			   width() const;
		int			   height() const;
		const GMatrix& matrix() const;
		GreyscaleWindow& operator = ( const GreyscaleWindow& other );
		QImage         toImage();
		QImage         toMonochromeImage();
	private:
		GMatrix GreyscaleMatrix;
		double  ugl;
		double  lgl;
		double  agl;
		void    calculateLevels();
};

#endif