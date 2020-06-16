/******************************************************************************
* File Name: main.c
*
* Description:This is the source code for the AnyCloud: BLE CTS Server Example
*             for ModusToolbox.
*
* Related Document: See README.md
*
*******************************************************************************/

/*******************************************************************************
* (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
*        Header Files
*******************************************************************************/
#include "app_bt_cfg.h"
#include "wiced_bt_stack.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <string.h>
#include "cybt_platform_trace.h"
#include "wiced_memory.h"
#include "cyhal.h"
#include "stdio.h"
#include "wiced_bt_dev.h"
#include "app_bt_utils.h"
#include "cyabs_rtos.h"
#include "timers.h"
#include "GeneratedSource/cycfg_gatt_db.h"


/******************************************************************************
 *                           Constants and Enumerations
 ******************************************************************************/
#define STRING_BUFFER_SIZE                  (80)
#define DAYS_PER_WEEK                       (7u)

/* Structure tm stores years since 1900 */
#define TM_YEAR_BASE                        (1900u)
/*******************************************************************************
*        Variable Definitions
*******************************************************************************/
static uint16_t                  bt_connection_id = 0;
static TimerHandle_t             ctss_app_msec_timer;
cyhal_rtc_t                      my_rtc;
/*******************************************************************************
*        Function Prototypes
*******************************************************************************/;
static void                   ble_app_init                   (void);
static void                   ctss_scan_result_cback         (wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data );
static void                   ctss_send_notification         (void);
static void                   ctss_app_msec_timer_cb         (TimerHandle_t timer_handle);
static int                    get_day_of_week                (int day, int month, int year);
gatt_db_lookup_table_t*       ctss_get_attribute             (uint16_t handle);

/* GATT Event Callback Functions */
static wiced_bt_gatt_status_t ble_app_write_handler          (wiced_bt_gatt_write_t *p_write_req, uint16_t conn_id);
static wiced_bt_gatt_status_t ble_app_read_handler           (wiced_bt_gatt_read_t *p_read_req, uint16_t conn_id);
static wiced_bt_gatt_status_t ble_app_connect_callback       (wiced_bt_gatt_connection_status_t *p_conn_status);
static wiced_bt_gatt_status_t ble_app_server_callback        (uint16_t conn_id, wiced_bt_gatt_request_type_t type, wiced_bt_gatt_request_data_t *p_data);
static wiced_bt_gatt_status_t ble_app_gatt_event_handler     (wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data);

/* Callback function for Bluetooth stack management type events */
static wiced_bt_dev_status_t  app_bt_management_callback     (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);

/******************************************************************************
 *                          Function Definitions
 ******************************************************************************/
/*
 *  Entry point to the application. Set device configuration and start BT
 *  stack initialization.  The actual application initialization will happen
 *  when stack reports that BT device is ready.
 */
int main()
{
    cy_rslt_t rslt;
    wiced_result_t result;

    /* Initialize the board support package */
    rslt = cybsp_init() ;
    if (CY_RSLT_SUCCESS != rslt)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                        CY_RETARGET_IO_BAUDRATE);

    cybt_platform_config_init(&bt_platform_cfg_settings);
    printf("*************AnyCloud Example*****************\n");
    printf("****** BLE CTS Server Application Start ******\n\n");

    /* Register call back and configuration with stack */
    result = wiced_bt_stack_init (app_bt_management_callback, &wiced_bt_cfg_settings);

    /* Check if stack initialization was successful */
    if( WICED_BT_SUCCESS == result)
    {
        printf("Bluetooth Stack Initialization Successful\n");
    }
    else
    {
        printf("Bluetooth Stack Initialization failed!!\n");
        CY_ASSERT(0);
    }

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler() ;

    /* Should never get here */
    CY_ASSERT(0) ;
}

