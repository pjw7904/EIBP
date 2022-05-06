#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "tierList.h"

extern char* tierAddress;
extern uint8_t  cache_BR_address[];
extern void findParntLongst(char* myTierAdd,char* parentTierAdd);
extern boolean setByTierOnly(char inTier[20], boolean setFWDFields);
extern int endNetworkSend(char[], uint8_t *, int);
extern int buildPayloadQuery(uint8_t *, char[], int);
extern int setTierInfo(char tierValue[]);
extern int packetForwardAlgorithm(char currentTier[], char desTier[]);
extern int cache_nIP;
extern char cache_bufferIP[];

int sendQueryRequest(unsigned char ipDestTemp[],char tierDest[], int nIP, char bufferIP[]){
    /*
    char parentTierAdd[20];
    memset(parentTierAdd,'\0',20);
    printf("ERROR: End destination tier address not available \n");
    findParntLongst(tierAddress,parentTierAdd);
    setByTierOnly(parentTierAdd, true);
    printf("\nSending a query to Tier-1 node for BR address");
							
    uint8_t *mplrPayload = allocate_ustrmem (IP_MAXPACKET);

    int mplrPayloadLen = 0;

    mplrPayloadLen = buildPayloadQuery(mplrPayload,ipDestTemp,MESSAGE_TYPE_QUERY_REQUEST);
							
    if (mplrPayloadLen) {
        endNetworkSend(fwdInterface, mplrPayload, mplrPayloadLen);
    }
    free(mplrPayload);

    */
   int ret = -1;
   if(strlen(cache_BR_address)!=0){
								printf("cached BR address %s",cache_BR_address);
								strcpy(tierDest, cache_BR_address);

								setTierInfo(headTL->tier);
								ret = packetForwardAlgorithm(tierAddress,
								tierDest);
							}
							else{
							cache_nIP = nIP;
							memset(cache_bufferIP, '\0', nIP);
							
							memcpy(cache_bufferIP,bufferIP,nIP);
							char parentTierAdd[20];
							memset(parentTierAdd,'\0',20);
							printf("ERROR: End destination tier address not available \n");
							findParntLongst(tierAddress,parentTierAdd);
							setByTierOnly(parentTierAdd, true);
							printf("\nSending a query to Tier-1 node for BR address");
							
							uint8_t *mplrPayload = allocate_ustrmem (IP_MAXPACKET);

							int mplrPayloadLen = 0;

							mplrPayloadLen = buildPayloadQuery(mplrPayload,ipDestTemp,MESSAGE_TYPE_QUERY_REQUEST);
							
							if (mplrPayloadLen) {
                        			endNetworkSend(fwdInterface, mplrPayload, mplrPayloadLen);
                			}
							free(mplrPayload);
							}
								
					return ret;

}