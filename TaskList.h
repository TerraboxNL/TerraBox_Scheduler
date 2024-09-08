/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                    A R D U I N O   S C H E D U L E R


              (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <TaskList.h> - Library for GUI widgets.
                            06 Aug 2024
                      Released into the public domain
              as GitHub project: TerraboxNL/TerraBox_Scheduler
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
#include <stdint.h>

#ifndef TASK_LIST_H_
#define TASK_LIST_H_


/*=================================================================================================
 *  T A S K  L I S T - Element to make a list of tasks
 *===============================================================================================*/
class TaskList {

  public:
    uint16_t       tid   = 0;             // The task id
    TaskList*      prev  = nullptr;       // The previous task in the list
    Task*          task  = nullptr;       // The task
    TaskList*      next  = nullptr;       // The next task in the list

    TaskList();
//    virtual ~TaskList();

    virtual void add(Task* task);         // Adds a task to the list
    virtual void insert(Task *task);      // Inserts a task to the list
    virtual void remove(Task* task);      // Removes a task to the list
};

#endif

