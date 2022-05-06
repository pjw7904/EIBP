/*
 * helloList.h
 *
 *  Created on: Mar 29, 2015
 *  Updated on: Apr 02, 2015
 *      Author: tsp3859
 */

#ifndef HELLOLIST_H_
#define HELLOLIST_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include "boolean.h"
#include "fwdAlgorithmHelper.h"
#include "tierList.h"

extern FILE *fptr;
extern int enableLogScreen;
extern int enableLogFiles;

void timestamp();
boolean containsTierAddress(char testStr[20]);
boolean setByTierPartial(char inTier[20], boolean setFWDFields);
boolean setByTierOnly(char inTier[20], boolean setFWDFields);
boolean setByTierManually(char inTier[20], boolean setFWDFields);
extern void getUID(char* curUID,char* currentTier);
struct timeval current_time;

struct nodeHL {
	char tier[20];          // tier value
	char port[20];  	    // received interface
	double lastUpdate;      // last updated time
	struct nodeHL *next;      // next node
};

struct nodeHL *headHL = NULL;
extern int myTierValue;
struct nodeHL *interAS_head = NULL; //to store interAS neighbors 
extern char unavailable_interface[5]; //to store unavailable interface when an interface goes down

struct QuarantineTL {
	char tier[20];
	char port[20];
	double label_lost_time;
	double label_remove_time;
	char purpose[20];           //Remove or Add labels
	struct QuarantineTL *next;      
}*Head_QTL;

/**
 * find()
 *
 * method to check whether a node is present or not in list
 *
 * @param inTier (char[]) - tier value
 * @param inPort (char[]) - interface value
 *
 * @return returnVal (int) - 0 for present otherwise 1
 */

int find(char inTier[20], char inPort[20], int interAS) {

	int returnVal = 1;

	struct nodeHL *fNode = NULL;
	if (interAS==1){ //interAS
		fNode = interAS_head;
	}
	else{
	 	fNode = headHL;
	}
	// traverse the list
	// testing
	while (fNode != NULL) {
		//while (fNode->next != NULL) {

		// Target Node
		// Length Check
		// Value check

		if (strlen(fNode->tier) == strlen(inTier)) {
			if (strncmp(fNode->tier, inTier, strlen(inTier)) == 0) {

				if (strlen(fNode->port) == strlen(inPort)) {

					//}
					if (strncmp(fNode->port, inPort, strlen(inPort)) == 0) {

						returnVal = 0;
						break;
					}
				}

			}

		}

		fNode = fNode->next;
	}

	return returnVal;
}

/**
 * append()
 *
 * method to add node after previous node, called by insert()
 *
 * @param inTier (char[]) - tier value
 * @param inPort (char[]) - interface value
 *
 */
void append(char inTier[20], char inPort[20], int interAS) {

	struct nodeHL *temp, *right;
	temp = (struct nodeHL *) malloc(sizeof(struct nodeHL));

	strcpy(temp->tier, inTier);
	strcpy(temp->port, inPort);
	gettimeofday(&current_time , NULL);
	temp->lastUpdate = ((double)current_time.tv_sec*1000000 + (double)current_time.tv_usec)/1000000;
	//interAS
	if (interAS==1){
		right = (struct nodeHL *) interAS_head;
	}
	else{
	right = (struct nodeHL *) headHL;
	}
	while (right->next != NULL)
		right = right->next;
	right->next = temp;
	right = temp;
	
	right->next = NULL;
	printf("\nTEST: Node appended successfully %s\n", temp->tier);
	
}

/**
 * add()
 *
 * method to add node after previous node, called by insert()
 *
 * @param inTier (char[]) - tier value
 * @param inPort (char[]) - interface value
 *
 */
void add(char inTier[20], char inPort[20], int interAS) {

	struct nodeHL *temp;
	temp = (struct nodeHL *) malloc(sizeof(struct nodeHL));
	strcpy(temp->tier, inTier);
	strcpy(temp->port, inPort);
	gettimeofday(&current_time , NULL);
	temp->lastUpdate = ((double)current_time.tv_sec*1000000 + (double)current_time.tv_usec)/1000000;
	//interAS
	if (interAS==1){
		if (interAS_head == NULL) {
		interAS_head = temp;
		interAS_head->next = NULL;
	} else {
		temp->next = interAS_head;
		interAS_head = temp;
	}
	}
	else{

	if (headHL == NULL) {
		headHL = temp;
		headHL->next = NULL;
	} else {
		temp->next = headHL;
		headHL = temp;
	}
	}
	printf("\nTEST: Node added successfully %s\n", temp->tier);
}

void update(char inTier[20], char inPort[20], int interAS) {

	// to be updated
	struct nodeHL *uNode = NULL;
	//interAS
	if (interAS==1){
		uNode = interAS_head;
	}
	else{
		uNode = headHL;
	}

	// traverse the list
	// testing
	while (uNode != NULL) {

		if (strlen(uNode->tier) == strlen(inTier)) {
			if (strncmp(uNode->tier, inTier, strlen(inTier)) == 0) {

				if (strlen(uNode->port) == strlen(inPort)) {

					//}
					if (strncmp(uNode->tier, inTier, strlen(inTier)) == 0) {
						gettimeofday(&current_time , NULL);
						uNode->lastUpdate = ((double)current_time.tv_sec*1000000 + (double)current_time.tv_usec)/1000000;
						;
						//printf("TEST: lastUpdate updated %s\n", uNode->tier);
						break;
					}
				}

			}

		}
		uNode = uNode->next;

	}

}


