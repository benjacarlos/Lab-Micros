/***************************************************************************/ /**
  @file     equalizer.c
  @brief    Music graphic equalizer using filters
  @author   Grupo 5
  @commentaries For more information use the following link https://www.keil.com/pack/doc/CMSIS/DSP/html/group__BiquadCascadeDF1.html#ga5563b156af44d1be2a7548626988bf4e
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "equalizer.h"

/*******************************************************************************
 *******************************************************************************
 	 	 	 	 CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 *******************************************************************************
 ******************************************************************************/

// Filters data
#define IIR_STAGES       			3     				// Number of stages per filter
#define IIR_COEFFS       			5     				// Number of coefficients per stages
#define IIR_STATE_VARS   			4     				// Number of state vars
#define BLOCKSIZE 					32					// Block size for the underlying processing is the number of sample that the filter will process
#define IIR_FRAME_SIZE				4096U				// Length of the overall data FRAME
#define NUMBER_OF_BLOCKS	(IIR_FRAME_SIZE/BLOCKSIZE)	// Total number of (block to run)/(times the filter is run)
#define COEF_PER_FILTER		(IIR_COEFFS*IIR_STAGES)

/*******************************************************************************
 *******************************************************************************
                        GLOBAL VARIABLES
 *******************************************************************************
 ******************************************************************************/

static arm_biquad_casd_df1_inst_q31 S1;
static arm_biquad_casd_df1_inst_q31 S2;
static arm_biquad_casd_df1_inst_q31 S3;
static arm_biquad_casd_df1_inst_q31 S4;
static arm_biquad_casd_df1_inst_q31 S5;
static arm_biquad_casd_df1_inst_q31 S6;
static arm_biquad_casd_df1_inst_q31 S7;
static arm_biquad_casd_df1_inst_q31 S8;

static arm_biquad_casd_df1_inst_q31 * Filters [NUMBER_OF_FILTERS] = {&S1, &S2, &S3, &S4, &S5, &S6, &S7, &S8};

