/*
 *	This file contains the implementations of the methods of class FlowTree.
 */

#include "fwFlowTree.h"
#include "fwTrackerUtilities.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;


/* Method that inserts the root of the FlowTree. */
void FlowTree::insertRoot(PRIMENode *curr){
    if(isEmpty()) {
        root = curr;
    }
    else{
        cout << "There is already a root..."<<endl;
    }
}

/* Method that is used to find a Diffusion Parent for a new node. */
void FlowTree::discoverDiffusionParent (PRIMENode *newNode, vector<StateModification> *stateModifications) {

    if(isEmpty()) {
        cout << "This tree is empty!" << endl;
        return;
    }
	
	/* 
	 *	Traverse all the tree nodes in BFS order, and place them in a priority queue. The key of the queue is the "distance"
	 *	between 2 nodes.	dist = newNode's_bwpf - currenNode's_bwpf
	 */
	queue < PRIMENode* >																treeNodesInBFS;
    priority_queue <CompareNodeDiff *,vector<CompareNodeDiff *>, NodeComparatorDiff >   pr_queue;
    
    treeNodesInBFS.push(root);
    PRIMENode* curr;
    curr = treeNodesInBFS.front();
    
    while ( curr != NULL ) {
        CompareNodeDiff *cNode = new CompareNodeDiff;
        cNode->node = curr;


		/* 
		 *	distance  =  newNode's_bwpf  -  currentNode's_bwpf     
		 *	newNode's_bwpf  =  DBW / #diff_parents  
		 *	currentNode's_bwpf  =  UBW / ( #diff_children + 1 )
		 */
        cNode->distFromCurrent = -(curr->data->m_UBW/(curr->diffChildren.size()+1)
                                 - newNode->data->m_DBW/(newNode->diffParents.size()+1));


		int temp = 0;
		
		/* 
		 *	Add some penalty to the distance,depending on the level of the "current node". 
		 *	The penalty is :  distance * (level + 1) / 20
		 *
		 *	The reason for this, is that the new node should prefer candidate parents in
		 *	the higher levels of the tree.
		 */
		if(cNode->node->level == 0 ) {
			temp = cNode->distFromCurrent*(1/(double)20);

			/*
			 *	Special penalty for the 1st level, depending on the UBW of the new node. 
			 *	The reason for this, is that if a node with low UBW is entered at the 1st level
			 *	(the level where are the roots of the Diffusion SubTrees), the Diffusion SubTree for which
			 *	new node is the root, will be preferred by almost none of the next new nodes.
			 *
			 *	Best Match: Root		Ratio :  (root's_UBW - newNode's_UBW) / root's_UBW	<=	1
			 *
			 *	Special Formula : (root's_UBW / 1000) * (ratio^2.5) * regular_penalty
			 *
			 *	The bigger the ratio is, the smaller the UBW of the new node.
			 *	The ratio is powered because it is <= 1. So if it is powered, big values will remain big,
			 *	while small will become smaller.
			 */
			double ratio = (cNode->node->data->m_UBW - newNode->data->m_UBW)/(double)cNode->node->data->m_UBW;
			temp += ((cNode->node->data->m_UBW*0.001)*pow(ratio, 2.5))*temp;
		}
		else
			temp = cNode->distFromCurrent*((cNode->node->level+1)/(double)20);



		if(cNode->distFromCurrent < 0)
			cNode->distFromCurrent = cNode->distFromCurrent - temp;
		else
			cNode->distFromCurrent = cNode->distFromCurrent + temp;
		


        pr_queue.push(cNode);
        
        for (int i = 0; i < curr->diffChildren.size(); i++){
            treeNodesInBFS.push(curr -> diffChildren.at(i));
        }
        
        treeNodesInBFS.pop();

		if(!treeNodesInBFS.empty())
			curr = treeNodesInBFS.front();
		else 
			curr = NULL;
    }




    /* Find best match */
    
    CompareNodeDiff*    bestMatch, *current;
    vector<PRIMENode *>   childrenSet;
    int bestsLevel = 0;

	bestMatch = pr_queue.top();
	bestsLevel = bestMatch->node->level;
    pr_queue.pop();
	


	/* Check if there are more than 1 node with the best distance. */
	if(!pr_queue.empty()){
		current = pr_queue.top();
		pr_queue.pop();
    
	
		//If there are more, keep as bestMatch the node in the lowest level.
		while(bestMatch->distFromCurrent == current->distFromCurrent){

			if(bestsLevel < current->node->level && bestsLevel == 0){ //////////////////
				delete bestMatch;
				bestMatch = current;
				bestsLevel = current->node->level;
			}
			/* If all of the bestMatches are in the same level, keep the node with the bigger DBW. */
			else if(bestsLevel == current->node->level){
				
				int max_DBW = bestMatch->node->data->m_DBW;

				if(max_DBW < current->node->data->m_DBW){
					delete bestMatch;
					bestMatch = current;
					bestsLevel = current->node->level;
				}

			}



			if(pr_queue.empty())
				break;
			else{
				current = pr_queue.top();
				pr_queue.pop();

				if(bestMatch->distFromCurrent != current->distFromCurrent)
					delete current;
			}
		}

	}

	current = bestMatch;
	


	childrenSet = bestMatch->node->diffChildren;

	
    /* 
	 *	If bestMatch has no other child, just add newNode to its children
	 *  and also make the bestMatch, newNode's father.
	 */
    if(childrenSet.size() == 0) {

		/* Assign newNode to bestMatch and vice versa. */
        bestMatch->node->diffChildren.push_back(newNode);
        newNode->diffParents.push_back(bestMatch->node);


		/* Assigning diffSubTreeID and level to the newNode and its children if any. */
		if (bestMatch->node->diffSubTreeID == 0) {	//meaning the parent is the root.
			newNode->diffSubTreeID = bestMatch->node->diffChildren.size();
			newNode->level = 1;
		}
		else {
			newNode->diffSubTreeID = bestMatch->node->diffSubTreeID;
			newNode->level = bestMatch->node->level + 1;
		}

		/* 
		 *	After the insertion of the newNode to the Network Tree, the respective
		 *	peer, and all the peers which had their states modified, will have to be
		 *	informed for the changes.
		 *	Every node which will have to be informed, is stored in "peersWithModifiedStates".
		 */
		//if(peersWithModifiedStates != NULL){
		//	if(!MethodsForMainProject::exists(peersWithModifiedStates, bestMatch->node))
		//		peersWithModifiedStates->push_back(bestMatch->node);
		//}
		if(stateModifications != NULL){
			TrackerPeerState* temp = bestMatch->node->data;
			TrackerPeerState* temp2 = newNode->data;
			StateModification* first = new StateModification(temp->m_Addr, temp->m_RepPort, 2, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);
			StateModification* second = new StateModification(temp2->m_Addr, temp2->m_RepPort, 0, temp->m_Addr, temp->m_TCPPort, temp->m_RepPort);

			if(!MethodsForMainProject::exists(stateModifications, first))
				stateModifications->push_back(*first);
			if(!MethodsForMainProject::exists(stateModifications, second))
				stateModifications->push_back(*second);
		}

    }
    else {
		/* 
		 *	In the case that bestMatch is the Root, check if newNode 
		 *	can serve as a better parent for one of besMatch's children.
		 *
		 *	These rearrangements are taking place in order to keep the number
		 *	of Diffusion SubTrees low.
		 */
		bool treeRearrangement = false;

		if(bestMatch->node->level == 0){	



			for (int i = 0; i < childrenSet.size(); i++) {
				double      t_up, r_up;
				double      t_down;

				//current parent's upload bandwidth
				r_up = bestMatch->node->data->m_UBW / (bestMatch->node->diffChildren.size()+1);

				//candidate parent's upload bandwidth (newNode)
				t_up = newNode->data->m_UBW / (newNode->diffChildren.size()+1);
            
				//i-th child's download requirements 
				t_down = childrenSet[i]->data->m_DBW;
        
				/*	
				 *	If the newNode can serve as a better parent...
				 *	
				 *	NewNode will be considered as a better parent, if it can provide the i-th child
				 *	with an upload bandwidth, closer to child's requirements than the old parents
				 *	bandwidth (by absolute value).
				 */
				if (   ((t_up < r_up) && (t_up >= t_down))
					|| ((t_up > r_up) && (t_up <= t_down)) 
					|| ((t_up > r_up) && (t_up >= t_down) && (r_up < t_down)) ) {
					
						
					/* Update state of all 3 nodes. */
					PRIMENode *orphan = childrenSet[i];
                
					orphan->findAndReplaceDiffParent(bestMatch->node, newNode);
					bestMatch->node->findAndDeleteDiffChild(orphan);
					newNode->diffChildren.push_back(orphan);
                
					treeRearrangement = true;

					/* peersWithModifiedStates */
					//if(peersWithModifiedStates != NULL){
					//	if(!MethodsForMainProject::exists(peersWithModifiedStates, orphan))
					//		peersWithModifiedStates->push_back(orphan);
					//}
					if(stateModifications != NULL){
						TrackerPeerState* temp = orphan->data;
						TrackerPeerState* temp2 = bestMatch->node->data;
						TrackerPeerState* temp3 = newNode->data;
						StateModification* first = new StateModification(temp->m_Addr, temp->m_RepPort, 1, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);
						StateModification* second = new StateModification(temp->m_Addr, temp->m_RepPort, 0, temp3->m_Addr, temp3->m_TCPPort, temp3->m_RepPort);
						StateModification* third = new StateModification(temp2->m_Addr, temp2->m_RepPort, 3, temp->m_Addr, temp->m_TCPPort, temp->m_RepPort);
						StateModification* fourth = new StateModification(temp3->m_Addr, temp3->m_RepPort, 2, temp->m_Addr, temp->m_TCPPort, temp->m_RepPort);						

						if(!MethodsForMainProject::exists(stateModifications, first))
							stateModifications->push_back(*first);
						if(!MethodsForMainProject::exists(stateModifications, second))
							stateModifications->push_back(*second);
						if(!MethodsForMainProject::exists(stateModifications, third))
							stateModifications->push_back(*third);
						if(!MethodsForMainProject::exists(stateModifications, fourth))
							stateModifications->push_back(*fourth);
					}

					/* No need to look any further. We are not looking for the best change. */
					break;
				}

			}

		}   


		/* Assign newNode to bestMatch and vice versa. */
        bestMatch->node->diffChildren.push_back(newNode);
        newNode->diffParents.push_back(bestMatch->node);
		

		/* Assigning diffSubTreeID and level to the newNode and its children if any. */
		if (bestMatch->node->diffSubTreeID == 0) {	//means the parent is the root.
			newNode->diffSubTreeID = bestMatch->node->diffChildren.size();
			newNode->level = 1;
		}
		else {
			newNode->diffSubTreeID = bestMatch->node->diffSubTreeID;
			newNode->level = bestMatch->node->level + 1;	
		}

		/* After the rearrangement, the IDs must be changed. */
		newNode->setDiffSubTreeIDofAll(newNode->diffSubTreeID, newNode->level);
		

		/* peersWithModifiedStates */
		//if(peersWithModifiedStates != NULL){
		//	if(!MethodsForMainProject::exists(peersWithModifiedStates, bestMatch->node))
		//		peersWithModifiedStates->push_back(bestMatch->node);
		//}
		if(stateModifications != NULL){
			TrackerPeerState* temp = bestMatch->node->data;
			TrackerPeerState* temp2 = newNode->data;
			StateModification* first = new StateModification(temp->m_Addr, temp->m_RepPort, 2, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);
			StateModification* second = new StateModification(temp2->m_Addr, temp2->m_RepPort, 0, temp->m_Addr, temp->m_TCPPort, temp->m_RepPort);

			if(!MethodsForMainProject::exists(stateModifications, first))
				stateModifications->push_back(*first);
			if(!MethodsForMainProject::exists(stateModifications, second))
				stateModifications->push_back(*second);
		}



		/*
		 *	If a new Diffusion SubTree is added, in other words, if bestMatch is the root, 
		 *	and no tree rearrangement takes place, then all nodes of the tree must acquire a 
		 *	swarming parent in the new Diffusion SubTree.
		 */
		if (bestMatch->node->diffSubTreeID == 0 && !treeRearrangement) {


			/*
			 *	First, make a priority queue with all the nodes except the root and newNode,
			 *	sorted by level in descending order.
			 *	A Swarming Parent is going to be assigned to each one of them.
			 */
			priority_queue <CompareNodeSwarm *,vector<CompareNodeSwarm *>, NodeComparatorSwarm >	pr_queue_swarm;	
			queue < PRIMENode* >																	treeInBFSQueue;

			treeInBFSQueue.push(root);

			PRIMENode* curr;
			curr = treeInBFSQueue.front();
    
			while ( curr != NULL ) {

				//If node is not the root, or newNode.
				if(curr->level > 0 && !(curr->level == 1 && curr->diffSubTreeID == newNode->diffSubTreeID)) {
					CompareNodeSwarm *cNode = new CompareNodeSwarm;
					cNode->node = curr;
				
					pr_queue_swarm.push(cNode);
				}


				for (int j = 0; j < curr->diffChildren.size(); j++){
					treeInBFSQueue.push(curr -> diffChildren.at(j));
				}

				treeInBFSQueue.pop();

				if(!treeInBFSQueue.empty())
					curr = treeInBFSQueue.front();
				else 
					curr = NULL;

			}




			CompareNodeSwarm*    candidateChild;
			candidateChild = pr_queue_swarm.top();

			/* Check if a node has to be a direct swarming child of the new Diffusion SubTree's node. */
			while( candidateChild != NULL ) {


				/*
				 *	Check if the candidate child, has swarming parents that are already swarming children
				 *	of new diffTree's node, in other words, a 2-hop connection exists between the new SubTree
				 *	and candidate child.
				 */
				bool alreadyParent = false, not_break = true;

				for(int i=0; i < candidateChild->node->swarmParents.size(); i++) {
					for(int j=0; j < newNode->swarmChildren.size(); j++) {

						if(candidateChild->node->swarmParents[i]->data->equals(newNode->swarmChildren[j]->data)){
							alreadyParent = true;
							break;
						}
	
					}

					if(alreadyParent){
						not_break = false;
						break;
					}

				}


				/*
				 *	If the candidate child has no swarm parents that are also newNode's swarm children,
				 *	search for a swarming parent from newNode's swarming children, and keep the node that
				 *	can provide the best UBW.
				 */
				if(!alreadyParent){

					PRIMENode*	candidateWithBestUpBWPF;
					double	bestsT_Up = 0.0;

					for(int j=0; j < newNode->swarmChildren.size(); j++) {
						PRIMENode* swarmChildOfNew = newNode->swarmChildren[j];

						/* Only accept nodes that are of equal or lower level than candidate child's. */
						if(swarmChildOfNew->level >= candidateChild->node->level) {

							double	t_up;
							t_up = swarmChildOfNew->data->m_UBW / (swarmChildOfNew->swarmChildren.size()+1);


							if(t_up > bestsT_Up){
								candidateWithBestUpBWPF = swarmChildOfNew;
								bestsT_Up = t_up;
							}

						}
					}

					/* Check if newNode can serve better than its best swarm child. */
					double	newsT_Up;
					newsT_Up = newNode->data->m_UBW / (newNode->swarmChildren.size()+1);

					if(newsT_Up > bestsT_Up){
						candidateWithBestUpBWPF = newNode;
						bestsT_Up = newsT_Up;
					}


					//assignment of the best.
					if(!candidateWithBestUpBWPF->swarmChildAlreadyExists(candidateChild->node)){
						candidateWithBestUpBWPF->swarmChildren.push_back(candidateChild->node);
						
						/* peersWithModifiedStates */
						//if(peersWithModifiedStates != NULL){
						//	if(!MethodsForMainProject::exists(peersWithModifiedStates, candidateWithBestUpBWPF))
						//		peersWithModifiedStates->push_back(candidateWithBestUpBWPF);
						//}
						if(stateModifications != NULL){
							TrackerPeerState* temp = candidateWithBestUpBWPF->data;
							TrackerPeerState* temp2 = candidateChild->node->data;
							StateModification* first = new StateModification(temp->m_Addr, temp->m_RepPort, 6, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);

							if(!MethodsForMainProject::exists(stateModifications, first))
								stateModifications->push_back(*first);
						}
					}

					if(!candidateChild->node->swarmParentAlreadyExists(candidateWithBestUpBWPF)){
						candidateChild->node->swarmParents.push_back(candidateWithBestUpBWPF);
						
						/* peersWithModifiedStates */
						//if(peersWithModifiedStates != NULL){
						//	if(!MethodsForMainProject::exists(peersWithModifiedStates, candidateChild->node))
						//		peersWithModifiedStates->push_back(candidateChild->node);
						//}
						if(stateModifications != NULL){
							TrackerPeerState* temp = candidateChild->node->data;
							TrackerPeerState* temp2 = candidateWithBestUpBWPF->data;
							StateModification* second = new StateModification(temp->m_Addr, temp->m_RepPort, 4, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);

							if(!MethodsForMainProject::exists(stateModifications, second))
								stateModifications->push_back(*second);
						}
					}
				}


				pr_queue_swarm.pop();
				delete candidateChild;
				
				if(!pr_queue_swarm.empty())
					candidateChild = pr_queue_swarm.top();
				else 
					candidateChild = NULL;

			}

		}

	}   

	
	/*
	 *	Free all the CompareNodes, stored in the priority_queue
	 *	DO NOT forget the previously popped bestMatch!
	 */
	delete bestMatch;
	while( !pr_queue.empty() ) {
		delete pr_queue.top();
		pr_queue.pop();
	}

}

