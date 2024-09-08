/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //


                    A R D U I N O   S C H E D U L E R


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <TaskList.cpp> - Library for GUI widgets.
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
#include "TaskList.h"

TaskList::TaskList() {

}

/*-------------------------------------------------------------------------------------------------
 *
 *  Adds a new task after this TaskList entry
 *
 *  task      The new task to insert
 *
 *-----------------------------------------------------------------------------------------------*/
void TaskList::add(Task* task) {
	TaskList* l = new TaskList();
	l->prev = this;
	l->next = next;
	l->task = task;

	next    = l;
}

/*-------------------------------------------------------------------------------------------------
 *
 *  Inserts a new task before this TaskList entry
 *
 *  task      The new task to insert
 *
 *-----------------------------------------------------------------------------------------------*/
void TaskList::insert(Task *task) {
	TaskList* l = new TaskList();
	l->prev = prev;
	l->next = this;
	l->task = task;

	prev    = l;
}

/*-------------------------------------------------------------------------------------------------
 *
 *  Removes this task from the task list. The task is still pointed to after removal.
 *  prev and next pointers contain nullptr after removal.
 *  delete task and delete this are left to the caller, if desired.
 *
 *  task      The new task to remove
 *
 *-----------------------------------------------------------------------------------------------*/
void TaskList::remove(Task* task) {
	prev->next = next;
	next->prev = prev;

	prev = nullptr;
	next = nullptr;
}