/**
 * insert()
 *
 * method to add node into a list (duplicate entry-safe)
 *
 * @param inTier (char[]) - tier value
 * @param inPort (char[]) - interface value
 *
 * @return isEntryNew
 */
 
void printNeighbourTable() {

	struct nodeHL *fNode = headHL;
	char* temp;
	if (fNode == NULL) {
		if(enableLogScreen)
			printf("ERROR: Neighbor List is empty (Isolated Node)\n");
		return;
	}
	// traverse the list
	// testing
	if(enableLogScreen)
		printf("\n*************** Neighbor Table *************");
	while (fNode->next != NULL) {
		temp  = fNode->tier;		
		if(enableLogScreen)
			printf("\n ------- %s -------",temp);
		fNode = fNode->next;
	}
	printf("\n ------- %s -------",fNode->tier);
	return;
}
//print the interAS neighbors if it exists
void printInterASNeighbourTable() {

	struct nodeHL *fNode = interAS_head;
	char* temp;
	if (fNode == NULL) {
		if(enableLogScreen)
			printf("ERROR: Neighbor List is empty (Isolated Node)\n");
		return;
	}
	// traverse the list
	// testing
	if(enableLogScreen)
		printf("\n*************** interAS Neighbor Table *************");
	while (fNode->next != NULL) {
		temp  = fNode->tier;		
		if(enableLogScreen)
			printf("\n ------- %s -------",temp);
		fNode = fNode->next;
	}
	printf("\n ------- %s -------",fNode->tier);
	return;
}
 
int insert(char inTier[20], char inPort[20], int interAS_neighbor) {
	
	int isEntryNew = 0;
	//interAS
	if (interAS_neighbor){
		
		struct nodeHL *temp;
		temp = interAS_head;

		if (temp == NULL) {

			add(inTier, inPort, 1);
			isEntryNew = 1;
			printInterASNeighbourTable();

		} else {

			int checkNode = find(inTier, inPort, 1);

			if (checkNode == 1) { // The entry is not present in the table, so append it to the table.

				append(inTier, inPort, 1);
				isEntryNew = 1;
				printInterASNeighbourTable();

			} else {

				update(inTier, inPort, 1);
			}

		}
	}
    else{
	struct nodeHL *temp;
	temp = headHL;

	

	if (temp == NULL) {

		add(inTier, inPort, 0);
		isEntryNew = 1;
		printNeighbourTable();

	} else {

		int checkNode = find(inTier, inPort, 0);

		if (checkNode == 1) { // The entry is not present in the table, so append it to the table.

			append(inTier, inPort, 0);
			isEntryNew = 1;
			printNeighbourTable();

		} else {

			update(inTier, inPort,0);
		}

	}
	}

	return isEntryNew;
}



/**
 * update()
 *
 * method to update the timer information of a node
 *
 * @param inTier (char[]) - tier value
 * @param inPort (char[]) - interface value
 */

/**
 * delete()
 *
 * method to delete node from neighbor based on timeout mechanism
 *
 * @return status (int) - method return value
 */

int delete() {

	//struct nodeHL *deletedTierAddr = NULL;
	struct nodeHL *temp = headHL;
	struct nodeHL *prev;
	//struct nodeTL *fNode = headTL; // headTL - head of the My label table
	struct nodeTL *temp1, *prev1;
	  // headHL is the head of the neighbor label table
	int ret = 0;

	while (temp != NULL) {

		gettimeofday(&current_time , NULL);
		double cur_time = ((double)current_time.tv_sec*1000000 + (double)current_time.tv_usec)/1000000;
		double delTimeDiff = cur_time - temp->lastUpdate;
		//printf("TEST: delTimeDiff: %f, label: %s, port: %s \n", delTimeDiff, temp->tier, temp->port);

		// If last updated local time is more than desired time
		if (delTimeDiff > 0.6) {
			//printf("TEST: Inside Time diff delete block (>30)\n");
			// if node to be removed is head
			if (temp == headHL) {
				//printf("TEST: Head node removed value was %s\n", temp->tier);
				headHL = temp->next;

			} else {
				prev->next = temp->next;
				//printf("TEST: other node removed value was %s\n", temp->tier);
				//free(temp);
				//return 1;
			}
			ret = 1;
		}
		else{
			prev = temp;
		}
		temp = temp->next;

	}
	return ret;
}

