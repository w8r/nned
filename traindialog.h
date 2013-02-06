/*
    traindialog.h
*/

#ifndef TRAINDIALOG_H
#define TRAINDIALOG_H

#include <QDialog>
#include <QtGui>

#include "ednetwork.h"
#include "greyscalewindow.h"

class TrainDialog : public QDialog
{
    Q_OBJECT

public:
    TrainDialog( EDNetwork* Net, QWidget* parent = 0 );
	
signals:
	void netchanged();
	
private slots: 
	void browse();
    void train();
	void reset();
	void browseForSettings();
	void saveSettings();
	
private:
	QPushButton *createButton( const QString &text, const char *member );
    QComboBox *createComboBox( const QString &text = QString() );
    void createFilesTable();
	void showFiles( const QStringList& files );
	
	QComboBox    *directoryComboBox;
    QLabel       *fileLabel;
    QLabel       *textLabel;
    QLabel       *directoryLabel;
    QLabel       *filesFoundLabel;
	QLabel       *subnetworksLabel;
	QLabel       *subnetworksCountLabel;
    QPushButton  *browseButton;
    QPushButton  *trainButton;
	QPushButton  *resetButton;
	QPushButton  *browseForSettingsButton;
	QLabel       *loadSettingsLabel;
	QPushButton  *saveSettingsButton;
	QLabel       *saveSettingsLabel;
    QTableWidget *filesTable;
	QSlider      *slider;
	
	QStringList   files;
	EDNetwork    *net;
	QLabel       *fl;
};

#endif