/**************************************************************************************************
* Function Name: app_bt_management_callback()
***************************************************************************************************
* Summary:
*   This is a Bluetooth stack event handler function to receive management events from
*   the BLE stack and process as per the application.
*
* Parameters:
*   wiced_bt_management_evt_t event             : BLE event code of one byte length
*   wiced_bt_management_evt_data_t *p_event_data: Pointer to BLE management event structures
*
* Return:
*  wiced_result_t: Error code from WICED_RESULT_LIST or BT_RESULT_LIST
*
*************************************************************************************************/
wiced_result_t app_bt_management_callback(wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_result_t result = WICED_BT_SUCCESS;
    wiced_bt_device_address_t bda = { 0 };

    switch (event)
    {
        case BTM_ENABLED_EVT:
            /* Bluetooth Controller and Host Stack Enabled */

            if (WICED_BT_SUCCESS == p_event_data->enabled.status)
            {
                /* Bluetooth is enabled */
                wiced_bt_dev_read_local_addr(bda);
                printf("Local Bluetooth Address: ");
                print_bd_address(bda);

                /* Perform application-specific initialization */
                ble_app_init();
            }

            break;

        case BTM_BLE_SCAN_STATE_CHANGED_EVT:

            if(p_event_data->ble_scan_state_changed == BTM_BLE_SCAN_TYPE_HIGH_DUTY)
            {
                printf("Scan State Change: BTM_BLE_SCAN_TYPE_HIGH_DUTY\n");
            }
            else if(p_event_data->ble_scan_state_changed == BTM_BLE_SCAN_TYPE_LOW_DUTY)
            {
                printf("Scan State Change: BTM_BLE_SCAN_TYPE_LOW_DUTY\n");
            }
            else if(p_event_data->ble_scan_state_changed == BTM_BLE_SCAN_TYPE_NONE)
            {
                printf("Scan stopped\n");
            }
            else
            {
                printf("Invalid scan state\n");
            }
            break;

        default:
            printf("Unhandled Bluetooth Management Event: 0x%x %s\n", event, get_bt_event_name(event));
            break;
    }

    return result;
}

/**************************************************************************************************
* Function Name: ble_app_init()
***************************************************************************************************
* Summary:
*   This function handles application level initialization tasks and is called from the BT
*   management callback once the BLE stack enabled event (BTM_ENABLED_EVT) is triggered
*   This function is executed in the BTM_ENABLED_EVT management callback.
*
* Parameters:
*   None
*
* Return:
*  None
*
*************************************************************************************************/
static void ble_app_init(void)
{
    cy_rslt_t rslt;
    wiced_result_t result;
    wiced_bt_gatt_status_t status;

    /* Initialize RTC */
    rslt = cyhal_rtc_init(&my_rtc);
    if(CY_RSLT_SUCCESS != rslt)
    {
        printf("[Error] : RTC Initialization failed 0x%lX\r\n", rslt);
        CY_ASSERT(0);
    }

    /* Initialize the timer for notification with 2000ms timeout period*/
    ctss_app_msec_timer = xTimerCreate("Ctss_timer", pdMS_TO_TICKS(2000), pdTRUE,
                                        NULL, ctss_app_msec_timer_cb);

    /* Timer init failed. Stop program execution */
    if (NULL == ctss_app_msec_timer)
    {
        printf("[Error] : Timer Initialization failed 0x%lX\r\n", rslt);
        CY_ASSERT(0);
    }

   /* Disable pairing for this application */
    wiced_bt_set_pairable_mode(WICED_FALSE, 0);

    /* Register with BT stack to receive GATT callback */
    status=wiced_bt_gatt_register(ble_app_gatt_event_handler);
    printf("GATT event Handler registration status: %s \n",get_bt_gatt_status_name(status));

    /* Initialize GATT Database */
    status = wiced_bt_gatt_db_init(gatt_database, gatt_database_len, NULL);
    printf("GATT database initialization status: %s \n",get_bt_gatt_status_name(status));

    result = wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, WICED_TRUE, ctss_scan_result_cback);
    if ((WICED_BT_PENDING == result) || (WICED_BT_BUSY == result))
    {
        printf("\r\nScanning.....\n");
    }
    else
    {
        printf("\rError: Starting scan failed. Error code: %d\n", result);
        return;
    }

}