/*Similar to checkIfSubstring but without printf statements, used to update the Lable list when an interface goes down or up
checkIfSubstring is used in forward algorithm, the print statements are useful for forward algorithm analysis*/
 boolean checkIfSubstr(char* add1 , char* add2){

 	int posAdd1 = 0;
 	int posAdd2 = 0;
 	int val1 = 0;
 	int val2 = 0;
 	
 	   

	while(add1[posAdd1++] != '.');
 	while(add2[posAdd2++] != '.');

 	while( (add1[posAdd1] != '\0') && (add2[posAdd2] != '\0'))
 	{
		// printf("\n posAdd1 = %d  \n",posAdd1);  
 		// printf("\n add1[posAdd1] =%c",add1[posAdd1]);
		while( (add1[posAdd1] != '.') && (add1[posAdd1] != '\0'))
 		{

 			// printf("\n add1[posAdd1] = %c  posAdd1=%d\n",add1[posAdd1],posAdd1);   
			val1 = (val1 * 10 )+  add1[posAdd1] - '0' ;
			posAdd1++;
 		}
	
	 	// printf("\n posAdd2 = %d \n",posAdd2);  
 		// printf("\n add2[posAdd2] =%c",add2[posAdd2]);
 		while( (add2[posAdd2] != '.') && (add2[posAdd2] != '\0'))
 		{
 			// printf("\n add2[posAdd2] = %c  posAdd2=%d\n",add2[posAdd2],posAdd2);   
 			val2 = (val2 * 10 )+  add2[posAdd2] - '0' ;
			posAdd2++;
 		}
 		//printf("\nval1 = %d val2 = %d\n",val1,val2);
 		// printf("\n  add1[posAdd1] =%c add2[posAdd2] =%c",add1[posAdd1],add2[posAdd2]);
 		if(val1 != val2)
 		{
 			

 			return false;
 		}

		if(add1[posAdd1] == '\0' || add2[posAdd2] == '\0')
			break;
		posAdd1++;
		posAdd2++;
 		val1 = val2 = 0;
 	} 
 	

 	return true;
 }


/*Compares each label in the Label list(LL) with the neighbor table, if the substring matches, returns 0, else return 1
return 0 - There is a substring match and the label shouldn't be removed from the LL
return 1 - No substring match was found, indicating the label's interface is down, hence the label should be removed from the LL*/
int compare_NT(char *lab){
	struct nodeHL *temp = headHL;
	while(temp!=NULL){
		if(checkIfSubstr(temp->tier,lab)){
			return 0;
		}
		temp = temp->next;
	}
	return 1;
}

void insertToQuarantineTable(char inTier[20], char recvPort[5], char purpose[20]) {

	struct QuarantineTL *temp1; //create a new pointer temp 
	temp1 = (struct QuarantineTL *) malloc(sizeof(struct QuarantineTL)); //allocate memory to temp
	memset(temp1->tier,'\0',20); //set null in the memory allocated
	memset(temp1->port,'\0',5);
	
	memset(temp1->tier,'\0',20);

	if(strcmp(purpose,"Remove")==0)
		strcpy(temp1->purpose,"Remove");
	else if(strcmp(purpose,"Add")==0)
		strcpy(temp1->purpose,"Add");

	strcpy(temp1->tier, inTier); //copy the tier address to temp
	strcpy(temp1->port, recvPort);
	gettimeofday(&current_time , NULL);
	temp1->label_lost_time = ((double)current_time.tv_sec*1000000 + (double)current_time.tv_usec)/1000000;
	temp1->label_remove_time = temp1->label_lost_time + 3;

	if (Head_QTL == NULL) {

		Head_QTL = temp1;
		Head_QTL->next = NULL;

	} else {

		struct QuarantineTL *current = Head_QTL;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = temp1;
		temp1->next = NULL;

	}
}

void Quarantine(char purpose[20]){
	struct nodeTL *temp = headTL;
	while (temp != NULL) {
		if(compare_NT(temp->tier)){
			insertToQuarantineTable(temp->tier,temp->port,purpose);
			deleteTierAddr(temp->tier);
		}
		temp = temp->next;
	}
}

/**
 * displayNeighbor()
 *
 * method to print neighbour list entries
 *
 * @param inTier (char[]) - tier value
 *
 */
//void displayNeighbor(struct node *r) {
void displayNeighbor() {

	struct nodeHL *r;
	r = headHL;
	if (r == NULL) {
		//printf("No Neighbors\n");
		return;
	}
	//printf("My Neighbors\n");
	int i = 1;
	while (r != NULL) {
		//printf("Tier Address %d - Length %zd : %s : Port: %s\n", i,
				//strlen(r->tier), r->tier, r->port);
		i = i + 1;
		r = r->next;
	}
	//printf("\n");
}

/**
 * count()
 *
 * method to count number of neighbours
 *
 * @return localCount (int) - count of neighbour nodes
 */
int count() {
	struct nodeHL *n;
	int localCount = 0;
	n = headHL;
	while (n != NULL) {
		n = n->next;
		localCount++;
	}
	return localCount;
}

/**
 * contains(char[])
 *
 * whether there is a tier address in neighbor table or not
 *
 * @return true or false
 */
boolean containsTierAddress(char testStr[20]) {

	boolean check = false;

	struct nodeHL *fNode = headHL;

	if (fNode == NULL) {

		if(enableLogScreen){
			printf("\nERROR: Neighbor List is empty (Isolated Node)\n");
			printf("\nTEST: Before return check %d \n", check);
		}
		if(enableLogFiles){
			fprintf(fptr,"\nERROR: Neighbor List is empty (Isolated Node)\n");
			fprintf(fptr,"\nTEST: Before return check %d \n", check);
		}
		return check;
	}

	// traverse the list
	// testing
	while (fNode != NULL) {
		//while (fNode->next != NULL) {

		if ((strlen(fNode->tier) == strlen(testStr))
				&& ((strncmp(fNode->tier, testStr, strlen(testStr)) == 0))) {
			check = true;
			break;

		} else {
			fNode = fNode->next;
		}

	}
	//printf("TEST: Before return check %d \n", check);
	return check;
}