/************************ Entire coefficient table ***********************
** Filter band used are calculated with the given frequency range defined
** in spectrometer.h.
** There are 5*3 = 15 coefficients per 6th order Biquad cascade filter.
** The first 15 coefficients correspond to the gain level 7 setting of band 1;
** the next coefficients correspond to the gain level 6 setting of band 1;
** and so on.  There are 15*10 = 150 coefficients in total
** for band 1 (gains = lvl1, lvl2, lvl3, lvl4, lvl5, lvl6, lvl7, lvl8).
** After this come the 150 coefficients for band 2 and so on.
**
** The coefficients are in Q31 format.
*************************************************************************/
static const q31_t coeffTable[NUMBER_OF_FILTERS * IIR_COEFFS * GAINS_LEVELS] = {
	/*1° band */
	19330301, 38213611, 18893482, -2107865319, -1035573325, 2147483647, -1695755833, 895673562, 221697944, -374930837, 2147483647, 1453406393, -629800832, -964297108, 66798992,
	23015573, 45523570, 22519012, -2106011293, -1033861229, 2147483647, -1806913806, 919044054, 154412987, -373172693, 2147483647, 1408645877, -676540868, -988896898, 41111284,
	27394290, 54212442, 26830075, -2104081867, -1032087259, 2147483647, -1914901680, 943518454, 86812141, -372562797, 2147483647, 1364556606, -722579972, -1013541903, 15376361,
	32593583, 64533501, 31952819, -2102075789, -1030251375, 2147483647, -2019634882, 968965901, 19044404, -373115609, 2147483647, 1321173148, -767882049, -1038203841, -10376243,
	38763054, 76784845, 38035743, -2099992094, -1028353920, 2147483647, -2121049166, 995256958, -48740039, -374838096, 2147483647, 1278527378, -812413815, -1062854352, -36116914,
	47615244, 94362311, 46762651, -2097830166, -1026395683, 2078178769, -2147483648, 989273561, -116391259, -377729636, 2147483647, 1236648425, -856144850, -1087465126, -61816092,
	58985685, 116945967, 57977835, -2095589800, -1024377965, 1993156300, -2147483648, 974417854, -183761012, -381782017, 2147483647, 1195562653, -899047627, -1112008040, -87444409,
	72806260, 144405863, 71619300, -2093271274, -1022302653, 1917523546, -2147483648, 962511109, -250704109, -386979552, 2147483647, 1155293646, -941097518, -1136455279, -112972818,
	/*2° band */
	5068139, 9409251, 4429119, -1895888714, -868300629, 1188040446, -2147483648, 994640370, 1808550493, -812395061, 2147483647, 81533687, -1558655098, -60736837, 635106153,
	6532590, 12173159, 5744027, -1883366834, -860065560, 1181934208, -2147483648, 997521274, 1791545732, -802435503, 2147483647, 78217065, -1582607433, -63088208, 618124774,
	8414102, 15734546, 7441953, -1870264315, -851625147, 1176181639, -2147483648, 1000344486, 1773751131, -792279384, 2147483647, 75015634, -1605727862, -65505410, 600667974,
	10829036, 20318253, 9631786, -1856562409, -842989755, 1170760135, -2147483648, 1003105381, 1755142062, -781944761, 2147483647, 71926990, -1628033755, -67988347, 582736435,
	13925330, 26210652, 12452431, -1842243086, -834172111, 1165648683, -2147483648, 1005800257, 1735695029, -771452738, 2147483647, 68948644, -1649543084, -70536766, 564331995,
	17890751, 33775847, 16080758, -1827289284, -825187521, 1160827742, -2147483648, 1008426225, 1715387991, -760827686, 2147483647, 66078038, -1670274333, -73150244, 545457706,
	22963212, 43475940, 20741560, -1811685175, -816054093, 1156279131, -2147483648, 1010981104, 1694200729, -750097453, 2147483647, 63312554, -1690246395, -75828182, 526117890,
	29443624, 55896298, 26719988, -1795416458, -806792965, 1151985926, -2147483648, 1013463333, 1672115237, -739293553, 2147483647, 60649532, -1709478488, -78569802, 506318177,
	/*3° band */
	3920451, -7626000, 3716725, 2082774195, -1012723674, 1737214841, 2147483647, 944735640, -1245342385, -556375754, 2147483647, -968596710, -1007360770, 519186609, 462613865,
	5023956, -9785308, 4774391, 2079646348, -1009957137, 1688108442, 2147483647, 939936890, -1201947437, -542870067, 2147483647, -934852900, -1047080180, 537187209, 441425586,
	6424547, -12528902, 6119533, 2076377429, -1007084092, 1643525147, 2147483647, 936500918, -1156941978, -529621623, 2147483647, -901889075, -1085881479, 555546268, 419815372,
	8198770, -16007962, 7826823, 2072962819, -1004103075, 1602940642, 2147483647, 934225373, -1110329914, -516692695, 2147483647, -869715105, -1123753051, 574252856, 397796085,
	10441942, -20410872, 9989365, 2069398037, -1001013022, 1565905899, 2147483647, 932937909, -1062123201, -504148096, 2147483647, -838338616, -1160685917, 593294813, 375382040,
	13272492, -25971842, 12722986, 2065678807, -997813345, 1532034187, 2147483647, 932491081, -1012342404, -492054779, 2147483647, -807765076, -1196673643, 612658752, 352588993,
	16837227, -32981387, 16171442, 2061801118, -994504015, 1500990680, 2147483647, 932758266, -961017192, -480481358, 2147483647, -777997875, -1231712237, 632330079, 329434124,
	21317718, -41799025, 20512734, 2057761306, -991085651, 1472484092, 2147483647, 933630358, -908186755, -469497562, 2147483647, -749038421, -1265800041, 652293015, 305936005,
	/*4° band */
	3019436, -5990031, 2971387, 2128422703, -1055021161, 2147483647, -292321224, 733655415, 281403059, -374824546, 2062121550, -2147483648, 117135621, 1168606816, -112155348,
	3359985, -6668101, 3308918, 2127520679, -1054152817, 2147483647, -155940093, 728968685, 348163713, -380434243, 2109909591, -2147483648, 69347580, 1193640894, -137559822,
	3758863, -7462354, 3704307, 2126580017, -1053249113, 2147483647, -19201924, 726523536, 414166083, -387077901, 2147483647, -2134994432, 19099520, 1218521622, -162808678,
	4280330, -8500500, 4221015, 2125599705, -1052309345, 2147483647, 117585736, 726327741, 479280734, -394717777, 2147483647, -2084240380, -32405473, 1243222067, -187874583,
	4874669, -9683975, 4810181, 2124578816, -1051332918, 2147483647, 254114196, 728374126, 543386660, -403310738, 2147483647, -2033523048, -83873204, 1267716095, -212731017,
	5552277, -11033535, 5482164, 2123516524, -1050319369, 2147483647, 390076698, 732640643, 606372130, -412808892, 2147483647, -1982900631, -135244615, 1291978477, -237352378,
	6325075, -12572980, 6248844, 2122412133, -1049268394, 2147483647, 525171448, 739090663, 668135342, -423160254, 2147483647, -1932430889, -186461092, 1315984988, -261714082,
	7206739, -14329616, 7123849, 2121265106, -1048179879, 2147483647, 659104556, 747673510, 728584907, -434309439, 2147483647, -1882170885, -237464728, 1339712490, -285792650,
	/*5° band */
	365919, -728502, 362614, 2136742693, -1063110195, 2147483647, -1797907653, 920962354, 1006418976, -486998661, 1626214036, -2147483648, 531387782, 1462972707, -396123884,
	391717, -780025, 388338, 2136236689, -1062615034, 2147483647, -1685600756, 895981208, 1057661769, -500901753, 1653113769, -2147483648, 504488049, 1484918798, -418173376,
	419191, -834900, 415738, 2135709268, -1062099515, 2147483647, -1570193247, 872190458, 1107215538, -515153520, 1681335347, -2147483648, 476266470, 1506458755, -439814823,
	448476, -893397, 444950, 2135159906, -1061563211, 2147483647, -1451807987, 849713676, 1155070147, -529695266, 1710943726, -2147483648, 446658091, 1527579668, -461035249,
	479723, -955818, 476122, 2134588127, -1061005759, 2147483647, -1330590175, 828671975, 1201224075, -544470364, 1742007053, -2147483648, 415594764, 1548270065, -481823132,
	513098, -1022493, 509422, 2133993523, -1060426865, 2147483647, -1206707291, 809182585, 1245683714, -559424595, 1774596823, -2147483648, 383004994, 1568519907, -502168384,
	548784, -1093790, 545032, 2133375765, -1059826327, 2147483647, -1080348719, 791357398, 1288462633, -574506413, 1808788046, -2147483648, 348813771, 1588320571, -522062342,
	586985, -1170116, 583156, 2132734617, -1059204050, 2147483647, -951725003, 775301500, 1329580824, -589667148, 1844659416, -2147483648, 312942401, 1607664822, -541497735,
	/*6° band */
	613038, -1223372, 610346, 2142288203, -1068572162, 1493946757, -2147483648, 934572052, 1601898609, -699593178, 1339949701, -2147483648, 809909751, 1752553160, -680750220,
	621598, -1240578, 618991, 2142044215, -1068330734, 1524890694, -2147483648, 933694132, 1629116074, -713191105, 1352972711, -2147483648, 796886741, 1767704903, -695918725,
	629252, -1255975, 626733, 2141789986, -1068079317, 1558683131, -2147483648, 933456111, 1655063759, -726533658, 1366635668, -2147483648, 783223784, 1782392863, -710622936,
	635937, -1269433, 633506, 2141525280, -1067817695, 1595669890, -2147483648, 933973037, 1679791195, -739606057, 1380970019, -2147483648, 768889433, 1796621886, -724867701,
	641581, -1280814, 639241, 2141249883, -1067545683, 1636251906, -2147483648, 935379082, 1703347734, -752395937, 1396008756, -2147483648, 753850696, 1810397542, -738658596,
	646112, -1289966, 643863, 2140963616, -1067263129, 1680896562, -2147483648, 937831376, 1725782262, -764893177, 1411786492, -2147483648, 738072959, 1823726060, -752001860,
	649447, -1296729, 647290, 2140666340, -1066969924, 1730151977, -2147483648, 941514805, 1747142960, -777089726, 1428339542, -2147483648, 721519909, 1836614267, -764904326,
	651499, -1300926, 649434, 2140357968, -1066666013, 1784665146, -2147483648, 946648104, 1767477093, -788979433, 1445706005, -2147483648, 704153446, 1849069528, -777373366,
	/*7° band */
	82111, -164058, 81947, 2145125712, -1071389220, 1222171713, -2147483648, 980495068, 1912218921, -879025600, 1194380255, -2147483648, 953593903, 1948545284, -875248530,
	82785, -165411, 82627, 2145015161, -1071279199, 1231166589, -2147483648, 977082998, 1924044511, -887376408, 1200296343, -2147483648, 947677815, 1957017757, -883722938,
	83416, -166680, 83264, 2144899991, -1071164611, 1240777235, -2147483648, 973634055, 1935282310, -895432648, 1206503146, -2147483648, 941471012, 1965162263, -891869304,
	84005, -167863, 83859, 2144780098, -1071045357, 1251054944, -2147483648, 970158820, 1945962408, -903200856, 1213014950, -2147483648, 934959208, 1972988648, -899697477,
	84550, -168960, 84410, 2144655389, -1070921349, 1262056203, -2147483648, 966669706, 1956113486, -910687893, 1219846742, -2147483648, 928127416, 1980506708, -907217254,
	85052, -169969, 84917, 2144525788, -1070792517, 1273843326, -2147483648, 963181197, 1965762855, -917900884, 1227014246, -2147483648, 920959912, 1987726164, -914438359,
	85509, -170888, 85380, 2144391237, -1070658808, 1286485200, -2147483648, 959710130, 1974936496, -924847145, 1234533960, -2147483648, 913440198, 1994656637, -921370416,
	85920, -171715, 85796, 2144251701, -1070520196, 1300058129, -2147483648, 956276008, 1983659098, -931534136, 1242423190, -2147483648, 905550968, 2001307637, -928022934,
	/*8° band */
	90827, -181554, 90727, 2146195358, -1072455128, 1147380092, -2147483648, 1016446616, 2022982690, -961965190, 1139623017, -2147483648, 1008007197, 2034083817, -960480820,
	91264, -182433, 91169, 2146134995, -1072394924, 1151497424, -2147483648, 1013966449, 2029156409, -967020106, 1142856800, -2147483648, 1004773414, 2039121061, -965518408,
	91678, -183264, 91586, 2146072114, -1072332217, 1155861694, -2147483648, 1011405678, 2035027288, -971870416, 1146249490, -2147483648, 1001380724, 2043945648, -970343324,
	92068, -184049, 91981, 2146006660, -1072266953, 1160490210, -2147483648, 1008764050, 2040611101, -976523465, 1149808896, -2147483648, 997821318, 2048565560, -974963551,
	92436, -184787, 92352, 2145938582, -1072199085, 1165401698, -2147483648, 1006041671, 2045922788, -980986467, 1153543211, -2147483648, 994087004, 2052988565, -979386859,
	92780, -185480, 92700, 2145867839, -1072128572, 1170616450, -2147483648, 1003239061, 2050976502, -985266488, 1157461028, -2147483648, 990169187, 2057222221, -983620803,
	93102, -186128, 93025, 2145794401, -1072055387, 1176156469, -2147483648, 1000357220, 2055785639, -989370434, 1161571366, -2147483648, 986058849, 2061273866, -987672724,
	93402, -186730, 93328, 2145718251, -1071979513, 1182045652, -2147483648, 997397691, 2060362882, -993305040, 1165883685, -2147483648, 981746530, 2065150623, -991549746,
};