/* This method is used to assign some Swarming Children to a new node, from its Diffusion Parent. */
void FlowTree::inheritSwarmingChildrenOfDiffusionParent (CompareNodeDiff* bestMatch, PRIMENode* newNode, vector<StateModification> *stateModifications){

	/* 
	 *	After a bestMatch has become newNode's Diffusion Parent, newNode has to inherit from its parent
	 *	some Swarming Children. The children that are going to be inherited, are those which have levels that
	 *	are lower than bestMatch's.
	 *	
	 *	In order to allow the adoption of the swarming children, the following requirement is needed:
	 *
	 *	NewNode must have direct Swarming Parents to all the Diffusion SubTrees that bestMatch has also
	 *	direct Swarming Parents.
	 */

	int numOfDiffSubTrees = this->getRoot()->diffChildren.size();
	bool* accessToSubTrees = new bool[numOfDiffSubTrees];

	for(int k=0; k < numOfDiffSubTrees; k++)
		accessToSubTrees[k] = false;

	for(int k=0; k < bestMatch->node->swarmParents.size(); k++){
		accessToSubTrees[bestMatch->node->swarmParents[k]->diffSubTreeID-1] = true;
	}

	for(int k=0; k < newNode->swarmParents.size(); k++){
		accessToSubTrees[newNode->swarmParents[k]->diffSubTreeID-1] = false;
	}
	
	bool adoptionAllowed = true;
	for(int k=0; k < numOfDiffSubTrees; k++){
		if(accessToSubTrees[k]){
			adoptionAllowed = false;
			break;
		}
	}

	
	if(adoptionAllowed){
		for(int i=0;i < bestMatch->node->swarmChildren.size(); i++){
			PRIMENode* swarmChild = bestMatch->node->swarmChildren[i];

			if(swarmChild->level > bestMatch->node->level){
				
				//delete child from bestMatch's children
				bestMatch->node->swarmChildren.erase(bestMatch->node->swarmChildren.begin()+i);

				if(stateModifications != NULL){
					TrackerPeerState* temp = bestMatch->node->data;
					TrackerPeerState* temp2 = swarmChild->data;
					StateModification* first = new StateModification(temp->m_Addr, temp->m_RepPort, 7, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);

					if(!MethodsForMainProject::exists(stateModifications, first))
						stateModifications->push_back(*first);
				}



				//delete best from child's parents
				for(int j=0; j<swarmChild->swarmParents.size(); j++){
					if(swarmChild->swarmParents[j]->data->equals(bestMatch->node->data)){
						swarmChild->swarmParents.erase(swarmChild->swarmParents.begin()+j);
						//maybe break;


						if(stateModifications != NULL){
							TrackerPeerState* temp = swarmChild->data;
							TrackerPeerState* temp2 = bestMatch->node->data;
							StateModification* first = new StateModification(temp->m_Addr, temp->m_RepPort, 5, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);

							if(!MethodsForMainProject::exists(stateModifications, first))
								stateModifications->push_back(*first);
						}

					}
				}


				//assign newNode as parent to child
				swarmChild->swarmParents.push_back(newNode);

				//assign child as Swarm Child to newNode
				newNode->swarmChildren.push_back(swarmChild);

				//if(peersWithModifiedStates != NULL){
				//	if(!MethodsForMainProject::exists(peersWithModifiedStates, swarmChild))
				//		peersWithModifiedStates->push_back(swarmChild);
				//}
				if(stateModifications != NULL){
					TrackerPeerState* temp = swarmChild->data;
					TrackerPeerState* temp2 = newNode->data;
					StateModification* first = new StateModification(temp->m_Addr, temp->m_RepPort, 4, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);
					StateModification* second = new StateModification(temp2->m_Addr, temp2->m_RepPort, 6, temp->m_Addr, temp->m_TCPPort, temp->m_RepPort);

					if(!MethodsForMainProject::exists(stateModifications, first))
						stateModifications->push_back(*first);
					if(!MethodsForMainProject::exists(stateModifications, second))
						stateModifications->push_back(*second);
				}
			}
		}
	}

}

