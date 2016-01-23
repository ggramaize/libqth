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
#include <ctype.h>
#include <errno.h>
#include <math.h>

#include "qth.h"

int is_valid_qth( const char *qthToTest, size_t qthStringSize, size_t *precision)
{
	if( qthStringSize > 12 || qthStringSize == 0 || qthStringSize % 2 != 0)
		return EDOM;

	// Sixth group is figures
	if (	qthStringSize == 12 && !( 
			isdigit(qthToTest[10]) && 
			isdigit(qthToTest[11]) )		
	   )
		return EDOM;

	// Fifth group is letters from A to X
	if ( 	qthStringSize >= 10 && !( 
			toupper( qthToTest[8] ) >= 'A' && 
			toupper( qthToTest[8] ) <= 'X' &&
			toupper( qthToTest[9] ) >= 'A' && 
			toupper( qthToTest[9] ) <= 'X' )
	   )
		return EDOM;
	
	// Fourth group is figures
	if (	qthStringSize >= 8 && !( 
			isdigit(qthToTest[6]) && 
			isdigit(qthToTest[7]) )		
	   )
		return EDOM;

	// Third group is letters A to X
	if ( 	qthStringSize >= 6 && !( 
			toupper(qthToTest[4]) >= 'A' &&
			toupper(qthToTest[4]) <= 'X' &&
			toupper(qthToTest[5]) >= 'A' && 
			toupper(qthToTest[5]) <= 'X' )
	   )
		return EDOM;

	// Second group is figures
	if (	qthStringSize >= 4 && !( 
			isdigit(qthToTest[2]) && 
			isdigit(qthToTest[3]) )
	   )
		return EDOM;

	// First group is letters A to R
	if ( 	!( 
			toupper(qthToTest[0]) >= 'A' &&
			toupper(qthToTest[0]) <= 'R' &&
			toupper(qthToTest[1]) >= 'A' && 
			toupper(qthToTest[1]) <= 'R' )
	   )
		return EDOM;
	
	// Down here we should end with a valid qth
	if ( precision != NULL )
	{
		*precision = (qthStringSize / 2) - 1;
	}
	
	return 0;	
}


int qth_to_coords( const char *qth, size_t qthStringSize, double *latitude, double *longitude, size_t *precision)
{
	size_t gridLevel = 0;
	// To begin with, we check if the locator is valid
	if( is_valid_qth( qth, qthStringSize, &gridLevel) != 0 )
		return EDOM;

	*longitude = 0.0;
	*latitude = 0.0;

	switch (gridLevel)
	{
	case 5:
		if ( gridLevel == 5)
		{
			*longitude += _QTH_HGRID_STEP5 /2;
			*latitude  += _QTH_VGRID_STEP5 /2;
		}

		*longitude += _QTH_HGRID_STEP5 * (         qth[10]  - '0' );
		*latitude  += _QTH_VGRID_STEP5 * (         qth[11]  - '0' );

	case 4:
		if ( gridLevel == 4)
		{
			*longitude += _QTH_HGRID_STEP4 /2;
			*latitude  += _QTH_VGRID_STEP4 /2;
		}

		*longitude += _QTH_HGRID_STEP4 * ( toupper(qth[8]) - 'A' );
		*latitude  += _QTH_VGRID_STEP4 * ( toupper(qth[9]) - 'A' );		

	case 3:
		if ( gridLevel == 3)
		{
			*longitude += _QTH_HGRID_STEP3 /2;
			*latitude  += _QTH_VGRID_STEP3 /2;
		}

		*longitude += _QTH_HGRID_STEP3 * (         qth[6]  - '0' );
		*latitude  += _QTH_VGRID_STEP3 * (         qth[7]  - '0' );		

	case 2:
		if ( gridLevel == 2)
		{
			*longitude += _QTH_HGRID_STEP2 /2;
			*latitude  += _QTH_VGRID_STEP2 /2;
		}

		*longitude += _QTH_HGRID_STEP2 * ( toupper(qth[4]) - 'A' );
		*latitude  += _QTH_VGRID_STEP2 * ( toupper(qth[5]) - 'A' );

	case 1:
		if ( gridLevel == 1)
		{
			*longitude += _QTH_HGRID_STEP1 /2;
			*latitude  += _QTH_VGRID_STEP1 /2;
		}

		*longitude += _QTH_HGRID_STEP1 * (         qth[2]  - '0' );
		*latitude  += _QTH_VGRID_STEP1 * (         qth[3]  - '0' );

	case 0:
		if ( gridLevel == 0)
		{
			*longitude += _QTH_HGRID_STEP0 /2;
			*latitude  += _QTH_VGRID_STEP0 /2;
		}

		*longitude += _QTH_HGRID_STEP0 * ( toupper(qth[0]) - 'A' );
		*latitude  += _QTH_VGRID_STEP0 * ( toupper(qth[1]) - 'A' );	
		break;

	default:
		return EDOM;

	} // switch

	if ( precision != NULL ) {
		*precision = gridLevel;
	}
	
	// Center back the coordinates
	*longitude -= 180.0;
	*latitude -= 90.0;
	return 0;
}

