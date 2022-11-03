//AUEB Operating Systems Project 2021-22

//Konstantinos Marko 3190112
//DImitrios Vovlas 3150016

//chmod +x test-res.sh
//./test-res.sh

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "p3190112-p3150016-res.h"

int* customerID;

pthread_mutex_t screenMutex;

int availableTicketLines = numTele;
pthread_mutex_t ticketLineMutex;
pthread_cond_t ticketLineCond;

int availableCashLines = numCash;
pthread_mutex_t cashLineMutex;
pthread_cond_t cashLineCond;

int earnings = 0;
pthread_mutex_t earningsMutex;

unsigned int randSeed;

//---------- Seats Arrays ----------
int seatsA[numZoneA][numSeats];
int seatsB[numZoneB][numSeats];
pthread_mutex_t seatsAMutex;
pthread_mutex_t seatsBMutex;
    
//---------- Available seats per row ----------
int availableSeatsA[numZoneA];
int availableSeatsB[numZoneB];

//---------- Variables for statistics ----------
int successCount = 0;
pthread_mutex_t successCountMutex;
int failCountSeat = 0;
pthread_mutex_t failCountSeatMutex;
int failCountCard = 0;
pthread_mutex_t failCountCardMutex;
int customersCountCashLine = 0;
pthread_mutex_t customersCountCashLineMutex;

int sumTicketLineTime = 0; // ticketLineTimer * 100 / numberOfCustomers
pthread_mutex_t sumTicketTimeMutex;
int sumCashLineTime = 0; // cashLineTimer * 100 / customersCountCashLine
pthread_mutex_t sumCashTimeMutex;
int sumCustomersTime = 0; // customerTimer * 100 / numberOfCustomers
pthread_mutex_t sumCustomersTimeMutex;

