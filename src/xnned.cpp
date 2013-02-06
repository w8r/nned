#ifndef XNNED_CPP
#define XNNED_CPP

#include "xnned.h"

xnned::xnned(QWidget *parent)
    : QMainWindow(parent)
{
	net = new EDNetwork( 16 );
	
	net->loadPreferences( "Preferences/default.xml" );
	net->savePreferences( "Preferences/default.backup.xml" );
	tabs = new QTabWidget( this );
	training = new TrainDialog( net, this );
	treeView = new nedTreeView( net, this );
	connect( training, SIGNAL( netchanged() ), treeView, SLOT( build() ) );
	tabs->addTab( training, tr("Training") );
	tabs->addTab( treeView, tr("Network") );
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole( QPalette::Base );
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored );
    imageLabel->setScaledContents( true );
	resultLabel = new QLabel;
    resultLabel->setBackgroundRole( QPalette::Base );
    resultLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    resultLabel->setScaledContents( true );
		

	QWidget     *detectWidget = new QWidget( this );
	QGridLayout *detectLayout = new QGridLayout;
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole( QPalette::Dark );
    scrollArea->setWidget( imageLabel );
	scrollArea2 = new QScrollArea;
    scrollArea2->setBackgroundRole( QPalette::Dark );
    scrollArea2->setWidget( resultLabel );
	splitter = new QSplitter( Qt::Horizontal );
	splitter->addWidget( scrollArea );
	splitter->addWidget( scrollArea2 );
	detectLayout->addWidget( splitter, 0, 0, 2, 4 );
	detectWidget->setLayout( detectLayout );
	QLabel *openLabel = new QLabel( tr( "Open image:" ) );
	QPushButton *openButton = new QPushButton( tr( "&Open" ) );
	connect( openButton, SIGNAL( clicked() ), this, SLOT( open() ) );
	QPushButton *detectButton = new QPushButton( tr( "&Detect" ) );
	connect( detectButton, SIGNAL( clicked() ), this, SLOT( detect() ) );

	detectLayout->addWidget( openLabel, 2, 0, 1, 2 );
	detectLayout->addWidget( openButton, 2, 2 );
	detectLayout->addWidget( detectButton, 2, 3 );
	tabs->addTab( detectWidget, tr("Image") );
    setCentralWidget( tabs );

    createActions();
    createMenus();

    setWindowTitle(tr("Neural Network Edge Detector"));
    resize(600, 500);
}

void xnned::open()
{
    QString fileName = QFileDialog::getOpenFileName( this,
                                    tr( "Open File" ), QDir::currentPath()+"../Samples/", tr("Images (*.bmp)") );
    if ( !fileName.isEmpty() )
	{
		if( image != NULL )
			delete image;
        image = new QImage( fileName );
        if ( image->isNull() )
		{
            QMessageBox::information( this, tr( "Image Viewer" ),
                                     tr( "Cannot load %1." ).arg( fileName ));
            return;
        }
        imageLabel->setPixmap( QPixmap::fromImage( *image ) );
		
        scaleFactor = 1.0;
        printAct->setEnabled(true);
        fitToWindowAct->setEnabled(true);
        updateActions();

        if (!fitToWindowAct->isChecked())
		{
            imageLabel->adjustSize();
		}
    }
}

void xnned::detect()
{
	if( image != NULL)
	{
		GreyscaleWindow *gsw = new GreyscaleWindow( *image );
		*gsw = net->detect( *gsw );
		resultLabel->setPixmap( QPixmap::fromImage( gsw->toMonochromeImage() ) );
		if (!fitToWindowAct->isChecked())
		{
			resultLabel->adjustSize();
		}
		delete gsw;
	}
}

void xnned::savedetected()
{
	if( resultLabel->pixmap() )
	{
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                            QDir::currentPath() + "Detected/detected.bmp",
                            tr("Images (*.bmp)"));
		resultLabel->pixmap()->save( fileName );
	}
}

void xnned::print()
{
    Q_ASSERT(imageLabel->pixmap());
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
}

void xnned::zoomIn()
{
    scaleImage(1.25);
}

void xnned::zoomOut()
{
    scaleImage(0.8);
}

void xnned::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void xnned::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}


void xnned::about()
{
    QMessageBox::about(this, tr("About Neural Network Edge Detector Viewer"),
}

void xnned::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
	
    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setEnabled(false);
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));
	
	saveDetAct = new QAction( tr("Save detected"), this );
	saveDetAct->setShortcut( tr("Ctrl+D") );
	connect( saveDetAct, SIGNAL( triggered() ), this, SLOT( savedetected() ) );

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void xnned::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
	fileMenu->addAction(saveDetAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    //helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(helpMenu);
}

void xnned::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void xnned::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void xnned::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

#endif

