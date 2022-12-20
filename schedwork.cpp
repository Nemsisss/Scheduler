

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here

/*  Checks whether a worker has reached the maxShifts limit or can work more shifts */
bool isAvailableToWork(DailySchedule& sched,const AvailabilityMatrix& avail,const size_t maxShifts,Worker_T id );
bool schedule_helper(const AvailabilityMatrix& avail,DailySchedule& sched,const size_t maxShifts, const size_t dailyNeed, size_t  row, size_t  col);
// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    bool result;
    sched.resize(avail.size()); 
    // Add your code below
    //initialize daily schedule to INVALID_ID
    for(long unsigned int i=0; i<avail.size();i++) // i= rows = days
    {
        for(long unsigned int j=0; j< dailyNeed; j++) // cols = worker ids
        {
            sched[i].push_back(INVALID_ID);
        }
    }
    result=schedule_helper(avail,sched,maxShifts,dailyNeed,0,0);
    return result;

}
bool schedule_helper(const AvailabilityMatrix& avail,DailySchedule& sched,const size_t maxShifts, const size_t dailyNeed, size_t row, size_t col)
{
    // cout<<"ROWWWW: "<<row<<endl;
    // cout<<"COLLLL: "<<col<<endl;
    if(row== avail.size()-1 && col==dailyNeed)
    {
        return true;
    }
    if(col==dailyNeed)
    {
        row++;
        col=0;
    }
    if(sched[row][col]==INVALID_ID)
    {
        for(size_t i=0; i<avail[0].size(); i++) //avail[0].size() == number of workers, i.e. number of columns
        {
            sched[row][col]= i; // i is the id of the worker
            if(isAvailableToWork(sched,avail,maxShifts,i) && avail[row][i]==1)
            {
                bool status=schedule_helper(avail,sched,maxShifts,dailyNeed,row,col+1);
                if(status)
                {
                    return true;
                }
            }
        //   sched[row][col]=INVALID_ID;
        }
        // bakctrack
         sched[row][col]=INVALID_ID;
    }
    else
    {
        return schedule_helper(avail,sched,maxShifts,dailyNeed,row,col+1);
    }
    return false;
}

bool isAvailableToWork(DailySchedule& sched, const AvailabilityMatrix& avail,const size_t maxShifts, Worker_T id )
{
     size_t numDays=0;
    for(long unsigned int i=0; i<sched.size();i++)
    {
        if(avail[i][id]==0)
        {
            continue;
        }
        if(count(sched[i].begin(),sched[i].end(),id)>1) //only 1 shift per day is allowed for each worker
        {
            return false;
        }
        for(long unsigned int j=0; j<sched[0].size();j++)
        {
            if( sched[i][j]==id)
            {
                numDays++;
                if(numDays>maxShifts)
                {
                    return false;
                }
            }
        }
    }
    return true;
}