/***************************************************************************************
* Function Name: void ctss_scan_result_cback()
****************************************************************************************
* Summary:
*   This function is registered as a callback to handle the scan results.
*   When the desired device is found, it will try to establish connection with
*   that device.
*
* Parameters:
*   wiced_bt_ble_scan_results_t *p_scan_result: Details of the new device found.
*   uint8_t                     *p_adv_data      : Advertisement data.
*
* Return:
*   None
*
****************************************************************************************/
void ctss_scan_result_cback(wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data )
{
    wiced_result_t         result = WICED_BT_SUCCESS;
    uint8_t                length = 0u;
    uint8_t                *p_data = NULL;
    uint8_t                client_device_name[15] = {'B','L','E',' ','C','T','S',' ','C','l','i','e','n','t','\0'};

    if (p_scan_result)
    {
        p_data = wiced_bt_ble_check_advertising_data(p_adv_data,
                                                    BTM_BLE_ADVERT_TYPE_NAME_COMPLETE,
                                                    &length);

        /* Check if the peer device's name is "BLE CTS Client" */
        if ((length == strlen((const char *)client_device_name)) &&
            (memcmp(p_data, (uint8_t *)client_device_name, length) == 0))
        {
            printf("\nFound the peer device! BD Addr: ");
            print_bd_address(p_scan_result->remote_bd_addr);

            /* Device found. Stop scan. */
            if((result = wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_NONE, WICED_TRUE,
                                           ctss_scan_result_cback))!= WICED_BT_SUCCESS)
            {
                printf("\r\nscan off status %d\n", result);
            }
            else
            {
                printf("Scan completed\n\n");
            }

            printf("Initiating connection\n");
            /* Initiate the connection */
            if(wiced_bt_gatt_le_connect(p_scan_result->remote_bd_addr,
                                        p_scan_result->ble_addr_type,
                                        BLE_CONN_MODE_HIGH_DUTY,
                                        WICED_TRUE)!= WICED_TRUE)
            {
                printf("\rwiced_bt_gatt_connect failed\n");
            }
        }
        else
        {
            printf("BD Addr: ");
            print_bd_address(p_scan_result->remote_bd_addr);
            return;    //Skip - This is not the device we are looking for.
        }
    }
}

/**************************************************************************************************
* Function Name: ble_app_gatt_event_handler()
***************************************************************************************************
* Summary:
*   This function handles GATT events from the BT stack.
*
* Parameters:
*   wiced_bt_gatt_evt_t event                   : BLE GATT event code of one byte length
*   wiced_bt_gatt_event_data_t *p_event_data    : Pointer to BLE GATT event structures
*
* Return:
*  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
static wiced_bt_gatt_status_t ble_app_gatt_event_handler(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;
    wiced_bt_gatt_attribute_request_t *p_attr_req = NULL;

    /* Call the appropriate callback function based on the GATT event type, and pass the relevant event
     * parameters to the callback function */
    switch ( event )
    {
        case GATT_CONNECTION_STATUS_EVT:
            status = ble_app_connect_callback( &p_event_data->connection_status );
            break;

        case GATT_ATTRIBUTE_REQUEST_EVT:
            p_attr_req = &p_event_data->attribute_request;
            status = ble_app_server_callback( p_attr_req->conn_id, p_attr_req->request_type, &p_attr_req->data );
            break;

        default:
            status = WICED_BT_GATT_SUCCESS;
            break;
    }

    return status;
}