// Gains table
static uint32_t bandsGain[NUMBER_OF_FILTERS]= {DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, DEFAULT_GAIN, };

// Table of filters state variables used by ARM for filtering with DSP module.
static q31_t stateVars [NUMBER_OF_FILTERS][IIR_STATE_VARS * IIR_STAGES];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void equalizer_init(void)
{
    /* Initialize the state and coefficient buffers for all Biquad cascade IIR sections*/
	arm_biquad_cascade_df1_init_q31(&S1, IIR_STAGES,
	          (q31_t *) &coeffTable[COEF_PER_FILTER*GAINS_LEVELS*0 + COEF_PER_FILTER*DEFAULT_GAIN],
	          &stateVars[0][0], 1);

	arm_biquad_cascade_df1_init_q31(&S1, IIR_STAGES,
	          (q31_t *) &coeffTable[COEF_PER_FILTER*GAINS_LEVELS*1 + COEF_PER_FILTER*DEFAULT_GAIN],
	          &stateVars[1][0], 1);

	arm_biquad_cascade_df1_init_q31(&S1, IIR_STAGES,
	          (q31_t *) &coeffTable[COEF_PER_FILTER*GAINS_LEVELS*2 + COEF_PER_FILTER*DEFAULT_GAIN],
	          &stateVars[2][0], 1);

	arm_biquad_cascade_df1_init_q31(&S1, IIR_STAGES,
	          (q31_t *) &coeffTable[COEF_PER_FILTER*GAINS_LEVELS*3 + COEF_PER_FILTER*DEFAULT_GAIN],
	          &stateVars[3][0], 1);

	arm_biquad_cascade_df1_init_q31(&S1, IIR_STAGES,
	          (q31_t *) &coeffTable[COEF_PER_FILTER*GAINS_LEVELS*4 + COEF_PER_FILTER*DEFAULT_GAIN],
	          &stateVars[4][0], 1);

	arm_biquad_cascade_df1_init_q31(&S1, IIR_STAGES,
	          (q31_t *) &coeffTable[COEF_PER_FILTER*GAINS_LEVELS*5 + COEF_PER_FILTER*DEFAULT_GAIN],
	          &stateVars[5][0], 1);

	arm_biquad_cascade_df1_init_q31(&S1, IIR_STAGES,
	          (q31_t *) &coeffTable[COEF_PER_FILTER*GAINS_LEVELS*6 + COEF_PER_FILTER*DEFAULT_GAIN],
	          &stateVars[6][0], 1);

	arm_biquad_cascade_df1_init_q31(&S1, IIR_STAGES,
	          (q31_t *) &coeffTable[COEF_PER_FILTER*GAINS_LEVELS*7 + COEF_PER_FILTER*DEFAULT_GAIN],
	          &stateVars[7][0], 1);
}

