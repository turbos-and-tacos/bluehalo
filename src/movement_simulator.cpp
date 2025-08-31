#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include "movement_simulator.hpp"

// Default Ctor
MotionSimulator::MotionSimulator(){}

MotionSimulator::MotionSimulator(const MotionPlan& mp) : _motion_plan(mp), _current_position(mp.start_position), _initialized(true) {
    if (_current_position > _motion_plan.bound.upper || _current_position < _motion_plan.bound.lower) {
        std::stringstream ss;
        ss << "Initial postion (" << _current_position << ")";
        ss << " exceeded bounds. [" << _motion_plan.bound.lower << ":" <<_motion_plan.bound.upper << "]";
        throw InvalidLocation(ss.str());
    }
}

// Calculate a goal state based on the supplied inputs. Consider the input limits.
float MotionSimulator::CalculateState(float current_position, float effort, float scale_factor) {

    // REQ-006
    if (effort > UPPER_EFFORT_LIMIT || effort < LOWER_EFFORT_LIMIT) {
        std::stringstream ss;
        ss << "Invalid Effort Requested " << effort;
        ss << " Limits are (" << UPPER_EFFORT_LIMIT << ":" << LOWER_EFFORT_LIMIT << ")";
        throw EffortError(ss.str());
    }

    // REQ-001
    return current_position + effort * scale_factor;
}

// Sets/resets the motion plan.
void MotionSimulator::SetMotionPlan(const MotionPlan &mp) {
    _motion_plan = mp;
    _current_position = mp.start_position;
    _initialized = true;
}

// Run a set of motion scenarios. Use the rules for Calculating
// sequential goal states.
std::vector<float> MotionSimulator::RunMotionPlan(){
    if (!_initialized) {
        throw InvalidMotionPlan("Motion Plan not initialized.");
    }

    // REQ-008
    float last_state = _current_position;
    std::vector<float> result_states;
    result_states.reserve(_motion_plan.effort.size());
    float state = 0.0;
    float max_impulse = 0.0; 
    for (float effort : _motion_plan.effort) {

        // Effort is positive, is it more than the max? (REQ-003)
        if (max_impulse >= 0.0 && effort >= 0.0) {
            max_impulse = std::max(effort, max_impulse);

        // Effort is negative, is it more than the max? (REQ-003)
        } else if (max_impulse < 0.0 && effort < 0.0) {
            max_impulse = std::min(effort, max_impulse);

        // Signs of the effort and max are different. Must apply this effort. (REQ-003)
        } else {
            max_impulse = effort;
        }

        // REQ-002
        if (max_impulse != 0.0){
            state = CalculateState(_current_position, max_impulse, _motion_plan.scale_factor);
            if (state > _motion_plan.bound.upper || state < _motion_plan.bound.lower) {
                std::stringstream ss;
                ss << "Calculated state (" << state << ")";
                ss << " exceeded bounds. [" << _motion_plan.bound.upper << ":" <<_motion_plan.bound.lower << "]";
                throw InvalidLocation(ss.str());
            }
            last_state = state;
        } else {
            state = last_state;
        }
        result_states.push_back(state);
    }
    return result_states;
}