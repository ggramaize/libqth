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


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qth.h"

void testQthValidity( const char *qth, size_t qthStringSize, int expectedRetCode, size_t expectedPrecision, size_t *takenTests, size_t *passedTests);

// acceptedBias is given for the longitude. Bias for latitude is derived from the preceding value.
void checkQthToCoords( const char *qth, size_t qthStringSize, size_t expectedPrecisionLvl, double expectedLat, double expectedLng, double acceptedBias, size_t *takenTests, size_t *passedTests);

void inCheckCoordsToQth ( double Lat, double Lng, size_t *takenTests, size_t *passedTests);

void checkCoordsToQth ( double Lat, double Lng, size_t expectedPrecisionLvl, const char *expectedQth, size_t expectedQthStringSize, size_t *takenTests, size_t *passedTests);

int main (void)
{
	size_t takenTests = 0, passedTests = 0;

	printf("Running QTH locator library test suite ...\n\n");

	// Test invalid format detection
	testQthValidity( ""            ,  0, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "      "      ,  6, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "XA22EF"      ,  6, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AX22EF"      ,  6, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "1F23DC"      ,  6, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "A323DC"      ,  6, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AFB3DC"      ,  6, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF2NDC"      ,  6, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF238C"      ,  6, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF23D4"      ,  6, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF23YC"      ,  6, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF23DZ"      ,  6, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF23DCU1eb"  , 10, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF23DC9peb"  , 10, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF23DC913b"  , 10, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF23DC91e7"  , 10, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF23DC91zb"  , 10, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF23DC91ey"  , 10, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF23DC91ebr5", 12, EDOM, 0,  &takenTests, &passedTests );
	testQthValidity( "AF23DC91eb9c", 12, EDOM, 0,  &takenTests, &passedTests );

	// Test precision and case insensitiveness
	testQthValidity( "AF23DC91XB95", 12,    0, 5,  &takenTests, &passedTests );	
	testQthValidity( "AF23DC91XB"  , 10,    0, 4,  &takenTests, &passedTests );
	testQthValidity( "AF23DC91EX"  , 10,    0, 4,  &takenTests, &passedTests );
	testQthValidity( "AF23DC91EB"  , 10,    0, 4,  &takenTests, &passedTests );
	testQthValidity( "AF23DC91eB"  , 10,    0, 4,  &takenTests, &passedTests );
	testQthValidity( "AF23DC91Eb"  , 10,    0, 4,  &takenTests, &passedTests );
	testQthValidity( "AF23DC91eb"  , 10,    0, 4,  &takenTests, &passedTests );
	testQthValidity( "AF23DC91"    ,  8,    0, 3,  &takenTests, &passedTests );
	testQthValidity( "AF23DX"      ,  6,    0, 2,  &takenTests, &passedTests );
	testQthValidity( "AF23XC"      ,  6,    0, 2,  &takenTests, &passedTests );
	testQthValidity( "AF23DC"      ,  6,    0, 2,  &takenTests, &passedTests );
	testQthValidity( "AF23dC"      ,  6,    0, 2,  &takenTests, &passedTests );
	testQthValidity( "AF23Dc"      ,  6,    0, 2,  &takenTests, &passedTests );
	testQthValidity( "AF23dc"      ,  6,    0, 2,  &takenTests, &passedTests );
	testQthValidity( "AF23dc"      ,  6,    0, 2,  &takenTests, &passedTests );
	testQthValidity( "AF23"        ,  4,    0, 1,  &takenTests, &passedTests );
	testQthValidity( "AF"          ,  2,    0, 0,  &takenTests, &passedTests );
	testQthValidity( "aF"          ,  2,    0, 0,  &takenTests, &passedTests );
	testQthValidity( "Af"          ,  2,    0, 0,  &takenTests, &passedTests );
	testQthValidity( "af"          ,  2,    0, 0,  &takenTests, &passedTests );

	// **** QTH to coords
	checkQthToCoords( "AA00aa00aa00", 12, 5, -90.0, -180.0, _QTH_HGRID_STEP5/2, &takenTests, &passedTests);
	checkQthToCoords( "RR99xx99xx99", 12, 5,  90.0,  180.0, _QTH_HGRID_STEP5/2, &takenTests, &passedTests);
	checkQthToCoords( "Ra90xa90xa90", 12, 5, -90.0,  180.0, _QTH_HGRID_STEP5/2, &takenTests, &passedTests);
	checkQthToCoords( "AR09ax09ax09", 12, 5,  90.0, -180.0, _QTH_HGRID_STEP5/2, &takenTests, &passedTests);
	checkQthToCoords( "JJ00aa00aa00", 12, 5,   0.0,    0.0, _QTH_HGRID_STEP5/2, &takenTests, &passedTests);
	
	checkQthToCoords( "AA00aa00aa"  , 10, 4, -90.0, -180.0, _QTH_HGRID_STEP4/2, &takenTests, &passedTests);
	checkQthToCoords( "RR99xx99xx"  , 10, 4,  90.0,  180.0, _QTH_HGRID_STEP4/2, &takenTests, &passedTests);
	checkQthToCoords( "Ra90xa90xa"  , 10, 4, -90.0,  180.0, _QTH_HGRID_STEP4/2, &takenTests, &passedTests);
	checkQthToCoords( "AR09ax09ax"  , 10, 4,  90.0, -180.0, _QTH_HGRID_STEP4/2, &takenTests, &passedTests);
	checkQthToCoords( "JJ00aa00aa"  , 10, 4,   0.0,    0.0, _QTH_HGRID_STEP4/2, &takenTests, &passedTests);

	checkQthToCoords( "AA00aa00"    ,  8, 3, -90.0, -180.0, _QTH_HGRID_STEP3/2, &takenTests, &passedTests);
	checkQthToCoords( "RR99xx99"    ,  8, 3,  90.0,  180.0, _QTH_HGRID_STEP3/2, &takenTests, &passedTests);
	checkQthToCoords( "Ra90xa90"    ,  8, 3, -90.0,  180.0, _QTH_HGRID_STEP3/2, &takenTests, &passedTests);
	checkQthToCoords( "AR09ax09"    ,  8, 3,  90.0, -180.0, _QTH_HGRID_STEP3/2, &takenTests, &passedTests);
	checkQthToCoords( "JJ00aa00"    ,  8, 3,   0.0,    0.0, _QTH_HGRID_STEP3/2, &takenTests, &passedTests);

	checkQthToCoords( "AA00aa"      ,  6, 2, -90.0, -180.0, _QTH_HGRID_STEP2/2, &takenTests, &passedTests);
	checkQthToCoords( "RR99xx"      ,  6, 2,  90.0,  180.0, _QTH_HGRID_STEP2/2, &takenTests, &passedTests);
	checkQthToCoords( "Ra90xa"      ,  6, 2, -90.0,  180.0, _QTH_HGRID_STEP2/2, &takenTests, &passedTests);
	checkQthToCoords( "AR09ax"      ,  6, 2,  90.0, -180.0, _QTH_HGRID_STEP2/2, &takenTests, &passedTests);
	checkQthToCoords( "JJ00aa"      ,  6, 2,   0.0,    0.0, _QTH_HGRID_STEP2/2, &takenTests, &passedTests);

	checkQthToCoords( "AA00"        ,  4, 1, -90.0, -180.0, _QTH_HGRID_STEP1/2, &takenTests, &passedTests);
	checkQthToCoords( "RR99"        ,  4, 1,  90.0,  180.0, _QTH_HGRID_STEP1/2, &takenTests, &passedTests);
	checkQthToCoords( "Ra90"        ,  4, 1, -90.0,  180.0, _QTH_HGRID_STEP1/2, &takenTests, &passedTests);
	checkQthToCoords( "AR09"        ,  4, 1,  90.0, -180.0, _QTH_HGRID_STEP1/2, &takenTests, &passedTests);
	checkQthToCoords( "JJ00"        ,  4, 1,   0.0,    0.0, _QTH_HGRID_STEP1/2, &takenTests, &passedTests);

	checkQthToCoords( "AA"          ,  2, 0, -90.0, -180.0, _QTH_HGRID_STEP0/2, &takenTests, &passedTests);
	checkQthToCoords( "RR"          ,  2, 0,  90.0,  180.0, _QTH_HGRID_STEP0/2, &takenTests, &passedTests);
	checkQthToCoords( "Ra"          ,  2, 0, -90.0,  180.0, _QTH_HGRID_STEP0/2, &takenTests, &passedTests);
	checkQthToCoords( "AR"          ,  2, 0,  90.0, -180.0, _QTH_HGRID_STEP0/2, &takenTests, &passedTests);
	checkQthToCoords( "JJ"          ,  2, 0,   0.0,    0.0, _QTH_HGRID_STEP0/2, &takenTests, &passedTests);

	// Check some "known" landmarks
	// Saint Etienne, France
	checkQthToCoords( "JN25EK"      ,  6, 2,  45.43894 ,    4.36844, _QTH_VGRID_STEP2/2, &takenTests, &passedTests);
	// Ueda-shi, Nagano-ken, Japan
	checkQthToCoords( "PM96CJ96RF"  , 10, 4,  36.401   ,  138.247667, _QTH_HGRID_STEP4/2, &takenTests, &passedTests);
	// Dunedin city, Otago, New Zealand   
	checkQthToCoords( "RE54GC57EH"  , 10, 4, -45.886167,  170.543167, _QTH_HGRID_STEP4/2, &takenTests, &passedTests);
	// Fairbanks, AK, USA
	checkQthToCoords( "BP64DV54AA"  , 10, 4,  64.891667, -147.708333, _QTH_HGRID_STEP4/2, &takenTests, &passedTests);
 	// Antartic base Marambio, Antartica
	checkQthToCoords( "GC15QS42QM"  , 10, 4, -64.2395  ,  -56.6275, _QTH_HGRID_STEP4/2, &takenTests, &passedTests);
	// Beringovsky, Chukotka Autonomous Okrug, Russian Federation
	checkQthToCoords( "RP93QB25NB16", 12, 5,  63.062778,  179.354549, _QTH_HGRID_STEP5/2, &takenTests, &passedTests);


	// **** Coords to QTH
	inCheckCoordsToQth ( -175.0, -375.0, &takenTests, &passedTests);
	inCheckCoordsToQth ( -175.0,  375.0, &takenTests, &passedTests);
	inCheckCoordsToQth (  175.0, -375.0, &takenTests, &passedTests);
	inCheckCoordsToQth (  175.0,  375.0, &takenTests, &passedTests);
	inCheckCoordsToQth (  -75.0, -175.0, &takenTests, &passedTests);
	inCheckCoordsToQth (  -75.0,  175.0, &takenTests, &passedTests);
	inCheckCoordsToQth (   75.0, -175.0, &takenTests, &passedTests);
	inCheckCoordsToQth (   75.0,  175.0, &takenTests, &passedTests);
	inCheckCoordsToQth ( -175.0,  -75.0, &takenTests, &passedTests);
	inCheckCoordsToQth ( -175.0,   75.0, &takenTests, &passedTests);
	inCheckCoordsToQth (  175.0,  -75.0, &takenTests, &passedTests);
	inCheckCoordsToQth (  175.0,   75.0, &takenTests, &passedTests);
	inCheckCoordsToQth (  -75.0, -375.0, &takenTests, &passedTests);
	inCheckCoordsToQth (  -75.0,  375.0, &takenTests, &passedTests);
	inCheckCoordsToQth (   75.0, -375.0, &takenTests, &passedTests);
	inCheckCoordsToQth (   75.0,  375.0, &takenTests, &passedTests);

	// 
	checkCoordsToQth ( -90.0      , -180.0      , 5, "AA00AA00AA00", 12, &takenTests, &passedTests);
	checkCoordsToQth (  89.9999999,  179.999999 , 5, "RR99XX99XX99", 12, &takenTests, &passedTests);
	checkCoordsToQth ( -90.0      ,  179.999999 , 5, "RA90XA90XA90", 12, &takenTests, &passedTests);
	checkCoordsToQth (  89.9999999, -180.0      , 5, "AR09AX09AX09", 12, &takenTests, &passedTests);
	checkCoordsToQth (   0.0      ,    0.0      , 5, "JJ00AA00AA00", 12, &takenTests, &passedTests);

	checkCoordsToQth ( -90.0     , -180.0      , 4, "AA00AA00AA"  , 10, &takenTests, &passedTests);
	checkCoordsToQth (  89.999999,  179.999999 , 4, "RR99XX99XX"  , 10, &takenTests, &passedTests);
	checkCoordsToQth ( -90.0     ,  179.999999 , 4, "RA90XA90XA"  , 10, &takenTests, &passedTests);
	checkCoordsToQth (  89.999999, -180.0      , 4, "AR09AX09AX"  , 10, &takenTests, &passedTests);
	checkCoordsToQth (   0.0     ,    0.0      , 4, "JJ00AA00AA"  , 10, &takenTests, &passedTests);

	checkCoordsToQth ( -90.0     , -180.0      , 3, "AA00AA00"    ,  8, &takenTests, &passedTests);
	checkCoordsToQth (  89.999999,  179.999999 , 3, "RR99XX99"    ,  8, &takenTests, &passedTests);
	checkCoordsToQth ( -90.0     ,  179.999999 , 3, "RA90XA90"    ,  8, &takenTests, &passedTests);
	checkCoordsToQth (  89.999999, -180.0      , 3, "AR09AX09"    ,  8, &takenTests, &passedTests);
	checkCoordsToQth (   0.0     ,    0.0      , 3, "JJ00AA00"    ,  8, &takenTests, &passedTests);

	checkCoordsToQth ( -90.0     , -180.0      , 2, "AA00AA"      ,  6, &takenTests, &passedTests);
	checkCoordsToQth (  89.999999,  179.999999 , 2, "RR99XX"      ,  6, &takenTests, &passedTests);
	checkCoordsToQth ( -90.0     ,  179.999999 , 2, "RA90XA"      ,  6, &takenTests, &passedTests);
	checkCoordsToQth (  89.999999, -180.0      , 2, "AR09AX"      ,  6, &takenTests, &passedTests);
	checkCoordsToQth (   0.0     ,    0.0      , 2, "JJ00AA"      ,  6, &takenTests, &passedTests);

	checkCoordsToQth ( -90.0     , -180.0      , 1, "AA00"        ,  4, &takenTests, &passedTests);
	checkCoordsToQth (  89.999999,  179.999999 , 1, "RR99"        ,  4, &takenTests, &passedTests);
	checkCoordsToQth ( -90.0     ,  179.999999 , 1, "RA90"        ,  4, &takenTests, &passedTests);
	checkCoordsToQth (  89.999999, -180.0      , 1, "AR09"        ,  4, &takenTests, &passedTests);
	checkCoordsToQth (   0.0     ,    0.0      , 1, "JJ00"        ,  4, &takenTests, &passedTests);

	checkCoordsToQth ( -90.0     , -180.0      , 0, "AA"          ,  2, &takenTests, &passedTests);
	checkCoordsToQth (  89.999999,  179.999999 , 0, "RR"          ,  2, &takenTests, &passedTests);
	checkCoordsToQth ( -90.0     ,  179.999999 , 0, "RA"          ,  2, &takenTests, &passedTests);
	checkCoordsToQth (  89.999999, -180.0      , 0, "AR"          ,  2, &takenTests, &passedTests);
	checkCoordsToQth (   0.0     ,    0.0      , 0, "JJ"          ,  2, &takenTests, &passedTests);

	// Check back our landmarks
	checkCoordsToQth (  45.43894 , 4.36844     , 2, "JN25EK"      ,  6, &takenTests, &passedTests);
	checkCoordsToQth (  36.401   ,   138.247667, 4, "PM96CJ96RF"  , 10, &takenTests, &passedTests);
	checkCoordsToQth ( -45.886167,   170.543167, 4, "RE54GC57EH"  , 10, &takenTests, &passedTests);
	checkCoordsToQth (  64.891667,  -147.708333, 4, "BP64DV54AA"  , 10, &takenTests, &passedTests);
	checkCoordsToQth ( -64.2395  ,   -56.6275  , 4, "GC15QS42QM"  , 10, &takenTests, &passedTests);
	checkCoordsToQth (  63.062778,   179.354549, 5, "RP93QB25NB16", 12, &takenTests, &passedTests);

    // void checkCoordsToQth ( double Lat, double Lng, size_t expectedPrecisionLvl, const char *expectedQth, size_t expectedQthStringSize, size_t *takenTests, size_t *passedTests);

	printf("\nTests results: %lu / %lu Passed.\n", passedTests, takenTests);

	return (passedTests != takenTests);
}