/**************************************************************************************************
* Function Name: ble_app_get_value()
***************************************************************************************************
* Summary:
*   This function handles reading of the attribute value from the GATT database and passing the
*   data to the BT stack. The value read from the GATT database is stored in a buffer whose
*   starting address is passed as one of the function parameters
*
* Parameters:
*   uint16_t attr_handle                    : Attribute handle for read operation
*   uint16_t conn_id                        : Connection ID
*   uint8_t *p_val                          : Pointer to the buffer to store read data
*   uint16_t max_len                        : Maximum buffer length available to store the read data
*   uint16_t *p_len                         : Actual length of data copied to the buffer
*
* Return:
*   wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
static wiced_bt_gatt_status_t ble_app_get_value(uint16_t attr_handle, uint16_t conn_id, uint8_t *p_val, uint16_t max_len, uint16_t *p_len)
{
    cy_rslt_t rslt;
    wiced_bool_t isHandleInTable = WICED_FALSE;
    wiced_bt_gatt_status_t res = WICED_BT_GATT_INVALID_HANDLE;
    char buffer[STRING_BUFFER_SIZE];
    struct tm date_time;

    /* Check for a matching handle entry */
    for (int i = 0; i < app_gatt_db_ext_attr_tbl_size; i++)
    {
        if (app_gatt_db_ext_attr_tbl[i].handle == attr_handle)
        {
            /* Detected a matching handle in external lookup table */
            isHandleInTable = WICED_TRUE;

            /* Check if the buffer has space to store the data */
            if (app_gatt_db_ext_attr_tbl[i].cur_len <= max_len)
            {
                /* Update the GATT DB with current RTC data */
                if(attr_handle == HDLC_CTS_CURRENT_TIME_VALUE)
                {
                    rslt = cyhal_rtc_read(&my_rtc, &date_time);
                    if (CY_RSLT_SUCCESS == rslt)
                    {
                        strftime(buffer, sizeof(buffer), "%c", &date_time);
                        printf("\r%s\r\n\n", buffer);
                    }

                    date_time.tm_year += TM_YEAR_BASE;
                    app_cts_current_time[0]= (uint8_t) (date_time.tm_year & 0xFF);
                    app_cts_current_time[1]= (uint8_t) ((date_time.tm_year) >> 8);
                    app_cts_current_time[2] = date_time.tm_mon + 1;
                    app_cts_current_time[3] = date_time.tm_mday;
                    app_cts_current_time[4] = date_time.tm_hour;
                    app_cts_current_time[5] = date_time.tm_min;
                    app_cts_current_time[6] = date_time.tm_sec;
                    app_cts_current_time[7] = get_day_of_week(date_time.tm_mday, date_time.tm_mon, date_time.tm_year);
                    app_cts_current_time[8] = 0;
                    app_cts_current_time[9] = 0;
                }

                /* Value fits within the supplied buffer; copy over the value */
                *p_len = app_gatt_db_ext_attr_tbl[i].cur_len;
                memcpy(p_val, app_gatt_db_ext_attr_tbl[i].p_data, app_gatt_db_ext_attr_tbl[i].cur_len);
                res = WICED_BT_GATT_SUCCESS;
            }
            else
            {
                /* Value to read will not fit within the buffer */
                res = WICED_BT_GATT_INVALID_ATTR_LEN;
            }
            break;
        }
    }

    if (!isHandleInTable)
    {
        /* Add code to read value for handles not contained within generated lookup table.
         * This is a custom logic that depends on the application, and is not used in the
         * current application. If the value for the current handle is successfully read in the
         * below code snippet, then set the result using:
         * res = WICED_BT_GATT_SUCCESS; */
        switch ( attr_handle )
        {
            default:
                /* The read operation was not performed for the indicated handle */
                printf("Read Request to Invalid Handle: 0x%x\n", attr_handle);
                res = WICED_BT_GATT_READ_NOT_PERMIT;
                break;
        }
    }

    return res;
}

