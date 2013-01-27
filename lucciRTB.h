/* 
 * File:   lucciRTB.h
 * Author: erupter
 *
 * Created on October 25, 2012, 12:19 PM
 */

#ifndef LUCCIRTB_H
#define	LUCCIRTB_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/param.h>
#include "lucciSERVICE.h"



/*! \brief RTB operating mode EUCLIDEAN constant
*
*
*  Constant used in defining RTB's behavior, it is used
* in conjunction with the RTB_GEO_MODE variable
* @see RTB_GEO_MODE
* @see RTB_GEO_MODE_LATLON
*/
#define RTB_GEO_MODE_EUCLIDEAN      0
    
/*! \brief RTB operating mode Geographical Coordinates constant
 *
 *      
 *  Constant used in defining RTB's behavior, it is used
 * in conjunction with the RTB_GEO_MODE variable
 * @see RTB_GEO_MODE
 * @see RTB_GEO_MODE_EUCLIDEAN
 */
#define RTB_GEO_MODE_LATLON         1

/*! \brief RTB Record-mode waypoint saving distance threshold
 *
 *      
 *  This value determines
 * the minimum distance between two consecutive points to use to
 * perform the automatic return-to-base. Points with relative
 * distance inferior to this value are discarded.
 * PLEASE NOTE: the software is unable to determine circular paths,
 * unless this value is such as to fully enclose the path.
 */
#define RTB_SAVE_DIST_TRSH          10.0

/*! \brief RTB Record-mode adaptive saving distance, speed rate
 *
 *      
 *  If this option is enabled the above value of maximum distance between two
 * consecutive waypoints, is modulated by the vehicle actual longitudinal speed.
 * The speed is calculated with a simple difference quotient method.
 * Using this capability requires adapting the maximum and minimum vehicle speed
 * values accordingly.
 * @see RTB_SAVE_DIST_TRSH
 * @see RTB_SAVE_DIST_ADAPTIVE_ANGULAR
 * @see RTB_SAVE_DIST_ADAPTIVE_MAX_SPEED
 * @see RTB_SAVE_DIST_ADAPTIVE_MIN_SPEED
 */
#define RTB_SAVE_DIST_ADAPTIVE_LONGITUDINAL 1 //bool, either 1 or 0

/*! \brief RTB Record-mode adaptive saving distance, angular rate
 *
 *      
 *  When this option is enabled, the distance between two consecutive waypoints is
 * modulated according to the angular speed of the vehicle. Meaning the faster the
 * turns, the more points will be used to store the trajectory.
 * @see RTB_SAVE_DIST_TRSH
 * @see RTB_SAVE_DIST_ADAPTIVE_LONGITUDINAL
 */
#define RTB_SAVE_DIST_ADAPTIVE_ANGULAR      1 //bool, either 1 or 0

/*! \brief RTB Record-mode adaptive saving distance, maximum speed
 *
 *      
 *  The maximum speed the vehicle is capable of. Used internally to normalize the
 * distance between consecutive waypoints.
 * @see RTB_SAVE_DIST_TRSH
 * @see RTB_SAVE_DIST_ADAPTIVE_LONGITUDINAL
 */
#define RTB_SAVE_DIST_ADAPTIVE_MAX_SPEED    2.0 //in meters per second

/*! \brief RTB Record-mode adaptive saving distance, minimum speed
 *
 *      
 *  The minimum speed the vehicle is capable of. Used internally to normalize the
 * distance between consecutive waypoints.
 * @see RTB_SAVE_DIST_TRSH
 * @see RTB_SAVE_DIST_ADAPTIVE_LONGITUDINAL
 */
#define RTB_SAVE_DIST_ADAPTIVE_MIN_SPEED    0.2 //in meters per second


/*! \brief RTB Record-mode adaptive saving distance, angular sensitivity
 *
 *      
 *  This value controls how intense the sensitivity to the angular rate is.
 * The higher the value the higher the number of stored waypoints during turns.
 * @see RTB_SAVE_DIST_TRSH
 * @see RTB_SAVE_DIST_ADAPTIVE_ANGULAR
 */
#define RTB_SAVE_DIST_ADAPTIVE_ANGULAR_SENSITIVITY 2 //absolute


