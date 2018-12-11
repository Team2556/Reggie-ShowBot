/* 2018 RobotMap.h */

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */

#ifndef SRC_ROBOTMAP_H_
#define SRC_ROBOTMAP_H_

// Turn on or off various features
// -------------------------------

// Gyros
#define NAVX
//#define ADXRS_GYRO

// Controllers
//#define JOYSTICK
#define XBOX

// Check to make sure JOYSTICK and XBOX aren't both enabled
#if defined(JOYSTICK) and defined(XBOX)
#pragma message "WARNING! Both joystick and XBox controllers are enabled!"
#endif

// Select arm and in/out speed control mode (the default) or position mode using
// position sensors.
//#define ARM_UP_DOWN_USING_POSITION
//#define IN_OUT_USING_POSITION

// CAN bus addresses
// -----------------
#define CAN_TALON_SS_RAISE 			5
#define CAN_PCM			6

// PCM Channels
#define PCM_CHAN_CLIMB_UP		2
#define PCM_CHAN_CLIMB_DOWN		3
#define PCM_CHAN_ARM_UP			0
#define PCM_CHAN_ARM_DOWN		1

// Digital I/O Channels
#define DIO_LIMIT_SW			0
#define DIO_LIMIT_ARM			1

// Relay Channels
#define RELAY_CHAN_CANNON		0


#endif /* SRC_ROBOTMAP_H_ */