void testQthValidity( const char *qth, size_t qthStringSize, int expectedRetCode, size_t expectedPrecision, size_t *takenTests, size_t *passedTests)
{
	++(*takenTests);
	size_t precision = 0;
	int retcode = is_valid_qth( qth, qthStringSize, &precision);

	// Check validity of the return code
	if ( retcode == expectedRetCode )
	{
		if( retcode == 0 && precision != expectedPrecision)
		{
			printf( "Test failed: is_valid_qth( \"%s\", %lu, &precision): got precision level %lu, expected level %lu. \n", qth, qthStringSize, precision, expectedPrecision);
			return;
		}

		++(*passedTests);
		return;
	}

	printf( "Test failed: is_valid_qth( \"%s\", %lu, &precision): got errcode %u, expected %d.\n", qth, qthStringSize, retcode, expectedRetCode);
	return;
}

void checkQthToCoords( const char *qth, size_t qthStringSize, size_t expectedPrecisionLvl, double expectedLat, double expectedLng, double acceptedBias, size_t *takenTests, size_t *passedTests)
{
	++(*takenTests);
	size_t precision = 0;
	double lat = -300;
	double lng = -300;	

	if( qth_to_coords( qth, qthStringSize, &lat, &lng, &precision) != 0 )
	{
		printf( "Test failed: qth_to_coords( \"%s\", %lu, &lat, &lng, &precision):\n", qth, qthStringSize);
		printf( "\tInvalid test sample.\n");
		return;
	}

	if( precision != expectedPrecisionLvl)
	{
		printf( "Test failed: qth_to_coords( \"%s\", %lu, &lat, &lng, &precision):\n", qth, qthStringSize);
		printf( "\tBad precision, got %lu, expected %lu. \n", precision, expectedPrecisionLvl);
		return;
	}
	
	if( lat > (expectedLat + (acceptedBias/2) + 0.000001) || lat < (expectedLat - (acceptedBias/2) - 0.000001) )
	{
		printf( "Test failed: qth_to_coords( \"%s\", %lu, &lat, &lng, &precision):\n", qth, qthStringSize);
		printf( "\tLatitude out of acceptable range: got %f, expected %f +/- %f.\n", lat, expectedLat, (acceptedBias/2)+0.000001);
		return;
	}

	if( lng > (expectedLng + acceptedBias + 0.000001) || lng < (expectedLng - acceptedBias - 0.000001) )
	{
		printf( "Test failed: qth_to_coords( \"%s\", %lu, &lat, &lng, &precision):\n", qth, qthStringSize);
		printf( "\tLongitude out of acceptable range: got %f, expected %f +/- %f.\n", lng, expectedLng, acceptedBias+0.000001);
		return;
	}

	++(*passedTests);
	return;
}

