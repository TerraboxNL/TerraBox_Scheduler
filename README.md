

       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                    A R D U I N O   S C H E D U L E R


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <README.md> - Library for GUI widgets.
                    Created by Cor Hofman, 07 Aug 2024
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

Releases
========

Release 1.0.0:
A first release of this scheduler.

Abstract
========
This is a simple scheduler of tasks for Arduino. It supports creating them, executing them at regular time intervals, destroying to free up resources, (de)activating and monitoring them. Its core classes are TaskScheduler, TaskList and Task.

TaskScheduler
=============
A scheduler is created using TaskScheduler::TaskScheduler(), but there is no actual need to do so. A scheduler is already created and available. You can readily use the object named scheduler.
In the Arduino loop() you invoke TaskScheduler::schedule() to have tasks scheduled according to their execution interval specified by setting the cycleTime. 

Scheduling strategy
===================
Every invocation of schedule() examines all executable tasks in a fixed order. The last task activated by invoking TaskScheduler::run(Task* task) for it is the first one to be examined (LIFO queue) if TaskScheduler::schedule() is invoked. Scheduler() calculates a remaining waiting time as it examines a Task. If the waiting time is up (zero or negative) it gets executed. If multiple tasks have the same waiting time, they compete for the CPU at the same execution time. Since it starts with the first Task, the one earlier in the row has priority over others. 

But because all the tasks are guaranteed to be examined when schedule() is invoked, it is guaranteed all tasks are allowed to run if their waiting time has passed. This scheduling method is also known as "Round Robin". Tasks are executed have their intervals honoured as good as possible. The implication is that specifying an interval does not guarantee it. If the scheduler is too busy it uses the strategy of gracefull degradation to keep going as well as possible. Potentially every task its execution interval can increase.

To prevent this as much as possible the scheduler has one more ace up its sleeve. It automatically shifts the starting time of competing task into the future. If the scheduler is not overloaded this will evolve into a situation where the intervals can still be honoured. At the cost of a slightly delayed Task start. After that the task has shifted in time and is no longer competing with this other task. The more tasks you schedule, the more likely it gets that it may never reach a stable state. There are simply too many Tasks than the CPU can handle. At that point all tasks will constantly get their start time shifted, result in permanently longer interval times than requested. This is called graceful degradation. Because the system will still keep on functioning, although it will react slower to its stimuli.

TaskList
========
The TaskScheduler is the mechanism by which tasks are ordered. The order in which TaskScheduler::run(Task* task) is invoked is decisive (LIFO). The last one run() has highest priority.

Task
====
A task is created through either the constructors Task(char* name) or Task(char* name, uint32_t timeInterval). Not specifying a timeInterval will make it have a default interval of CYCLE_TIME. This #define can be found in the <Task.h> header file and is set at 5000 ms.
 
To get tasks to execute use TaskScheduler::run(Task* task) during setup(). If run() returns zero, the task is not accepted by the scheduler. Probably because a Task with a same name already exists. So the Task its name has to be unique as. If successful it returns a TID (Task ID), which also identifies the task uniquely. The run() method does not excuse your Task however. It only places the task on the schedulers TaskList and makes therefore executable. Tasks will be ran only by invoking TaskScheduler:schedule() regularly. 

A Task can be prevented from execution, although it is on the TaskList, by calling Task::activate(false). The false indicates it has to be postponed. Calling Task::activate(true) will have it executed again.

You can access tasks by using TaskScheduler::find(uint16_t TID) or TaskScheduler::find(char* name).

A task has a number of informative public attributes. They have a default value. Which are:

``` C++
    uint32_t cycleTime = CYCLE_TIME;       // Cycle in ms. Restarts every cycle.
  ! uint8_t  state     = STATE_NEW;        // Default state of task just created
    uint16_t stalling  = 0;                // Counts number of stalls
  ! uint32_t started   = 0;                // time stamp when started
  ! uint32_t waiting   = 0;                // Last waiting time before exec()
    uint32_t elapsed   = 0;                // Last elapsed run time
    uint16_t late      = 0;                // Times ran later than its cycle
    uint32_t invoked   = 0;                // Times the task ran.
    char*    name      = nullptr;          // Contains the name of the task.
```

All these attributes can be read and modified. Although it is strongly suggested not modify the attribute with an exclamation mark. As this may disrupt the proper functioning of the TaskScheduler.
Attribute name is initialised by the Task constructor. The cycleTime attribute may optionally be initialised by the constructor. The attribute state, started and waiting are managed by the TaskScheduler::schedule() method. The other attributes are merely statistics that can be used to diagnose scheduler issues. For instance an overloaded schedular will show a lot of task with non zero late counter. A high stalling count implicates an interval being too short for the task. Such task can also be recognised by the state attribute indicating STATE_STALLING (i.e. 3). 