/**
 * setByTierPartial(char[],boolean)
 *
 * If there is a tier address partial match, it will set forwarding fields
 * Should only be used in an uplink scenario
 *
 * @param inTier       - tier address (partial) to be probed
 * @param setFWDFields - if true will set forwarding fields
 *
 * @return true or false
 */
boolean setByTierPartial(char inTier[20], boolean setFWDFields) {

	//printf("Inside setByTierPartial - helloList.h \n");
	boolean returnVal = false;

	struct nodeHL *fNode = headHL;

	if (fNode == NULL) {

		if(enableLogScreen)
			printf("\nERROR: Failed to set FWD Tier Address (Isolated Node)\n");
		if(enableLogFiles)
			fprintf(fptr,"\nERROR: Failed to set FWD Tier Address (Isolated Node)\n");
		return returnVal;
	}

	// traverse the list
	// testing
	while (fNode != NULL) {
		//while (fNode->next != NULL) {

		// Target Node
		// Length Check
		// Value check

		//	if (strlen(fNode->tier) == strlen(inTier)) {
		if (strncmp(fNode->tier, inTier, strlen(inTier)) == 0) {

			if (setFWDFields == true) {

				fwdTierAddr = (char *) malloc(20);
				memset(fwdTierAddr, '\0', 20);
				strcpy(fwdTierAddr, fNode->tier);

				// set fwd tier port
				fwdInterface = (char *) malloc(20);
				memset(fwdInterface, '\0', 20);
				strcpy(fwdInterface, fNode->port);

				returnVal = true;

			}

			returnVal = true;
			break;

		}

		//	}

		fNode = fNode->next;
	}

	return returnVal;
}

/**
 * setByTierOnly(char[],boolean)
 *
 * If there is a tier address complete match, it will set forwarding fields
 * smart method detects error before forwarding packet
 *
 * @param inTier       - tier address to be probed
 * @param setFWDFields - if true will set forwarding fields
 *
 * @return true or false
 */
boolean setByTierOnly(char inTier[20], boolean setFWDFields) {

	//printf("Inside setByTierOnly - helloList.h \n");
	boolean returnVal = false;

	struct nodeHL *fNode = headHL;

	if (fNode == NULL) {

		if(enableLogScreen)
			printf("ERROR: Failed to set FWD Tier Address (Isolated Node)\n");
		return returnVal;
	}

	// traverse the list
	// testing
	while (fNode != NULL) {
		//while (fNode->next != NULL) {

		// Target Node
		// Length Check
		// Value check
		if ((strlen(fNode->tier) == strlen(inTier))
				&& ((strncmp(fNode->tier, inTier, strlen(inTier)) == 0)) ) {

			if (setFWDFields == true) {
// NS what is happening with fwdTierAddr and fwdInterface
				fwdTierAddr = (char *) malloc(20);
				memset(fwdTierAddr, '\0', 20);
				strcpy(fwdTierAddr, fNode->tier);

				// set fwd tier port
				fwdInterface = (char *) malloc(20);
				memset(fwdInterface, '\0', 20);
				strcpy(fwdInterface, fNode->port);
				

				returnVal = true;

			}

			returnVal = true;
			break;

			//	}

		}

		fNode = fNode->next;
	}

	return returnVal;
}

// TODO
/**
 * setByTierManually(char[],boolean)
 *
 * a Manual method to set fwd fields
 * does not perform pre-check
 * AVOID IT
 *
 * Currently Used by:
 *  1. Forwarding Algorithm - Case A
 * @param inTier       - tier address to be set directly
 * @param setFWDFields - if true will set forwarding fields
 *
 * @return true or false
 */
boolean setByTierManually(char inTier[20], boolean setFWDFields) {

	//printf("Inside setByTierManually - helloList.h \n");
	boolean returnVal = false;

	if (setFWDFields == true) {

		// set fwd tier address
		fwdTierAddr = (char *) malloc(20);
		memset(fwdTierAddr, '\0', 20);
		strcpy(fwdTierAddr, inTier);

		// set fwd tier port
		fwdInterface = (char *) malloc(20);
		memset(fwdInterface, '\0', 20);
		strcpy(fwdInterface, "xxx");

		returnVal = true;

	}

	return returnVal;
}



/**
 * printNeighbourTable()
 *
 * print the neighbour table
 *
 * @return void
 extern void printNeighbourTable() {
 */



