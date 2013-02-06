#ifndef NEDTREEVIEW_H
#define NEDTREEVIEW_H

#include <QWidget>
#include <QtGui>
#include <QTreeWidget>

#include "ednetwork.h"

class nedTreeView : public QWidget
{
    Q_OBJECT

public:
    nedTreeView(EDNetwork *network, QWidget *parent);
	
public slots:
	void build();
	
private:
	EDNetwork *net;
	QTreeWidget *tree;
};

#endif
