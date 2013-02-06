 #ifndef EDSUBNETWORK_H
 #define EDSUBNETWORK_H
 
 #include "globals.h"
 #include "ednetworkmember.h"
 #include "neuron.h"
 #include "greyscalewindow.h"
  
 class EDSubnetwork: public EDNetworkMember
 {
	public:
		EDSubnetwork();
		EDSubnetwork( double illuminationLevelPrototype,
					  double neuron1UpperThreshold,
					  double neuron1LowerThreshold,
					  double neuron2UpperThreshold,
					  double neuron2LowerThreshold );
		EDSubnetwork( const EDSubnetwork& other );
		~EDSubnetwork();
		EDSubnetwork& operator = ( const EDSubnetwork& other );
		Neuron& neuronCompetition( GreyscaleWindow& win );
		EdgeConfiguration& detect( GreyscaleWindow& win );
		double illuminationPrototype() const;
		Neuron& weakNeuron() const;
		Neuron& strongNeuron() const;
	private:
		bool A2( double mmax, double mmin );
		bool A3( EdgeConfiguration& edge );
		Neuron *strong;
		Neuron *weak;
		double pr;
};
 
 
 #endif

