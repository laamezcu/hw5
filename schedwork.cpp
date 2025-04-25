#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduler(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts,
                DailySchedule& sched, vector<size_t>& shiftsCnt, size_t day, size_t workerPos);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U || avail[0].size() == 0U || dailyNeed == 0) {
        return false;
    }
    
    const size_t n = avail.size();
    const size_t k = avail[0].size();

    // Add your code below
    // Initialize sched w/ INVALID_ID
    sched.assign(n, vector<Worker_T>(dailyNeed, INVALID_ID));


    // Track amnt of shifts assigned to each worker
    vector<size_t> shiftsCnt(k, 0);

    return scheduler(avail, dailyNeed, maxShifts, sched, shiftsCnt, 0, 0);
}

bool scheduler(const AvailabilityMatrix& avail, size_t dailyNeed, size_t maxShifts,
                DailySchedule& sched, vector<size_t>& shiftsCnt, size_t day, size_t workerPos) {
    const size_t n = avail.size();
    const size_t k = avail[0].size();

    // Base case: all days are scheduled
    if (day == n) {
        return true;
    }

    // Cur day completed
    if (workerPos >= dailyNeed) {
        return scheduler(avail, dailyNeed, maxShifts, sched, shiftsCnt, day + 1, 0);
    }

    // Try all workers available
    for (Worker_T w = 0; w < k; ++w) {
        // Check if worker is available and not overworked
        if (avail[day][w] && shiftsCnt[w] < maxShifts) {
            // Assign worker to the current day
            bool alreadyAssigned = false;
            for (size_t i = 0; i < workerPos; ++i) {
                if (sched[day][i] == w) {
                    alreadyAssigned = true;
                    break;
                }
            }
            if (alreadyAssigned) {
                continue; // Skip if already assigned
            }

            // Assign the worker
            sched[day][workerPos] = w;
            shiftsCnt[w]++;

            // Recur to assign the next worker
            if (scheduler(avail, dailyNeed, maxShifts, sched, shiftsCnt, day, workerPos + 1)) {
                return true;
            }

            // Backtrack: unassign the worker
            sched[day][workerPos] = INVALID_ID;
            shiftsCnt[w]--;
        }
    }
    // No valid assignment found for this day
    return false;
}