/* This method is used to find Swarming Parents for a new node. It calls discoverSwarmingParentInDiffusionSubtree(). */
void FlowTree::discoverSwarmingParents (PRIMENode *newNode, vector<StateModification> *stateModifications) {

	if(isEmpty()) {
        cout << "This tree is empty!" << endl;
        return;
    }
	
	/* For every Diffusion SubTree except newNode's */
	for(int i=0; i < root->diffChildren.size(); i++) {
		if(root->diffChildren[i]->diffSubTreeID != newNode->diffSubTreeID) {

			/* Check if newNode has already a Swarming Parent from this Diffusion SubTree */
			bool already = false;
			for(int j=0; j < newNode->swarmParents.size();j++){
				if(newNode->swarmParents[j]->diffSubTreeID == root->diffChildren[i]->diffSubTreeID){
					already = true;
					break;
				}
			}
			if(!already)
				discoverSwarmingParentInDiffusionSubtree(newNode, i, stateModifications);


			/* 
			 *	If bestMatch has Swarm Children to a lower level than its level,
			 *	then assign them to newNode.
			 */
			CompareNodeDiff* bestMatch = new CompareNodeDiff;
			bestMatch->node = newNode->diffParents[0];
			inheritSwarmingChildrenOfDiffusionParent(bestMatch, newNode, stateModifications);
			delete bestMatch;
		}
	}

}

