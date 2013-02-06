#ifndef XNNED_H
#define XNNED_H

#include <QPrinter>
#include <QTextEdit>
#include <QMainWindow>
#include <QtGui>

#include "ednetwork.h"
#include "traindialog.h"
#include "nedtreeview.h"

class xnned : public QMainWindow
{
    Q_OBJECT

public:
    xnned( QWidget *parent = 0 );
	EDNetwork* network() { return net; };

private slots:
    void open();
	void detect();
	void savedetected();
    void print();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();

private:
    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage( double factor );
    void adjustScrollBar( QScrollBar *scrollBar, double factor );
	
	EDNetwork   *net;
	QImage      *image;

	QTabWidget  *tabs;
	TrainDialog *training;
	nedTreeView *treeView;
	QSplitter   *splitter;
    QLabel      *imageLabel;
	QLabel      *resultLabel;
    QScrollArea *scrollArea;
    QScrollArea *scrollArea2;
    double      scaleFactor;

    QPrinter printer;

    QAction *openAct;
	QAction *saveDetAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
	
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
};


#endif