void inCheckCoordsToQth ( double Lat, double Lng, size_t *takenTests, size_t *passedTests)
{
	++(*takenTests);

	// Set a string, and zeroize it.
	char qth[13];
	memset( qth, 0, 13);

	int retcode = coords_to_qth( Lat, Lng, 5, qth);

	if( retcode != 0 && retcode != EDOM )
	{
		printf( "Test failed: coords_to_qth( %lf, %lf, %d, &qth):\n", Lat, Lng, 5);
		printf( "\tUnexpected error code. All \"out of domain\" errors must be returned as errno's EDOM.\n");
		return;
	}

	if( Lng < -180.0 || Lng > 180.0 || Lat < -90.0 || Lat > 90.0 )
	{
		if( retcode != EDOM )
		{
			printf( "Test failed: coords_to_qth( %lf, %lf, %d, &qth):\n", Lat, Lng, 5);
			printf( "\tFunction doesn't report invalid data properly.\n");
			return;
		}
		++(*passedTests);
		return;
	}

	if( retcode != 0 )
	{
		printf( "Test failed: coords_to_qth( %lf, %lf, %d, &qth):\n", Lat, Lng, 5);
		printf( "\tFunction doesn't report valid data properly.\n");
		return;
	}	

	++(*passedTests);
	return;
}

