 /*THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * IMACS/emBRICK BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE. */

/* 
 * File:   bBConfigs.h
 * Author: jfecher / wd
 *
 * This file must get the name "bBConfigs.h" and will contain all project specific macros to interface the bB_master.c functions.
 *
 *
 */

#ifndef BBCONFIGS_H
#define	BBCONFIGS_H
#define USE_BRICKBUS	1

extern long BB_Timeout;

#define max(a,b) 	((a) > (b)) ? (a) : (b)
#define min(a,b) 	((a) < (b)) ? (a) : (b)



/*
 * Value:	            SynchGap as transferred to the Modules in us. This value shall be approximately twice as large as the largest possible gap between 2 bytes of a message.
 *						In cases using more than 1 node with different transaction speed, define BB_SYNCHGAP for the slowliest bus. Values for faster busses may then be overwritten by the application.
 *						BB_SYNCHGAP must be in the range 90 .. 50000.
 *
*/
#define					BB_SYNCHGAP			1000L


/*
 * Value:	            Time for 1 byte transfer in �s. Normally this is 8 divided by the Bus frequency, in case of a delay between the bytes this delay must be added. 
 *						In cases using more than 1 node with different speed, define BB_BYTETIME for the slwliest bus. Values for faster busses may then be overwritten by the application.
 *
 *
*/
#define					BB_BYTETIME			bB_getTransferTime()



/*
 * Function:            switches Timing of brickBUS
 * 
 * Notes:               The complete Timing of the brickBUS must be made by the application. Since Timings are different for Init and normal opreation,
 *                      timing can be switched here.
 *                      In case of low bus speed or when different timing is not implemented, this macro can be left empty.
 *
 * Input:               node affected, a : 1 = Init-Timing / 0 = normal Operation Timing
 *
 * Output:              none
 *
*/
#define					SET_BB_TIMING(node,a)             bB_setTransferSpeed(a?75000:75000) //


/*
 * Function:            Closes the SPI-Module, sets Daisy chain to deselect
 *						When this macro is invoked, after BB_SPI_INIT_TIME the macro
 *						INIT_BB_SPI is invoked to start the (Re-) Init of the Bus
 *
 * Input:               node affected
 *
 * Output:              none
 */
#define					CLOSE_BB_SPI(node)			bB_Close()

/*
 * Function:            Beginning of the Bus- and Module Initialization sequence.
 *
 * Notes:               This macro is always preceeded by CLOSE_BB_SPI.
 *						therefore if everything necessary is done by CLOSE_BB_SPI, this macro can be left empty
 *						BB_SLAVE_SELECT is invoked separatly after this macro.
 *
 * Input:               node affected
 *
 * Output:              none
 */
#define					INIT_BB_SPI(node)

/*
 * Function:            Activates the Slave-Select-Line. Slaves will be ready to be initialized.
 *
 * Input:               node affected
 *
 * Output:              none
 */
#define					BB_SLAVE_SELECT(node)	bB_SlaveSelect()

/*
 * Function:            Deactivates the Slave-Select-Line. Slaves will be reset.
 *
 * Input:               node affected
 *
 * Output:              none
 */
#define					BB_SLAVE_DESELECT(node)	bB_SlaveDeSelect()

/*
 * Function:            Triggers the data exchange on the bB
 *
 * Notes:               The Buffers brickBUS[node].bB_Data.txData and brickBUS[node].bB_Data.rxData 
 *                      remain unaffected after this macro is invoked until DATA_XCHANGE_ACTIVE returns 0
 *
 * Input:               the node affected
 *
 * Output:              0 if transmission started correctly
 */
#define					START_DATA_XCHANGE(node)			bB_StartDataXChange()

