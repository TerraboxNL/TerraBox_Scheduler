/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                    A R D U I N O   S C H E D U L E R


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <${name}> - Library for GUI widgets.
                    Created by Cor Hofman, ${date}
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
#include <Arduino.h>
#include <stdint.h>

#ifndef TASK_H_
#define TASK_H_

#define CYCLE_TIME                  5000     // Run once per CYCLE_TIME ms.

#define STATE_NEW                   0        // Newly added task, invoke begin()
#define STATE_IDLE                  1        // Not running, but active
#define STATE_RUNNING               2        // Currently running
#define STATE_STALLING              3        // Task takes longer than its cycle
#define STATE_PENDING               4        // Inactive, must be activated
#define STATE_2B_KILLED             254      // Task to be killed, invoke end()
#define STATE_KILLED                255      // Killed task, can be removed

/*=================================================================================================
 *  T A S K - Defines a task that must be executed periodically.
 *===============================================================================================*/
class Task {

  public:
    uint32_t cycleTime = CYCLE_TIME;       // Cycle in ms. Restarts every cycle.
    uint8_t  state     = STATE_NEW;        // Default state of task just created
    uint16_t stalling  = 0;                // Counts number of stalls
    uint32_t started   = 0;                // time stamp when started
    uint32_t waiting   = 0;                // Last waiting time before exec()
    uint32_t elapsed   = 0;                // Last elapsed run time
    uint16_t late      = 0;                // Times ran later than its cycle
    uint32_t invoked   = 0;                // Times the task ran.
    char*    name      = nullptr;          // Contains the name of the task.

    Task(char *name);                      // Create task with default cycle.
    Task(char *name, uint32_t cycleTime);  // Create task with specific cycle.
 //   virtual ~Task();
  
    virtual void     begin();                        // Init before task start
    virtual void     exec();                         // Periodically exec task
    virtual void     end();                          // Invoked before task end.

    virtual void     activate(bool a);               // (De-)Activate task

    virtual void     setCycleTime(uint32_t time);    // Set the cycle time in ms
    virtual uint32_t getCycleTime();                 // Get the current cycle
};

#endif

