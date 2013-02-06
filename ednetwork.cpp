/*
 *  ednetwork.cpp
 *  nned
 *
 *  Created by w8r on 4/22/08.
 *
 */
#ifndef EDNETWORK_CPP
#define EDNETWORK_CPP

#include "ednetwork.h"

QDomElement makeElement( QDomDocument&  domDoc, 
                         const QString& strName,
						 const QString& strAttrName = QString::null,
                         const QString& strAttr = QString::null,
                         const QString& strText = QString::null
                       )
{
    QDomElement domElement = domDoc.createElement( strName );
	/* если атрибут номера задан */
    if( !strAttr.isEmpty() )
	{
        QDomAttr domAttr = domDoc.createAttribute( strAttrName );
        domAttr.setValue( strAttr );
        domElement.setAttributeNode( domAttr );
    }

    if ( !strText.isEmpty() )
	{
		QDomText domText = domDoc.createTextNode( strText );
		domElement.appendChild( domText );
    }
    return domElement;
}

QDomElement domNeuron( QDomDocument&  domDoc,
					   const QString& neuronType,
					   const double   upperThreshold,
					   const double   lowerThreshold )
{
	QDomElement domWeakNeuron = makeElement( domDoc, "neuron", "type", neuronType );
	domWeakNeuron.appendChild( makeElement( domDoc, "upperthreshold", "", "", QString().setNum( upperThreshold ) ) );
	domWeakNeuron.appendChild( makeElement( domDoc, "lowerthreshold", "", "", QString().setNum( lowerThreshold ) ) );
	return domWeakNeuron;
}

QDomElement domSubnetwork( QDomDocument&  domDoc, 
						   const double   illuminationLevel,
						   const double   weakNeuronUpperThreshold,
						   const double   weakNeuronLowerThreshold,
						   const double   strongNeuronUpperThreshold,
						   const double   strongNeuronLowerThreshold )
{
    QDomElement domSubnetworkElement = makeElement( domDoc, 
												    "subnetwork", 
													"illumination", 
													QString().setNum( illuminationLevel ) );
	QDomElement domWeakNeuron = domNeuron( domDoc, 
										   "weak", 
										   weakNeuronUpperThreshold, 
										   weakNeuronLowerThreshold );
	domSubnetworkElement.appendChild( domWeakNeuron );
	QDomElement domStrongNeuron = domNeuron( domDoc, 
											 "strong", 
											 strongNeuronUpperThreshold, 
											 strongNeuronLowerThreshold );
	domSubnetworkElement.appendChild( domStrongNeuron );
	return domSubnetworkElement;
}

EDNetwork::EDNetwork( int subnetworksCount )
{
	/*
	QFile log( "netw.txt" );
	log.open( QIODevice::Text | QIODevice::WriteOnly );
	QTextStream out( &log );
	*/
	double tmp           = 0.0;
	double gradientWidth = 256.0;
	double divider       = 4.0;
	double quantity      = subnetworksCount;
	//out << "\t **** Network construct: " << subnetworksCount << " subnetworks\n\n";
	for( int i = 0; i < subnetworksCount; i++ )
	{
		double n1lower = tmp;
		tmp += ( gradientWidth / ( quantity * divider ) );
		double n2lower = tmp;
		tmp += ( gradientWidth / ( quantity * divider ) );
		double illuminationPrototype = tmp;
		tmp += ( gradientWidth / ( quantity * divider ) );
		double n2upper = tmp;
		tmp += ( gradientWidth / ( quantity * divider ) );
		double n1upper = tmp;
		subnetworks.push_back( EDSubnetwork( illuminationPrototype, n1upper, n1lower, n2upper, n2lower ) );
		/*
		out << "\t Subnetwork " << i << "\n";
		out << "\t\t Illumination prototype: " << subnetworks.last().illuminationPrototype() << "\n";
		out << "\t\t Strong neuron: (" << subnetworks.last().strongNeuron().upperThreshold() << ","
			<< subnetworks.last().strongNeuron().lowerThreshold() << ")\n";
		out << "\t\t Weak neuron: (" << subnetworks.last().weakNeuron().upperThreshold() << ","
			<< subnetworks.last().weakNeuron().lowerThreshold() << ")\n\n";
		*/
	}
	//log.close();
}
EDNetwork::EDNetwork(const QString& fileName )
{
	loadPreferences( fileName );
}

