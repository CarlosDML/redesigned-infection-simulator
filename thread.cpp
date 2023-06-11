#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <queue>

/*
Author: Carlos D Maldonado Lizardo

Purpose: Small basic infection simulator.

Reason for multi-threading: will increase the speed of the simulation as several threads can work on different parts of the population, instead of one thread
moving though the population itteratively.

Shared variable: Both the infected counter and the population array are shared but only the infected counter can be a cause for a race condition as the threads
work on different parts of the array at a time.
*/


//initialization for infected counter(shared variable)
int infected = 0;
int populationID;
//initialization for population array(shared variable), in array 0=healthy 1=infected
int population[100];
std::queue<int> populationInfected;
//random chance of infected
double infectchance = 0.2;
//Mutex lock
std::mutex criticalLock;

//function for running infection
void runInfection(int maxRange){
    if (!populationInfected.empty())
    {
        for (int i = 0; i < populationInfected.size(); i++)
        {
            if (population[populationInfected.front()] < 5)
            {
                populationID = population[populationInfected.front()];
                populationID++;
                populationInfected.pop();
                populationInfected.push(populationID);
                continue;
            }
            populationInfected.pop();
            
        }
        
    }
    
    //loops though set area of the array
    for (int i = (maxRange-50); i < maxRange; i++)
    {
        //generates a random number between 0 and 1, if less than the infection chance the person become infected
        if (((double) rand()/(RAND_MAX)) < infectchance)
        {
            //sets the person as infected in array, prints someone was infected and increases the counter
            population[i] = 1;
            //pushed infected indavidual onto the stack
            populationInfected.push(i);
            //std::cout << "Person infected" << std::endl;
            //locks critical section
            criticalLock.lock();
            //Increases infected counter, can cause race condition if both threads access at the same time
            infected++;
            //unlocks critical section
            criticalLock.unlock();
        }
        
    }
}

int main(){

    //initial infected person
    population[0] = 1;

    //creates threads to run infection each thread had 50 induvidulas to run the simulation on
    std::thread t(runInfection,50);
    std::thread t1(runInfection,100);
    
    t.detach(); //Lets thread be run seperately from main
    t1.detach(); //Lets thread be run seperately from main
   
   std::cout << "Running Simulation...." << std::endl;

    std::cout << "Total infected = " << infected << std::endl;
}