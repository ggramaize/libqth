import libqth

EDOM = False

# The grid division size per level
_QTH_HGRID_STEP0 = 20.0
_QTH_HGRID_STEP1 =  2.0
_QTH_HGRID_STEP2 = (1.0 / 12)
_QTH_HGRID_STEP3 = (1.0 / 120)
_QTH_HGRID_STEP4 = (1.0 / 2880)
_QTH_HGRID_STEP5 = (1.0 / 28800)

_QTH_VGRID_STEP0 = 10.0
_QTH_VGRID_STEP1 =  1.0
_QTH_VGRID_STEP2 = (1.0 / 24)
_QTH_VGRID_STEP3 = (1.0 / 240)
_QTH_VGRID_STEP4 = (1.0 / 5760)
_QTH_VGRID_STEP5 = (1.0 / 57600)

class tests:
	def __init__(self):
		self.done = 0
		self.failed = 0

	def testQthValidity(self, _qth, _size, _errcode_wanted, _precision):
		_retcode = libqth.is_valid_qth(_qth, _precision)
		if _retcode == _errcode_wanted:
			self.done+=1
		else:
			self.failed+=1

	def checkQthToCoords(self, _qth, _size, _expectPrecision, _expectLat, _expectLng, _bias):
		_ret = libqth.qth_to_coords(_qth, _size)
		if not _ret:
			self.failed += 1
			return
		if _ret['precision'] != _expectPrecision:
			self.failed += 1
			return
		if _ret['latitude'] > (_expectLat + (_bias/2) + 0.00001) or _ret['latitude'] < (_expectLat - (_bias/2) - 0.000001):
			self.failed += 1
			return
		if _ret['longitude'] > (_expectLng + _bias + 0.00001) or _ret['longitude'] < (_expectLng - _bias - 0.000001):
			self.failed += 1
			return

		self.done += 1

	def inCheckCoordsToQth(self, _lat, _lng):
		_ret = libqth.coords_to_qth(_lat, _lng, 5)
		if _lng < -180.0 or _lng > 180.0 or _lat < -90.0 or _lat > 90.0:
			if _ret:
				self.failed += 1
				return
			self.done+=1
			return

		self.done += 1

	def checkCoordsToQth(self, _lat, _lng, _expectPrecision, _expectQth, _expectQthSize):
		_ret = libqth.coords_to_qth(_lat, _lng, _expectPrecision)
		if len(_ret['qth']) != _expectQthSize:
			self.failed += 1
			return

		if _ret['qth'] != _expectQth:
			self.failed += 1
			return

		self.done += 1

# Run tests
t = tests()

# Test invalid format detection
t.testQthValidity(""            ,  0, EDOM, 0)
t.testQthValidity("      "      ,  6, EDOM, 0)
t.testQthValidity("XA22EF"      ,  6, EDOM, 0)
t.testQthValidity("AX22EF"      ,  6, EDOM, 0)
t.testQthValidity("1F23DC"      ,  6, EDOM, 0)
t.testQthValidity("A323DC"      ,  6, EDOM, 0)
t.testQthValidity("AFB3DC"      ,  6, EDOM, 0)
t.testQthValidity("AF2NDC"      ,  6, EDOM, 0)
t.testQthValidity("AF238C"      ,  6, EDOM, 0)
t.testQthValidity("AF23D4"      ,  6, EDOM, 0)
t.testQthValidity("AF23YC"      ,  6, EDOM, 0)
t.testQthValidity("AF23DZ"      ,  6, EDOM, 0)
t.testQthValidity("AF23DCU1eb"  , 10, EDOM, 0)
t.testQthValidity("AF23DC9peb"  , 10, EDOM, 0)
t.testQthValidity("AF23DC913b"  , 10, EDOM, 0)
t.testQthValidity("AF23DC91e7"  , 10, EDOM, 0)
t.testQthValidity("AF23DC91zb"  , 10, EDOM, 0)
t.testQthValidity("AF23DC91ey"  , 10, EDOM, 0)
t.testQthValidity("AF23DC91ebr5", 12, EDOM, 0)
t.testQthValidity("AF23DC91eb9c", 12, EDOM, 0)

