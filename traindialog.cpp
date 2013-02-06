/*
    traindialog.cpp
*/
#ifndef TRAINDIALOG_CPP
#define TRAINDIALOG_CPP

#include "traindialog.h"

TrainDialog::TrainDialog( EDNetwork* Network, QWidget *parent)
    : QDialog(parent)
{
	net = Network;
		slider = new QSlider( Qt::Horizontal );
    subnetworksLabel = new QLabel( tr("Subnetworks: ") );
	subnetworksCountLabel = new QLabel( "32" );

    slider->setRange( 0, 128 );
    slider->setPageStep( 2 );
    slider->setValue( 32 );
    slider->setTickInterval( 2 );
    slider->setTickPosition( QSlider::TicksBelow );
    QObject::connect( slider, SIGNAL( valueChanged( int ) ), 
                      subnetworksCountLabel, SLOT( setNum( int ) ) );
	browseButton = createButton( tr( "&Browse for images..." ), SLOT( browse() ) );
	trainButton  = createButton( tr( "&Train network" ),        SLOT( train() ) );
	resetButton = createButton( tr( "Reset network" ),          SLOT( reset() ) );
	browseForSettingsButton = createButton( tr( "&Load settings..." ), SLOT( browseForSettings() ) );
	loadSettingsLabel = new QLabel( tr("File not selected") );
	saveSettingsButton = createButton( tr( "&Save Training results..." ), SLOT( saveSettings() ) );
	saveSettingsLabel = new QLabel( tr( "File not selected" ) );
    directoryComboBox = createComboBox( QDir::currentPath() );

    directoryLabel  = new QLabel(tr("In directory:"));
    filesFoundLabel = new QLabel;

    createFilesTable();

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
	buttonsLayout->addWidget( resetButton );
    buttonsLayout->addWidget( trainButton );

    QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget( subnetworksLabel, 0, 0 );
    mainLayout->addWidget( slider, 0, 1, 1, 2 );
    mainLayout->addWidget( subnetworksCountLabel, 0, 3 );
    mainLayout->addWidget( directoryLabel, 1, 0 );
    mainLayout->addWidget( directoryComboBox, 1, 1 );
    mainLayout->addWidget( browseButton, 1, 2 );
    mainLayout->addWidget( filesTable, 2, 0, 2, 4 );
    mainLayout->addWidget( filesFoundLabel, 4, 0 );
    mainLayout->addLayout( buttonsLayout, 4, 1, 1, 3 );
	mainLayout->addWidget( browseForSettingsButton, 5, 0 );
	mainLayout->addWidget( loadSettingsLabel, 5, 1 );
	mainLayout->addWidget( saveSettingsButton, 5, 2 );
	mainLayout->addWidget( saveSettingsLabel, 5, 3 );
    setLayout( mainLayout );

    setWindowTitle( tr( "Find Files" ) );
    resize( 700, 300 );    
}

void TrainDialog::browse()
{
	files = QFileDialog::getOpenFileNames( this,
                         tr("Select one or more files to open"),
                         QDir::currentPath() + "/Samples/",
                         tr("Images (*.bmp)") );
	showFiles( files );
}

void TrainDialog::train()
{
	if( ( !subnetworksCountLabel->text().isEmpty() ) && ( filesTable->rowCount() != 0 ) )
	{
		if( net != NULL )
			delete net;
		net = new EDNetwork( subnetworksCountLabel->text().toInt() );
		QVector<GreyscaleWindow> images;
		//fl = new QLabel;
		//fl->setText( filesTable->item( 0, 1 )->text() );
		//fl->show();
		for( int i = 0; i < filesTable->rowCount(); i++ )
			images.push_back( GreyscaleWindow( QImage( filesTable->item( i, 1 )->text() ) ) );
		net->train( images );
		filesFoundLabel->setText( tr( "%1" ).arg( net->subnetworksCount() ) );
		saveSettingsLabel->setText( tr( "Settings not saved" ) );
		filesTable->clearContents();
		filesTable->setRowCount( 0 );
		emit netchanged();
	}	
}

void TrainDialog::browseForSettings()
{
	QString fileName = QFileDialog::getOpenFileName( this,
													 tr("Select settings file"),
													 QDir::currentPath() + "Preferences/",
													 tr("Settings files (*.xml)") );
	//QLabel *l = new QLabel;
	//l->setText( fileName );
	//l->show();
	if( net == NULL )
		net = new EDNetwork( fileName );
	else
		net->loadPreferences( fileName );
	loadSettingsLabel->setText( tr( "settings loaded" ) );
	emit netchanged();
}

void TrainDialog::saveSettings()
{
	QString fileName = QFileDialog::getSaveFileName( this,
													 tr("Save settings file"),
													 "Preferences/preferences.xml",
													 tr("Settings files (*.xml)"), 0, QFileDialog::DontConfirmOverwrite );
	//QLabel *l = new QLabel;
	if( net != NULL )
		if( net->isTrained() )
		{
			net->savePreferences( fileName );
			saveSettingsLabel->setText( "...saved" );
		}
}
void TrainDialog::reset()
{
	if( net != NULL)
		delete net;
	net = new EDNetwork( subnetworksCountLabel->text().toInt() );
	emit netchanged();
}

QPushButton* TrainDialog::createButton( const QString &text, const char *member )
{
    QPushButton *button = new QPushButton( text );
    connect( button, SIGNAL( clicked() ), this, member );
    return button;
}

QComboBox* TrainDialog::createComboBox( const QString &text )
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable( true );
    comboBox->addItem( text );
    comboBox->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred );
    return comboBox;
}

void TrainDialog::createFilesTable()
{
    filesTable = new QTableWidget( 0, 4 );
    QStringList labels;
    labels << tr( "Preview" ) << tr( "File Name" ) << tr( "Size(KB)" ) << tr( "Size(px)" );
    filesTable->setHorizontalHeaderLabels( labels );
    filesTable->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    filesTable->verticalHeader()->hide();
    filesTable->setShowGrid( false );
}
void TrainDialog::showFiles( const QStringList &files )
{
    for( int i = 0; i < files.size(); i++ )
	{
        QFile file( files[i] );
        qint64 size = QFileInfo( file ).size();
        QTableWidgetItem *fileNameItem = new QTableWidgetItem( files[i] );
        fileNameItem->setFlags( Qt::ItemIsEnabled );
        QTableWidgetItem *sizeItem = new QTableWidgetItem(tr("%1 KB")
                                             .arg(int((size + 1023) / 1024)));
		sizeItem->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
        sizeItem->setFlags( Qt::ItemIsEnabled );
		QImage tIm( files[i] );
		QIcon tIc( QPixmap::fromImage( tIm ) );
		QTableWidgetItem *imageIconItem = new QTableWidgetItem( tIc, "" );
        imageIconItem->setFlags( Qt::ItemIsEnabled );
		QTableWidgetItem *imageSizeItem = new QTableWidgetItem( tr( "%1 x %2" ).arg( tIm.width() ).arg( tIm.height() ) );
        imageSizeItem->setFlags( Qt::ItemIsEnabled );

        int row = filesTable->rowCount();
        filesTable->insertRow( row );
		filesTable->setItem( row, 0, imageIconItem );
        filesTable->setItem( row, 1, fileNameItem );
		filesTable->setItem( row, 2, imageSizeItem );
        filesTable->setItem( row, 3, sizeItem );
    }
    filesFoundLabel->setText( tr( "%1 file(s) found" ).arg( files.size() ) );
}

#endif