Writing a Task is much like create an Arduino sketch. Instead of a setup() method a Task has a begin() method which is only ran once by the TaskSchedular::run() method.
The actual task logic must be coded in the Task::exec() method. Which is potentially run by the TaskScheduler:schedule() invoked by the Arduino loop() method.
And it has a Task::end() method, which is meant to clean up resources and is invoked by TaskSchedular::kill().
During setup() a Task is instantiated by calling its constructor and invoking TaskScheduler::run(Task* task) with the instantiated task as its argument.

TaskMonitor
===========
The TaskMonitor shows the contents of the TaskScheduler its TaskList. It provides an overview of all tasks managed by the scheduler.
The monitor is a Task itself so it it can be scheduled by the TaskScheduler as well, which provides a task overview at a configurable time interval.

Another way to invoke the monitor is invoking the monitor once by calling TaskScheduler::monitor().
This enables the possibility to show the TaskList information at will from within other application Taks code or in whatever non-scheduled logic created.

Example Task
============

``` cpp
Hello::Hello(char* name, uint32_t interval) :
       Task(name, interval) {
   // Nothing further to do
}

Hello::begin() {
  // No resources need to be acquired
}

Hello::exec() {
  Serial.print(millis()); Serial.println(": Hello world");
}

Hello::end() {
  // No resource to free
}
```

Typical Arduino sketch
======================
The sketch can be found in the Arduino library under the name Scheduler_Example.ino in the folder named Scheduler_Example as well.

``` cpp
#include <TaskScheduler.h>
#include <Task.h>
#include <TaskMonitor.h>

//
//  Create the Hello Task, which outputs the "Hello world" message
//  Note that the begin() and end() methods are left out,
//  because no initiation or termination actions are needed. 
//
class Hello : public Task {

  public:

    //
    //  Task constructor
    //
    //  name       Specifies the task name
    //  cycleTime  Specifies the execution interval
    //
    Hello(char * name, uint32_t cycleTime) :
        Task(name, cycleTime) {

    }

    //
    //  Print the "Hello world" message
    //
    void exec() {
      Serial.print(millis()); Serial.println(" Hello world");
    }

};

//
//  Create out two tasks
//
Hello hello("Hello world", 5000);  // The task printing "Hello world"
TaskMonitor monitor(15000);        // The task printing the TaskList

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
  //  Schedule whatever Task needs to be executed
  //
  scheduler.schedule();

}
```

Example Output
==============
Note that the times for the Hello world message shifts with 20 ms after the monitor has been invoked for the first time. Also note that the interval for Hello te execute is 5000 ms. Theinterval for the monitor is 15000 ms. So at the third 5000 ms interval the monitor and Hello tasks compete in getting scheduled at that interval first. Because the last run() Task is the first in the scheduling list it has higher priority. So the monitor runs first. Running the monitor takes 20 ms. So the Hello task is delayed in running that 20 ms and runs after 5020 ms instead of 5000.

The scheduler uses the last actual start time of a Task + its interval times specified, all following cycles for the Hello task will actually be 5000 ms again. So the scheduler adapts the Task start times in such a way that the interval times are properly maintained.

``` txt
<SerialMonitorOutput>
   :
   :
14:05:28.978 -> 5000 Hello world
14:05:33.992 -> 10000 Hello world
14:05:39.045 -> TID         Name  Cycle STA      Start   Wait  Stall    Elapsed Invoke
14:05:39.045 -> ----------------------------------------------------------------------
14:05:39.045 ->   2      Monitor  15000   2      15000  15000      0          0      1    
14:05:39.086 ->   1  Hello world   5000   1      10000   4999      0          0      2    
14:05:39.086 -> 
14:05:39.086 -> 15020 Hello world
14:05:44.125 -> 20020 Hello world
14:05:49.160 -> 25020 Hello world
14:05:54.194 -> TID         Name  Cycle STA      Start   Wait  Stall    Elapsed Invoke
14:05:54.194 -> ----------------------------------------------------------------------
14:05:54.194 ->   2      Monitor  15000   2      30000  15000      0         20      2    
14:05:54.194 ->   1  Hello world   5000   1      25020   4980      0          0      5    
14:05:54.194 -> 
14:05:54.194 -> 30020 Hello world
14:05:59.241 -> 35020 Hello world
14:06:04.291 -> 40020 Hello world
14:06:09.309 -> TID         Name  Cycle STA      Start   Wait  Stall    Elapsed Invoke
14:06:09.309 -> ----------------------------------------------------------------------
14:06:09.309 ->   2      Monitor  15000   2      45000  15000      0         19      3    
14:06:09.352 ->   1  Hello world   5000   1      40020   4979      0          0      8    
14:06:09.352 -> 
14:06:09.352 -> 45020 Hello world
   :
   :
</SerialMonitorOutput>
```