/**************************************************************************************************
* Function Name: ble_app_set_value()
***************************************************************************************************
* Summary:
*   This function handles writing to the attribute handle in the GATT database using the
*   data passed from the BT stack. The value to write is stored in a buffer
*   whose starting address is passed as one of the function parameters
*
* Parameters:
*   uint16_t attr_handle                    : Attribute handle for write operation
*   uint16_t conn_id                        : Connection ID
*   uint8_t *p_val                          : Pointer to the buffer that stores the data to be written
*   uint16_t len                            : Length of data to be written
*
* Return:
*   wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
static wiced_bt_gatt_status_t ble_app_set_value(uint16_t attr_handle, uint16_t conn_id, uint8_t *p_val, uint16_t len)
{
    int i = 0;
    wiced_bool_t isHandleInTable = WICED_FALSE;
    wiced_bool_t validLen = WICED_FALSE;
    wiced_bt_gatt_status_t res = WICED_BT_GATT_INVALID_HANDLE;
    gatt_db_lookup_table_t *puAttribute;


    /* Check for a matching handle entry */
    for (i = 0; i < app_gatt_db_ext_attr_tbl_size; i++)
    {
        if (app_gatt_db_ext_attr_tbl[i].handle == attr_handle)
        {
            /* Detected a matching handle in external lookup table */
            isHandleInTable = WICED_TRUE;

            /* Check if the buffer has space to store the data */
            validLen = (app_gatt_db_ext_attr_tbl[i].max_len >= len);

            if (validLen)
            {
                /* Value fits within the supplied buffer; copy over the value */
                app_gatt_db_ext_attr_tbl[i].cur_len = len;
                memcpy(app_gatt_db_ext_attr_tbl[i].p_data, p_val, len);
                res = WICED_BT_GATT_SUCCESS;

                /* Add code for any action required when this attribute is written.
                 * In this case, we update the IAS led based on the IAS alert
                 * level characteristic value */

                switch ( attr_handle )
                {
                    case HDLD_CTS_CURRENT_TIME_CLIENT_CHAR_CONFIG:
                        if (len!= app_cts_current_time_client_char_config_len)
                        {
                            printf("Invalid attribute length\n");
                            return WICED_BT_GATT_INVALID_ATTR_LEN;
                        }

                        app_cts_current_time_client_char_config[0] = p_val[0];
                        app_cts_current_time_client_char_config[1] = p_val[1];

                        /* Update GATT DB */
                        if ((puAttribute = ctss_get_attribute(attr_handle)) != NULL)
                        {
                            memcpy(puAttribute->p_data, (uint8_t *)app_cts_current_time_client_char_config,
                                                                        puAttribute->max_len);
                        }

                        if(app_cts_current_time_client_char_config[0])
                        {
                            printf("\r\nNotifications enabled\n");
                            if(pdPASS != xTimerStart(ctss_app_msec_timer, 0))   
                            {
                                printf("\rMillisecond timer start failed\n");
                            }
                        }
                        else
                        {
                            printf("\rNotifications disabled\n");
                            xTimerStop(ctss_app_msec_timer, 0);
                        }
                        break;

                }           
            }
            else
            {
                /* Value to write does not meet size constraints */
                res = WICED_BT_GATT_INVALID_ATTR_LEN;
            }
            break;
        }
    }

    if (!isHandleInTable)
    {
        /* TODO: Add code to read value for handles not contained within generated lookup table.
         * This is a custom logic that depends on the application, and is not used in the
         * current application. If the value for the current handle is successfully written in the
         * below code snippet, then set the result using:
         * res = WICED_BT_GATT_SUCCESS; */
        switch ( attr_handle )
        {
            default:
                /* The write operation was not performed for the indicated handle */
                printf("Write Request to Invalid Handle: 0x%x\n", attr_handle);
                res = WICED_BT_GATT_WRITE_NOT_PERMIT;
                break;
        }
    }

    return res;
}

/**************************************************************************************************
* Function Name: ble_app_write_handler()
***************************************************************************************************
* Summary:
*   This function handles Write Requests received from the client device
*
* Parameters:
*   wiced_bt_gatt_write_t *p_write_req          : Pointer that contains details of Write Request
*                                                 including the attribute handle
*   uint16_t conn_id                            : Connection ID
*
* Return:
*  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
static wiced_bt_gatt_status_t ble_app_write_handler(wiced_bt_gatt_write_t *p_write_req, uint16_t conn_id)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_INVALID_HANDLE;

    /* Attempt to perform the Write Request */
    status = ble_app_set_value(p_write_req->handle, conn_id, p_write_req->p_val, p_write_req->val_len);

    return status;
}