/**
 * findParntLongst(char[],char[])
 *
 * return the longest matching parent adress in the table 
 *
 * @return void

 */
 
 boolean checkIfSubstring(char* add1 , char* add2){

 	int posAdd1 = 0;
 	int posAdd2 = 0;
 	int val1 = 0;
 	int val2 = 0;
 	
 	if(enableLogScreen)
 		printf("\n %s Compare : label1 = %s and label2 = %s \n",__FUNCTION__,add1,add2);   

	while(add1[posAdd1++] != '.');
 	while(add2[posAdd2++] != '.');

 	while( (add1[posAdd1] != '\0') && (add2[posAdd2] != '\0'))
 	{
		// printf("\n posAdd1 = %d  \n",posAdd1);  
 		// printf("\n add1[posAdd1] =%c",add1[posAdd1]);
		while( (add1[posAdd1] != '.') && (add1[posAdd1] != '\0'))
 		{

 			// printf("\n add1[posAdd1] = %c  posAdd1=%d\n",add1[posAdd1],posAdd1);   
			val1 = (val1 * 10 )+  add1[posAdd1] - '0' ;
			posAdd1++;
 		}
	
	 	// printf("\n posAdd2 = %d \n",posAdd2);  
 		// printf("\n add2[posAdd2] =%c",add2[posAdd2]);
 		while( (add2[posAdd2] != '.') && (add2[posAdd2] != '\0'))
 		{
 			// printf("\n add2[posAdd2] = %c  posAdd2=%d\n",add2[posAdd2],posAdd2);   
 			val2 = (val2 * 10 )+  add2[posAdd2] - '0' ;
			posAdd2++;
 		}
 		//printf("\nval1 = %d val2 = %d\n",val1,val2);
 		// printf("\n  add1[posAdd1] =%c add2[posAdd2] =%c",add1[posAdd1],add2[posAdd2]);
 		if(val1 != val2)
 		{
 			if(enableLogScreen)
 				printf("\n%s :No substring match found in between (%s) and (%s)\n",__FUNCTION__, add1, add2);

 			return false;
 		}

		if(add1[posAdd1] == '\0' || add2[posAdd2] == '\0')
			break;
		posAdd1++;
		posAdd2++;
 		val1 = val2 = 0;
 	} 
 	if(enableLogScreen)
 		printf("\n%s :Exit- Matched substring: (%s) and (%s)\n",__FUNCTION__, add1, add2);

 	return true;
 }
 
 void findParntLongst(char* myTierAdd,char* parentTierAdd) 
 {
	struct nodeHL *fNode = headHL;  //NEIGHBOR TABLE
	char* temp;
	if (fNode == NULL) {
		if(enableLogScreen)
			printf("\nERROR: Neighbor List is empty (Isolated Node)\n");

	}

	while (fNode != NULL) {
		temp  = fNode->tier;		
		struct nodeTL *fNode1 = headTL;
		while(fNode1 !=NULL) {
			char *temp1 = fNode1->tier;

		if(strlen(temp1) > strlen(temp)){
           if(enableLogScreen)
			//printf("findParntLongst Inside first if temp =%s  and temp1 = %s", temp, temp1);
			//struct nodeTL *temp1 = headTL; 

			//int tempLen = findMatchedTeirAddrLength(myTierAdd,temp);
			if(strlen(unavailable_interface)!=0){
				if(strcmp(fNode->port,unavailable_interface)!=0){
					//printf("in in in in in in in %s",fNode->port);
					strcpy(parentTierAdd, temp);
					return;
				}
			}
			else if(checkIfSubstring(temp1,temp)){
				//longestMtchLength = tempLen;
				//if(enableLogScreen)
			     //printf("findParntLongst Inside second if temp = %s and temp1 = %s", temp, temp1);
				strcpy(parentTierAdd, temp);
				return;
			}
		}
		fNode1 = fNode1->next;
 		}
		fNode = fNode->next;
	}
	return;
 }



/**
 * findChildLongst(char[],char[])
 *
 * return the longest matching child adress in the table 
 *
 * @return void

 */

 //Modified by Supriya, on September 6, 2017.

 void findChildLongst(char* desTierAdd,char* childTierAdd, char* myLabel)
 {
	struct nodeHL *fNode = headHL;
	char* temp;
	if (fNode == NULL) {
		if(enableLogScreen)
			printf("\nERROR: Neighbor List is empty (Isolated Node)\n");

		return;
	}
	
	//initializing the longest matching length to 0
	//int longestMtchLength = 0;
	if(enableLogScreen)
			printf("\n%s: Finding the appropriate child to forward the packet to\n",__FUNCTION__);

	while (fNode != NULL) {
		temp  = fNode->tier;		
		// if(enableLogScreen)
		// 	printf("\nfindChildLongst : Current Neighbour = %s \n",temp);
		// if(enableLogFiles)
		// 	fprintf(fptr,"\nfindChildLongst : Current Neighbour = %s \n",temp);
		if(strlen(temp) >= strlen(myLabel)){// new line added by Supriya
			if(strlen(temp) <= strlen(desTierAdd)){

				//int tempLen = findUIDtoDestinationMatch(desTierAdd,temp);
				if(checkIfSubstring(desTierAdd,temp)){
					//ongestMtchLength = tempLen;
					strcpy(childTierAdd, temp);
					// if(enableLogScreen)
					// 	printf("\nfindChildLongst : Result = %s \n",childTierAdd);
					// if(enableLogFiles)
					// 	fprintf(fptr,"\nfindChildLongst : Result = %s \n",childTierAdd);
					return;
				}
			}
		}
		fNode = fNode->next;
	}

	// if(enableLogScreen)
	// 	printf("\n findChildLongst : Result = %s \n",childTierAdd);
	// if(enableLogFiles)
	// 	fprintf(fptr,"\n findChildLongst : Result = %s \n",childTierAdd);
	return;
 }

