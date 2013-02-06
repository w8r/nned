#ifndef EDGECONFIGURATION_CPP
#define EDGECONFIGURATION_CPP 

#include "edgeconfiguration.h"

EdgeConfiguration::EdgeConfiguration()
{
	GVector temp;
	int     i;
	for( i = 0; i < N; i++ )
		temp.push_back( WHITE );
	for( i = 0; i < N; i++ )
		edge.push_back( temp );			
}

EdgeConfiguration::EdgeConfiguration( const GMatrix& matrix, const double& mmax, const double& mmin )
{
	if( ( matrix[0].size() == N ) && ( matrix.size() == N ) )
	{
		edge = matrix;
		for( int i = 0; i < N; i++ )
			for( int j = 0; j < N; j++ )
			{
				if( distance( edge[i][j], mmin) < distance( edge[i][j], mmax )  )
					edge[i][j] = BLACK;
				else
					edge[i][j] = WHITE;
			}
	}
}

EdgeConfiguration::EdgeConfiguration( const EdgeConfiguration& other )
{
	if( &other != this )
	{
		edge = other.edgemap();
	}
}
		
EdgeConfiguration& EdgeConfiguration::operator = ( const EdgeConfiguration& other )
{
	if( &other != this )
	{
		edge = other.edgemap();
	}
	return *this;
}
bool EdgeConfiguration::operator == ( const EdgeConfiguration& other ) const
{
	return ( edge == other.edgemap() );
}
bool EdgeConfiguration::operator != ( const EdgeConfiguration& other ) const
{
	return ( edge != other.edgemap() );
}
QImage EdgeConfiguration::toImage()
{
	QImage result( N, N, QImage::Format_MonoLSB );
	for( int i = 0; i < N; i++ )
		for( int j = 0; j < N; j++ )
			result.setPixel( i, j, edge[i][j] );
	return result;
}

EdgeConfiguration EdgeConfiguration::rotate45()
{
	/* центральный поворот */
	EdgeConfiguration cp( *this );	
	cp.set( 0, 0, edge[1][0] );
	cp.set( 0, 1, edge[0][0] );
	cp.set( 0, 2, edge[0][1] );
	cp.set( 1, 2, edge[0][2] );
	cp.set( 2, 2, edge[1][2] );
	cp.set( 2, 1, edge[2][2] );
	cp.set( 2, 0, edge[2][1] );
	cp.set( 1, 0, edge[2][0] );
	cp.set( 1, 1, edge[1][1] );
	return cp; 		
}
		
const GMatrix& EdgeConfiguration::edgemap() const
{
	return edge;
}

void EdgeConfiguration::set( int x, int y, uint value )
{
	if( x < N)
		if( y < N )
			edge[x][y] = value;
}
uint EdgeConfiguration::pixel( int x, int y )
{
	return edge[x][y];
}



#endif

