/*
 *	This file contains the basic classes of the Overlay Network Tree, 
 *	and some helper classes of minor significance.
 */

#ifndef __FlowTree__H__
#define __FlowTree__H__


#include <vector>
#include <queue>
#include <iostream>
#include <functional>
#include "fwTrackerPeerState.h"
#include "fwTrackerUtilities.h"

using namespace std;


/*
 *	This class implements a tree node, with information for all of its connections with other PRIMENode objects.
 *	All the nodes of the tree can be encountered, by using information of just one PRIMENode.
 */
class PRIMENode {
public:
    vector <PRIMENode *>	swarmParents;
	vector <PRIMENode *>	swarmChildren;
    vector <PRIMENode *>	diffParents;
    vector <PRIMENode *>	diffChildren;

    TrackerPeerState*		data;			//network information about the peer which is represented by this PRIMENode object.
	short			diffSubTreeID;	//to which diffusion sub tree this peer belongs.
	short			level;			//to which level of the flow tree this peer belongs.

    PRIMENode() {
        data = NULL;
		diffSubTreeID = -1;
		level = -1;
    }

    /* Method that replaces an existing Diffusion Parent with a new one.*/
    void findAndReplaceDiffParent(PRIMENode *par, PRIMENode *newNode) {
		for (int i = 0; i < diffParents.size(); i++) {
			if(diffParents[i]->data->equals(par->data)){
				diffParents.erase(diffParents.begin() + i);
			}
        }

        diffParents.push_back(newNode);
    }
    
	/* Method that deletes an existing Diffusion Child.*/
    void findAndDeleteDiffChild(PRIMENode *child) {
        for (int i = 0; i < diffChildren.size(); i++) {
			if(diffChildren[i]->data->equals(child->data)){

				diffChildren.erase(diffChildren.begin() + i);

				//If *this is the root, change diffSubTreeID of the remaining children
				if(diffSubTreeID == 0){
					for(int j=i; j < diffChildren.size(); j++) {
						diffChildren[j]->diffSubTreeID--;

						//And of the children's children.
						short temp_id = diffChildren[j]->diffSubTreeID;
						diffChildren[j]->setDiffSubTreeIDofAll(temp_id, -2);
					}
				}

			}
        }
    }

	/* Recursive method that changes the diffSubTreeID and level of all peers rooted by *this. */
	void setDiffSubTreeIDofAll(short ID , short level){

		if(!diffChildren.empty()){
			for(int i=0;i < diffChildren.size(); i++){
				if(level != -2)	
					/* in this case, we change the level too. */
					diffChildren[i]->setDiffSubTreeIDofAll(ID, level+1);
				else
					diffChildren[i]->setDiffSubTreeIDofAll(ID, level);


				diffChildren[i]->diffSubTreeID = ID;
				if(level != -2)
					diffChildren[i]->level = level + 1;
			}
		}
	}

	/* Method that checks if a node already exists in the Swarming Children list. */
	bool swarmChildAlreadyExists(PRIMENode* toBeAdded) {
		bool exists = false;
		for(int k=0; k < this->swarmChildren.size(); k++) {
			if(this->swarmChildren[k]->data->equals(toBeAdded->data)){
				exists = true;
				break;
			}
		}

		return exists;
	}

	/* Method that checks if a node already exists in the Swarming Parents list. */
	bool swarmParentAlreadyExists(PRIMENode* toBeAdded) {
		bool exists = false;
		for(int k=0; k < this->swarmParents.size(); k++) {
			if(this->swarmParents[k]->data->equals(toBeAdded->data)){
				exists = true;
				break;
			}
		}

		return exists;
	}

};



//Special form of PRIMENode node, suitable for custom compare. 
class CompareNodeDiff {
public:
    PRIMENode *node;
    int distFromCurrent;	// the "bwpf" distance between 2 nodes.

    CompareNodeDiff () {
        node = NULL;
    }
};



//Special form of PRIMENode node, suitable for custom compare.
class CompareNodeSwarm {
public:
    PRIMENode *node;

    CompareNodeSwarm () {
        node = NULL;
    }
};



/*This class implements the tree of FLOW's overlay network.
*/
class FlowTree {
private:
    PRIMENode* root;

	/* This method is used to find a Swarming Parent in a specific Diffusion SubTree, for a new node. */
	//void discoverSwarmingParentInDiffusionSubtree(PRIMENode *newNode, int i, vector<PRIMENode*> *peersWithModifiedStates);
	void discoverSwarmingParentInDiffusionSubtree(PRIMENode *newNode, int i, vector<StateModification> *stateModifications);

	/* This method is used to assign some Swarming Children to a new node, from its Diffusion Parent. */
	//void inheritSwarmingChildrenOfDiffusionParent (CompareNodeDiff* bestMatch, PRIMENode* newNode, vector<PRIMENode*> *peersWithModifiedStates);
	void inheritSwarmingChildrenOfDiffusionParent (CompareNodeDiff* bestMatch, PRIMENode* newNode, vector<StateModification> *stateModifications);
public:
    FlowTree() { root = NULL; }
	~FlowTree();

	/* This method is used to find a Diffusion Parent for a new node. */
    //void discoverDiffusionParent (PRIMENode *newNode, vector<PRIMENode*> *peersWithModifiedStates);
	void discoverDiffusionParent (PRIMENode *newNode, vector<StateModification> *stateModifications);

	/* This method is used to find Swarming Parents for a new node. It calls discoverSwarmingParentInDiffusionSubtree().*/
	//void discoverSwarmingParents (PRIMENode *newNode, vector<PRIMENode*> *peersWithModifiedStates);
	void discoverSwarmingParents (PRIMENode *newNode,  vector<StateModification> *stateModifications);

    bool isEmpty() const {return root == NULL;}
    void insertRoot(PRIMENode *curr);
	PRIMENode* getRoot() {return this->root;}

	/* Method that prints all the connections between the nodes of the Overlay Network, Swarming and Diffusion. */
    void print();
};

// Struct used with CompareNodeDiff, for comparing between nodes. (Called by a priority queue.)
struct NodeComparatorDiff
{
    bool operator()(const CompareNodeDiff* lhs, const CompareNodeDiff* rhs) const
    {
        return lhs->distFromCurrent > rhs->distFromCurrent;
    }
};

// Struct used with CompareNodeSwarm, for comparing between nodes. (Called by a priority queue.)
struct NodeComparatorSwarm
{
    bool operator()(const CompareNodeSwarm* lhs, const CompareNodeSwarm* rhs) const
    {
		return lhs->node->level < rhs->node->level;
    }
};
       

#endif /* defined(__FlowTree__H__) */
