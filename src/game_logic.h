// Nozomi Engine
// game_logic.h

// probably not going to use this for the first game actually
// but this is something i'm considering to heavily expand 
// the non-source code development process in the future

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

typedef struct
{
	uint32_t unsigned_value;
	int32_t signed_value;
	char *string_value;
} logic_value_t;

typedef enum
{
	LOGIC_DATA_UNSIGNED, // number
	LOGIC_DATA_SIGNED, // number
	LOGIC_DATA_STRING, // string
	LOGIC_DATA_OBJECT // object data
} logic_data_type_e;

typedef enum
{
	LOGIC_DATA_GENERIC_ABSOLUTEVALUE, // abs(-1) == 1
	LOGIC_DATA_GENERIC_CONTAINS // string contains 'x'
} logic_data_generic_e;

typedef enum
{
    LOGIC_OBJECT_TARGET_SELF,
    LOGIC_OBJECT_TARGET_NEAR,
    LOGIC_OBJECT_TARGET_FAR,
    LOGIC_OBJECT_TARGET_ANY, // first one
    LOGIC_OBJECT_TARGET_ALL, // all of them
} logic_object_target_e;

typedef enum
{
	LOGIC_DATA_OBJECT_ID, // object id
	LOGIC_DATA_OBJECT_HEALTH, // object health
	LOGIC_DATA_OBJECT_X, // object x position
	LOGIC_DATA_OBJECT_Y, // object y position
	LOGIC_DATA_OBJECT_MOMX, // object x momentum
	LOGIC_DATA_OBJECT_MOMY, // object y momentum
	LOGIC_DATA_OBJECT_DIR, // object current facing direction
	LOGIC_DATA_OBJECT_LAYER, // object world layer
	LOGIC_DATA_OBJECT_FLAGS, // object special flags
} logic_data_object_e;

typedef struct 
{
    logic_object_target_e target; // what object are we comparing with?
    logic_data_object_e data; // what data from the object?
} logic_data_object_t;

typedef enum
{
	LOGIC_COMPARE_NOTEQUAL,
	LOGIC_COMPARE_EQUAL,
	LOGIC_COMPARE_LESS,
	LOGIC_COMPARE_LESSEQUAL,
	LOGIC_COMPARE_GREATER,
	LOGIC_COMPARE_GREATEREQUAL,
	LOGIC_COMPARE_DISTANCE
} logic_comparison_type_e;

typedef struct
{
	uint32_t next_action; // action to run after this, if 0, don't run anymore

    logic_action_parameter_t *parameters; // things to affect
	logic_value_t *values; // if applicable ofc
} logic_action_t;

typedef struct
{
	// see comparison_type_e;
	logic_comparison_type_e comparison_type;

	// see data_type_e
	logic_data_type_e parameter_one_type;
	logic_data_type_e parameter_two_type;

    // if parameter_xxx_type is an object, these will be populated
    logic_data_object_t object_data;
	logic_data_object_t object_two_data;

    // generic values if applicable
    logic_value_t parameter_one;
	logic_value_t parameter_two;

    // what action to run if comparison is met
    uint32_t action_num;
} logic_condition_t;

typedef struct
{
	uint32_t num_conditions;
	uint32_t num_actions;

	logic_condition_t *conditions;
	logic_action_t *actions;
} logic_t;

#endif