EDSubnetwork* EDNetwork::subnetworkCompetition( GreyscaleWindow& win )
{
	/*
	QFile log( "comp.txt" );
	log.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append );
	QTextStream out( &log );
	*/
	
	EDSubnetwork* res = 0;
	double dist = 256;
	//out << "\t **** Subnetworks Competition: \n";
	for( int i = 0; i < subnetworks.size(); i++ )
	{
		if( ( subnetworks[i].illuminationPrototype() >= win.lowerGreyLevel() ) && 
			( subnetworks[i].illuminationPrototype() <= win.upperGreyLevel() ) )
		{			
			if( distance( subnetworks[i].illuminationPrototype(), win.averageGreyLevel() ) < dist )
			{
				dist = distance( subnetworks[i].illuminationPrototype(), win.averageGreyLevel() );
				res = &subnetworks[i];
				//out << win.averageGreyLevel() << " : " <<  subnetworks[i].illuminationPrototype() << "\n";
			}
		}		
	}
	//log.close();
	return res;
}

GreyscaleWindow EDNetwork::resize( GreyscaleWindow& img )
{
	/*
	QFile log( "size.txt" );
	log.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append );
	QTextStream out( &log );
	out << "\t **** Resize image: (" << img.width() << "x" << img.height() << ") -> ";
	*/
	widthIncrement = 0;
	heightIncrement = 0;
	
	int i, j;
	
	while( ( ( img.matrix().size() + heightIncrement ) % N ) != 0 )
		heightIncrement++;
	while( ( ( img.matrix()[0].size() + widthIncrement ) % N ) != 0 )
		widthIncrement++;
	
	GMatrix data = img.matrix();
	for( i = 0; i < data.size(); i++ )
		for( j = 0; j < widthIncrement; j++ )
			data[i].push_back( WHITE );
	for( i = 0; i < heightIncrement; i++ )
		data.push_back( GVector( data[0].size(), Qt::color0 ) );
	GreyscaleWindow res( data );
	/*
	out << "(" << res.width() << "x" << res.height() << "). successful\n";
	log.close();
	*/
	return res;
}
GreyscaleWindow EDNetwork::crop( GreyscaleWindow& img )
{
	/*
	QFile log( "size.txt" );
	log.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append );
	QTextStream out( &log );
	out << "\t ***** Crop image: (" << img.width() << "x" << img.height() << ") -> ";
	*/
	GMatrix data = img.matrix();
	while( heightIncrement )
	{
		data.pop_back();
		heightIncrement--;
	}
	for( int i = 0; i < data.size(); i++ )
	{
		data[i].resize( img.matrix()[i].size() - widthIncrement );
	}
	widthIncrement = 0;
	GreyscaleWindow res( data );
	/*
	out << "(" << res.width() << "x" << res.height() << "): successful.\n";
	log.close();
	*/
	return data;
}