# Test precision and case insensitiveness
t.testQthValidity("AF23DC91XB95", 12,    True, 5)
t.testQthValidity("AF23DC91XB"  , 10,    True, 4)
t.testQthValidity("AF23DC91EX"  , 10,    True, 4)
t.testQthValidity("AF23DC91EB"  , 10,    True, 4)
t.testQthValidity("AF23DC91eB"  , 10,    True, 4)
t.testQthValidity("AF23DC91Eb"  , 10,    True, 4)
t.testQthValidity("AF23DC91eb"  , 10,    True, 4)
t.testQthValidity("AF23DC91"    ,  8,    True, 3)
t.testQthValidity("AF23DX"      ,  6,    True, 2)
t.testQthValidity("AF23XC"      ,  6,    True, 2)
t.testQthValidity("AF23DC"      ,  6,    True, 2)
t.testQthValidity("AF23dC"      ,  6,    True, 2)
t.testQthValidity("AF23Dc"      ,  6,    True, 2)
t.testQthValidity("AF23dc"      ,  6,    True, 2)
t.testQthValidity("AF23dc"      ,  6,    True, 2)
t.testQthValidity("AF23"        ,  4,    True, 1)
t.testQthValidity("AF"          ,  2,    True, 0)
t.testQthValidity("aF"          ,  2,    True, 0)
t.testQthValidity("Af"          ,  2,    True, 0)
t.testQthValidity("af"          ,  2,    True, 0)

t.checkQthToCoords( "AA00aa00aa00", 12, 5, -90.0, -180.0, _QTH_HGRID_STEP5/2)
t.checkQthToCoords( "RR99xx99xx99", 12, 5,  90.0,  180.0, _QTH_HGRID_STEP5/2)
t.checkQthToCoords( "Ra90xa90xa90", 12, 5, -90.0,  180.0, _QTH_HGRID_STEP5/2)
t.checkQthToCoords( "AR09ax09ax09", 12, 5,  90.0, -180.0, _QTH_HGRID_STEP5/2)
t.checkQthToCoords( "JJ00aa00aa00", 12, 5,   0.0,    0.0, _QTH_HGRID_STEP5/2)

t.checkQthToCoords( "AA00aa00aa"  , 10, 4, -90.0, -180.0, _QTH_HGRID_STEP4/2)
t.checkQthToCoords( "RR99xx99xx"  , 10, 4,  90.0,  180.0, _QTH_HGRID_STEP4/2)
t.checkQthToCoords( "Ra90xa90xa"  , 10, 4, -90.0,  180.0, _QTH_HGRID_STEP4/2)
t.checkQthToCoords( "AR09ax09ax"  , 10, 4,  90.0, -180.0, _QTH_HGRID_STEP4/2)
t.checkQthToCoords( "JJ00aa00aa"  , 10, 4,   0.0,    0.0, _QTH_HGRID_STEP4/2)

t.checkQthToCoords( "AA00aa00"    ,  8, 3, -90.0, -180.0, _QTH_HGRID_STEP3/2)
t.checkQthToCoords( "RR99xx99"    ,  8, 3,  90.0,  180.0, _QTH_HGRID_STEP3/2)
t.checkQthToCoords( "Ra90xa90"    ,  8, 3, -90.0,  180.0, _QTH_HGRID_STEP3/2)
t.checkQthToCoords( "AR09ax09"    ,  8, 3,  90.0, -180.0, _QTH_HGRID_STEP3/2)
t.checkQthToCoords( "JJ00aa00"    ,  8, 3,   0.0,    0.0, _QTH_HGRID_STEP3/2)