/*
 * Function:            Triggers the data exchange on the bB and thereafter the Timout as defined in bB_BUS.lengthTimeout.
 *
 * Notes:               The Buffers brickBUS[node].bB_Data.txData and brickBUS[node].bB_Data.rxData 
 *                      remain unaffected after this macro is invoked until DATA_XCHANGE_ACTIVE returns 0.
 *						No new START_DATA_EXCHANGE will be invoked until DATA_BUS_IDLE returns a value
 *
 * Input:               the node affected
 *
 * Output:              0 if transmission started correctly
 */
#define					START_DATA_XCHANGE_AND_TIMEOUT(node)			bB_StartDataXChange()

/*
 * Function:            Returns a value !=0 until data exchange is finished.
 *
 * Notes:               With High speed data transmission, make sure a time gap of at least 120 us is between the last byte of a message and the first byte fo the next message.
 *                      Therefore it might be required to delay the time between the receive of the last byte and a value return of DATA_BUS_IDLE
 *						The Buffers brickBUS[node].bB_Data.txData and brickBUS[node].bB_Data.rxData 
 *                      remain unaffected after this macro is invoked until DATA_XCHANGE_ACTIVE returns 0.
*						No new START_DATA_EXCHANGE will be invoked until DATA_BUS_IDLE returns a value
 *
 * Input:               the node affected
 *
 * Output:              0 : transmission is finished.
 */
#define					DATA_XCHANGE_ACTIVE(node)			0

/*
 * Function:            Returns 0 while either data exchange or timeout are running
 *						see Notes at DATA_XCHANGE_ACTIVE
 *
 * Input:               the node affected
 *
 * Output:              0 : bus is busy
 */
#define					DATA_BUS_IDLE(node)			1

/*
 * Function:            Pause trigger.
 *
 * Note :				It is necessary and sufficient to have one discrete Timer per node. 
 *						More then one running timeout per node will never happen
 *
 * Notes:               time is specified in milliseconds
 *
 * Input:               The length of the Pause in milliseconds
 *
 * Output:              a handle (long value) to retrieve the timer in IsPauseMS.
 *						If the returned handle is 0L, the Pause has already exhausted.
 */
#define					SetPauseMS(zeit)		bB_SetPauseMS(zeit)
//#define					SetPauseMS(zeit)			(BB_Timeout = zeit)

/*
 * Function:            Pause request.
 *
 * Notes:               returns something != 0 if a pause started by SetPauseMS is still active.
 *						If SetPauseMS() does not return before the time has exhausted, shall always return 0
 *						must return 0 if handle is 0L
 *
 * Input:               handle returned by SetPauseMS (long value)
 *
 * Output:              0 : Pause exhausted
 */
#define					IsPauseMS(handle)			bB_IsPauseMS(handle)
//#define					IsPauseMS(handle)			(BB_Timeout > 0)

/*
 * Function:            Test signal high
 *
 * Notes:               If implemented, can be used to set a test output (e.g. a pin for oscilloscope or something simular) to high.
 *						The test output is set high at the beginnning of a normal bB transmission cycle (all bricks of the node) and set low when the cycle ends
 *
 * Input:               the node affected
 *
 * Output:              none
 */
#define					BBTESTHIGH(handle)

/*
 * Function:            Test signal low
 *
 * Notes:               If implemented, can be used to set a test output (e.g. a pin for oscilloscope or something simular) to high.
 *						The test output is set high at the beginnning of a normal bB transmission cycle (all bricks of the node) and set low when the cycle ends
 *
 * Input:               the node affected
 *
 * Output:              none
 */
#define					BBTESTLOW(handle)

/*
 * Function:            After brickBUS was initialized,	set MOSI Buffer according to actual settings
 *
 * Notes:               In a radCASE environment, this macro must be set to bB_UpdateAfterInit() or might be omitted (then bB_UpdateAfterInit() is set by default).
 *						In a non-radCASE environment, this macro must be set, either as empty or as a call to a function void xxx(void) which does the necessary settings
 *
 * Input:               none
 *
 * Output:              none
 */
#define					BBUPDATEAFTERINIT				bB_UpdateAfterInit();

#include <bBDefines.h>

#endif	/* BBCONFIGS_H */