GreyscaleWindow EDNetwork::detect( GreyscaleWindow& img )
{
	/*
	QFile log( "detect.txt" );
	log.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append );
	QTextStream out( &log );
	out << "\t **** Network: detection: \n";
	*/
	/* по необходимости - увеличение изображения для разбиения */
	GreyscaleWindow resizedImage = resize( img );
	/*
	out << "\t resize: " << img.width() << "x" << img.height()
		<< " >> " << resizedImage.width() << "x" << resizedImage.height() << "\n";
	*/
	/* сохранение его размеров - для удобства доступа */
	resizedImageSize.setWidth( resizedImage.width() );
	resizedImageSize.setHeight( resizedImage.height() );
	edgeTableRows = resizedImageSize.height() / N;
	edgeTableCols = resizedImageSize.width() / N;	
	/* выделение первичных участков контура подсетями */
	detectPrimaryEdgePoints( resizedImage );
	/*
	out << "\n" << greyscaleWindows.size() << " windows NxN, "
		<< edgeMaps.size() << " edge maps (should be "
		<< ( ( resizedImageSize.width() / N ) * ( resizedImageSize.height() / N ) )
		<< ")\n";
	*/
	/* выделение вторичных связанных с ними участков контура динамическим нейроном */
	detectSecondaryEdgePoints();
	drawEdges( resizedImage );
	/* отмена преобразования размеров исходного изображения */
	resizedImage = crop( resizedImage );
	/* очистка памяти */
	edgeMaps.clear();
	greyscaleWindows.clear();
	primaryEdgePoints.clear();
	/* результат - монохромная карта контура */
	/*
	out << "successful\n";
	log.close();
	*/
	return resizedImage;
}

QVector<GreyscaleWindow> EDNetwork::train( QVector<GreyscaleWindow>& imgs )
{
	/*
	QFile log( "train.txt" );
	log.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append );
	QTextStream out( &log );
	*/
	/* набор результатов обработки */
	QVector<GreyscaleWindow> res;
	/* активация процесса обучения */
	for( int i = 0; i < imgs.size(); i++ )
	{
		/* расчёт общего числа итераций обучения. это количество всех
		   обрабатываемых участков всех входящих в набор изображений,
		   в ином случае обучение на каждом новом образце полностью
		   уничтожало бы результаты предыдущего цикла обучения. таким
		   образом, конечный результат обучения зависит не только от 
		   количества и содержания образцов обучающего набора, но и от 
		   их порядка в наборе */
		long tmpw = 0;
		long tmph = 0;
		while( ( tmpw * N ) < imgs[i].width() )
			tmpw++;
		while( ( tmph * N) < imgs[i].height() )
			tmph++;
		totalTrainIterations += ( tmpw * tmph );
	}
	/*
	out << "\t **** Training: " << imgs.size() << " images in set, "
		<< totalTrainIterations << " training iterations\n";
	*/
	trainIteration = 0;
	for( int i = 0; i < imgs.size(); i++ )
	{
		/* по необходимости - увеличение изображения для разбиения */
		GreyscaleWindow resizedImage = resize( imgs[i] );
		/* сохранение его размеров - для удобства */
		resizedImageSize.setWidth( resizedImage.width() );
		resizedImageSize.setHeight( resizedImage.height() );
		edgeTableRows = resizedImageSize.height() / N;
		edgeTableCols = resizedImageSize.width() / N;	
		/* выделение первичных участков контура подсетями */
		detectPrimaryEdgePoints( resizedImage );
		/* выделение вторичных связанных с ними участков контура 
		   динамическим нейроном при обучении не проводится */
		/* отмена преобразования размеров исходного изображения */
		drawEdges( resizedImage );
		resizedImage = crop( resizedImage );
		/*
		out<< resizedImage.width() << "x" << resizedImage.height() << "\n";
		for( int m = 0; m < resizedImage.height(); m++ )
		{
			for( int n = 0; n < resizedImage.width(); n++ )
			{
				out << resizedImage.pixel( m, n );
			}
			out << "\n";
		}
		*/
		/* результат - монохромная карта контура */
		edgeMaps.clear();
		greyscaleWindows.clear();
		primaryEdgePoints.clear();
		res.push_back( resizedImage );
		//out << "\t **** stored \n";
	}
	/*
	out << "Result: " << validEdges.size() << " valid edge prototypes\n";	
	for( int j = 0; j < validEdges.size(); j++ )
	{
		out << "\n\t edge " << j << ": \n";
		for( int k = 0; k < N; k++ )
		{
			for( int l = 0; l < N; l++ )
			{
				out << validEdges[j].pixel( k, l ) << ".";
			}
			out << "\n";
		}
	}
	
	out << "\t **** successful";
	log.close();
	*/
	totalTrainIterations = 0;
	return res;		
}