//-------------------------------------------------- This is the thread fucntion --------------------------------------------------
void *threadRunner(void *customerID){
    
    //---------- Initializations for timing statistics ----------
    struct timespec customerStart;
    struct timespec customerFinish;
    struct timespec ticketLineWaitFinish;
    struct timespec cashLineWaitStart;
    struct timespec cashLineWaitFInish;
    
    int customerThreadID = *((int *)customerID);
    int seatsAmount = (rand_r(&randSeed) % numSeatHigh) + numSeatLow;
    
    int zoneA = 0; // 0 = false | 1 = true
    int zoneB = 0; // 0 = false | 1 = true
    int isAvailableA = 0; // 0 = false | 1 = true
    int isAvailableB = 0; // 0 = false | 1 = true
    int foundSeats = 0; // 0 = false | 1 = true
    
    printf("New customer ID: %d \n", customerThreadID); 
    
    //---------- Ticket Line Timer Start | Customer Timer Start ----------
    clock_gettime(CLOCK_REALTIME, &customerStart);
    
    //---------- Checks ticket lines availability ----------
    pthread_mutex_lock(&ticketLineMutex);
    while(availableTicketLines < 1){
        pthread_cond_wait(&ticketLineCond, &ticketLineMutex);
    }
    pthread_mutex_unlock(&ticketLineMutex);
    
    pthread_mutex_lock(&ticketLineMutex);
    availableTicketLines = availableTicketLines - 1;
    pthread_mutex_unlock(&ticketLineMutex); 
    
    //---------- Ticket Line Time Finish ----------
    clock_gettime(CLOCK_REALTIME, &ticketLineWaitFinish);
    pthread_mutex_lock(&sumTicketTimeMutex);
    sumTicketLineTime = sumTicketLineTime + (ticketLineWaitFinish.tv_sec - customerStart.tv_sec);
    pthread_mutex_unlock(&sumTicketTimeMutex);
    
    //---------- Ticket Line Sleep [timeSeatLow, timeSeatHigh] seconds ----------
    sleep(rand_r(&randSeed) % (timeSeatHigh +5 - timeSeatLow) + timeSeatLow); 
    
    //---------- Decide Zone ----------
    int zoneDecision = (rand_r(&randSeed) % (100)) + 1;
    if(zoneDecision <= probZoneA){
        
        pthread_mutex_lock(&screenMutex);
        printf("Customer ID: %d | Zone A | Amount of seats: %d | ", customerThreadID, seatsAmount);
        zoneA = 1;
        
        //---------- Search for available Zone A seats ----------
        int seatCount = 0;
        int rowCount = -1;
        
        pthread_mutex_lock(&seatsAMutex);
        for(int i=0; i < numZoneA; i++){ //Checks every row for availability
            if(rowCount == -1){
                if(availableSeatsA[i] >= seatsAmount){ //For the empty row
                    printf("Row: %d | Seat: ", i+1);
                    availableSeatsA[i] = availableSeatsA[i] - seatsAmount;
                    rowCount = i;
                    isAvailableA = 1;
                    for(int j=0; j < numSeats; j++){
                        if(seatsA[i][j] == -1 && seatCount < seatsAmount){
                            printf("%d ", j+1);
                            seatsA[i][j] = customerThreadID;
                            seatCount = seatCount + 1;
                        }
                    }
                }
            }
        }
        pthread_mutex_unlock(&seatsAMutex);
        
        if(isAvailableA == 1){
            printf("| Seats Reserved\n");
            foundSeats = 1;
        }else{
            printf("| Reservation Failed | No Available Seats in Zone A \n");
            foundSeats = 0;
            
            //---------- Counting Failed Reservations ----------
            pthread_mutex_lock(&failCountSeatMutex);
            failCountSeat = failCountSeat + 1;
            pthread_mutex_unlock(&failCountSeatMutex);
        }
        pthread_mutex_unlock(&screenMutex);
        
    }else{
        
        pthread_mutex_lock(&screenMutex);
        printf("Customer ID: %d | Zone B | Amount of seats: %d | ", customerThreadID, seatsAmount);
        zoneB = 1;
        
        //---------- Search for available Zone B seats ----------
        int seatCount = 0;
        int rowCount = -1;
        
        pthread_mutex_lock(&seatsBMutex);
        for(int i=0; i < numZoneB; i++){ //Checks every row for availability
            if(rowCount == -1){
                if(availableSeatsB[i] >= seatsAmount){
                    printf("Row: %d | Seat: ", i+1);
                    availableSeatsB[i] = availableSeatsB[i] - seatsAmount;
                    rowCount = i;
                    isAvailableB = 1;
                    for(int j=0; j < numSeats; j++){ //For the empty row
                        if(seatsB[i][j] == -1 && seatCount < seatsAmount){
                            printf("%d ", j+1);
                            seatsB[i][j] = customerThreadID;
                            seatCount = seatCount + 1;
                        }
                    }
                }
            }
        }
        pthread_mutex_unlock(&seatsBMutex);
        
        if(isAvailableB == 1){
            printf("| Seats Reserved \n");
            foundSeats = 1;
            
        }else{
            //---------- No seats available so customer avoids cash line ----------  
            printf("| Reservation Failed | No Available Seats in Zone B \n");
            foundSeats = 0;
            
            //---------- Counting Failed Reservations ----------
            pthread_mutex_lock(&failCountSeatMutex);
            failCountSeat = failCountSeat + 1;
            pthread_mutex_unlock(&failCountSeatMutex);
        }
        pthread_mutex_unlock(&screenMutex);
    }
    
    //---------- Free up a position on ticket line ----------
    pthread_mutex_lock(&ticketLineMutex);
    availableTicketLines = availableTicketLines + 1;
    pthread_mutex_unlock(&ticketLineMutex);
    pthread_cond_signal(&ticketLineCond);
    
    //---------- Checks cash lines availability ----------
    if(foundSeats == 1){
        
        //---------- Cash Line Timer Start ---------- 
        clock_gettime(CLOCK_REALTIME, &cashLineWaitStart);
        
        //---------- Counting Cash Line Customers ----------
        pthread_mutex_lock(&customersCountCashLineMutex);
        customersCountCashLine = customersCountCashLine + 1;
        pthread_mutex_unlock(&customersCountCashLineMutex);
        
        pthread_mutex_lock(&cashLineMutex);
        while(availableCashLines < 1){
            pthread_cond_wait(&cashLineCond, &cashLineMutex);
        }
        pthread_mutex_unlock(&cashLineMutex);
            
        pthread_mutex_lock(&cashLineMutex);
        availableCashLines = availableCashLines - 1;
        pthread_mutex_unlock(&cashLineMutex);
        
        //---------- Cash Line Timer Finish ---------- 
        clock_gettime(CLOCK_REALTIME, &cashLineWaitFInish);
        pthread_mutex_lock(&sumCashTimeMutex);
        sumCashLineTime = sumCashLineTime + ( cashLineWaitFInish.tv_sec - cashLineWaitStart.tv_sec);
        pthread_mutex_unlock(&sumCashTimeMutex);
        
        //---------- Cash Line Sleep [timeCashLow, timeCashHigh] seconds ---------- 
        sleep(rand_r(&randSeed) % (timeCashHigh +1 - timeCashLow) + timeCashLow); 
        
        //---------- Checks if payment is successful ---------- 
        int paymentProcedure = (rand_r(&randSeed) % 100) + 1;
        
        if(paymentProcedure < probCardSuccess){
            
            if(zoneA == 1 || zoneB == 1){
                
                //---------- Counting Successful Payments ----------
                pthread_mutex_lock(&successCountMutex);
                successCount = successCount + 1;
                pthread_mutex_unlock(&successCountMutex);
                
                //---------- Add payment amount to earnings ----------
                pthread_mutex_lock(&screenMutex);
                pthread_mutex_lock(&earningsMutex);
                if(zoneA == 1){
                    printf("Customer ID: %d | Successful payment | Total: %d euros \n",  customerThreadID, seatsAmount*costZoneA);
                    earnings = earnings + seatsAmount*costZoneA;
                }else if(zoneB == 1){
                    printf("Customer ID: %d | Successful payment | Total: %d euros \n",  customerThreadID, seatsAmount*costZoneB);
                    earnings = earnings + seatsAmount*costZoneB;
                }
                pthread_mutex_unlock(&earningsMutex);
                pthread_mutex_unlock(&screenMutex);
            }
            
        }else{
            
            pthread_mutex_lock(&screenMutex);
            printf("Customer ID: %d | Payment failed \n", customerThreadID);
            pthread_mutex_unlock(&screenMutex);
            
            //---------- Counting Failed Payments ----------
            pthread_mutex_lock(&failCountCardMutex);
            failCountCard = failCountCard + 1;
            pthread_mutex_unlock(&failCountCardMutex);
            
            //---------- Return seats and money because the payment failed ----------
            if(zoneA == 1){
                
                pthread_mutex_lock(&seatsAMutex);
                int lockCountA = 0;
                for(int i=0; i < numZoneA; i++){
                    for(int j=0; j < numSeats; j++){
                        if(seatsA[i][j] == customerThreadID){
                            seatsA[i][j] = -1;
                            if(lockCountA == 0){
                                availableSeatsA[i] = availableSeatsA[i] + seatsAmount;
                                lockCountA = 1;
                            }
                        }
                    }
                }
                pthread_mutex_unlock(&seatsAMutex);
                
            }
            
            if(zoneB == 1){
                
                pthread_mutex_lock(&seatsBMutex);
                int lockCountB = 0;
                for(int i=0; i < numZoneB; i++){
                    for(int j=0; j < numSeats; j++){
                        if(seatsB[i][j] == customerThreadID){
                            seatsB[i][j] = -1;
                            if(lockCountB == 0){
                                availableSeatsB[i] = availableSeatsB[i] + seatsAmount;
                                lockCountB = 1;
                            }
                        }
                    }
                }
                pthread_mutex_unlock(&seatsBMutex);
                
                pthread_mutex_lock(&earningsMutex);
                earnings = earnings - seatsAmount*costZoneB;
                pthread_mutex_unlock(&earningsMutex);
            }
        }
        
        //---------- Free up a position on cash line ----------
        pthread_mutex_lock(&cashLineMutex); 
        availableCashLines = availableCashLines + 1;
        pthread_mutex_unlock(&cashLineMutex);
        pthread_cond_signal(&cashLineCond);
    }
    
    //---------- Customer Time Finish ---------- 
    clock_gettime(CLOCK_REALTIME, &customerFinish); 
    pthread_mutex_lock(&sumCustomersTimeMutex);
    sumCustomersTime = sumCustomersTime + (customerFinish.tv_sec - customerStart.tv_sec);
    pthread_mutex_unlock(&sumCustomersTimeMutex);
    
    pthread_exit(&customerThreadID);
    return NULL;
}