/**************************************************************************************************
* Function Name: ble_app_read_handler()
***************************************************************************************************
* Summary:
*   This function handles Read Requests received from the client device
*
* Parameters:
*   wiced_bt_gatt_write_t *p_read_req           : Pointer that contains details of Read Request
*                                                 including the attribute handle
*   uint16_t conn_id                            : Connection ID
*
* Return:
*  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
static wiced_bt_gatt_status_t ble_app_read_handler( wiced_bt_gatt_read_t *p_read_req, uint16_t conn_id )
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_INVALID_HANDLE;

    /* Attempt to perform the Read Request */
    status = ble_app_get_value(p_read_req->handle, conn_id, p_read_req->p_val, *p_read_req->p_val_len, p_read_req->p_val_len);

    return status;
}

/**************************************************************************************************
* Function Name: ble_app_connect_callback()
***************************************************************************************************
* Summary:
*   This callback function handles connection status changes.
*
* Parameters:
*   wiced_bt_gatt_connection_status_t *p_conn_status  : Pointer to data that has connection details
*
* Return:
*  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
static wiced_bt_gatt_status_t ble_app_connect_callback(wiced_bt_gatt_connection_status_t *p_conn_status)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;
    wiced_result_t result;

    if ( NULL != p_conn_status )
    {
        if ( p_conn_status->connected )
        {
            /* Device has connected */
            printf("Connected : BD Addr: " );
            print_bd_address(p_conn_status->bd_addr);
            printf("Connection ID '%d'\n", p_conn_status->conn_id );

            /* Store the connection ID */
            bt_connection_id = p_conn_status->conn_id;

        }
        else
        {
            /* Device has disconnected */
            printf("\nDisconnected : BD Addr: " );
            print_bd_address(p_conn_status->bd_addr);
            printf("Connection ID '%d', Reason '%s'\n", p_conn_status->conn_id, get_bt_gatt_disconn_reason_name(p_conn_status->reason) );

            /* Set the connection id to zero to indicate disconnected state */
            bt_connection_id = 0;

            /*restart the scan*/
            result = wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, WICED_TRUE, ctss_scan_result_cback);
            if(WICED_BT_PENDING != result)
            {
                printf("Cannot restart scanning. Error: %d \n", result);
            }
            else
            {
                printf("\r\nScanning.....\n");
            }
            
        }

        status = WICED_BT_GATT_SUCCESS;
    }

    return status;
}

/**************************************************************************************************
* Function Name: ble_app_server_callback()
***************************************************************************************************
* Summary:
*   This function handles GATT server events from the BT stack.
*
* Parameters:
*   uint16_t conn_id                            : Connection ID
*   wiced_bt_gatt_request_type_t type           : Type of GATT server event
*   wiced_bt_gatt_request_data_t *p_data        : Pointer to GATT server event data
*
* Return:
*  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
static wiced_bt_gatt_status_t ble_app_server_callback(uint16_t conn_id, wiced_bt_gatt_request_type_t type, wiced_bt_gatt_request_data_t *p_data)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;

    switch ( type )
    {
        case GATTS_REQ_TYPE_READ:
            /* Attribute read request */
            status = ble_app_read_handler( &p_data->read_req, conn_id );
            break;

        case GATTS_REQ_TYPE_WRITE:
            /* Attribute write request */
            status = ble_app_write_handler( &p_data->write_req, conn_id );
            break;
    }

    return status;
}

/*********************************************************************
* Function Name: static void ctss_send_notification()
**********************************************************************
* Summary:
*   Send GATT notification every millisecond.
*
* Parameters:
*   None
*
* Return:
*   None
*
**********************************************************************/

