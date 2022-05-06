/*
 * tierList.h
 *
 *  Created on: Aug 19, 2015
 *  Updated on: Aug 19, 2015
 *      Author: tsp3859
 */

#ifndef TIERLIST_H_
#define TIERLIST_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "boolean.h"
#include "errorControl.h"

int findTierAddr(char inTier[20]);
boolean freeGetTierAddr();

int myTotalTierAddress = 0;  // total number of tier address
char *returnAddr = NULL;     // tier address to return

struct nodeTL {
	char tier[20];          		// tier value
	char port[20];                  // received interface 
	struct nodeTL *next;      // next node
}*headTL;

/**
 * appendTierAddr()
 *
 * method to add tier address after previous tier address, called by insertTierAddr()
 *
 * @param inTier (char[]) - tier value
 *
 */
void appendTierAddr(char inTier[20], char recvPort[5]) {

	struct nodeTL *temp, *right;
	temp = (struct nodeTL *) malloc(sizeof(struct nodeTL));

	memset(temp->tier,'\0',20);
	memset(temp->port,'\0',5);

	strcpy(temp->tier, inTier);
	strcpy(temp->port, recvPort);

	right = (struct nodeTL *) headTL;
	while (right->next != NULL)
		right = right->next;
	right->next = temp;
	right = temp;
	right->next = NULL;
	//printf("TEST: Tier Address appended successfully %s\n", temp->tier);
}

/**
 * addTierAddr()
 *
 * method to add new tier address after previous one, called by insertTierAddr()
 *
 * @param inTier (char[]) - tier value
 *
 */
void addTierAddr(char inTier[20], char recvPort[5]) {

	struct nodeTL *temp; //create a new pointer temp 
	temp = (struct nodeTL *) malloc(sizeof(struct nodeTL)); //allocate memory to temp
	memset(temp->tier,'\0',20); //set null in the memory allocated
	memset(temp->port,'\0',5);

	strcpy(temp->tier, inTier); //copy the tier address to temp
	strcpy(temp->port, recvPort);
	if (headTL == NULL) {   
		headTL = temp;//if there is nothing in headTL, we set temp as headTL
		headTL->next = NULL; //the pointer will then point to null
	} else {
		temp->next = headTL; //next of temp is set to headTL. we set the next pointer of temp to the head of the linked list 
		headTL = temp; // now the headTL is set as temp. the head of the linkedlist is set as temp.
	}
	//printf("\naddTierAddr: Tier Address added successfully %s\n", temp->tier);
}

/**
 * insertTierAddr()
 *
 * method to add tier address into a list (duplicate entry-free)
 *
 * @param inTier (char[]) - tier value
 *
 */
int insertTierAddr(char inTier[20], char recvPort[5]) {

    printf("\ninsertTierAddr is called , label=%s labelLength=%d\n",inTier,(int)strlen(inTier));
	struct nodeTL *temp;
	temp = headTL;
	int checkNode = findTierAddr(inTier);
	if (temp == NULL) { //if their is no initial tier address, we will call addTierAddress to add tier address to list

		addTierAddr(inTier, recvPort); //this fuction adds tier address ti list
		myTotalTierAddress++;//keeps track of the tier address. incremented once we add tier address to list

	} else { //if there is initioal tier address in headTL, we call append TierAddr, which will append tier address to list

		
        printf("\ninsertTierAddr checkNode=%d ",checkNode);
        if (checkNode == 1) {
            appendTierAddr(inTier, recvPort);
			myTotalTierAddress++; //keeps track of the tier address. incremented once we add tier address to list
	    }
	}
	return checkNode;
}

/**
 * findTierAddr()
 *
 * method to check whether a Tier Address is present or not in tierList
 * (strict comparison)
 *
 * @param inTier (char[]) - tier value
 *
 * @return returnVal (int) - 0 for present otherwise 1
 */
