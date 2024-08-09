/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                    A R D U I N O   S C H E D U L E R


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <Scheduler_Example.ino> - Library for GUI widgets.
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
#include <Task.h>
#include <TaskMonitor.h>

//
//  Create the Hello Task, which outputs the "Hello world" message
//
class Hello : public Task {
  public:
    Hello(char * name, uint32_t cycleTime) :
        Task(name, cycleTime) {

        }
 //   void begin() {
    // Nothing to initialize/acquire for this task
 //   }

    //
    //  Print the "Hello world" message
    //
    void exec() {
      Serial.print(millis()); Serial.println(" Hello world");
    }

//    void end() {
      // Nothing to terminate/release for this task
//    }
};

Hello hello("Hello world", 5000);
TaskMonitor monitor(15000);

void setup() {
  //
  //  Initialize Serial for use by both 
  //  the Hello Task and the TaskMonitor
  Serial.begin(115200);

  //
  //  Put both tasks on the TaskList
  //
  scheduler.run(&hello);
  scheduler.run(&monitor);
}

void loop() {

  //
  //  Schedule whatever needs to be scheduled
  //
  scheduler.schedule();

}