static void ctss_send_notification(void)
{
    cy_rslt_t rslt;
    struct tm date_time;
    char buffer[STRING_BUFFER_SIZE];
    wiced_bt_gatt_status_t status = WICED_BT_GATT_SUCCESS;

    rslt = cyhal_rtc_read(&my_rtc, &date_time);
    if (CY_RSLT_SUCCESS == rslt)
    {
        strftime(buffer, sizeof(buffer), "%c", &date_time);
        printf("\r%s\r\n\n", buffer);
    }

    date_time.tm_year += TM_YEAR_BASE;
    app_cts_current_time[0]= (uint8_t) (date_time.tm_year & 0xFF);
    app_cts_current_time[1]= (uint8_t) ((date_time.tm_year) >> 8);
    app_cts_current_time[2] = date_time.tm_mon + 1;
    app_cts_current_time[3] = date_time.tm_mday;
    app_cts_current_time[4] = date_time.tm_hour;
    app_cts_current_time[5] = date_time.tm_min;
    app_cts_current_time[6] = date_time.tm_sec;
    app_cts_current_time[7] = get_day_of_week(date_time.tm_mday, date_time.tm_mon, date_time.tm_year);
    app_cts_current_time[8] = 0;
    app_cts_current_time[9] = 0;

    status = wiced_bt_gatt_send_notification(bt_connection_id,
                                             HDLC_CTS_CURRENT_TIME_VALUE,
                                             app_cts_current_time_len,
                                             app_cts_current_time);

    if (WICED_BT_GATT_SUCCESS != status)
    {
        printf("Send notification failed\n");
    }

}

/****************************************************************************
* Function Name: void ctss_app_msec_timer_cb()
*****************************************************************************
* Summary:
*   Millisecond timer callback.
*   Send GATT notifications if enabled by the GATT Client.
*
* Parameters:
*   timer_handle :  Software timers are reference variable.
*
* Return:
*   None
*
****************************************************************************/

static void ctss_app_msec_timer_cb(TimerHandle_t timer_handle)
{
    /* Send GATT Notification */
    if (app_cts_current_time_client_char_config[0])
    {
        /* sending GATT notification. */
        ctss_send_notification();
    }
}

/*************************************************************************************
* Function Name: gatt_db_lookup_table_t* ctss_get_attribute()
**************************************************************************************
* Summary: Find attribute description by handle
*
* Parameters:
*   uint16_t handle: Attribute handle
*
* Return:
*   gatt_db_lookup_table_t*: Pointer to BLE GATT attribute handle
*
*************************************************************************************/
gatt_db_lookup_table_t* ctss_get_attribute(uint16_t handle)
{
    for (uint16_t i = 0; i < app_gatt_db_ext_attr_tbl_size; i++)
    {
        if (app_gatt_db_ext_attr_tbl[i].handle == handle)
        {
            return (&app_gatt_db_ext_attr_tbl[i]);
        }
    }

    printf("Attribute not found:%x\n", handle);

    return NULL;
}

/*******************************************************************************
* Function Name: get_day_of_week()
********************************************************************************
* Summary:
*  Returns a day of the week for a year, month, and day of month that are passed
*  through parameters. Zeller's congruence is used to calculate the day of
*  the week. See https://en.wikipedia.org/wiki/Zeller%27s_congruence for more
*  details.
*
*  Note: In this algorithm January and February are counted as months 13 and 14
*  of the previous year.
*
* Parameter:
*  int day          : The day of the month, Valid range 1..31.
*  int month        : The month of the year
*  int year         : The year value. Valid range non-zero value.
*
* Return:
*  Returns a day of the week (1 = Monday, 2 = Tuesday, ., 7 = Sunnday)
*
*******************************************************************************/
static int get_day_of_week(int day, int month, int year)
{
    int ret;

    /*Adding 1 to start the index of month from 1. (1-Jan, 2-Feb, ..., 12-Dec)*/
    month+=1;

    if (month == 1) 
    { 
        month = 13; 
        year--; 
    } 
    if (month == 2) 
    { 
        month = 14; 
        year--; 
    } 
    
    int i = 13*(month+1)/5;
    int k = year % 100; 
    int j = year / 100;

    ret = (((day + i + k + k/4 + j/4 + 5*j) + 5) % DAYS_PER_WEEK) + 1;

    return ret;
}

/* [] END OF FILE */