/**
 * examineNeighbourTable(char[])
 *
 * return the longest matching adress in the table with the destination address
 *
 *type = 1: case 3: check substring only if len_neighbor label < len_destination label
 *type = 2: case 4: check substring only if len_neighbor label <= len_my label and len_neighor label > len_dest label
 *type = 3: case 5: check substring only if len_neighbor label >= len_my label and len_neighor label < len_dest label
 */

 //modified by Supriya on August 28,2017
 int examineNeighbourTable(char* desTierAdd, char* longstMatchingNgbr,char* myLabel, int type) 
 {
 	int retVal = 1; //ERROR / FAILURE
	struct nodeHL *fNode = headHL;  // pointer to neighbor table 
	char* temp;
	if (fNode == NULL) {
		if(enableLogScreen)
			printf("\nERROR: Neighbor List is empty (Isolated Node)\n");

		return retVal;
	}
	
	//initializing the longest matching length to 0
	int longestMtchLength = 0; //changed to 0 from 1 on august 25, 2017
	//int tempLen = 0; 

	if(type == 1){
		while (fNode != NULL) {
			temp  = fNode->tier;	
			if(enableLogScreen){
				//printf("\n%s temp->%s desTierAdd-->%s",__FUNCTION__,temp,desTierAdd);
				printf("\n%s: Check if my neighbor: %s is a substring of destination label(uid): %s",__FUNCTION__,temp,desTierAdd);	
			}

			if(strlen(temp) < strlen(desTierAdd)){
				if(strlen(unavailable_interface)!=0){
							if(strcmp(fNode->port,unavailable_interface)!=0 && myTierValue > atoi(fNode->tier)){
								//printf("in in in in in in in %s",fNode->port);
								strcpy(longstMatchingNgbr, temp);
								return 0;
				
							}
						}

				else if(checkIfSubstring(desTierAdd,temp)){
					strcpy(longstMatchingNgbr, temp);
					return 0;
				}
			}

			fNode = fNode->next;
		}
	}
	else {
		if (type == 2){
			while (fNode != NULL) {
				temp  = fNode->tier;	
				if(enableLogScreen){
					//printf("\n%s temp->%s desTierAdd-->%s",__FUNCTION__,temp,desTierAdd);
					printf("\n%s: Check if destination label(uid): %s is a substring of my neighbor: %s",__FUNCTION__,desTierAdd,temp);	
				}

				if(strlen(temp) <= strlen(myLabel)){
					if(strlen(temp) > strlen(desTierAdd)){
						if(checkIfSubstring(desTierAdd,temp)){
							strcpy(longstMatchingNgbr, temp);
							return 0;
						}
					}
				}
				// tempLen = findMatchedTeirAddrLength(desTierAdd,temp);
				// if(enableLogScreen)
				// 	printf("\n %s Matched Length = %d",__FUNCTION__,tempLen);	
				// if(enableLogFiles)
				// 	fprintf(fptr,"\n %s Matched Length = %d",__FUNCTION__,tempLen);	
				// if(tempLen > longestMtchLength){
				// 	longestMtchLength = tempLen;
				// 	strcpy(longstMatchingNgbr, temp);
				// 	retVal = 0;
				// }
				fNode = fNode->next;
			}
		}
		else{
			while (fNode != NULL) {
				temp  = fNode->tier;	
				if(enableLogScreen){
					//printf("\n%s temp->%s desTierAdd-->%s",__FUNCTION__,temp,desTierAdd);
					printf("\n%s: Check if my neighbor: %s is a substring of destination label(uid): %s",__FUNCTION__,desTierAdd, temp);	
				}


				if(strlen(temp) >= strlen(myLabel)){
					if(strlen(temp) < strlen(desTierAdd)){
						if(checkIfSubstring(desTierAdd,temp)){
							strcpy(longstMatchingNgbr, temp);
							return 0;
						}
					}
				}	
				
				fNode = fNode->next;
			}

		}
	}

	return retVal;
 }


/**
 * findMatchedTeirAddrLength(char[],char[])
 *
 * find the matched length of two tier values
 *
 * @return length (int)
 */



 int findMatchedTeirAddrLength(char* add1 , char* add2){

 	int matchedLength = 0;
 	int posAdd1 = 0;
 	int posAdd2 = 0;
 	int val1 = 0;
 	int val2 = 0;
 	
 	if(enableLogScreen)
 		printf("\n %s Enter : label1 = %s label2 = %s \n",__FUNCTION__,add1,add2);   
 	if(enableLogFiles)
		fprintf(fptr,"\n %s Enter : label1 = %s label2 = %s \n",__FUNCTION__,add1,add2); 
	// skip the tier value of both the addresses


	while(add1[posAdd1++] != '.');
 	while(add2[posAdd2++] != '.');
		
 	// printf("\n posAdd1 = %d posAdd2 = %d \n",posAdd1,posAdd2);  

 	while( (add1[posAdd1] != '\0') && (add2[posAdd2] != '\0'))
 	{
		
 		// printf("\n posAdd1 = %d  \n",posAdd1);  
 		// printf("\n add1[posAdd1] =%c",add1[posAdd1]);
		while( (add1[posAdd1] != '.') && (add1[posAdd1] != '\0'))
 		{

 			// printf("\n add1[posAdd1] = %c  posAdd1=%d\n",add1[posAdd1],posAdd1);   
			val1 = (val1 * 10 )+  add1[posAdd1] - '0' ;
			posAdd1++;
 		}
		posAdd1++;

		
	 	// printf("\n posAdd2 = %d \n",posAdd2);  
 		// printf("\n add2[posAdd2] =%c",add2[posAdd2]);
 		while( (add2[posAdd2] != '.') && (add2[posAdd2] != '\0'))
 		{
 			// printf("\n add2[posAdd2] = %c  posAdd2=%d\n",add2[posAdd2],posAdd2);   
 			val2 = (val2 * 10 )+  add2[posAdd2] - '0' ;
			posAdd2++;
 		}
		posAdd2++;

 		// printf("\nval1 = %d val2 = %d\n",val1,val2);

 		// printf("\n  add1[posAdd1] =%c add2[posAdd2] =%c",add1[posAdd1],add2[posAdd2]);

 		if(val1 == val2)
 		{
 			matchedLength++;
 		}
 		else
 		{
 			break;
 		}
		
		if(add1[posAdd1] == '\0' || add2[posAdd2] == '\0')
			break;

 		val1 = val2 = 0;

 	} 
 	if(enableLogScreen)
 		printf("\n %s :Exit- Matched Length = %d",__FUNCTION__,matchedLength);
 	if(enableLogFiles)
		fprintf(fptr,"\n %s :Exit- Matched Length = %d",__FUNCTION__,matchedLength);
 	return matchedLength;
 }

