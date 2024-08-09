/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                    A R D U I N O   S C H E D U L E R


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <TaskScheduler.h}> - Library for GUI widgets.
                    Created by Cor Hofman, 06 Aug 2024
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
#include <TaskList.h>

#ifndef TASK_SCHEDULER_H_
#define TASK_SCHEDULER_H_

/*=================================================================================================
 *  T A S K  S C H E D U L A R - Schedules tasks.
 *===============================================================================================*/
class TaskScheduler : public Task {
  private:
    TaskList*  tasklist  = nullptr;
    uint16_t   lastTID   = 0;

    uint32_t   timeSlice = 0;
    uint32_t   overhead  = 0;
    uint32_t   busy      = 0;

  public:
    

    TaskScheduler();

    TaskList* find(char* name);                     // Return the TaskList entry of the task
    TaskList* find(uint16_t TID);                   // Return the TaskList entry of the Task

    virtual void           schedule();              // Schedule whatever task needs to be scheduled.
    virtual uint16_t run(Task* task);               // Inserts a task to be scheduled, returns TID
    virtual void           kill(Task* task);        // Kills the specified task.
    virtual uint16_t kill(char* name);              // Kills the specified task.
    virtual uint16_t kill(uint_least16_t  tid);     // Kills the specified task.
    virtual uint16_t getTID(char* taskName);        // Return the Task ID based on the name
    virtual void     monitor();
};

extern TaskScheduler scheduler;

#endif

