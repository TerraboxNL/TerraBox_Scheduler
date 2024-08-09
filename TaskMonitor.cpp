/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                    A R D U I N O   S C H E D U L E R


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <TaskMonitor.cpp> - Library for GUI widgets.
                    Created by Cor Hofman, 9 Aug 2024
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
#include <TaskMonitor.h>

/*----------------------------------------------------------------------------
 *
 *  Create a monitoring task, Reserved name is "Monitor"
 *
 *  cycleTime      The desired cycle time
 *
 *--------------------------------------------------------------------------*/
TaskMonitor::TaskMonitor(uint32_t cycleTime) :
             Task("Monitor", cycleTime) {
}

/*----------------------------------------------------------------------------
 *
 *  Invokes the monitor once and prints out Task information.
 *
 *--------------------------------------------------------------------------*/
void TaskMonitor::exec() {
	scheduler.monitor();
}