void EDNetwork::detectPrimaryEdgePoints( GreyscaleWindow& img )
{
	/*
	QFile log( "primary.txt" );
	log.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append );
	QTextStream out( &log );
	out << "\t **** Network: primary points detection: ";
	*/
	int i, j, k;
	/* цикл с шагом N по строкам матрицы уровней яркости */
	for( i = 0; i < img.height(); i += N )
	{
		/* цикл с шагом N по столбцам матрицы уровней яркости */
		for( j = 0; j < img.width(); j += N )
		{
			/*
			out << "\n\t training Iteration " << trainIteration << "/" << totalTrainIterations;
			out << "\nwindow " << greyscaleWindows.size()
				<< ", (" << i << "," << j << "), ("
				<< i << "," << ( j + N ) << "), ("
				<< ( i + N ) << "," << ( j + N ) << "), ("
				<< ( i + N ) << "," << j << ")";
			*/
			GMatrix data;
			/* запись в матрицу подстрок длины N от позиции j строк 
			   матрицы уровней яркости длины N, начиная с i-ой строки */
			for( k = 0; k < N; k++ )
				data.push_back( img.matrix()[i + k].mid( j, N ) );
			/* создание участка по этой матрице */
			GreyscaleWindow currentWindow( data );
			/* сохранение его в стек участков */
			greyscaleWindows.push_back( currentWindow );			
			/* соревнование подсетей за участок */
			EDSubnetwork* Winner = subnetworkCompetition( currentWindow );
			/* запись в стек первичного участка контура, выделенного победителем */
			if( Winner != NULL )
				edgeMaps.push_back( Winner->detect( currentWindow ) );
			else
			{
				trainIteration++;
				EdgeConfiguration blank;
				edgeMaps.push_back( blank );
			}
		}
	}
	/*
	out << "\t **** primary edge detection complete";
	log.close();
	*/
}
void EDNetwork::detectSecondaryEdgePoints()
{
	/*
	QFile log( "sec.txt" );
	log.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append );
	QTextStream out( &log );
	out << "\t **** Secondary points detection: \n";
	*/
	int x = 0;
	int y = 0;
	for( int i = 0; i < edgeMaps.size(); i++ )
	{
		if( x == ( resizedImageSize.width() / N ) )
		{
			x = 0;
			y++;
		}
		/* найден первичный участок контура */
		if( edgeMaps[i] != blankEdge() )
		{
			//out << "Edge detected in window " << i << "(" << y << ", " << x << ")\n";
			/* создание стека его окружения */
			QVector<GreyscaleWindow> env;
			/* определение координат существующих участков окружения */
			getPrimaryPointEnvironment( i, env );
			/* инициализация динамического нейрона параметрами первичного участка */
			dynamicNeuron = new DTNeuron( greyscaleWindows[i] );
			/* запись результатов обработки стека динамическим нейроном */
			setPrimaryPointEnvironment( i, dynamicNeuron->detect( env ) );
			/* очистка параметров динамического нейрона */
			delete dynamicNeuron;
		}
		x++;
	}
	//log.close();
}