/**
 * findUIDmatchfromNeighborTable(char[])
 *
 * return the neighbor table entry whose UID matches with the substring of the destination address
 *

 * @return int

 */
int findUIDmatchfromNeighborTable(char* desTierAdd,char* longstMatchingNgbr)
{
	int retVal = 1; //ERROR / FAILURE
	struct nodeHL *fNode = headHL;
	char* temp;
	if (fNode == NULL) {
		if(enableLogScreen)
			printf("\n ERROR: Neighbor List is empty (Isolated Node)\n");
		if(enableLogFiles)
			fprintf(fptr,"\n ERROR: Neighbor List is empty (Isolated Node)\n");
		return retVal;
	}

	//initializing the longest matching length to 0
	int longestMtchLength = 0;
	//int tempLen = 0;

	while (fNode != NULL) {
		temp  = fNode->tier;
		// if(enableLogScreen){
		// 	printf("\n%s temp->%s desTierAdd-->%s",__FUNCTION__,temp,desTierAdd);
		// 	printf("\n %s Checking the match for %s in desTierAdd=%s",__FUNCTION__,temp,desTierAdd);
		// }

		//tempLen = findUIDtoDestinationMatch(desTierAdd,temp);
		//if(enableLogScreen)
		//	printf("\n %s Matched Length = %d",__FUNCTION__,tempLen);

		if(checkIfSubstring(desTierAdd,temp)){
			//longestMtchLength = tempLen;
			strcpy(longstMatchingNgbr, temp);
			return 0;// success
		}
		fNode = fNode->next;
	}

	return retVal;
}


/**
 * findUIDtoDestinationMatch(char[],char[])
 *
 * find whether myUID is a substring of destinationUID (Here add1 is destination).
 *
 * @return length (int)
 */


//Not used anymore August 25, 2017 instead using checkIfSubstring()
int findUIDtoDestinationMatch(char* destAddr , char* neighborAddr){

	int matchedLength = 0;
	matchedLength  = findMatchedTeirAddrLength(destAddr, neighborAddr);
	printf("\n%s : Checking the neighbour address (%s) match with destination address(%s),  MatchedLength = %d",__FUNCTION__,neighborAddr, destAddr,matchedLength);
	return matchedLength;

}



//Code by JOE commented on August 25, 2017
// int findUIDtoDestinationMatch(char* add1 , char* add2){

// 	char destUID[20];
// 	char neighbourUID[20];

// 	// replaced myUID with neighbour - Aug 24
// 	getUID(neighbourUID,add2);
// 	getUID(destUID,add1);

//     if (strstr(destUID, neighbourUID) == NULL) {
//         printf("Substring not matched : %s not in %s", neighbourUID, destUID);
//         return 0;
//     }
//     else {
//         printf("Substring matched : %s", strstr(destUID, neighbourUID));
//         // Get the length
//         return sizeof(neighbourUID)-1;
//     }

// }

/**
 * CheckAllDestinationLabels(char[])
 *
 * check all the destination labels and find whether if any label matches with the destination label.
 *
 * @return length (int)
 */



int CheckAllDestinationLabels(char* dest){

	int retVal = 1; //ERROR / FAILURE
	struct nodeTL *fNode = headTL;
	char* temp;

	while (fNode != NULL) {
		temp  = fNode->tier;
		//printf("\n CheckAllDestinationLabels : The element is : [%s]",temp);
		printf("\n%s: Comparing destination label : [%s] with my label [%s]\n",__FUNCTION__,dest,temp);
		if ((strlen(temp) == strlen(dest))
			&& ((strncmp(temp, dest, strlen(dest)) == 0))){
			return 0;
		}
		fNode = fNode->next;
	}
	return 1;
}

//Function Added by supriya on August 31, 2017. This function checks if the destination label is a subsrting of 
//any of my labels(Which means that the destination is either my parent or grandparent). 
//IF yes then returns true and generates the address of the parent of the current label to whom the 
//the packet will be sent. 

boolean isDestSubstringOfMyLabels(char* destLabel,char* myMatchedLabel) 
 {
	struct nodeTL *fNode = headTL;
	char* temp;
	if (fNode == NULL) {
		if(enableLogScreen)
			printf("\nERROR: My label List is empty.\n");
		if(enableLogFiles)
			fprintf(fptr,"\nERROR: My label List is empty.\n");
		return false;
	}
	
	while (fNode != NULL) {
		temp  = fNode->tier;
		if(strlen(destLabel) < strlen(temp)){

			if(checkIfSubstring(destLabel,temp)){ // NS defiend up 

				strcpy(myMatchedLabel, temp);
				return true;
			}
		}
		fNode = fNode->next;
	}
	return false;
 }