t.checkQthToCoords( "AA00aa"      ,  6, 2, -90.0, -180.0, _QTH_HGRID_STEP2/2)
t.checkQthToCoords( "RR99xx"      ,  6, 2,  90.0,  180.0, _QTH_HGRID_STEP2/2)
t.checkQthToCoords( "Ra90xa"      ,  6, 2, -90.0,  180.0, _QTH_HGRID_STEP2/2)
t.checkQthToCoords( "AR09ax"      ,  6, 2,  90.0, -180.0, _QTH_HGRID_STEP2/2)
t.checkQthToCoords( "JJ00aa"      ,  6, 2,   0.0,    0.0, _QTH_HGRID_STEP2/2)

t.checkQthToCoords( "AA00"        ,  4, 1, -90.0, -180.0, _QTH_HGRID_STEP1/2)
t.checkQthToCoords( "RR99"        ,  4, 1,  90.0,  180.0, _QTH_HGRID_STEP1/2)
t.checkQthToCoords( "Ra90"        ,  4, 1, -90.0,  180.0, _QTH_HGRID_STEP1/2)
t.checkQthToCoords( "AR09"        ,  4, 1,  90.0, -180.0, _QTH_HGRID_STEP1/2)
t.checkQthToCoords( "JJ00"        ,  4, 1,   0.0,    0.0, _QTH_HGRID_STEP1/2)

t.checkQthToCoords( "AA"          ,  2, 0, -90.0, -180.0, _QTH_HGRID_STEP0/2)
t.checkQthToCoords( "RR"          ,  2, 0,  90.0,  180.0, _QTH_HGRID_STEP0/2)
t.checkQthToCoords( "Ra"          ,  2, 0, -90.0,  180.0, _QTH_HGRID_STEP0/2)
t.checkQthToCoords( "AR"          ,  2, 0,  90.0, -180.0, _QTH_HGRID_STEP0/2)
t.checkQthToCoords( "JJ"          ,  2, 0,   0.0,    0.0, _QTH_HGRID_STEP0/2)

# Check some "known" landmarks
# Saint Etienne, France
t.checkQthToCoords( "JN25EK"      ,  6, 2,  45.43894 ,    4.36844, _QTH_VGRID_STEP2/2)
# Ueda-shi, Nagano-ken, Japan
t.checkQthToCoords( "PM96CJ96RF"  , 10, 4,  36.401   ,  138.247667, _QTH_HGRID_STEP4/2)
# Dunedin city, Otago, New Zealand
t.checkQthToCoords( "RE54GC57EH"  , 10, 4, -45.886167,  170.543167, _QTH_HGRID_STEP4/2)
# Fairbanks, AK, USA
t.checkQthToCoords( "BP64DV54AA"  , 10, 4,  64.891667, -147.708333, _QTH_HGRID_STEP4/2)
# Antartic base Marambio, Antartica
t.checkQthToCoords( "GC15QS42QM"  , 10, 4, -64.2395  ,  -56.6275, _QTH_HGRID_STEP4/2)
# Beringovsky, Chukotka Autonomous Okrug, Russian Federation
t.checkQthToCoords( "RP93QB25NB16", 12, 5,  63.062778,  179.354549, _QTH_HGRID_STEP5/2)

# **** Coords to QTH
t.inCheckCoordsToQth( -175.0, -375.0)
t.inCheckCoordsToQth( -175.0,  375.0)
t.inCheckCoordsToQth(  175.0, -375.0)
t.inCheckCoordsToQth(  175.0,  375.0)
t.inCheckCoordsToQth(  -75.0, -175.0)
t.inCheckCoordsToQth(  -75.0,  175.0)
t.inCheckCoordsToQth(   75.0, -175.0)
t.inCheckCoordsToQth(   75.0,  175.0)
t.inCheckCoordsToQth( -175.0,  -75.0)
t.inCheckCoordsToQth( -175.0,   75.0)
t.inCheckCoordsToQth(  175.0,  -75.0)
t.inCheckCoordsToQth(  175.0,   75.0)
t.inCheckCoordsToQth(  -75.0, -375.0)
t.inCheckCoordsToQth(  -75.0,  375.0)
t.inCheckCoordsToQth(   75.0, -375.0)
t.inCheckCoordsToQth(   75.0,  375.0)