QVector<GreyscaleWindow>& EDNetwork::getPrimaryPointEnvironment( int number, QVector<GreyscaleWindow>& env )
{
	/*
	QFile log( "getenv.txt" );
	log.open( QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append );
	QTextStream out( &log );
	out << "\n\t **** get primary point environment for point " << number << "\n";
	*/
	env.clear();
	QVector<int> numbers( 8, -1 );
	/* получение координат окружения */
	setEnvironmentPointsNumbers( number, numbers );
	//out << "Env: ";
	for( int i = 0; i < numbers.size(); i++ )
		/* участок окружения существует */
		if( ( numbers[i] != -1 ) )
		{
			//out << numbers[i] <<",";
			/* запись его в стек */
			env.push_back( greyscaleWindows[numbers[i]] );
		}
	//log.close();
	return env;
}
void EDNetwork::setEnvironmentPointsNumbers( int number, QVector<int>& nums )
{
	/*
			1 2 3
			0 x 4
			7 6 5
	*/
	/* вычисление координат участка в матрице участков */
	/*
	QFile log( "setenvnums.txt" );
	log.open( QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append );
	QTextStream out( &log );
	*/
	int col = number;
	int row = 0;
	while( col > ( edgeTableCols - 1 ) )
	{
		col -= edgeTableCols;
		row++;
	}
	//out << "point " << number << "(" << row << "/" << edgeTableRows << ", " << col << "/" << edgeTableCols << ")\n";
	/* по умолчанию ни одного участка в окружении нет */
	/* у участка есть соседи слева */
	if( col > 0 )
	{
		nums[0] = number - 1;
		/* у участка есть соседи слева сверху */
		if( row > 0 )
		{
			nums[1] = ( number - 1 - edgeTableCols );
		}
		/* у участка есть соседи слева снизу */
		if( row < ( edgeTableRows - 1 ) )
		{
			nums[7] = ( number - 1 + edgeTableCols );
		}			
	}
	/* у участка есть соседи справа */
	if( col < ( edgeTableCols - 1 ) )
	{
		nums[4] = number + 1;
		/* у участка есть соседи справа сверху */
		if( row > 0 )
			nums[3] = ( number + 1 - edgeTableCols );
		/* у участка есть соседи справа снизу */
		if( row < ( edgeTableRows - 1 ) )
			nums[5] = ( number + 1 + edgeTableCols );
	}
	/* у участка есть соседи снизу */
	if( row < ( edgeTableRows - 1 ) )
		nums[6] = number + edgeTableCols;
	/* у участка есть соседи сверху */
	if( row > 0 )
		nums[2] = number - edgeTableCols;
	
	/*
	out << nums[1] << "." << nums[2] << "." << nums[3] << "\n";
	out << nums[0] << "." << number << "." << nums[4] << "\n";
	out << nums[7] << "." << nums[6] << "." << nums[5] << "\n";
	
	log.close();
	*/
}
void EDNetwork::setPrimaryPointEnvironment( int number, QVector<EdgeConfiguration> env )
{
	QVector<int> numbers( 8, -1 );
	/* получение координат участков окружения */
	setEnvironmentPointsNumbers( number, numbers );
	int i = 0;
	for( int j = 0; j < numbers.size(); j++ )
	{
		/* если такой участок был */
		if( numbers[j] != -1 )
		{
			/* записываем полученную для него карту контура в общую карту */
			if( edgeMaps[numbers[j]] == blankEdge() )
				edgeMaps[numbers[j]] = env[i];
			i++;
		}
	}
}

void EDNetwork::drawEdges( GreyscaleWindow& img )
{
	/*
	QFile log( "draw.txt" );
	log.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append );
	QTextStream out( &log );
	
	out << "Detection results: \n\t" << edgeMaps.size() << " windows processed\n";	
	*/
	int row  = 0;
	int col  = 0;
	/*
	out << "\n" << edgeTableRows << " rows\n";
	out << "\n" << edgeTableCols << " cols\n";
	*/
	
	while( row < edgeTableRows )
	{
		//out << " \nrow: " << row << ", ";
		while( col < edgeTableCols )
		{
			//out << " col: " << col << " ";
			for( int i = 0; i < N; i++ )
			{
				for( int j = 0; j < N; j++ )
				{
					img.setPixel( row * N + i, col * N + j, edgeMaps[row*edgeTableCols + col].pixel( i, j ) );
					//out << img.pixel(  row * N + i, col * N + j );
				}
				//out << "\n";
			}
			col++;
		}
		col = 0;
		row++;
	}
	/*
	out << "\n edges compiled, temporary edgemaps cleared\n";
	log.close();
	*/
}

