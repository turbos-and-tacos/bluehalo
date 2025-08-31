# Goal State Calculator

## Overview
This project is a simple calculator for the end state of a rigid body given a set of inputs. The inputs are defined as the following.
| Input | Definition |
|-------|------------|
| `current_position` | The position of the rigid body before any movement is done. |
| `effort` | A generalized term for the amount of movement that will be applied. |
| `scale` | A scale factor for the amount of movement that will be applied. |

### Scope
The scope of this project is to be a very basic tool to validate assumptions about motion. It does **NOT** take into account things like friction, thermals, or storage of energy for effort. This simulator considers motion in only one dimension. It is reasonable to consider using the underlying class as a building block for a more more complex motion calculator, but additional requirements would be neded support that use case.

### Requirements
| Requirement ID | Definition |
|----------------|------------|
| REQ-001 | `goal_state` = `current_position` + `effort`  * `scale_factor` |
| REQ-002 | The goal state should not be updated for a zero effort input. |
| REQ-003 | The goal state must be monotonically increasing or decreasing until the effort input changes sign. |
| REQ-004 | Ensure that the targeted position does not exceed specified lower and upper bounds. |
| REQ-005 | There should be a method to set the current position. |
| REQ-006 | The effort command will be in the range of [-1.0, 1.0]. |
| REQ-007 | For the given test cases, we will assume the current position is constant while the input effort is streamed continuously.|
| REQ-008 | At the beginning of each test case, you can assume the goal state is the same as current position. |


## Installation

The `build.sh` script will build the main executable and the unit tests. The executables for both `main.cpp` and `test_movement_simulator.cpp` can be found in `<project_root>/build/`.
The unit tests uses the Google Test framework. Assuming that it has been added to the project using the `get_googletest.sh` script, the build script will use it to compile the unit tests. 

### Prerequisites
1. `g++` (Minimum)
2. `make`   (Optional: See `--no-gtest`)
3. `cmake`  (Optional: See `--no-gtest`)
4. `git`    (Optional: See `--no-gtest`)

### Setup Workspace
1. Make sure you are in the project root. From the command line, your folder should look like this: 
 ```
├── build.sh
├── get_googletest.sh
├── include
├── README.md
├── src
└── tests
```
2. `./get_googletest.sh` ==> **NOTE: THIS SCRIPT WILL CLONE GOOGLE'S TESTING FRAMEWORK. IT REQUIRES GIT TO CLONE THE FRAMEWORK.**
3. `./build.sh`

**NOTE: IF YOU DO NOT WISH TO USE GTEST**

Provide the flag `--no-gtest` to the build script. This will result in just the main executable being built.
```
./build.sh --no-gtest
```

The main executable is a more basic means of providing input and checking the output. The main executable REQUIRES a CSV as an input. Each row is a set of inputs. Technically the upper limit to the input of test cases is `2,147,483,647` and the length of input efforts is constrained by the memory of your computer. Here is the breakdown of the csv with an example.

| Start Position | Scale | Effort 1-N | Result 1-N |
|----------------|-------|------------|------------|
| 5.0 | 2.0 | -0.2, -0.5, -1.0, -0.5, 0.0 | 4.6, 4.0, 3.0, 3.0, 3.0 |

Here is how this would show up in a raw csv file.
```
5.0, 2.0, -0.2, -0.5, -1.0, -0.5, 0.0, 4.6, 4.0, 3.0, 3.0, 3.0
```
## Usage
### Unit Tests
```
cd <project_root>
./build.sh
./build/test_runner
```
### Main Executable
```
cd <project_root>
./build.sh
# OR
./build.sh --no-gtest
./build/main ./test_input.csv
```
**NOTICE: The second supplied test case in `test_input.csv` is intended to fail.**

### Classes/Structs
#### MotionBound
Controls the limits that will be considered a valid target position.

**USAGE:**
`MotionBound(UPPER_LIMIT, LOWER_LIMIT)`

#### MotionPlan
Holds relevant information for the estimation of target states. This should be used in lieu of a more active means of "streaming" positions.
**NOTE: The simulator MUST be passed a valid motion plan!**

**USAGE:**
`MotionPlan(VECT_OF_EFFORTS, START_POSITION, SCALE_FACTOR, MOTION_BOUND)`

#### MotionSimulator
The main class that is responsible for determining the result or if a result of a motion plan can be reached.

**USAGE:**
`MotionSimulator ms()` then `ms.SetMotionPlan(MOTION_PLAN)` and finally `ms.RunMotionPlan()`.

**USAGE:**
`MotionSimulator ms(MOTION_PLAN)` then `ms.RunMotionPlan()` will return a vector of goal states.
## Contact
Ethan Plummer - (724)-372-2467

[Github](https://github.com/turbos-and-tacos)

[Linkedin](https://www.linkedin.com/in/ethan-plummer-67609b75/)