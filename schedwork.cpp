#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here

// Add your implementation of schedule() and other helper functions here
size_t totalWorkTime(const AvailabilityMatrix& avail, Worker_T workerID, size_t startDay, size_t endDay) 
{
    if (startDay > endDay) 
    {
        return 0;
    } 
    else if (startDay == endDay) 
    {
        return avail[startDay][workerID];
    } 
    else 
    {
        size_t mid = (startDay + endDay) / 2;
        int workTime1 = totalWorkTime(avail, workerID, startDay, mid);
        int workTime2 = totalWorkTime(avail, workerID, mid+1, endDay);
        return workTime1 + workTime2;
    }
}
bool sortBySecond(const pair<Worker_T, size_t>& a, const pair<Worker_T, size_t>& b) {
    return a.second < b.second;
}
std::vector<std::pair<Worker_T, size_t>> findPriot(const AvailabilityMatrix& avail)
{
  std::vector<std::pair<Worker_T, size_t>> workersPrior;
  size_t total_worker = avail[0].size();
  size_t total_days = avail.size();
  for(Worker_T i = 0; i < total_worker; i++) 
    {
        size_t worktime = totalWorkTime(avail,i,0,total_days-1);
        //cout<<"worktime "<<worktime<<endl;
        workersPrior.push_back(make_pair(i,worktime));
    }//rank workers by priority
  sort(workersPrior.begin(),workersPrior.end(),sortBySecond);
  return workersPrior;
} 
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t day_number,
    std::vector<size_t>& workingShifts,
    std::vector<std::pair<Worker_T, size_t>>& workersPrior
)
{
    AvailabilityMatrix avail2 = avail;
    size_t total_worker = avail[0].size();
    size_t total_days = avail.size();
    //reached the last day, check if the schedule is valid
    if (day_number == total_days) 
    {
        for (size_t i = 0; i < total_days; i++) 
        {
            if (sched[i].size() != dailyNeed) 
            {
                return false; // Not enough workers
            }
            if (workingShifts[i] > maxShifts) 
            {
                return false; // too many shifts
            }
        }
        return true; // Valid
    }
    //cout<<workersPrior.size()<<endl;
    // for (auto i : workersPrior)
    // cout<<i.first<<endl;
    // Try to schedule each worker who is available and has not reached max shifts
    for (auto i : workersPrior) //set workers with high priority
    {
        //cout<<i.first<<endl;
        if (avail[day_number][i.first] && (workingShifts[i.first] < maxShifts)) 
        {
            //cout<<i.first<<"!"<<endl;
            sched[day_number].push_back(i.first);
            workingShifts[i.first]++;
            avail2[day_number][i.first] = false;
            // If we have scheduled the required number of workers for the day, move to the next day
            if (sched[day_number].size() == dailyNeed)
            {
                //cout<<"go this"<<endl;
                if(scheduleHelper(avail2, dailyNeed, maxShifts, sched, day_number + 1, workingShifts, workersPrior))
                {
                    return true; // A valid schedule was found
                }
            }
            else
            {
                if(scheduleHelper(avail2, dailyNeed, maxShifts, sched, day_number, workingShifts, workersPrior))
                {
                    return true; // A valid schedule was found
                }
            }
            sched[day_number].pop_back(); // Remove the worker from the schedule
            workingShifts[i.first]--;
            avail2[day_number][i.first] = true;
            if (day_number == 0 && sched[day_number].empty()) 
            {
                return false; // No valid schedule for the first day
            }
            if (sched[day_number].empty()) 
            {
                return scheduleHelper(avail, dailyNeed, maxShifts, sched, day_number - 1, workingShifts, workersPrior);
            }
        }
    }
    // If we get here, there is no valid schedule for this day
    return false;
}



bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    //need d out of k workers per day
    //n day period
    //no more than m
    //std::vector<std::vector<bool>> AvailabilityMatrix;
    //std::vector<std::vector<Worker_T> > DailySchedule;
    //unsigned int Worker_T;
    // AvailabilityMatrix avail = { 
    //     {1, 1, 1, 1},
    //     {1, 0, 1, 0},
    //     {1, 1, 0, 1},
    //     {1, 0, 0, 1}
    // };
    // DailySchedule sched;
    // bool solutionFound = schedule(avail, 2, 2, sched);
    if(avail.size() == 0U){
        return false;
    }
    size_t total_worker = avail[0].size();
    size_t total_days = avail.size();
    sched.clear();
    sched.resize(total_days);
    std::vector<size_t> workingShifts(total_worker);
    // Add your code below
    std::vector<std::pair<Worker_T, size_t>> workersPrior = findPriot(avail);
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, 0,workingShifts,workersPrior);
}