void EDNetwork::savePreferences( const QString& fileName )
{
	int i, j, k;
	QDomDocument prefDoc;
    QDomElement  domPrefElement = prefDoc.createElement( "preferences" );	
	prefDoc.appendChild( domPrefElement );
	QDomElement domNetworkElement = makeElement( prefDoc, "network" );	
	domPrefElement.appendChild( domNetworkElement );

    for( i = 0; i < subnetworks.size(); i++ )
	{
		QDomElement domTempSub = domSubnetwork( prefDoc,
											    subnetworks[i].illuminationPrototype(),
												subnetworks[i].weakNeuron().upperThreshold(),
												subnetworks[i].weakNeuron().lowerThreshold(),
												subnetworks[i].strongNeuron().upperThreshold(),
												subnetworks[i].strongNeuron().lowerThreshold() );
		domNetworkElement.appendChild( domTempSub );
	}
	
	QDomElement  domValidEdgesElement = makeElement( prefDoc, "validedges" );
	domPrefElement.appendChild( domValidEdgesElement );
	for( i = 0; i < validEdges.size(); i++ )
	{
		QString edgeStr;
		for( j = 0; j < N; j++ )
			for( k = 0; k < N; k++ )
				edgeStr += QString().setNum( validEdges[i].pixel( j, k ) );
		QDomElement edgeElement = makeElement( prefDoc, "edgemap", "", "", edgeStr );
		domValidEdgesElement.appendChild( edgeElement );
	}
		
	QFile networkPrefs( fileName );
	if( networkPrefs.open( QIODevice::WriteOnly ) )
	{
		QTextStream( &networkPrefs ) << prefDoc.toString();
        networkPrefs.close();
	}
}
void EDNetwork::loadPreferences( const QString& fileName )
{
	subnetworks.clear();
	validEdges.clear();
	QDomDocument domDoc;
    QFile        file( fileName );
	QFile		 log( "readxml.txt" );
	log.open( QIODevice::WriteOnly | QIODevice::Text );
	QTextStream out( &log );

    if( file.open( QIODevice::ReadOnly ) )
	{
        if( domDoc.setContent( &file ) )
		{
            QDomElement domElement = domDoc.documentElement();
            QDomNode    domNode    = domElement.firstChild().firstChild();
			while( !domNode.isNull() )
			{
				EDSubnetwork tmp( domNode.toElement().attribute( "illumination", "" ).toDouble(),
								  domNode.firstChild().firstChild().toElement().text().toDouble(),
								  domNode.firstChild().firstChild().nextSibling().toElement().text().toDouble(),
								  domNode.firstChild().nextSibling().firstChild().toElement().text().toDouble(),
								  domNode.firstChild().nextSibling().firstChild().nextSibling().toElement().text().toDouble() );
				subnetworks += tmp;
				domNode = domNode.nextSibling();
			}
			domNode = domDoc.documentElement().firstChild().nextSibling();
			domNode = domNode.firstChild();
			while( !domNode.isNull() )
			{
				QString edgeNums = domNode.toElement().text();
				int     n        = edgeNums.size();
				int     posx     = 0;
				int     posy     = 0;
				EdgeConfiguration edge;
				while( n-- )
				{
					edge.set( posy, posx, QString( edgeNums[edgeNums.size() - 1 - n] ).toUInt() );
					if( ( ++posx % N ) == 0 )
					{
						posx = 0;
						posy++;
					}
				}
				validEdges += edge;	
				domNode = domNode.nextSibling();
			}			
        }
        file.close();
		log.close();
		savePreferences( "copy.xml" );
    }
}

int EDNetwork::subnetworksCount()
{
	return subnetworks.size();
}

bool EDNetwork::isTrained()
{
	if( validEdges.size() == 0 )
		return false;
	else
		return true;
}

const EDSubnetwork& EDNetwork::subnetwork( int i )
{
	return subnetworks[i];
}

#endif
