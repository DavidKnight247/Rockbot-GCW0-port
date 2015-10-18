#ifndef ENUM_NAMES_H
#define ENUM_NAMES_H

#include <vector>

std::vector<std::string> PROJECTILE_TRAJECTORIES_NAMES;
std::vector<std::string> AI_ACTION_NAMES;
std::vector<std::string> AI_ACTION_SHOT_OPTIONS;
std::vector<std::string> AI_ACTION_WALK_OPTIONS;
std::vector<std::string> AI_ACTION_FLY_OPTIONS;
std::vector<std::string> AI_ACTION_JUMP_OPTIONS;
std::vector<std::string> AI_ACTION_AIR_WALK_OPTIONS;
std::vector<std::string> AI_ACTION_TELEPORT_OPTIONS;
std::vector<std::string> AI_ACTION_DASH_OPTIONS;
std::vector<std::string> AI_ACTION_GRAB_WALL_OPTIONS;

void init_enum_names() {
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_LINEAR");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_ARC");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_SIN");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_CHAIN");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_QUAKE");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_BOMB");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_FOLLOW");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_PROGRESSIVE");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_FREEZE");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_DIAGONAL_UP");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_DIAGONAL_DOWN");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_CENTERED");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_ZIGZAG");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_TARGET_DIRECTION");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_ARC_TO_TARGET");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY_TARGET_EXACT");
    PROJECTILE_TRAJECTORIES_NAMES.push_back("TRAJECTORY FALL BOMB");
    // -------------------------------------------------------------- //
    AI_ACTION_NAMES.push_back("WALK");
    AI_ACTION_NAMES.push_back("FLY");
    AI_ACTION_NAMES.push_back("JUMP");
    AI_ACTION_NAMES.push_back("WAIT UNTIL PLAYER IS IN RANGE");
    AI_ACTION_NAMES.push_back("SAVE POINT");
    AI_ACTION_NAMES.push_back("SHOT PROJECTILE #1");
    AI_ACTION_NAMES.push_back("SHOT PROJECTILE #2");
    AI_ACTION_NAMES.push_back("SHOT PROJECTILE #1 (INVERT DIRECTION)");
    AI_ACTION_NAMES.push_back("AIR WALK");
    AI_ACTION_NAMES.push_back("FALL TO GROUND");
    AI_ACTION_NAMES.push_back("TELEPORT");
    AI_ACTION_NAMES.push_back("DASH");
    AI_ACTION_NAMES.push_back("GRAB WALL");
    AI_ACTION_NAMES.push_back("SPAWN NPC");
    // -------------------------------------------------------------- //
    AI_ACTION_SHOT_OPTIONS.push_back("FACE PLAYER");
    AI_ACTION_SHOT_OPTIONS.push_back("AHEAD");
    // -------------------------------------------------------------- //
    AI_ACTION_WALK_OPTIONS.push_back("HORIZONTAL AHEAD");
    AI_ACTION_WALK_OPTIONS.push_back("HORIZONTAL TURN");
    AI_ACTION_WALK_OPTIONS.push_back("TO PLAYER");
    AI_ACTION_WALK_OPTIONS.push_back("TO SAVED POINT");
    AI_ACTION_WALK_OPTIONS.push_back("TURN TO PLAYER");

    AI_ACTION_FLY_OPTIONS.push_back("AHEAD");
    AI_ACTION_FLY_OPTIONS.push_back("TURN");
    AI_ACTION_FLY_OPTIONS.push_back("UP");
    AI_ACTION_FLY_OPTIONS.push_back("DOWN");
    AI_ACTION_FLY_OPTIONS.push_back("TO PLAYER");
    AI_ACTION_FLY_OPTIONS.push_back("TO SAVED POINT");
    AI_ACTION_FLY_OPTIONS.push_back("TO RANDOM POINT");
    AI_ACTION_FLY_OPTIONS.push_back("FALL");
    AI_ACTION_FLY_OPTIONS.push_back("DRILL DOWN");
    AI_ACTION_FLY_OPTIONS.push_back("VERTICAL CENTER");
    AI_ACTION_FLY_OPTIONS.push_back("PLAYER DIRECTION");
    AI_ACTION_FLY_OPTIONS.push_back("TO OPOSITE WALL");
    AI_ACTION_FLY_OPTIONS.push_back("TO OPOSITE (SHOOT #1)");
    AI_ACTION_FLY_OPTIONS.push_back("TO PLAYER X");
    AI_ACTION_FLY_OPTIONS.push_back("TO PLAYER Y");
    AI_ACTION_FLY_OPTIONS.push_back("AHEAD [Y-ZIG-ZAG]");
    // -------------------------------------------------------------- //
    AI_ACTION_JUMP_OPTIONS.push_back("AHEAD");
    AI_ACTION_JUMP_OPTIONS.push_back("TO RANDOM POINT");
    AI_ACTION_JUMP_OPTIONS.push_back("UP");
    AI_ACTION_JUMP_OPTIONS.push_back("TO PLAYER");
    AI_ACTION_JUMP_OPTIONS.push_back("TO ROOT");
    AI_ACTION_JUMP_OPTIONS.push_back("TO SAVED POINT");
    AI_ACTION_JUMP_OPTIONS.push_back("JUMP ONCE AHEAD");
    // -------------------------------------------------------------- //
    AI_ACTION_AIR_WALK_OPTIONS.push_back("LEFT");
    AI_ACTION_AIR_WALK_OPTIONS.push_back("RIGHT");
    AI_ACTION_AIR_WALK_OPTIONS.push_back("TO PLAYER");
    // -------------------------------------------------------------- //
    AI_ACTION_TELEPORT_OPTIONS.push_back("LEFT");
    AI_ACTION_TELEPORT_OPTIONS.push_back("RIGHT");
    AI_ACTION_TELEPORT_OPTIONS.push_back("TO PLAYER");
    AI_ACTION_TELEPORT_OPTIONS.push_back("RANDOM X");
    AI_ACTION_TELEPORT_OPTIONS.push_back("RANDOM Y");
    AI_ACTION_TELEPORT_OPTIONS.push_back("RANDOM POINT (X&Y)");
    // -------------------------------------------------------------- //
    AI_ACTION_DASH_OPTIONS.push_back("LEFT");
    AI_ACTION_DASH_OPTIONS.push_back("RIGHT");
    AI_ACTION_DASH_OPTIONS.push_back("TO PLAYER");
    // -------------------------------------------------------------- //
    AI_ACTION_GRAB_WALL_OPTIONS.push_back("LEFT");
    AI_ACTION_GRAB_WALL_OPTIONS.push_back("RIGHT");
}




#endif // ENUM_NAMES_H
