/*
 *	This file contains several static methods, used for evaluation and testing purposes,
 *	and serves as a temporary header file for some methods that are going to be used in the
 *	main FLOW Server project.
 */

#ifndef __TrackerUtilities__H__
#define __TrackerUtilities__H__

#include "fwTrackerPeerState.h"
#include <string>

using namespace std;

class FlowTree; //forward declaration

class Evaluation {
public:

	/* 
	 *	This method evaluates several aspects of the overlay network, by analyzing its infrastructure 
	 *	and exporting several statistical results.
	 */
	static void evaluate(FlowTree* tree, int numOfNodes);

};




/* An instance of this class contains the IDs of the Diffusion SubTrees to which a peer has no access. */
class MissingSwarmConnections {
public:
	int nodeTCPPort;				/* change to Node ID */
	vector<int> missingConnections;	/* IDs of the subtrees */

	MissingSwarmConnections(){
		nodeTCPPort = -1;
	}
};


class StateModification {
public:

	/** KIND OF MODIFICATION VALUE MEANING
	 *
	 * 0 = ADD diffParent
	 * 1 = DEL diffParent
	 * 2 = ADD diffChild
	 * 3 = DEL diffChild
	 * 4 = ADD swarmParent
	 * 5 = DEL swarmParent
	 * 6 = ADD swarmChild
	 * 7 = DEL swarmChild
	 *
	 *  e.g. print State Modification =>  10.2.0.100  ,  30000  ,  2  ,  192.168.1.103 ,  3008  ,  20006
	 *
	 *  means : "ADD 192.168.1.103  TO  diffChildren OF 10.2.0.100"
	 */

	std::string		recipientsIPv4, entryIPv4;
	unsigned short			recipientsRepPort, kindOfModification, entryTCPPort, entryRepPort;
	
	StateModification(std::string rIP, unsigned short rPort, unsigned short mod, std::string eIP, unsigned short eTPort, unsigned short eRPort){
		recipientsIPv4		= rIP;
		recipientsRepPort	= rPort;
		kindOfModification	= mod;
		entryIPv4			= eIP;
		entryTCPPort		= eTPort;
		entryRepPort		= eRPort;
	}


	bool equals(StateModification* node){
		if(this->recipientsIPv4		!=  node->recipientsIPv4	 ||
		   this->recipientsRepPort  !=  node->recipientsRepPort	 ||
		   this->kindOfModification	!=  node->kindOfModification ||
		   this->entryIPv4			!=  node->entryIPv4			 ||
		   this->entryTCPPort		!=  node->entryTCPPort		 ||
		   this->entryRepPort		!=  node->entryRepPort )

		   return false;


		return true;
	}

};

struct CompareStateModifications {
	bool operator() (StateModification a, StateModification b) {
		if(a.recipientsIPv4.compare(b.recipientsIPv4) < 0)
			return true;
		else if (a.recipientsIPv4.compare(b.recipientsIPv4) > 0)
			return false;
		else{
			if(a.recipientsRepPort < b.recipientsRepPort)
				return true;
			else 
				return false;
		}
	}
};



/* This temporary class contains some static methods that are going to be used in the main project. */
class MethodsForMainProject {
public:

	/* Method which checks whether a PRIMENode object exists in a vector or not. */
	//static bool exists(vector<PRIMENode*> *peersWithModifiedStates, PRIMENode *compare){
	static bool exists(vector<StateModification> *stateModifications, StateModification *compare){

		bool exist = false;
		//for(int i=0; i < peersWithModifiedStates->size(); i++){
		for(int i=0; i < stateModifications->size(); i++){
			//if(peersWithModifiedStates->at(i)->data->equals(compare->data)){
			if(stateModifications->at(i).equals(compare)){
				exist = true;
				break;
			}
		}

		return exist;
	}

	/* This method initializes the tree of the Overlay Network. */
	static FlowTree* initializeTree(TrackerPeerState* rootNode);

	/* This method is used to enter a new peer in the network tree. */
	//static vector<PRIMENode*> bootstrapNewNode(FlowTree* tree, TrackerPeerState* newNode);
	static vector<StateModification> bootstrapNewNode(FlowTree* tree, TrackerPeerState* newNode);
};


#endif /* defined(__TrackerUtilities__H__) */