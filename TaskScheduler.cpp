/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                    A R D U I N O   S C H E D U L E R


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <TaskScheduler.cpp> - Library for GUI widgets.
                               06 Aug 2024
                       Released into the public domain
                     as GitHub project: TerraBox_Scheduler
                   under the GNU General public license V3.0
                          
      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <https://www.gnu.org/licenses/>.

 *---------------------------------------------------------------------------*
 *
 *  C H A N G E  L O G :
 *  ==========================================================================
 *  P0001 - Initial release 
 *  ==========================================================================
 *
 *--------------------------------------------------------------------------*/
#include <TaskScheduler.h>
#include <TerraBox_Widgets.h>


/*--------------------------------------------------------------------------------------------------
 *
 *  Schedule whatever task is needed
 *
 *------------------------------------------------------------------------------------------------*/
TaskScheduler::TaskScheduler() :
               Task("Scheduler", 0) {

}

/*--------------------------------------------------------------------------------------------------
 *
 *  Return the release number
 *
 *------------------------------------------------------------------------------------------------*/
const char* getRelease() {
  return (const char*) RELEASE;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Schedule whatever task is needed
 *
 *------------------------------------------------------------------------------------------------*/
void TaskScheduler::schedule() {

  long now  = millis();             // Register start of the next time slice
  long busy = 0;                    // Time busy executing tasks

  //
  // Have every task in the task list do it's thing
  //
  for (TaskList* tasks = tasklist; tasks; tasks = tasks->next) {
    //
    // Get hold of the current task in the list
    //
    Task* task = tasks->task;

    //
    // If the task must be killed, then remove it from the list
    // and move on the next task.
    //
    switch (task->state) {

      //
      //  Task in pending state is ignored until it is IDLE again.
      //
      case STATE_PENDING: {
        // do nothing with this task, since it has been made inactive
        break;
      }

      //
      // If the task its state is new, have it run the begin() method
      // and make its state STATE_IDLE.
      //
      case STATE_NEW: {
//        task->begin();
        task->state    = STATE_IDLE;
        task->started  = 0;
        task->elapsed  = 0;
        task->waiting  = 0;
        task->stalling = 0;
        task->invoked  = 0;
        task->late     = 0;
        break;
      }

      case STATE_STALLING:
      case STATE_IDLE: {

        //
        // Calculate time past since last started.
        // See if it is time to start.
        // No more time left (i.e. <= 0) then we start.
        //

      	int32_t timeLeft = 0;
        task->waiting = millis() - task->started;
        timeLeft      = task->cycleTime - task->waiting;

        if (timeLeft <= 0) {
          if (timeLeft < -10) {
            task->late++;
          }

          //
          // Let the task do it's thing
          //
          task->invoked++;                           // Keep track of how many times it is invoked.
          task->state   = STATE_RUNNING;             // Mark task as running

          task->started = millis();                  // Register the starting time.
          task->exec();                                // Have the task run.
          task->elapsed = millis() - task->started;  // Keep track of the elapsed time.
          busy          = busy + task->elapsed;

          if (task->elapsed > task->getCycleTime()) {
            task->state = STATE_STALLING;            // Mark as running longer than cycleTime permits
            task->stalling++;
          }
          else {
            task->state = STATE_IDLE;
          }

//          Serial.print(task->name);Serial.print(F(" "));Serial.print(task->started); Serial.println(F(" ms."));
        }

        break;
      }  // end case STATE_IDLE

    }  // end switch ()

  }  // End for ()

  //
  //  Calculate the maximum length of the scheduler its time slice.
  //
  elapsed  = millis() - now;
  overhead = elapsed - busy;
  if (timeSlice < elapsed) {
    timeSlice = elapsed;
  }


}

/*--------------------------------------------------------------------------------------------------
 *
 *  Adds a task to the scheduler task list.
 *
 *  If successful it return a TID > 0
 *  If unsuccessful it returns a TID = 0;
 *
 *------------------------------------------------------------------------------------------------*/
uint16_t TaskScheduler::run(Task* task) {

  //
  // Check to see if the name already exists
  //
  if (find(task->name)) {
    return 0;
  }

  //
  //  Initialize the task
  //
  task->begin();

  //
  // Assign it its maiden state.
  //
  task->state   = STATE_NEW;

  //
  //  Create a taskList entry for it.
  //
  TaskList* tl = new TaskList();
  tl->tid      = ++lastTID;
  tl->prev     = nullptr;
  tl->next     = tasklist;
  tl->task     = task;

  //
  //  Add it at the head of the list
  //
  if (tasklist) {
    tasklist->prev = tl;
  }
  tasklist       = tl;

  //
  //  Return the assigned TID
  //
  return tasklist->tid;
}


/*--------------------------------------------------------------------------------------------------
 *
 *  Tries to find a task with the specified name.
 *
 *  taskName      The name of the task to find.
 *
 *  Return:
 *  If successful it return task found
 *  If unsuccessful it returns nullptr;
 *
 *------------------------------------------------------------------------------------------------*/
TaskList* TaskScheduler::find(char* taskName) {

  for (TaskList* l = tasklist; l; l = l->next) {
    if (!strcmp(l->task->name, taskName)) {
      return l;
    }
  }

  return nullptr;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Tries to find a task with the specified TID.
 *
 *  TID      The Task ID to find.
 *
 *  Return:
 *  If successful it return task associated with the TID
 *  If unsuccessful it returns nullptr;
 *
 *------------------------------------------------------------------------------------------------*/
TaskList* TaskScheduler::find(uint16_t TID) {

  for (TaskList* l = tasklist; l; l = l->next) {
    if (l->tid == TID) {
      return l;
    }
  }

  return nullptr;
}

uint16_t TaskScheduler::getTID(char* name) {
	TaskList* l = find(name);
	if (!l) {
		return 0;
	}

	return l->tid;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Kills the task.
 *
 *  task      The task to kill.
 *
 *------------------------------------------------------------------------------------------------*/
void TaskScheduler::kill(Task* task) {
  if (task) {
    task->state = STATE_2B_KILLED;

    TaskList* l   = find(task->name);
    if (l) {

    	//
    	//  If it is the first task in the list
    	//
    	if (l == tasklist) {
            tasklist = l->next;          // next becomes first now
            if (tasklist)
              tasklist->prev = nullptr;  // next is now first, so has no previous
    	}
    	//
    	//  Otherwise it has a next and a previous
    	//
    	else {
            l->prev->next = l->next;
        	if (l->next)
              l->next->prev = l->prev;
    	}

        l->next = nullptr;
        l->prev = nullptr;
    }
    delete l;

    task->state = STATE_KILLED;
  }
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Kills the task.
 *
 *  TID      The Task ID of the task to kill.
 *
 *  Return:  The TID of the task killed or 0 if task was not killed/unknown
 *
 *------------------------------------------------------------------------------------------------*/
uint16_t TaskScheduler::kill(uint16_t TID) {
  TaskList* l = find(TID);
  if (!l || !l->task)
    return 0;
  
  kill(l->task);

  return TID;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Kills the task.
 *
 *  name     The name of the task to kill.
 *
 *  Return:  The TID of the task killed or 0 if task was not killed/unknown
 *
 *------------------------------------------------------------------------------------------------*/
uint16_t TaskScheduler::kill(char *name) {
  TaskList* l = find(name);
  if (!l || !l->task)
    return 0;
  
  kill(l->task);

  return l->tid;
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Prints a list with tasks and there states.
 *
 *------------------------------------------------------------------------------------------------*/
void TaskScheduler::monitor() {

    char line[100];
//	Serial.println(F("         1         2         3         4         5         6         7         8"));
//	Serial.println(F("12345678901234567890123456789012345678901234567890123456789012345678901234567890"));
	Serial.println(F("TID             Name  Cycle STA      Start   Wait  Stall    Elapsed Invoke"));
	Serial.println(F("--------------------------------------------------------------------------"));
	for (TaskList* l = tasklist; l; l = l->next) {
		Task* t = l->task;
		sprintf(line, "%3d %16s %6lu %3u %10lu %6lu %6u %10lu %6lu    ",
				       l->tid,
					       t->name,
							    t->cycleTime,
								     t->state,
									     t->started,
										       t->waiting,
											        t->stalling,
													     t->elapsed,
														      t->invoked
														     );
		Serial.println(line);
	}

	Serial.println();

}

TaskScheduler scheduler;
