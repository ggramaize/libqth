/*
 * Copyright (c) 2016, Geoffroy GRAMAIZE
 * 
 * This file is part of libqth
 *
 * libqth is free software; you can redistribute it and/or modify it
 * under the terms of the 3-clause BSD License as published by the
 * Free Software Foundation: http://directory.fsf.org/wiki/License:BSD_3Clause
 * 
 * libqth is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * You should have received a copy of the 3-clause BSD License along with GA;
 * if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef QTH_H
#define QTH_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The grid division size per level
 */
// (360 / 18) => 20°/div
#define _QTH_HGRID_STEP0	20.0
// (360 / 18*10) => 20/10°/div
#define _QTH_HGRID_STEP1	 2.0
// (360 / 18*10*24) => 20/240°/div
#define _QTH_HGRID_STEP2	(1.0 / 12)
// (360 / 18*10*24*10) => 20/2400°/div
#define _QTH_HGRID_STEP3	(1.0 / 120)
// (360 / 18*10*24*10*24) => 20/57600°/div
#define _QTH_HGRID_STEP4	(1.0 / 2880)
// (360 / 18*10*24*10*24*10) => 20/576000°/div
#define _QTH_HGRID_STEP5	(1.0 / 28800)

// (180 / 18) => 10°/div
#define _QTH_VGRID_STEP0	10.0
// (180 / 18*10) => 10/10°/div
#define _QTH_VGRID_STEP1	 1.0
// (180 / 18*10*24) => 10/240°/div
#define _QTH_VGRID_STEP2	(1.0 / 24)
// (180 / 18*10*24*10) => 10/2400°/div
#define _QTH_VGRID_STEP3	(1.0 / 240)
// (180 / 18*10*24*10*24) => 10/57600°/div
#define _QTH_VGRID_STEP4	(1.0 / 5760)
// (180 / 18*10*24*10*24*10) => 10/576000°/div
#define _QTH_VGRID_STEP5	(1.0 / 57600)

/** @brief Validates that a C string is a valid QTH.
 * This function validates that a provided C string is a valid QTH locator,
 * and return the grid precision order. This function doesn't perform partial
 * content validation.
 * 
 * @param[in]	qthToTest	The C string to be checked.
 * @param[in]	qthStringSize	The preceding C string size computed with strlen().
 * @param[out]	precision	The grid precision level. Set as NULL if you don't care of the result.
 * @return	The validation result. Its value is an errno: 0 if qthToTest is a valid QTH Locator, EDOM if not.
 */
int is_valid_qth( const char *qthToTest, size_t qthStringSize, size_t *precision);

/** @brief Converts a C string QTH locator to floating point coordinates.
 *  
 * @param[in]	qth		The QTH to convert, stored in a C string.
 * @param[in]	qthStringSize	The preceding C string size computed with strlen().
 * @param[out]	latitude	Latitude in degrees. 0° is the equator.
 * @param[out]	longitude	Longitude in degrees. 0 is the greenwich meridian.
 * @param[out]	precision	The desired level of precision.
 * @return	The validation result. Its value is an errno: EDOM if the qth string is not a valid QTH locator, 0 if it is a valid one.
 */
int qth_to_coords( const char *qth, size_t qthStringSize, double *latitude, double *longitude, size_t *precision);

/** @brief Converts floating point coordinates to a QTH locator.
 *  
 * @param[in]	latitude	Latitude in degrees. 0° is the equator.
 * @param[in]	longitude	Longitude in degrees. 0 is the greenwich meridian.
 * @param[in]	precision	The desired level of precision.
 * @param[out] qth		The converted QTH. You must pass a zeroized char[13] to put the result. qth[12] is never touched. 
 * @return	The validation result. Its value is an errno: EDOM if any of the coordinates is out of bounds, 0 if not.
 */
int coords_to_qth( double latitude, double longitude, size_t precision, char *qth);

#ifdef __cplusplus
} // extern "C"
#endif

#endif	// QTH_H