/* This method is used to find a Swarming Parent in a specific Diffusion SubTree, for a new node. */
void FlowTree::discoverSwarmingParentInDiffusionSubtree(PRIMENode *newNode, int i, vector<StateModification> *stateModifications) {

	/* 
	 *	Traverse all the tree nodes of i-th Diffusion SubTreein BFS order, and place them 
	 *	in a priority queue. The key of the queue is the level of the nodes. 
	 */
	queue < PRIMENode* >																	subTreeInBFSQueue;	
	priority_queue <CompareNodeSwarm *,vector<CompareNodeSwarm *>, NodeComparatorSwarm >	pr_queue;

	subTreeInBFSQueue.push(root->diffChildren[i]);

	PRIMENode* curr,* lowerNotCandidate = NULL;
	curr = subTreeInBFSQueue.front();
	double	lowestsT_Up = 0.0;
    

	while ( curr != NULL ) {

		/* If node's level is lower or equal to newNode's, make it a candidate swarm parent. */
		if(curr->level >= newNode->level) {

			if(!curr->data->equals(newNode->data)){
				CompareNodeSwarm *cNode = new CompareNodeSwarm;
				cNode->node = curr;
				
				pr_queue.push(cNode);

				if(lowerNotCandidate == NULL){
					//has to be not null, to avoid runtime errors.
					lowerNotCandidate = curr;
				}
			}
		}
		else {

			/* 
			 *	In the case that a Diffusion SubTree has no nodes in a level lower or equal than newNode's, a back-up
			 *	node must be kept, because newNode MUST have Swarming Parents from all the Diffusion SubTrees.
			 *	Keep a back-up node (lowerNotCandidate) from the lowest level of the Diffusion SubTree, and, if there
			 *	are more than 1, we keep the node which can serve the best UBW.
			 */

			if(lowerNotCandidate != NULL){
				if(lowerNotCandidate->level < curr->level){
					lowerNotCandidate = curr;
					lowestsT_Up = lowerNotCandidate->data->m_UBW / (lowerNotCandidate->swarmChildren.size()+1);
				}
				else if(lowerNotCandidate->level == curr->level){
					double	t_up;

					//current node's UBW if he becomes newNode's swarm parent.
					t_up = curr->data->m_UBW / (curr->swarmChildren.size()+1);

					//keep the lowerNotCandidate that serves the best UBW.
					if(t_up > lowestsT_Up){
						lowerNotCandidate = curr;
						lowestsT_Up = lowerNotCandidate->data->m_UBW / (lowerNotCandidate->swarmChildren.size()+1);
					}
				}
			}
			else{
				lowerNotCandidate = curr;
				lowestsT_Up = lowerNotCandidate->data->m_UBW / (lowerNotCandidate->swarmChildren.size()+1);
			}
		}


        
		for (int j = 0; j < curr->diffChildren.size(); j++){
			subTreeInBFSQueue.push(curr -> diffChildren.at(j));
		}

		subTreeInBFSQueue.pop();

		if(!subTreeInBFSQueue.empty())
			curr = subTreeInBFSQueue.front();
		else 
			curr = NULL;

	}

	

	CompareNodeSwarm*    candidateSwarmParent, *candidateWithBestUpBWPF;
	double	bestsT_Up = 0.0;
	bool	assignedLegitSwarmParent = false;

	/* If there are candidate parents. */
	if(!pr_queue.empty()) {

		
		/* Look in the priority queue for the node that can serve the best UBW. */
		
		candidateSwarmParent = pr_queue.top();
		bool firstTime = true;

		while( candidateSwarmParent != NULL ) {

			double	t_up;
			bool toDelete = true;

			//candidate's UBW if he becomes newNode's swarm parent.
			t_up = candidateSwarmParent->node->data->m_UBW / (candidateSwarmParent->node->swarmChildren.size()+1);

			if(t_up > bestsT_Up){
				if(!firstTime)
					delete candidateWithBestUpBWPF;
				candidateWithBestUpBWPF = candidateSwarmParent;
				bestsT_Up = t_up;
				toDelete = false;
				firstTime = false;
			}


			pr_queue.pop();
			if(toDelete)
				delete candidateSwarmParent;
				
			if(!pr_queue.empty())
				candidateSwarmParent = pr_queue.top();
			else 
				candidateSwarmParent = NULL;

		}

		/* 
		 *	After finding the node with the best UBW (candidateWithBestUpBWPF) in the current Diffusion SubTree, 
		 *	check  if candidateWithBestUpBWPF has any Swarming Children that can serve better UBW than candidateWithBestUpBWPF.
		 *
		 *	This is OK, because candidateWithBestUpBWPF's Swarming Children can provide newNode with data from the current
		 *	Diffusion Subtree, after receiving this data from candidateWithBestUpBWPF which is their Swarming Parent.
		 */

		bool del = true;
		vector<PRIMENode*> swarminChildren = candidateWithBestUpBWPF->node->swarmChildren;

		for(int j=0; j < swarminChildren.size(); j++) {
			PRIMENode* swarmChild = swarminChildren[j];

			double	t_up;

			//candidate's UBW if he becomes newNode's swarm parent.
			t_up = swarmChild->data->m_UBW / (swarmChild->swarmChildren.size()+1);


			if(t_up > bestsT_Up){
				if(!swarmChild->data->equals(newNode->data) && swarmChild->level >= newNode->level){
					delete candidateWithBestUpBWPF;
					CompareNodeSwarm *temp = new CompareNodeSwarm;
					temp->node = swarmChild;
					del = false;
					candidateWithBestUpBWPF = temp;
					bestsT_Up = t_up;
				}
			}
		}


		/* 
		 *	In the end, we keep as Swarming Parent the node that can serve newNode with the best UBW, and its distance
		 *	with newNode is 1 or 2 hops away.
		 */

		if(!candidateWithBestUpBWPF->node->swarmChildAlreadyExists(newNode)){
			candidateWithBestUpBWPF->node->swarmChildren.push_back(newNode);
			
			/* peersWithModifiedStates */
			//if(peersWithModifiedStates != NULL){
			//	if(!MethodsForMainProject::exists(peersWithModifiedStates, candidateWithBestUpBWPF->node))
			//		peersWithModifiedStates->push_back(candidateWithBestUpBWPF->node);
			//}
			if(stateModifications != NULL){
				TrackerPeerState* temp = candidateWithBestUpBWPF->node->data;
				TrackerPeerState* temp2 = newNode->data;
				StateModification* first = new StateModification(temp->m_Addr, temp->m_RepPort, 6, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);

				if(!MethodsForMainProject::exists(stateModifications, first))
					stateModifications->push_back(*first);
			}

		}

		if(!newNode->swarmParentAlreadyExists(candidateWithBestUpBWPF->node)){
			newNode->swarmParents.push_back(candidateWithBestUpBWPF->node);

			if(stateModifications != NULL){
				TrackerPeerState* temp = newNode->data;
				TrackerPeerState* temp2 = candidateWithBestUpBWPF->node->data;
				StateModification* second = new StateModification(temp->m_Addr, temp->m_RepPort, 4, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);

				if(!MethodsForMainProject::exists(stateModifications, second))
					stateModifications->push_back(*second);
			}
		}
		
		
		
		
		
		
		assignedLegitSwarmParent = true;

		if(del)
			delete candidateWithBestUpBWPF;


	}
	




	/*
	 *	If there are no candidate parents, in other words,if for the specific Diffusion SubTree, there are not
	 *	nodes of equal or lower level to newNode's level.
	 */
	if(!assignedLegitSwarmParent && lowerNotCandidate != NULL) {
		bool takeTheLower = false, alreadyParent = false;

		/* 
		 *	Check Swarming Children of "lowerNotCandidate", if there are any.
		 *	If there are not, take "lowerNotCandidate" as Swarming Parent.
		 */

		if(!lowerNotCandidate->swarmChildren.empty()) {
			PRIMENode* childWithBestUpBWPF;

			for(int j=0; j < lowerNotCandidate->swarmChildren.size(); j++) {
				PRIMENode* lowersSwarmChild = lowerNotCandidate->swarmChildren[j];

				//Check if a swarmChild of lowerNotCandidate, is already a swarm parent of newNode.
				for(int k=0; k < newNode->swarmParents.size(); k++) {
					if(newNode->swarmParents[k]->data->equals(lowersSwarmChild->data)){
						alreadyParent = true;
						break;
					}
				}

				/* 
				 *	If a swarmChild of lowerNotCandidate, is already a swarm parent of newNode, do nothing.
				 *	There is no need to acquire an additional Swarming Parent.
				 */
				if(alreadyParent){
					break;
				}


				/* 
				 *	Else, check only those Swarming Children which have equal or lower level than newNode.
				 *	Keep the swarm child that can serve the best UBW to newNode.
				 */
				if(lowersSwarmChild->level >= newNode->level) {

					double	t_up;
					t_up = lowersSwarmChild->data->m_UBW / (lowersSwarmChild->swarmChildren.size()+1);

					if(t_up > lowestsT_Up){
						childWithBestUpBWPF = lowersSwarmChild;
						lowestsT_Up = t_up;
						assignedLegitSwarmParent = true;
					}


				}

			}

			/* Assign the best Swarming Child to newNode.*/
			if(!alreadyParent){

				if(assignedLegitSwarmParent){
					if(!childWithBestUpBWPF->data->equals(newNode->data)){
						if(!childWithBestUpBWPF->swarmChildAlreadyExists(newNode)){
							childWithBestUpBWPF->swarmChildren.push_back(newNode);
							
							/* peersWithModifiedStates */
							//if(peersWithModifiedStates != NULL){
							//	if(!MethodsForMainProject::exists(peersWithModifiedStates, childWithBestUpBWPF))
							//		peersWithModifiedStates->push_back(childWithBestUpBWPF);
							//}
							if(stateModifications != NULL){
								TrackerPeerState* temp = childWithBestUpBWPF->data;
								TrackerPeerState* temp2 = newNode->data;
								StateModification* first = new StateModification(temp->m_Addr, temp->m_RepPort, 6, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);

								if(!MethodsForMainProject::exists(stateModifications, first))
									stateModifications->push_back(*first);
							}

						}

						if(!newNode->swarmParentAlreadyExists(childWithBestUpBWPF)){
							newNode->swarmParents.push_back(childWithBestUpBWPF);

							if(stateModifications != NULL){
								TrackerPeerState* temp = newNode->data;
								TrackerPeerState* temp2 = childWithBestUpBWPF->data;
								StateModification* second = new StateModification(temp->m_Addr, temp->m_RepPort, 4, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);

								if(!MethodsForMainProject::exists(stateModifications, second))
									stateModifications->push_back(*second);
							}

						}
					}
				}
				else
					takeTheLower = true;
			}


		}
		else {
			takeTheLower = true;
		}

		/* Acquire the lower as new swarm parent. */
		if(takeTheLower){
			
			if(!lowerNotCandidate->swarmChildAlreadyExists(newNode)){
				lowerNotCandidate->swarmChildren.push_back(newNode);
				
				//if(peersWithModifiedStates != NULL){
				//	if(!MethodsForMainProject::exists(peersWithModifiedStates, lowerNotCandidate))
				//		peersWithModifiedStates->push_back(lowerNotCandidate);
				//}
				if(stateModifications != NULL){
					TrackerPeerState* temp = lowerNotCandidate->data;
					TrackerPeerState* temp2 = newNode->data;
					StateModification* first = new StateModification(temp->m_Addr, temp->m_RepPort, 6, temp2->m_Addr, temp2->m_TCPPort, temp2->m_RepPort);

					if(!MethodsForMainProject::exists(stateModifications, first))
						stateModifications->push_back(*first);
				}

			}

			if(!newNode->swarmParentAlreadyExists(lowerNotCandidate))
				newNode->swarmParents.push_back(lowerNotCandidate);
		}

	}

}