int coords_to_qth( double latitude, double longitude, size_t precision, char *qth)
{
	double lng = longitude + 180;
 	double lat = latitude + 90;

	if( lng < 0.0 || lng >= 360.0 || lat < 0.0 || lat >= 180.0 )
		return EDOM;

	// first, we get the position of the 1st square.
	qth[0] = 'A' + floor( lng / _QTH_HGRID_STEP0 );
	qth[1] = 'A' + floor( lat / _QTH_VGRID_STEP0 );
	
	if( precision == 0)
		return 0;

	// To get the n-th level position, get the shift modulo grid n-1-th step
	lng -= floor( lng / _QTH_HGRID_STEP0 ) * _QTH_HGRID_STEP0;
	lat -= floor( lat / _QTH_VGRID_STEP0 ) * _QTH_VGRID_STEP0;


	// write 2nd level square 
	qth[2] = '0' + floor( lng / _QTH_HGRID_STEP1 );
	qth[3] = '0' + floor( lat / _QTH_VGRID_STEP1 );

	if( precision == 1)
		return 0;

	// To get the n-th level position, get the shift modulo grid n-1-th step
	lng -= floor( lng / _QTH_HGRID_STEP1 ) * _QTH_HGRID_STEP1;
	lat -= floor( lat / _QTH_VGRID_STEP1 ) * _QTH_VGRID_STEP1;


	// write the 3rd level square
	qth[4] = 'A' + floor( lng / _QTH_HGRID_STEP2 );
	qth[5] = 'A' + floor( lat / _QTH_VGRID_STEP2 );	

	if( precision == 2)
		return 0;

	// To get the n-th level position, get the shift modulo grid n-1-th step
	lng -= floor( lng / _QTH_HGRID_STEP2 ) * _QTH_HGRID_STEP2;
	lat -= floor( lat / _QTH_VGRID_STEP2 ) * _QTH_VGRID_STEP2;

	
	// write 4th level square 
	qth[6] = '0' + floor( lng / _QTH_HGRID_STEP3 );
	qth[7] = '0' + floor( lat / _QTH_VGRID_STEP3 );

	if( precision == 3)
		return 0;

	// To get the n-th level position, get the shift modulo grid n-1-th step
	lng -= floor( lng / _QTH_HGRID_STEP3 ) * _QTH_HGRID_STEP3;
	lat -= floor( lat / _QTH_VGRID_STEP3 ) * _QTH_VGRID_STEP3;


	// write the 5th level square
	qth[8] = 'A' + floor( lng / _QTH_HGRID_STEP4 );
	qth[9] = 'A' + floor( lat / _QTH_VGRID_STEP4 );	

	if( precision == 4)
		return 0;

	// To get the n-th level position, get the shift modulo grid n-1-th step
	lng -= floor( lng / _QTH_HGRID_STEP4 ) * _QTH_HGRID_STEP4;
	lat -= floor( lat / _QTH_VGRID_STEP4 ) * _QTH_VGRID_STEP4;

	// write 6th level square 
	qth[10] = '0' + floor( lng / _QTH_HGRID_STEP5 );
	qth[11] = '0' + floor( lat / _QTH_VGRID_STEP5 );

	return 0;
}
