/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                    A R D U I N O   S C H E D U L E R


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <Task.cpp> - Library for GUI widgets.
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
#include <Task.h>

/*---------------------------------------------------------------------------------------------
 *
 *  Creates a task.
 *
 *  name       The unique task name.
 *  cycleTime  The interval in number of milliseconds at which this task should run repeatedly.
 *
 *-------------------------------------------------------------------------------------------*/
Task::Task(char *pName, uint32_t cycleTime) {
	name = pName;
	setCycleTime(cycleTime);
}

/*---------------------------------------------------------------------------------------------
 *
 *  Creates a task. It has a standard cycle time defined by the #define CYCLE_TIME.
 *  CYCLE_TIME is defined in the included header file <TerraBox_Scheduler.h>
 *
 *  name       The unique task name.
 *
 *-------------------------------------------------------------------------------------------*/
Task::Task(char *pName){
	name = pName;
}

/*---------------------------------------------------------------------------------------------
 *
 *  Is ran if the task is inserted.
 *  Its objective is toe obtain any resources it needs and initializes its state.
 *
 *-------------------------------------------------------------------------------------------*/
void Task::begin() {
	// Initialisation needed before starting the task
}

/*---------------------------------------------------------------------------------------------
 *
 *  Is ran if the task is killed to release any obtained resources.
 *
 *-------------------------------------------------------------------------------------------*/
void Task::end() {
	// Called if the task needs to be ended
}

/*---------------------------------------------------------------------------------------------
 *
 *  Contains code that must be run every cycleTime ms.
 *
 *-------------------------------------------------------------------------------------------*/
void Task::exec() {
	// Execute the periodical task
}

/*---------------------------------------------------------------------------------------------
 *
 *  Activates or deactivates a task. If deactivated its has state STATE_PENDING and will
 *  not run.
 *
 *  a      True to activate, False to deactivate.
 *
 *-------------------------------------------------------------------------------------------*/
void Task::activate(bool a) {
  if (a) {
	  state = STATE_IDLE;
  }
  else{
	  state = STATE_PENDING;
  }
}

/*---------------------------------------------------------------------------------------------
 *
 *  Assigns a cycle time. A cycle time specifies that this task should run every cycleTime ms.
 *
 *  time      The cycle time in ms.
 *
 *-------------------------------------------------------------------------------------------*/
void Task::setCycleTime(uint32_t time) {
  cycleTime = time;
}

/*---------------------------------------------------------------------------------------------
 *
 *  Returns the assigned cycle time.
 *
 *-------------------------------------------------------------------------------------------*/
uint32_t Task::getCycleTime() {
  return cycleTime;
}
