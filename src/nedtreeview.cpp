#include "nedtreeview.h"

nedTreeView::nedTreeView( EDNetwork *network, QWidget *parent )
    : QWidget(parent)
{
	net = network;
	tree = new QTreeWidget;
	QStringList lst;
	lst << tr("Network") << tr("Subnetworks") << tr("Neuron type") << tr("Upper threshold") << tr("Lower Threshold");
	tree->setHeaderLabels(lst);
	tree->setSortingEnabled(true);
	QGridLayout *treeLayout = new QGridLayout;
	treeLayout->addWidget( tree, 0, 0, 2, 2 );
	setLayout( treeLayout );
	build();
}

void nedTreeView::build()
{
	if( net != NULL )
	{
		tree->clear();
		QTreeWidgetItem *networkItem;
		networkItem = new QTreeWidgetItem( tree );
		networkItem->setText( 0, tr( "Network" ) );
		networkItem->setText( 1, QString().setNum( net->subnetworksCount() ) + tr( " subnetworks") );
		for( int i = 0; i < net->subnetworksCount(); i++ )
		{
			QTreeWidgetItem *subnetworkItem = new QTreeWidgetItem( networkItem );

			subnetworkItem->setText( 0, tr( "Subnetwork" ) );
			QString subText( "S" );
			subText += QString().setNum( i + 1 );
			subText += ": (";
			subText += QString().setNum( net->subnetwork( i ).illuminationPrototype() );
			subText += ")";
			subnetworkItem->setText( 1, subText );
			QTreeWidgetItem *neuronItem = new QTreeWidgetItem( subnetworkItem );
			neuronItem->setText( 0, tr( "Neuron" ) );
			neuronItem->setText( 2, tr("weak") );
			neuronItem->setText( 3, QString().setNum( net->subnetwork( i ).weakNeuron().upperThreshold() ) );
			neuronItem->setText( 4, QString().setNum( net->subnetwork( i ).weakNeuron().lowerThreshold() ) );
			neuronItem = new QTreeWidgetItem( subnetworkItem );
			neuronItem->setText( 0, tr( "Neuron" ) );
			neuronItem->setText( 2, tr("strong") );
			neuronItem->setText( 3, QString().setNum( net->subnetwork( i ).strongNeuron().upperThreshold() ) );
			neuronItem->setText( 4, QString().setNum( net->subnetwork( i ).strongNeuron().lowerThreshold() ) );
			tree->setItemExpanded( subnetworkItem, true );
		}
		tree->setItemExpanded( networkItem, true );
		networkItem->setSizeHint( 0, QSize( 300, 40 ) );
		tree->resizeColumnToContents( 0 );
	}
}