//Function added by supriya 
 //This function checks if any of my label is a subsrting of the destination label.
 boolean isMyLabelSubstringOfDest(char* destLabel,char* myMatchedLabel) 
 {
	struct nodeTL *fNode = headTL;
	char* temp;
	if (fNode == NULL) {
		if(enableLogScreen)
			printf("\nERROR: My label List is empty.\n");
		return false;
	}
	
	while (fNode != NULL) {
		temp  = fNode->tier;
		if(strlen(destLabel) > strlen(temp)){

			if(checkIfSubstring(destLabel,temp)){

				strcpy(myMatchedLabel, temp);
				return true;
			}
		}
		fNode = fNode->next;
	}
	return false;
 }

/**
 * getParent()
 *
 * method to get the Parent from the current Tier address and store it in curParent.
 *
 * @return returns the parents tier value. returns 0 if the node is tier 1 node
 */

int getParentName(char* curParent,char* currentTier){

	int i = 0;
	int count = 0;
	int count1 = 0;
	int tierValue = 0;
	char* tierVal;

	//get the count of the '.'
	while(currentTier[i] != '\0'){
		if(currentTier[i] == '.'){
			count++;
		}
		i++;
	}

	i=0;

	//skip the first tier value
	while(currentTier[i] != '.'){
		tierVal[i] = currentTier[i];
		i++;
	}
	tierVal[i] = '\0';
	i = i+1;

	int k = 0;

	//store the parent address in 'curParent'
	while(currentTier[i] != '\0'){
		if(currentTier[i] == '.'){
			count1++;
		}
		if(count1 == count){
			break;
		} else{
			curParent[k] = currentTier[i];
			i++;
			k++;
		}
	}
	curParent[k] = '\0';

	tierValue = atoi(tierVal);
	printf("\nThe parent tier value is : %d\n", tierValue-1);
	strcat(tierVal, ".");
	strcat(tierVal, curParent);
	printf("\nThe parent is here : %s\n", tierVal);
	strcpy(curParent,tierVal);
	return tierValue-1;


}

void removeLabels(){

	int retVal = 1; //ERROR / FAILURE
	struct nodeTL *fNodeML = headTL;
	char* tempML;

	struct nodeHL *fNodeNT = headHL;
	char* tempNT;

	//for deletion
	struct nodeTL *temp, *prev;
	temp = headTL;

	int del = 0;
	int nodeDel = 0;

	while (fNodeML != NULL) {
		tempML  = fNodeML->tier;

		char curParent[20];
		memset(curParent,'\0',20);
		int prnt = getParentName(curParent, tempML);
		printf("\nThe parent of [%s] is [%s]\n",tempML,curParent);
		if(prnt>0) { //returns 0 if the present node is tier 1 node
			while (fNodeNT != NULL) {
				tempNT = fNodeNT->tier;
				if((strlen(tempNT) == strlen(curParent))
				   && ((strncmp(tempNT, curParent, strlen(curParent)) == 0))){
					del = 1;
					break;
				}
				fNodeNT = fNodeNT->next;
			}
			if(del ==0){
				printf("\nThe parent of [%s] is [%s]\n",tempML,curParent);
				if (fNodeML == headTL) {
					headTL = fNodeML->next;
				} else {
					prev->next = fNodeML->next;
					nodeDel = 1;
				}
			}
		}
		if(nodeDel == 0){
			prev = fNodeML;
		}
		fNodeML = fNodeML->next;
		del = 0;
		nodeDel = 0;
	}

}

#endif

int getTimeStamp(char *buf, uint len, struct timespec *ts)
{
  int ret;
  struct tm t;

  tzset();

  if(localtime_r(&(ts->tv_sec), &t) == NULL)
  {
    return 1;
  }

  ret = strftime(buf, len, "%F %T", &t);
  if(ret == 0)
  {
    return 2;
  }

  len -= ret - 1;

  ret = snprintf(&buf[strlen(buf)], len, ".%09ld", ts->tv_nsec);
  if(ret >= len)
  {
    return 3;
  }

  return 0;
}

void timestamp()
{
  clockid_t clk_id = CLOCK_REALTIME;
  const uint TIME_FORMAT_LENGTH = strlen("2000-12-31 12:59:59.123456789") + 1;
  char timeStampFormat[TIME_FORMAT_LENGTH];
  struct timespec ts;
  clock_gettime(clk_id, &ts);

  if(getTimeStamp(timeStampFormat, sizeof(timeStampFormat), &ts) != 0)
  {
    printf("getTimeStamp failed!\n");
  }

  else
  {

    printf("\nTimestamp - %s", timeStampFormat);
   
  }

  return;
}
//determine if the neighbor is a tier-1 node or not
int isTier1Neighbor(char port[20]){
	int ret = 0;
	
	struct nodeHL *fNode = headHL; 

	while (fNode != NULL) {

		if (strlen(fNode->port) == strlen(port)) {

			if (strncmp(fNode->port, port, strlen(port)) == 0 && strncmp(fNode->tier,"1",1) == 0) {
				return 1;
			}
		}

		fNode = fNode->next;
	}

	return ret;
}