void checkCoordsToQth ( double Lat, double Lng, size_t expectedPrecisionLvl, const char *expectedQth, size_t expectedQthStringSize, size_t *takenTests, size_t *passedTests)
{
	++(*takenTests);

	// Set a string, and zeroize it.
	char qth[13];
	memset( qth, 0, 13);

	if( coords_to_qth( Lat, Lng, expectedPrecisionLvl, qth) != 0 )
	{
		printf( "Test failed: coords_to_qth( %lf, %lf, %lu, &qth):\n", Lat, Lng, expectedPrecisionLvl);
		printf( "\tInvalid test sample.\n");
		return;
	}

	if( strlen( qth) != expectedQthStringSize )
	{
		printf( "Test failed: coords_to_qth( %lf, %lf, %lu, &qth):\n", Lat, Lng, expectedPrecisionLvl);
		printf( "\tQTH not matching: got \"%s\", expected \"%s\".\n", qth, expectedQth);
		return;
	}

	if( strncmp( expectedQth, qth, (expectedQthStringSize < 12) ? expectedQthStringSize : 12 ) != 0 )
	{
		printf( "Test failed: coords_to_qth( %lf, %lf, %lu, &qth):\n", Lat, Lng, expectedPrecisionLvl);
		printf( "\tQTH not matching: got \"%s\", expected \"%s\".\n", qth, expectedQth);
		return;
	}

	++(*passedTests);
	return;
}