/*! \brief RTB Record-mode adaptive saving distance, maximum angular speed
 *
 *      
 *  The maximum speed the vehicle is capable of turning at. Used internally to normalize the
 * distance between consecutive waypoints.
 * @see RTB_SAVE_DIST_TRSH
 * @see RTB_SAVE_DIST_ADAPTIVE_ANGULAR
 */
#define RTB_SAVE_DIST_ADAPTIVE_MAX_ANGULAR_SPEED    0.8 //in radians per second

/*! \brief RTB Record-mode waypoint flushing distance threshold
 *
 *      
 *  During recording the algorithm attempts to optimize the cache of recorded waypoints.
 * This distance is the threshold to which every old point is confronted: if an already known
 * waypoint is found that passes this threshold, the chain of waypoints saved between the actual
 * position and the found one will be deleted. (circular path detection attempt)
 */
#define RTB_FLUSH_DIST_TRSH         1.0


/*! \brief RTB Track-mode waypoint change threshold
 *
 *      
 *  This value determines the distance from the current waypoint at which the
 * auto-guidance switches to the next waypoint.
 */
#define RTB_GUIDE_CHANGE_DIST_TRSH  1.0

        
/*! \brief RTB Track-mode global update frequency
 *
 *      
 *  This represents the decimator used to to perform a global update of all distances
 * of all cached waypoints. The base frequency is represented by the super-loop, thus
 * it is not know to the algorithm. It just chops down the number of times ti gets
 * called by this number, thus performing the global update at a frequency equal to
 * SUPERLOOP-CALLS / RTB_GUIDE_UPDATE_TIME_TRSH
 */        
#define RTB_GUIDE_UPDATE_TIME_TRSH  10
    
    
/*! \brief RTB operating mode selection
 *
 *      
 *  This value takes either of the RTB_GEO_MODE constants to shape
 * the software's behavior.
 * @see RTB_GEO_MODE_EUCLIDEAN
 * @see RTB_GEO_MODE_LATLON
 */    
#define RTB_GEO_MODE                RTB_GEO_MODE_EUCLIDEAN
    
    
/*! \brief RTB generalized Earth radius in Km
 *
 *      
 *  Generalized Earth radius used in mathematical operations
 */
#define EARTH                       6371
    
    
/*! \brief RTB minimum distance between waypoints
 *
 *      
 *  Minimum distance between waypoints
 */
#define RTB_MIN_WAYPOINT_DIST       1.0 // meters
    
        
    
    
/*! \brief RTB initialization call
 *
 *      
 *  This function must be called when the module is loaded, it sets the internal status to a known state.
 * It is not necessary to call this function any further after initialization.
 */
void RTB_init(void);

/*! \brief RTB mode selection
 *
 *      
 *  Used in conjunction with RTB_mode
 * @see RTB_mode
 * 
 * Parent program must actively call this function to change operating mode of the module.
 */
void RTB_set_mode(RTB_mode);

/*! \brief RTB main cycle handler
 *
 *      
 *  This function must be periodically called to guarantee correct recording of positions.
 * It is designed to be integrated in a superloop.
 * If the module is disabled, calling this function won't affect performance, as no operations
 * will be performed in such a case (the module will check its state and immediately return).
 * @param X value either euclidean or lon
 * @param Y value either euclidean or lat
 * @return a pointer to the current status
 * @see RTB_status, RTB_GEO_MODE
 */
void RTB_update(RTB_FLOAT_TYPE localx, RTB_FLOAT_TYPE localy, RTB_FLOAT_TYPE xspeed, RTB_FLOAT_TYPE aspeed);

#if (RTB_GEO_MODE == RTB_GEO_MODE_EUCLIDEAN)
typedef struct RTB_point{
    RTB_FLOAT_TYPE x;
    RTB_FLOAT_TYPE y;
    RTB_FLOAT_TYPE distance_from_start;
    struct RTB_point* next;
    struct RTB_point* previous;
    
} RTB_point;
#elif (RTB_GEO_MODE == RTB_GEO_MODE_LATLON)


typedef struct RTB_point{
    RTB_FLOAT_TYPE lat;
    RTB_FLOAT_TYPE lon;
    RTB_FLOAT_TYPE distance_from_start;
    struct RTB_point* next;
    struct RTB_point* previous;
    
} RTB_point;

#else
#error ("No geo mode specified in lucciRTB.h")
#endif

#define DEBUG   0
#define SIMUL   0

#ifdef	__cplusplus
}
#endif

#endif	/* LUCCIRTB_H */