/* Method that prints all the connections between the nodes of the Overlay Network, Swarming and Diffusion. */
void FlowTree::print() {
    if(isEmpty())
    {
        cout << "This tree is empty!" << endl;
        return;
    }

    queue < PRIMENode* > treeNodesInBFS;
    treeNodesInBFS.push(root);
    PRIMENode* curr;
    curr = treeNodesInBFS.front();
    while (curr != NULL){

		printf("Tcp %d   TreeID %d   Level %d   Children : ", curr->data->getTCPPort(), curr->diffSubTreeID, curr->level);
		vector<PRIMENode *>   childrenSet;
		childrenSet = curr->diffChildren;

		if(childrenSet.size() == 0) {
			printf(" ----- \n");
		}
		else{
			for (int i = 0; i < childrenSet.size(); i++) {
				printf("  %d", childrenSet[i]->data->getTCPPort());
			}
			printf("\n");
		}




        for (int i= 0; i < curr -> diffChildren.size(); i++){
            treeNodesInBFS.push(curr->diffChildren.at(i));
        }

		//Always check to see if the last pop left the queue empty
        treeNodesInBFS.pop();
        if(!treeNodesInBFS.empty())
			curr = treeNodesInBFS.front();
		else 
			curr = NULL;
    }



	//Print swarm children of each node.
	treeNodesInBFS.push(root);
    curr = treeNodesInBFS.front();
	printf("\n\n\n");
    while (curr != NULL) {


		printf("Tcp %d   TreeID %d   Level %d   SwarmChildren : ", curr->data->getTCPPort(), curr->diffSubTreeID, curr->level);
		vector<PRIMENode *>   childrenSet;
		childrenSet = curr->swarmChildren;

		if(childrenSet.size() == 0) {
			printf(" ----- \n");
		}
		else{
			for (int i = 0; i < childrenSet.size(); i++) {
				printf("  %d", childrenSet[i]->data->getTCPPort());
			}
			printf("\n");
		}

		printf("                                SwarmParents :  ");
		vector<PRIMENode *>   parentSet;
		parentSet = curr->swarmParents;

		if(parentSet.size() == 0) {
			printf(" ----- \n");
		}
		else{
			for (int i = 0; i < parentSet.size(); i++) {
				printf("  %d", parentSet[i]->data->getTCPPort());
			}
			printf("\n");
		}




		for (int i= 0; i < curr -> diffChildren.size(); i++){
            treeNodesInBFS.push(curr->diffChildren.at(i));
        }

		//Always check to see if the last pop left the queue empty
        treeNodesInBFS.pop();
        if(!treeNodesInBFS.empty())
			curr = treeNodesInBFS.front();
		else 
			curr = NULL;
	}

}

FlowTree::~FlowTree() {
		if(isEmpty())
			return;
    
		queue < PRIMENode* > treeNodesInBFS;
		treeNodesInBFS.push(root);
		PRIMENode* curr;
		curr = treeNodesInBFS.front();
		while (curr != NULL){
			for (int i= 0; i < curr -> diffChildren.size(); i++){
				treeNodesInBFS.push(curr->diffChildren.at(i));
			}
		
			//Delete Node data first and then the PRIMENode
			delete curr->data;
			delete curr;

			//Always check to see if the last pop left the queue empty
			treeNodesInBFS.pop();
			if(!treeNodesInBFS.empty())
				curr = treeNodesInBFS.front();
			else 
				curr = NULL;

		}

	}