//-------------------------------------------------- This is the main fucntion --------------------------------------------------
int main(int argc, char *argv[]){
    
    if(argc != 3){
        printf("Error: Invalid program argument...  \n");
        exit(-1);
    }
    
    //---------- Zone A seats arrays initialization ----------              
    for(int x=0; x < numZoneA; x++){                                   // -1: a seat is empty
        availableSeatsA[x] = numSeats;                                    //  x: a seat is reserved from customer with ID x
        for(int y=0; y < numSeats; y++){
            seatsA[x][y] = -1;
        }
    }
    
    //---------- Zone B seats arrays initialization ----------
     for(int x=0; x < numZoneB; x++){
        availableSeatsB[x] = numSeats;
        for(int y=0; y < numSeats; y++){
            seatsB[x][y] = -1;
        }
    }
    
    //---------- Initialize Mutexes ----------
    pthread_mutex_init(&ticketLineMutex, NULL);
    pthread_mutex_init(&cashLineMutex, NULL);
    pthread_mutex_init(&earningsMutex, NULL);
    pthread_mutex_init(&seatsAMutex, NULL);
    pthread_mutex_init(&seatsBMutex, NULL);
    pthread_cond_init(&ticketLineCond, NULL);
    pthread_cond_init(&cashLineCond, NULL);
    pthread_mutex_init (&successCountMutex, NULL);
    pthread_mutex_init (&failCountSeatMutex, NULL);
    pthread_mutex_init (&failCountCardMutex, NULL);
    pthread_mutex_init (&customersCountCashLineMutex, NULL);
    pthread_mutex_init (&screenMutex, NULL);
    pthread_mutex_init (&sumTicketTimeMutex, NULL);
    pthread_mutex_init (&sumCashTimeMutex, NULL);
    pthread_mutex_init (&sumCustomersTimeMutex, NULL);
    
    int customerThread;
    
    //---------- Gets number of customers from arguments ----------
    int numberOfCustomers = atoi(argv[1]);
    
    //---------- Initialization for threads ----------
    customerID = (int*) malloc (numberOfCustomers * sizeof(int));
    pthread_t customerThreads[numberOfCustomers];
    
    //---------- Gets random seed from arguments ----------
    randSeed = (unsigned int) atoi(argv[2]);
    
    printf("----------------------------------------------------------------------------\n");
    printf("----- Welcome to Theater Ticketing App | There are %d Customers Today -----\n", numberOfCustomers);
    printf("----------------------------------------------------------------------------\n");
    
    //---------- Make a thread for every customer and run thread function ----------
    for(int i = 0; i < numberOfCustomers; i++){
        customerID[i] = i +  1;
        
        //---------- Telephone Sleep [timeTeleLow, timeTeleHigh] seconds ----------
        if(i != 0){
            sleep(rand_r(&randSeed) % (timeTeleHigh +1 - timeTeleLow) + timeTeleLow);
        }
        customerThread = pthread_create(&customerThreads[i], NULL, threadRunner, &customerID[i]);
    }
    
    //---------- Parent thread waits for every child thread to exit ----------
    for(int j = 0; j < numberOfCustomers; j++){
        pthread_join(customerThreads[j], NULL);
    }
    
    //-------------------------------------------------- This is the final print section --------------------------------------------------
    
    printf("\n");
    printf("------------------------\n");
    printf("----- Theater Plan -----\n");
    printf("------------------------\n");
    printf("\n");
    
    printf("------------------------- Zone A -------------------------\n");
    for(int x=0; x < numZoneA; x++){
            printf("ROW %d -> ", x + 1);
        for(int y=0; y < numSeats; y++){
            if(seatsA[x][y] == -1){
            	printf("[ ] ");
            }else{
            	printf("[%d] ", seatsA[x][y]);
            }
        }
        printf("\n");
    }
    printf("\n");
    printf("------------------------- Zone B-------------------------\n");
    for(int x=0; x < numZoneB; x++){
            printf("ROW %d -> ", x + 1);
        for(int y=0; y < numSeats; y++){
        	if(seatsB[x][y] == -1){
            	printf("[ ] ");
            }else{
            	printf("[%d] ", seatsB[x][y]);
            }
        }
        printf("\n");
    }
    
    printf("\n");
    printf("----------------------\n");
    printf("----- Statistics -----\n");
    printf("----------------------\n");
    printf("\n");
    
    //---------- Prints earnings ----------
    printf("Today's earnings: %d euros\n", earnings);
    printf("\n");
     //---------- Prints successful persentage ----------
    printf("Successful ticket purchases persentage: %.2f%% \n", (float) successCount * 100 / numberOfCustomers);
    printf("\n");
     //---------- Prints failed seat persentage ----------
    printf("Failed ticket purchases persentage due to seats unavailability: %.2f%% \n", (float) failCountSeat * 100 / numberOfCustomers);
    printf("\n");
     //---------- Prints failed card persentage ----------
    printf("Failed ticket purchases persentage due to card problem: %.2f%% \n", (float) failCountCard * 100 / numberOfCustomers);
    printf("\n");
    printf("Ticket Line Average Waiting Time: %.2f sec\n", (float) sumTicketLineTime / numberOfCustomers);
    printf("\n");
    printf("Cash Line Average Waiting Time: %.2f sec\n", (float) sumCashLineTime / customersCountCashLine);
    printf("\n");
    printf("Whole Process Average Time: %.2f sec\n", (float) sumCustomersTime / numberOfCustomers);
    
    //---------- Deallocates threads, mutexes and memory ----------
    pthread_mutex_destroy(&ticketLineMutex);
    pthread_mutex_destroy(&cashLineMutex);
    pthread_mutex_destroy(&earningsMutex);
    pthread_mutex_destroy(&seatsAMutex);
    pthread_mutex_destroy(&seatsBMutex);
    pthread_cond_destroy(&ticketLineCond);
    pthread_cond_destroy(&cashLineCond);
    pthread_mutex_destroy (&successCountMutex);
    pthread_mutex_destroy (&failCountSeatMutex);
    pthread_mutex_destroy (&failCountCardMutex);
    pthread_mutex_destroy (&customersCountCashLineMutex);
    pthread_mutex_destroy (&screenMutex);
    pthread_mutex_destroy (&sumTicketTimeMutex);
    pthread_mutex_destroy (&sumCashTimeMutex);
    pthread_mutex_destroy (&sumCustomersTimeMutex);
    free(customerID);
    return 0;
}