/**
 * @brief Applies the filter to the data in input and stores the result in output
 * @param input  pointer to an array of size FRAME_SIZE with input data
 * @param output pointer to an array of size FRAME_SIZE that will be overwritten
 */
void equalize_data(float32_t* input, float32_t * output)
{
	q31_t inQ31[BLOCKSIZE];
	q31_t outQ31[BLOCKSIZE];
	for(int i=0; i < NUMBER_OF_BLOCKS; i++)
	  {
		/* --------------- Convert block of input data from float to Q31 --------------- */

	    arm_float_to_q31(input + (i*BLOCKSIZE), inQ31, BLOCKSIZE);

	    /* --------------- Scale down by 1/8.   --------------- */
	    // This provides additional head room so that the graphic EQ can apply gain.

	    arm_scale_q31(inQ31, 0x7FFFFFFF, -3, inQ31, BLOCKSIZE);

	    /* --- Call the Q31 Biquad Cascade DF1 process function for all bands --- */

	    arm_biquad_cascade_df1_fast_q31(&S1, outQ31, outQ31, BLOCKSIZE);
	    arm_biquad_cascade_df1_fast_q31(&S2, outQ31, outQ31, BLOCKSIZE);
	    arm_biquad_cascade_df1_fast_q31(&S3, outQ31, outQ31, BLOCKSIZE);
	    arm_biquad_cascade_df1_fast_q31(&S4, outQ31, outQ31, BLOCKSIZE);
	    arm_biquad_cascade_df1_fast_q31(&S5, outQ31, outQ31, BLOCKSIZE);
	    arm_biquad_cascade_df1_fast_q31(&S6, outQ31, outQ31, BLOCKSIZE);
	    arm_biquad_cascade_df1_fast_q31(&S7, outQ31, outQ31, BLOCKSIZE);
	    arm_biquad_cascade_df1_fast_q31(&S8, outQ31, outQ31, BLOCKSIZE);

	    /* --------------- Convert Q31 result back to float --------------- */

	    arm_q31_to_float(outQ31, output + (i * BLOCKSIZE), BLOCKSIZE);

	    /* ----------------------------- Scale back up ----------------------------- */

	    arm_scale_f32(output + (i * BLOCKSIZE), 8.0f, output + (i * BLOCKSIZE), BLOCKSIZE);
	  };
}

/**
 * @brief Sets  equalizer filter gains.
 * @param gain  gain level of filter (between GAIN_LV1 and GAIN_LV8)
 * @param band selected band in which to change gain (between 1 and NUMBER_OF_FILTERS)
 */
void equalizer_set_band_gain (uint8_t band, gains_id_t gain)
{
	if (band < NUMBER_OF_FILTERS)
	{
		if (gain < GAINS_LEVELS )
		{
			bandsGain[band-1] = gain;
			// Initialize filter with new gain
			arm_biquad_cascade_df1_init_q31(Filters[band-1], IIR_STAGES,
				          (q31_t *) &coeffTable[COEF_PER_FILTER*GAINS_LEVELS*(band-1) + COEF_PER_FILTER*gain],
				          &biquadStateBandQ31[band-1][0], 1);
		}
	}
}

/**
 * @brief Sets  equalizer filter gains.
 * @param band selected band in which to change gain (between 1 and NUMBER_OF_FILTERS)
 */
uint32_t equalizer_get_band_gain (uint8_t band)
{
	uint32_t gain;
	// If band is not valid value then we get gain of first filter
	if (band > NUMBER_OF_FILTERS)
		gain = bandsGain[0];
	else
		gain = bandsGain[band-1];
	return gain;
}