#
t.checkCoordsToQth ( -90.0      , -180.0      , 5, "AA00AA00AA00", 12)
t.checkCoordsToQth (  89.9999999,  179.999999 , 5, "RR99XX99XX99", 12)
t.checkCoordsToQth ( -90.0      ,  179.999999 , 5, "RA90XA90XA90", 12)
t.checkCoordsToQth (  89.9999999, -180.0      , 5, "AR09AX09AX09", 12)
t.checkCoordsToQth (   0.0      ,    0.0      , 5, "JJ00AA00AA00", 12)

t.checkCoordsToQth ( -90.0     , -180.0      , 4, "AA00AA00AA"  , 10)
t.checkCoordsToQth (  89.999999,  179.999999 , 4, "RR99XX99XX"  , 10)
t.checkCoordsToQth ( -90.0     ,  179.999999 , 4, "RA90XA90XA"  , 10)
t.checkCoordsToQth (  89.999999, -180.0      , 4, "AR09AX09AX"  , 10)
t.checkCoordsToQth (   0.0     ,    0.0      , 4, "JJ00AA00AA"  , 10)

t.checkCoordsToQth ( -90.0     , -180.0      , 3, "AA00AA00"    ,  8)
t.checkCoordsToQth (  89.999999,  179.999999 , 3, "RR99XX99"    ,  8)
t.checkCoordsToQth ( -90.0     ,  179.999999 , 3, "RA90XA90"    ,  8)
t.checkCoordsToQth (  89.999999, -180.0      , 3, "AR09AX09"    ,  8)
t.checkCoordsToQth (   0.0     ,    0.0      , 3, "JJ00AA00"    ,  8)

t.checkCoordsToQth ( -90.0     , -180.0      , 2, "AA00AA"      ,  6)
t.checkCoordsToQth (  89.999999,  179.999999 , 2, "RR99XX"      ,  6)
t.checkCoordsToQth ( -90.0     ,  179.999999 , 2, "RA90XA"      ,  6)
t.checkCoordsToQth (  89.999999, -180.0      , 2, "AR09AX"      ,  6)
t.checkCoordsToQth (   0.0     ,    0.0      , 2, "JJ00AA"      ,  6)

t.checkCoordsToQth ( -90.0     , -180.0      , 1, "AA00"        ,  4)
t.checkCoordsToQth (  89.999999,  179.999999 , 1, "RR99"        ,  4)
t.checkCoordsToQth ( -90.0     ,  179.999999 , 1, "RA90"        ,  4)
t.checkCoordsToQth (  89.999999, -180.0      , 1, "AR09"        ,  4)
t.checkCoordsToQth (   0.0     ,    0.0      , 1, "JJ00"        ,  4)

t.checkCoordsToQth ( -90.0     , -180.0      , 0, "AA"          ,  2)
t.checkCoordsToQth (  89.999999,  179.999999 , 0, "RR"          ,  2)
t.checkCoordsToQth ( -90.0     ,  179.999999 , 0, "RA"          ,  2)
t.checkCoordsToQth (  89.999999, -180.0      , 0, "AR"          ,  2)
t.checkCoordsToQth (   0.0     ,    0.0      , 0, "JJ"          ,  2)

# Check back our landmarks
t.checkCoordsToQth (  45.43894 , 4.36844     , 2, "JN25EK"      ,  6)
t.checkCoordsToQth (  36.401   ,   138.247667, 4, "PM96CJ96RF"  , 10)
t.checkCoordsToQth ( -45.886167,   170.543167, 4, "RE54GC57EH"  , 10)
t.checkCoordsToQth (  64.891667,  -147.708333, 4, "BP64DV54AA"  , 10)
t.checkCoordsToQth ( -64.2395  ,   -56.6275  , 4, "GC15QS42QM"  , 10)
t.checkCoordsToQth (  63.062778,   179.354549, 5, "RP93QB25NB16", 12)

# Print summary
print("Tests passed: {0} failed: {1}".format(t.done, t.failed))