int findTierAddr(char inTier[20]) {

	int returnVal = 1;

	struct nodeTL *fNode = headTL;

	// traverse the list

	while (fNode != NULL) {
		//while (fNode->next != NULL) {

		// Target Tier Address
		// Length Check
		// Value check

        printf("\n findTierAddr : Label to check=%s, current label=%s \n",inTier,fNode->tier);

		if (strlen(fNode->tier) == strlen(inTier)) {
			if (strncmp(fNode->tier, inTier, strlen(inTier)) == 0) {

				returnVal = 0;
				break;

			}

		}

		fNode = fNode->next;
	}

	return returnVal;
}

/**
 * deleteTierAddr()
 *
 * method to delete Tier Address from the list
 *
 * @return status (int) - method return value
 */
int deleteTierAddr(char inTier[20]) {

	if (headTL == NULL) {
				printf("My Label list is empty");
                return 0;
        } else {
                struct nodeTL *current = headTL;
                struct nodeTL *prev = NULL;
                while (current != NULL) {
					
                        if (strcmp(inTier,current->tier)==0) {

                                //printf("Removing %s \n", inet_ntoa(current->ip_addr));
                                if (headTL == current) {
                                        headTL = headTL->next;
										free(current);
                                        current = headTL;
                                        continue;
                                } else {
                                        prev->next = current->next;
										free(current);
                                        current = prev;
                                }
                        }
                        prev = current;
                        current= current->next;
                }
        }

	return 1;
}

/**
 * displayTierAddr()
 *
 * method to print all Tier Address entries
 *
 * @param inTier (char[]) - tier value
 *
 */
void displayTierAddr() {

	struct nodeTL *r;
	r = headTL;
	if (r == NULL) {
		//printf("I do not have any Tier Address\n");
		return;
	}

	//printf("My Tier Addresses\n");
	int i = 1;
	while (r != NULL) {
		//printf("Tier Address %d - Length %zd : %s \n", i,strlen(r->tier), r->tier);
		i = i + 1;
		r = r->next;
	}
	//printf("\n");
}

/**
 * getCountOfTierAddr()
 *
 * method to return count of tier address for a current node
 *
 * @return localCount (int) - count of Tier Address
 */
int getCountOfTierAddr() {
	myTotalTierAddress = 0;
	struct nodeTL *fNode = headTL;
	while(fNode!=NULL){
		myTotalTierAddress++;
		fNode = fNode->next;
	}
	return myTotalTierAddress;
}

/**
 * containsMyTierAddr(char[])
 *
 * whether there is a tier address in tier list or not
 *
 * @return true or false
 */
boolean containsMyTierAddr(char testStr[20]) {

	//printf("TEST: Inside containsTierAddress - tierList.h \n");
	boolean check = false;

	struct nodeTL *fNode = headTL;

	if (fNode == NULL) {

		//printf("ERROR: No Tier Address (Ready to roll out)\n");
		errorCount++;
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
 * getTierAddr(int index)
 *
 * method to get tier address specified by index
 *
 * @param index (int) - index position
 *
 * @return returnVal (int) - 0 for present otherwise 1
 */
char* getTierAddr(int index) {

	struct nodeTL *fNode = headTL;

	int pos = 0;
	while (fNode != NULL) {

		if (pos == index) {

			int tempSize = strlen(fNode->tier);
			returnAddr = (char*) malloc(tempSize);
			memset(returnAddr, '\0', tempSize);
			strcpy(returnAddr, fNode->tier);

			//memcpy(returnAddr, fNode->tier, tempSize);
			//printf("TEST: getTierAddr is called: TA %s} \n",returnAddr);

			break;

		} else {
			pos++;
			fNode = fNode->next;
		}

	}

	return returnAddr;
}

/**
 * freeGetTierAddr()
 *
 * method to free return tier address field
 *
 * @return returnVal (boolean) - true if success
 */
boolean freeGetTierAddr() {

	boolean returnValue = false;
	if (returnAddr != NULL) {

		free(returnAddr);
	//	printf("TEST: Return Address freed \n");
		returnValue = true;
	}

	return returnValue;
}

#endif

