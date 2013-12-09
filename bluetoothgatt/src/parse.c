#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/slog.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include "parse.h"

/**
 * Constants
 */
#define BUFSIZE    1024
#define LINE_WIDTH 16
#define FLOAT_MAX_MANTISSA  0x01000000 /* 2^24 */
#define SFLOAT_MAX_MANTISSA  0x1000 /* 2^24 */

/**
 * Prototypes for parsing and utility routines.
 */
static int parse_time(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_reference_time_information(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_local_time_information(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_appearance(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_current_time(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_blood_pressure_measurement(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_heartrate_measurement(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_glucose_measurement(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_glucose_measurement_context(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_record_access_control_point(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_temperature(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_weight(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_manufacturer_name_string(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_model_number_string(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_serial_number_string(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_software_revision_string(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_firmware_revision_string(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_system_id(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_pnp_id(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_alert_level(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_tx_power(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_battery_level(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_scan_refresh(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_blood_pressure_feature(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_alert_category_id_bit_mask(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_alert_status(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_time_with_dst(const uint8_t *in, int in_len, char *out, int out_len);

static int parse_ti_sensor_humidity(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_ti_sensor_temperature(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_ti_sensor_gyroscope(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_ti_sensor_accelerometer(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_ti_sensor_accelerometer_period(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_ti_sensor_magnetometer(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_ti_sensor_magnetometer_period(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_ti_sensor_keypressed(const uint8_t *in, int in_len, char *out, int out_len);

static float convert_float(uint32_t value);
static float convert_sfloat(uint16_t value);
static int dump_binary(const uint8_t *in, int in_len, char *out, int out_len);
static char *strip_uuid_128(const char *uuid);

typedef int (*parse_api)( const uint8_t *in, int in_len, char *out, int out_len );

typedef struct  {
    int uuid;
    parse_api parser;
    char *name;
} parser_16bit_entry_t;

parser_16bit_entry_t parser_16bit_list[] = {
    { 0x2A43, NULL,        "ALERT_CATEGORY_ID" },
    { 0x2A42, NULL,        "ALERT_CATEGORY_ID_BIT_MASK" },
    { 0x2A06, parse_alert_level,        "ALERT_LEVEL" },
    { 0x2A44, NULL,        "ALERT_NOTIFICATION_CONTROL_POINT" },
    { 0x2A3F, NULL,        "ALERT_STATUS" },
    { 0x2A01, parse_appearance,    "GAP_APPEARANCE" },
    { 0x2A19, parse_battery_level,        "BATTERY_LEVEL" },
    { 0x2A49, parse_blood_pressure_feature,        "BLOOD_PRESSURE_FEATURE" },
    { 0x2A35, parse_blood_pressure_measurement,    "BLOOD_PRESSURE_MEASUREMENT" },
    { 0x2A38, NULL,        "BODY_SENSOR_LOCATION" },
    { 0x2A22, NULL,        "BOOT_KEYBOARD_INPUT_REPORT" },
    { 0x2A32, NULL,        "BOOT_KEYBOARD_out_REPORT" },
    { 0x2A33, NULL,        "BOOT_MOUSE_INPUT_REPORT" },
    { 0x2A2B, parse_current_time,        "CURRENT_TIME" },
    { 0x2A08, NULL,        "DATE_TIME" },
    { 0x2A0A, NULL,        "DAY_DATE_TIME" },
    { 0x2A09, NULL,        "DAY_OF_WEEK" },
    { 0x2A00, NULL,        "GAP_DEVICE_NAME" },
    { 0x2A0D, NULL,        "DST_OFFSET" },
    { 0x2A0C, NULL,        "EXACT_TIME_256" },
    { 0x2A26, parse_firmware_revision_string,        "FIRMWARE_REVISION_STRING" },
    { 0x2A51, NULL,        "GLUCOSE_FEATURE" },
    { 0x2A18, parse_glucose_measurement,    "GLUCOSE_MEASUREMENT" },
    { 0x2A34, parse_glucose_measurement_context,    "GLUCOSE_MEASUREMENT_CONTEXT" },
    { 0x2A27, NULL,        "HARDWARE_REVISION_STRING" },
    { 0x2A39, NULL,        "HEART_RATE_CONTROL_POINT" },
    { 0x2A37, parse_heartrate_measurement,    "HEART_RATE_MEASUREMENT" },
    { 0x2A4C, NULL,        "HID_CONTROL_POINT" },
    { 0x2A4A, NULL,        "HID_INFORMATION" },
    { 0x2A2A, NULL,        "IEEE_11073_20601_REGULATORY_CERTIFICATION_in_LIST" },
    { 0x2A36, parse_blood_pressure_measurement,    "INTERMEDIATE_BLOOD_PRESSURE" },
    { 0x2A1E, parse_temperature,    "INTERMEDIATE_TEMPERATURE" },
    { 0x2A0F, parse_local_time_information,        "LOCAL_TIME_INFORMATION" },
    { 0x2A29, parse_manufacturer_name_string,        "MANUFACTURER_NAME_STRING" },
    { 0x2A21, NULL,        "MEASUREMENT_INTERVAL" },
    { 0x2A24, parse_model_number_string,        "MODEL_NUMBER_STRING" },
    { 0x2A46, parse_alert_status,        "NEW_ALERT" },
    { 0x2A04, NULL,        "GAP_PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS" },
    { 0x2A02, NULL,        "GAP_PERIPHERAL_PRIVACY_FLAG" },
    { 0x2A50, parse_pnp_id,        "PNP_ID" },
    { 0x2A4E, NULL,        "PROTOCOL_MODE" },
    { 0x2A03, NULL,        "GAP_RECONNECTION_ADDRESS" },
    { 0x2A52, parse_record_access_control_point,    "RECORD_ACCESS_CONTROL_POINT" },
    { 0x2A14, parse_reference_time_information,        "REFERENCE_TIME_INFORMATION" },
    { 0x2A4D, NULL,        "REPORT" },
    { 0x2A4B, NULL,        "REPORT_MAP" },
    { 0x2A40, NULL,        "RINGER_CONTROL_POINT" },
    { 0x2A41, NULL,        "RINGER_SETTING" },
    { 0x2A4F, NULL,        "SCAN_TERVAL_WINDOW" },
    { 0x2A31, parse_scan_refresh,        "SCAN_REFRESH" },
    { 0x2A25, parse_serial_number_string,        "SERIAL_NUMBER_STRING" },
    { 0x2A05, NULL,        "GATT_SERVICE_CHANGED" },
    { 0x2A28, parse_software_revision_string,        "SOFTWARE_REVISION_STRING" },
    { 0x2A47, parse_alert_category_id_bit_mask,        "SUPPORTED_NEW_ALERT_CATEGORY" },
    { 0x2A48, parse_alert_category_id_bit_mask,        "SUPPORTED_UNREAD_ALERT_CATEGORY" },
    { 0x2A23, parse_system_id,        "SYSTEM_ID" },
    { 0x2A1C, parse_temperature,    "TEMPERATURE_MEASUREMENT" },
    { 0x2A1D, NULL,        "TEMPERATURE_TYPE" },
    { 0x2A12, NULL,        "TIME_ACCURACY" },
    { 0x2A13, NULL,        "TIME_SOURCE" },
    { 0x2A16, NULL,        "TIME_UPDATE_CONTROL_POINT" },
    { 0x2A17, NULL,        "TIME_UPDATE_STATE" },
    { 0x2A11, parse_time_with_dst,        "TIME_WITH_DST" },
    { 0x2A0E, NULL,        "TIME_ZONE" },
    { 0x2A07, parse_tx_power,        "TX_POWER" },
    { 0x2A45, parse_alert_status,        "UNREAD_ALERT_STATUS" },
    /* As of November 2013, the following are not yet considered "Adopted"
     * (see https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicsHome.aspx)
     */
    { 0x2A1A, NULL,        "BATTERY_POWER_STATE" },
    { 0x2A1B, NULL,        "BATTERY_LEVEL_STATE" },
    { 0x2B01, parse_weight,           "WEIGHT_MEASUREMENT" },
    { -1, NULL, NULL }
};

typedef struct  {
    char *uuid;
    parse_api parser;
    char *name;
} parser_128bit_entry_t;

parser_128bit_entry_t parser_128bit_list[] = {
    /* TI Sensor Tag */
    { "F000AA0104514000B000000000000000", parse_ti_sensor_temperature,          "TI: IR Temperature Sensor" },
    { "F000AA0204514000B000000000000000", NULL,                                 "TI: IR Temperature Sensor Configuration" },
    { "F000AA1104514000B000000000000000", parse_ti_sensor_accelerometer,        "TI: Accelerometer" },
    { "F000AA1204514000B000000000000000", NULL,                                 "TI: Accelerometer Configuration" },
    { "F000AA1304514000B000000000000000", parse_ti_sensor_accelerometer_period, "TI: Accelerometer Period" },
    { "F000AA2104514000B000000000000000", parse_ti_sensor_humidity,             "TI: Humidity Sensor" },
    { "F000AA2204514000B000000000000000", NULL,                                 "TI: Humidity Sensor Configuration" },
    { "F000AA3104514000B000000000000000", parse_ti_sensor_magnetometer,         "TI: Magnetometer" },
    { "F000AA3204514000B000000000000000", NULL,                                 "TI: Magnetometer Configuration" },
    { "F000AA3304514000B000000000000000", parse_ti_sensor_magnetometer_period,  "TI: Magnetometer Period" },
    { "F000AA4104514000B000000000000000", NULL,                                 "TI: Barometric Pressure Sensor" },
    { "F000AA4204514000B000000000000000", NULL,                                 "TI: Barometric Pressure Sensor Configuration" },
    { "F000AA4304514000B000000000000000", NULL,                                 "TI: Barometric Pressure Sensor Calibration" },
    { "F000AA5104514000B000000000000000", parse_ti_sensor_gyroscope,            "TI: Gyroscope" },
    { "F000AA5204514000B000000000000000", NULL,                                 "TI: Gyroscope Configuration" },
    { "F000AA6104514000B000000000000000", NULL,                                 "TI: Test Service" },
    { "F000AA6204514000B000000000000000", NULL,                                 "TI: Test Service Configuration" },
    { "F000FFE104514000B000000000000000", parse_ti_sensor_keypressed,           "TI: Simple Key service" },
    { NULL, NULL, NULL },
};

typedef struct {
    int uuid;
    const char *name;
} uuid_names_t;

typedef struct {
    const char *uuid;
    const char *name;
} uuid_names_128_t;

const uuid_names_t service_list[] = {
        { 0x1800, "Generic Access" },
        { 0x1801, "Generic Attribute" },
        { 0x1802, "ImmediateAlert" },
        { 0x1803, "Link Loss" },
        { 0x1804, "Tx Power" },
        { 0x1805, "Current Time Service" },
        { 0x1806, "Reference Time Update Service" },
        { 0x1807, "Next DST Change Service" },
        { 0x1808, "Glucose" },
        { 0x1809, "Health Thermometer" },
        { 0x180A, "Device Information" },
        { 0x180D, "Heart Rate" },
        { 0x180E, ":Phone Alert Status Service" },
        { 0x180F, "Battery Service" },
        { 0x1810, "Blood Pressure" },
        { 0x1811, "Alert Notification Service" },
        { 0x1812, "Human Interface Device" },
        { 0x1813, "Scan Parameters" },
        { 0x1814, "Running Speed and Cadance" },
        { 0x1816, "Cycling Speed and Cadance" },
        /* As of November 2013, these are not considered "Adopted"
         * (see https://developer.bluetooth.org/gatt/services/Pages/ServicesHome.aspx)
         */
        { 0x1901, "Weight Service" },
        { -1, 0 },
};

const uuid_names_128_t service_list_128[] = {
        { "F000AA0004514000B000000000000000", "TI IR Temperature Service" },
        { "F000AA1004514000B000000000000000", "TI Accelerometer Service" },
        { "F000AA2004514000B000000000000000", "TI Humidity Service" },
        { "F000AA3004514000B000000000000000", "TI Magnetometer Service" },
        { "F000AA4004514000B000000000000000", "TI Barometer Service" },
        { "F000AA5004514000B000000000000000", "TI Gyroscope Service" },
        { "F000AA6004514000B000000000000000", "TI Test Service" },
        { NULL, NULL },
};

const uuid_names_t descriptor_list[] = {
        { 0x2905, "Characteristic Aggregate Format" },
        { 0x2900, "Characteristic Extended Properties" },
        { 0x2904, "Characteristic Presentation Format" },
        { 0x2901, "Characteristic User Description" },
        { 0x2902, "Client Characteristic Configuration" },
        { 0x2907, "External Report Reference" },
        { 0x2908, "Report Reference" },
        { 0x2903, "Server Characteristic Configuration" },
        { 0x2906, "Valid Range" },
        { -1, 0 },
};


/**
 * MAIN PARSING ROUTINE.
 */
int parse_characteristic_uuid_buffer(const char *uuid, const uint8_t *in, int in_len, char *out, int out_len) {

    uint16_t id = 0;
    int len = 0;

    if (NULL == uuid) return -1;

    if (strncasecmp(uuid, "0x", 2) == 0) uuid += 2;

    /* 16-bit UUID */
    if (strlen(uuid) == 4) {

        if (sscanf(uuid, "%04hx", &id) != 1) return -1;

        int i;
        for( i = 0; parser_16bit_list[i].uuid >= 0; i++ ) {
            if( parser_16bit_list[i].uuid == id ) {
                if( NULL == parser_16bit_list[i].parser ) {
                    break;
                } else {
                    len = parser_16bit_list[i].parser( in, in_len, out, out_len );
                    if ((len >= 0) && (len < out_len)) out[len] = 0;
                    return len;
                }
            }
        }
    } else {
        char *stripped = strip_uuid_128(uuid);
        if (NULL != stripped) {

            int i;
            for( i = 0; NULL != parser_128bit_list[i].uuid; i++ ) {
                if( strcasecmp(parser_128bit_list[i].uuid, stripped) == 0 ) {

                    if( NULL == parser_128bit_list[i].parser ) {
                        break;
                    } else {
                        free(stripped);
                        len = parser_128bit_list[i].parser( in, in_len, out, out_len );
                        if ((len >= 0) && (len < out_len)) out[len] = 0;
                        return len;
                    }
                }
            }
            free(stripped);
        }
    }
    return -1;
}

const char *get_service_name(const char *uuid) {

    int i;

    if (NULL == uuid) return NULL;

    if (strncasecmp(uuid, "0x", 2) == 0) uuid += 2;

    /* 16-bit UUID */
    if (strlen(uuid) == 4) {
        uint16_t id = 0;
        if (sscanf(uuid, "%04hx", &id) != 1) return uuid;
        for( i = 0; service_list[i].uuid >= 0; i++ ) {
            if( service_list[i].uuid == id ) {
                return service_list[i].name;
            }
        }
    } else {
        char *stripped = strip_uuid_128(uuid);
        if (NULL != stripped) {
            for( i = 0; NULL != service_list_128[i].uuid; i++ ) {
                if( strcasecmp(service_list_128[i].uuid, stripped) == 0 ) {
                    free(stripped);
                    return service_list_128[i].name;
                }
            }
            free(stripped);
        }
    }
    return uuid;
}

const char *get_characteristic_name(const char *uuid) {

    int i;

    if (NULL == uuid) return NULL;

    if (strncasecmp(uuid, "0x", 2) == 0) uuid += 2;

    /* 16-bit UUID */
    if (strlen(uuid) == 4) {
        uint16_t id = 0;
        if (sscanf(uuid, "%04hx", &id) != 1) return uuid;
        for( i = 0; parser_16bit_list[i].uuid >= 0; i++ ) {
            if( parser_16bit_list[i].uuid == id ) {
                return parser_16bit_list[i].name;
            }
        }
    } else {
        char *stripped = strip_uuid_128(uuid);
        if (NULL != stripped) {
            for( i = 0; NULL != parser_128bit_list[i].uuid; i++ ) {
                if( strcasecmp(parser_128bit_list[i].uuid, stripped) == 0 ) {
                    free(stripped);
                    return parser_128bit_list[i].name;
                }
            }
            free(stripped);
        }
    }
    return uuid;
}

const char *get_descriptor_name(const char *uuid) {

    if (NULL == uuid) return NULL;

    if (strncasecmp(uuid, "0x", 2) == 0) uuid += 2;

    /* 16-bit UUID */
    if (strlen(uuid) == 4) {
        int i;
        uint16_t id = 0;
        if (sscanf(uuid, "%04hx", &id) != 1) return uuid;
        for( i = 0; descriptor_list[i].uuid >= 0; i++ ) {
            if( descriptor_list[i].uuid == id ) {
                return descriptor_list[i].name;
            }
        }
    }
    return uuid;
}

/**
 * Internal sub-parsing routines.
 *
 * Add new parsing code here and a matching entry in parser_list.
 */
static int parse_appearance(const uint8_t *in, int in_len, char *out, int out_len) {

    if (in_len != 2) return -1;

    uint16_t *appearance = (uint16_t *)in;
    int ret = 0;

    switch(*appearance) {
        case 64:
            ret += snprintf(out, out_len,"Generic Phone");
            break;
        case 128:
            ret += snprintf(out, out_len,"Generic Computer");
            break;
        case 192:
            ret += snprintf(out, out_len,"Generic Watch");
            break;
        case 193:
            ret += snprintf(out, out_len,"Watch: Sports Watch");
            break;
        case 256:
            ret += snprintf(out, out_len,"Generic Clock");
            break;
        case 320:
            ret += snprintf(out, out_len,"Generic Display");
            break;
        case 384:
            ret += snprintf(out, out_len,"Generic Remote Control");
            break;
        case 448:
            ret += snprintf(out, out_len,"Generic Eye-glasses");
            break;
        case 512:
            ret += snprintf(out, out_len,"Generic Tag");
            break;
        case 576:
            ret += snprintf(out, out_len,"Generic Keyring");
            break;
        case 640:
            ret += snprintf(out, out_len,"Generic Media Player");
            break;
        case 704:
            ret += snprintf(out, out_len,"Generic Barcode Scanner");
            break;
        case 768:
            ret += snprintf(out, out_len,"Generic Thermometer");
            break;
        case 769:
            ret += snprintf(out, out_len,"Thermometer: Ear");
            break;
        case 832:
            ret += snprintf(out, out_len,"Generic Heart rate Sensor");
            break;
        case 833:
            ret += snprintf(out, out_len,"Heart Rate Sensor: Heart Rate Belt");
            break;
        case 896:
            ret += snprintf(out, out_len,"Generic Blood Pressure");
            break;
        case 897:
            ret += snprintf(out, out_len,"Blood Pressure: Arm");
            break;
        case 898:
            ret += snprintf(out, out_len,"Blood Pressure: Wrist");
            break;
        case 960:
            ret += snprintf(out, out_len,"Human Interface Device (HID)");
            break;
        case 961:
            ret += snprintf(out, out_len,"Keyboard");
            break;
        case 962:
            ret += snprintf(out, out_len,"Mouse");
            break;
        case 963:
            ret += snprintf(out, out_len,"Joystick");
            break;
        case 964:
            ret += snprintf(out, out_len,"Gamepad");
            break;
        case 965:
            ret += snprintf(out, out_len,"Digitizer Tablet");
            break;
        case 966:
            ret += snprintf(out, out_len,"Card Reader");
            break;
        case 967:
            ret += snprintf(out, out_len,"Digital Pen");
            break;
        case 968:
            ret += snprintf(out, out_len,"Barcode Scanner");
            break;
        case 1024:
            ret += snprintf(out, out_len,"Generic Glucose Meter");
            break;
        case 1088:
            ret += snprintf(out, out_len,"Generic: Running Walking Sensor");
            break;
        case 1089:
            ret += snprintf(out, out_len,"Running Walking Sensor: In-Shoe");
            break;
        case 1090:
            ret += snprintf(out, out_len,"Running Walking Sensor: On-Shoe");
            break;
        case 1091:
            ret += snprintf(out, out_len,"Running Walking Sensor: On-Hip");
            break;
        case 1152:
            ret += snprintf(out, out_len,"Generic: Cycling");
            break;
        case 1153:
            ret += snprintf(out, out_len,"Cycling: Cycling Computer");
            break;
        case 1154:
            ret += snprintf(out, out_len,"Cycling: Speed Sensor");
            break;
        case 1155:
            ret += snprintf(out, out_len,"Cycling: Cadence Sensor");
            break;
        case 1156:
            ret += snprintf(out, out_len,"Cycling: Power Sensor");
            break;
        case 1157:
            ret += snprintf(out, out_len,"Cycling: Speed and Cadence Sensor");
            break;
        default:
            ret += snprintf(out, out_len,"Unknown\n");
            break;
    }

    return ret;
}

static int parse_time(const uint8_t *in, int in_len, char *out, int out_len) {

    if ((out == NULL) || (in_len < 7)) {
        printf("Time length is too small %d < 7", in_len);
        return 0;
    }

    uint16_t year = *((uint16_t*)&in[0]);
    uint8_t month = in[2];
    uint8_t day = in[3];
    uint8_t hour = in[4];
    uint8_t min = in[5];
    uint8_t sec = in[6];

    return snprintf(out, out_len, "Time: %04d/%02d/%02d %02d:%02d:%02d\n", year, month, day, hour, min, sec);
}

static int parse_time_with_dst(const uint8_t *in, int in_len, char *out, int out_len) {
    char *o = out;

    if (in_len < 9) return -1;

    o += snprintf(o, out_len - (o - out),"Time with DST: \n");
   o += parse_time(in, in_len, o,  out_len - (o - out));

    if (in[8] == 0) {
        o += snprintf(o, out_len - (o - out),"\tDST Offset: Standard Time\n");
    } else if ((in[8] == 2) || (in[8] == 4) || (in[8] == 8)) {
        o += snprintf(o, out_len - (o - out),"\tDST Offset: %d:%02d\n", ((int8_t)in[8]) / 4, (((int8_t)in[8]) % 4)*15);
    } else if (in[8] == 255) {
        o += snprintf(o, out_len - (o - out),"\tDST Offset: Unknown\n");
    } else {
        return -1;
    }

    return o - out;
}

static int parse_reference_time_information(const uint8_t *in, int in_len, char *out, int out_len) {
    char *o = out;

    if (in_len < 4) return -1;
    o += snprintf(o, out_len - (o - out),"Reference Time Information: \n");
    switch(in[0]) {
        case 0:
        o += snprintf(o, out_len - (o - out),"\tTime Source: Unknown\n");
            break;
        case 1:
        o += snprintf(o, out_len - (o - out),"\tTime Source: Network Time Protocol\n");
            break;
        case 2:
        o += snprintf(o, out_len - (o - out),"\tTime Source: GPS\n");
            break;
        case 3:
        o += snprintf(o, out_len - (o - out),"\tTime Source: Radio Time Signal\n");
            break;
        case 4:
        o += snprintf(o, out_len - (o - out),"\tTime Source: Manual\n");
            break;
        case 5:
        o += snprintf(o, out_len - (o - out),"\tTime Source: Atomic Clock\n");
            break;
        case 6:
        o += snprintf(o, out_len - (o - out),"\tTime Source: Cellular Network\n");
            break;
        default:
            return -1;
    }

    if (in[1] == 254) {
        o += snprintf(o, out_len - (o - out),"\tAccuracy: Accuracy out of range\n");
    } else if (in[1] == 255) {
        o += snprintf(o, out_len - (o - out),"\tAccuracy: Accuracy Unknown\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tAccuracy: %f\n", ((float)in[1]) * 0.125f);
    }

    if (in[2] == 255 || in[3] == 255) {
        o += snprintf(o, out_len - (o - out),"\tDays/Hours since update: 255 or more days\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tDays since update: %d\n", in[2]);
        o += snprintf(o, out_len - (o - out),"\tHours since update: %d\n", in[3]);
    }

    return o - out;
}

static int parse_local_time_information(const uint8_t *in, int in_len, char *out, int out_len) {
    char *o = out;

    if (in_len < 2) return -1;

    o += snprintf(o, out_len - (o - out),"Local Time Information: \n");

    o += snprintf(o, out_len - (o - out),"\tTime Zone: %d:%02d\n", ((int8_t)in[0]) / 4, (((int8_t)in[0]) % 4)*15);
    if (in[1] == 0) {
        o += snprintf(o, out_len - (o - out),"\tDST Offset: Standard Time\n");
    } else if ((in[1] == 2) || (in[1] == 4) || (in[1] == 8)) {
        o += snprintf(o, out_len - (o - out),"\tDST Offset: %d:%02d\n", ((int8_t)in[1]) / 4, (((int8_t)in[1]) % 4)*15);
    } else if (in[1] == 255) {
        o += snprintf(o, out_len - (o - out),"\tDST Offset: Unknown\n");
    } else {
        return -1;
    }

    return o - out;
}
static int parse_current_time(const uint8_t *in, int in_len, char *out, int out_len) {
    char *o = out;

   if (in_len < 10) return -1;

   o += snprintf(o, out_len - (o - out),"Current Time: \n\t");
   o += parse_time(in, in_len, o,  out_len - (o - out));

   switch(in[7]) {
       case 0:
           o += snprintf(o, out_len - (o - out),"\tDay of Week: Unknown\n");
           break;
       case 1:
           o += snprintf(o, out_len - (o - out),"\tDay of Week: Monday\n");
           break;
       case 2:
           o += snprintf(o, out_len - (o - out),"\tDay of Week: Tuesday\n");
           break;
       case 3:
           o += snprintf(o, out_len - (o - out),"\tDay of Week: Wednesday\n");
           break;
       case 4:
           o += snprintf(o, out_len - (o - out),"\tDay of Week: Thursday\n");
           break;
       case 5:
           o += snprintf(o, out_len - (o - out),"\tDay of Week: Friday\n");
           break;
       case 6:
           o += snprintf(o, out_len - (o - out),"\tDay of Week: Saturday\n");
           break;
       case 7:
           o += snprintf(o, out_len - (o - out),"\tDay of Week: Sunday\n");
           break;
       default:
           return -1;
   }

   o += snprintf(o, out_len - (o - out),"\tFraction: %d/256\n", in[8]);

   if (in[9] & 0x01) o += snprintf(o, out_len - (o - out),"\tAdjust Reason: Manual Time Update\n");
   if (in[9] & 0x02) o += snprintf(o, out_len - (o - out),"\tAdjust Reason: External Reference Time Update\n");
   if (in[9] & 0x04) o += snprintf(o, out_len - (o - out),"\tAdjust Reason: Change of Time Zone\n");
   if (in[9] & 0x08) o += snprintf(o, out_len - (o - out),"\tAdjust Reason: Change of DST\n");

   return o - out;
}

static int parse_alert_status(const uint8_t *in, int in_len, char *out, int out_len) {
    char *o = out;

    if (in_len < 2) return -1;

    o += snprintf(o, out_len - (o - out),"Catagory ID: %d\n", in[0]);
    switch(in[0]) {
        case 0:
            o += snprintf(o, out_len - (o - out),"\tSimple Alert: General text alert or non-text alert\n");
            break;
        case 1:
            o += snprintf(o, out_len - (o - out),"\tEmail: Alert when Email messages arrives\n");
            break;
        case 2:
            o += snprintf(o, out_len - (o - out),"\tNews: News feeds such as RSS, Atom\n");
            break;
        case 3:
            o += snprintf(o, out_len - (o - out),"\tCall: Incoming call\n");
            break;
        case 4:
            o += snprintf(o, out_len - (o - out),"\tMissed call: Missed Call\n");
            break;
        case 5:
            o += snprintf(o, out_len - (o - out),"\tSMS/MMS: SMS/MMS message arrives\n");
            break;
        case 6:
            o += snprintf(o, out_len - (o - out),"\tVoice mail: Voice mail\n");
            break;
        case 7:
            o += snprintf(o, out_len - (o - out),"\tSchedule: Alert occurred on calendar, planner\n");
            break;
        case 8:
            o += snprintf(o, out_len - (o - out),"\tHigh Prioritized Alert: Alert that should be handled as high priority\n");
            break;
        case 9:
            o += snprintf(o, out_len - (o - out),"\tInstant Message: Alert for incoming instant messages\n");
            break;
        default:
            o += snprintf(o, out_len - (o - out),"\t{UNKNOWN}\n");
            break;
    }

    o += snprintf(o, out_len - (o - out),"Count: %d\n", in[1]);

    if (in_len > 2) {
        o += snprintf(o, out_len - (o - out),"Text: %s\n",(char *) &in[2]);
    }

    return o - out;


}
static int parse_alert_category_id_bit_mask(const uint8_t *in, int in_len, char *out, int out_len) {


    char *o = out;

    if (in_len < 1) return -1;

    if (in_len == 1) {
        o += snprintf(o, out_len - (o - out),"Flags: 0x%02x\n", in[0]);
    } else {
        o += snprintf(o, out_len - (o - out),"Flags: 0x%02x%02x\n", in[0], in[1]);
    }
    if (in[0] & 0x01) {
        o += snprintf(o, out_len - (o - out),"\tSimple Alert: Supported\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tSimple Alert: Not Supported\n");
    }

    if (in[0] & 0x02) {
        o += snprintf(o, out_len - (o - out),"\tEmail: Supported\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tEmail: Not Supported\n");
    }

    if (in[0] & 0x04) {
        o += snprintf(o, out_len - (o - out),"\tNews: Supported\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\nNews: Not Supported\n");
    }
    if (in[0] & 0x08) {
        o += snprintf(o, out_len - (o - out),"\tCall: Supported\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tCall: Not Supported\n");
    }

    if (in[0] & 0x10) {
        o += snprintf(o, out_len - (o - out),"\tMissed Call: Supported\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tMissed Call: Not Supported\n");
    }

    if (in[0] & 0x20) {
        o += snprintf(o, out_len - (o - out),"\tSMS/MMS: Supported\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tSMS/MMS: Not Supported\n");
    }

    if (in[0] & 0x40) {
        o += snprintf(o, out_len - (o - out),"\tVoice Mail: Supported\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tVoice Mail: Not Supported\n");
    }

    if (in[0] & 0x80) {
        o += snprintf(o, out_len - (o - out),"\tSchedule: Supported\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tSchedule: Not Supported\n");
    }

    if (in_len > 1) {
        if (in[1] & 0x01) {
            o += snprintf(o, out_len - (o - out),"\tHigh Prioritized Alert: Supported\n");
        } else {
            o += snprintf(o, out_len - (o - out),"\tHigh Prioritized Alert: Not Supported\n");
        }

        if (in[1] & 0x02) {
            o += snprintf(o, out_len - (o - out),"\tInstant Message: Supported\n");
        } else {
            o += snprintf(o, out_len - (o - out),"\tInstant Message: Not Supported\n");
        }
    }

    return o - out;
}
static int parse_alert_level(const uint8_t *in, int __attribute__((__unused__))in_len, char *out, int out_len) {

    char *o = out;

    switch(in[0]){
        case 0:
            o += snprintf(o, out_len - (o - out),"Alert Level: No Alert\n");
            break;
        case 1:
            o += snprintf(o, out_len - (o - out),"Alert Level: Mild Alert\n");
            break;
        case 2:
            o += snprintf(o, out_len - (o - out),"Alert Level: High Alert\n");
            break;
        default:
            o += snprintf(o, out_len - (o - out),"Alert Level: Unknown\n");
            break;
    }

    return o - out;
}
static int parse_scan_refresh(const uint8_t *in, int __attribute__((__unused__))in_len, char *out, int out_len) {

    char *o = out;

    if (in[0] & 0x01) {
        o += snprintf(o, out_len - (o - out),"Server Requires Refresh\n");
    } else {
        o += snprintf(o, out_len - (o - out),"Server Refresh Not Required\n");
    }

    return o - out;
}
static int parse_battery_level(const uint8_t *in, int __attribute__((__unused__))in_len, char *out, int out_len) {

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Battery Level: %d%%\n", in[0]);

    return o - out;
}
static int parse_tx_power(const uint8_t *in, int __attribute__((__unused__))in_len, char *out, int out_len) {

    char *o = out;

    o += snprintf(o, out_len - (o - out),"TX Power: %d\n", (int8_t)in[0]);

    return o - out;
}
static int parse_manufacturer_name_string(const uint8_t *in, int __attribute__((__unused__))in_len, char *out, int out_len) {

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Manufacturer: %s\n", (char*)in);

    return o - out;
}
static int parse_model_number_string(const uint8_t *in, int __attribute__((__unused__))in_len, char *out, int out_len) {

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Model Number: %s\n", (char*)in);

    return o - out;
}
static int parse_serial_number_string(const uint8_t *in, int __attribute__((__unused__))in_len, char *out, int out_len) {

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Serial Number: %s\n", (char*)in);

    return o - out;
}
static int parse_software_revision_string(const uint8_t *in, int __attribute__((__unused__))in_len, char *out, int out_len) {

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Software Revision: %s\n", (char*)in);

    return o - out;
}
static int parse_firmware_revision_string(const uint8_t *in, int __attribute__((__unused__))in_len, char *out, int out_len) {

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Firmware Revision: %s\n", (char*)in);

    return o - out;
}
static int parse_pnp_id(const uint8_t *in, int in_len, char *out, int out_len) {

    if (in_len < 7) return -1;

    char *o = out;

    uint8_t vendor_id_src = in[0];
    uint16_t vendor_id = ((uint16_t)in[2]) << 8 | in[1];
    uint16_t product_id = ((uint16_t)in[4]) << 8 | in[3];
    uint16_t product_ver = ((uint16_t) in[6]) << 8 | in[5];

    o += snprintf(o, out_len - (o - out),"Vendor ID Source: %u\n", vendor_id_src);
    o += snprintf(o, out_len - (o - out),"Vendor ID: %u\n", vendor_id);
    o += snprintf(o, out_len - (o - out),"Product ID: %u\n", product_id);
    o += snprintf(o, out_len - (o - out),"Product Version: %u\n", product_ver);

    return o - out;
}
static int parse_system_id(const uint8_t *in, int in_len, char *out, int out_len) {

    if (in_len < 8) return -1;
    char *o = out;
    uint64_t *system_id = (uint64_t*)in;

    o += snprintf(o, out_len - (o - out),"Manufacturer: %llu\n", (*system_id) >> 24);
    o += snprintf(o, out_len - (o - out),"Organization UID: %u\n", ((uint32_t)*system_id) & 0x00FFFFFF);

    return o - out;
}
static int parse_blood_pressure_feature(const uint8_t *in, int __attribute__((__unused__))in_len, char *out, int out_len) {

    char *o = out;

    const uint16_t *feature = (uint16_t*)in;

    o += snprintf(o, out_len - (o - out),"Blood Pressure Feature: %u\n", *feature);

    return o - out;
}
static int parse_blood_pressure_measurement(const uint8_t *in, int in_len, char *out, int out_len) {

    if (in_len < 2) return -1;

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Flags: 0x%02x\n", in[0]);
    if (in[0] & 0x01) {
        o += snprintf(o, out_len - (o - out),"\tUnits in kPa\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tUnits in mmHg\n");
    }

    if (in[0] & 0x02) {
        o += snprintf(o, out_len - (o - out),"\tTime Stamp present\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tNo Time Stamp\n");
    }

    if (in[0] & 0x04) {
        o += snprintf(o, out_len - (o - out),"\tPulse Rate present\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tNo Pulse Rate\n");
    }
    if (in[0] & 0x08) {
        o += snprintf(o, out_len - (o - out),"\tUser ID Present\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tNo User ID\n");
    }

    if (in[0] & 0x10) {
        o += snprintf(o, out_len - (o - out),"\tMeasurement Status present\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tNo Measurement Status\n");
    }


    float systolic = convert_sfloat(*((uint16_t*)&in[1]));
    float diastolic = convert_sfloat(*((uint16_t*)&in[3]));
    float mean = convert_sfloat(*((uint16_t*)&in[5]));

    o += snprintf(o, out_len - (o - out),"Blood Presure Measurement (Systolic): %f %s\n", systolic, in[0] & 0x01 ? "kPa" : "mmHg");
    o += snprintf(o, out_len - (o - out),"Blood Presure Measurement (Diastolic): %f %s\n", diastolic, in[0] & 0x01 ? "kPa" : "mmHg");
    o += snprintf(o, out_len - (o - out),"Blood Presure Measurement (Mean Arterial Pressure): %f %s\n", mean, in[0] & 0x01 ? "kPa" : "mmHg");

    int offset = 7;
    if (in[0] & 0x02) {
        if (in_len - offset < 7) {
            printf("Error not enough room for time\n");
            return -1;
        }
        o += parse_time(&in[offset], 7, o, out_len - (o - out));
        offset += 7;
    }

    if (in[0] & 0x04) {
        if (in_len - offset < 2) {
            printf("Error not enough room for pulse\n");
            return -1;
        }
        float pulse = convert_sfloat(*((uint32_t*)&in[offset]));
        offset += 2;
        o += snprintf(o, out_len - (o - out),"\tPulse Rate: %f\n", pulse);
    }

    if (in[0] & 0x08) {
        if (in_len - offset < 1) {
            printf("Error not enough room for user ID\n");
            return -1;
        }
        o += snprintf(o, out_len - (o - out),"\tkey: 0x%02x (%d)\n", in[offset], in[offset]);
        offset++;
    }

    if (in[0] & 0x10) {
        if (in_len - offset < 2) {
            printf("Error not enough room for Measurement Status (%d bytes left)\n", in_len - offset);
            return -1;
        }
        o += snprintf(o, out_len - (o - out),"Measurement Status:\n");

        o += dump_binary(&in[offset], in_len - offset, o, o - out);

        if (in[offset] & 0x01) {
            o += snprintf(o, out_len - (o - out),"\tBody movement during measurement\n");
        } else {
            o += snprintf(o, out_len - (o - out),"\tNo body movement\n");
        }

        if (in[offset] & 0x02) {
            o += snprintf(o, out_len - (o - out),"\tCuff too loose\n");
        } else {
            o += snprintf(o, out_len - (o - out),"\tCuff fits properly\n");
        }

        if (in[offset] & 0x04) {
            o += snprintf(o, out_len - (o - out),"\tIrregular pulse detected\n");
        } else {
            o += snprintf(o, out_len - (o - out),"\tNo irregular pulse detected\n");
        }

        switch (in[(offset >> 3) & 0x03]) {
            case 0:
                o += snprintf(o, out_len - (o - out),"\tPulse rate is within the range\n");
                break;
            case 1:
                o += snprintf(o, out_len - (o - out),"\tPulse rate exceeds upper limit\n");
                break;
            case 2:
                o += snprintf(o, out_len - (o - out),"\tPulse rate is less than lower limit\n");
                break;
        }

        if (in[offset] & 0x20) {
            o += snprintf(o, out_len - (o - out),"\tImproper measurement position\n");
        } else {
            o += snprintf(o, out_len - (o - out),"\tProper measurement position\n");
        }
    }

    return o - out;
}
static int parse_heartrate_measurement(const uint8_t *in, int in_len, char *out, int out_len) {

    if (in_len < 2) return -1;

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Flags: 0x%02x\n", in[0]);
    if (in[0] & 0x01) {
        o += snprintf(o, out_len - (o - out),"\tHeart Format UINT16\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tHeart Format UINT8\n");
    }

    switch ((in[0] >> 1)& 0x03) {
        case 0:
            o += snprintf(o, out_len - (o - out),"\tSensor contact feature not supported\n");
            break;
        case 1:
            o += snprintf(o, out_len - (o - out),"\tSensor contact feature not supported\n");
            break;
        case 2:
            o += snprintf(o, out_len - (o - out),"\tSensor contact feature supported, contact is not detected\n");
            break;
        case 3:
            o += snprintf(o, out_len - (o - out),"\tSensor contact feature supported, contact is detected\n");
            break;
    }
    if (in[0] & 0x08) {
        o += snprintf(o, out_len - (o - out),"\tEnergy Expanded present - Units kilo Joules\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tEnergy Expanded not present\n");
    }
    if (in[0] & 0x10) {
        o += snprintf(o, out_len - (o - out),"\tOne or more RR-Intervals are presents - Units 1/1024\n");
    } else {
        o += snprintf(o, out_len - (o - out),"\tRR-Intervals not present\n");
    }

    int offset = 1;
    uint8_t *value8 = NULL;
    uint16_t *value16 = NULL;
    if (in[0] & 0x01) {
        value16 = (uint16_t *)&in[offset];
        offset +=2;
        o += snprintf(o, out_len - (o - out),"\t Heart Rate Measurement: 0x%04x\n", *value16);
    } else {
        value8 = (uint8_t *)&in[offset];
        offset +=1;
        o += snprintf(o, out_len - (o - out),"\t Heart Rate Measurement: 0x%02x\n", *value8);
    }

    if (in[0] & 0x08) {
        value16 = (uint16_t *)&in[offset];
        offset +=2;
        o += snprintf(o, out_len - (o - out),"\t Energy Expanded: 0x%04x\n", *value16);

    }
    if (in[0] & 0x10) {
        while (offset < in_len) {
            value16 = (uint16_t *)&in[offset];
            offset +=2;
            o += snprintf(o, out_len - (o - out),"\tRR-Interval: 0x%04x\n", *value16);
        }
    }

    return o - out;
}

static int parse_glucose_measurement(const uint8_t *in, int in_len, char *out, int out_len) {

    if (in_len < 10) return -1;

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Flags: 0x%02x\n", in[0]);
    if (in[0] & 0x01) o += snprintf(o, out_len - (o - out),"\tTime Offset Present\n");
    else o += snprintf(o, out_len - (o - out),"\tNo Time Offset\n");

    if (in[0] & 0x02) o += snprintf(o, out_len - (o - out),"\tGlucose Concentration, Type and Sample Location Present\n");
    else o += snprintf(o, out_len - (o - out),"\tNo Glucose Concentration, Type and Sample Location Present\n");

    if (in[0] & 0x04) o += snprintf(o, out_len - (o - out),"\tConcentration units mol/L\n");
    else o += snprintf(o, out_len - (o - out),"\tConcentration units kg/L\n");

    if (in[0] & 0x08) o += snprintf(o, out_len - (o - out),"\tSensor Status Annunciation Present\n");
    else o += snprintf(o, out_len - (o - out),"\tNo Sensor Status Annunciation Present\n");

    if (in[0] & 0x10) o += snprintf(o, out_len - (o - out),"\tContext information follows\n");
    else o += snprintf(o, out_len - (o - out),"\tNo Context Information\n");

    o += snprintf(o, out_len - (o - out),"Sequence Number: 0x%04x\n", *((uint16_t*)&in[1]));
    int offset = 3;

    o += parse_time(&in[offset], 7, o, out_len - (o - out));

    offset += 7;

    if (in[0] & 0x01) {
        if (in_len - offset < 1) {
            printf("Error not enough time offset\n");
            return -1;
        }
        o += snprintf(o, out_len - (o - out),"Time Offset: %d\n", *((int16_t*)&in[offset]));
        offset += 2;
    }
    if (in[0] & 0x02) {
        if (in_len - offset < 2) {
            printf("Error not enough Glucose Concentration\n");
            return -1;
        }
        float concentration = convert_sfloat(*((uint16_t*)&in[offset]));
        o += snprintf(o, out_len - (o - out),"Glucose Concentration: %f %s\n", concentration, in[0] & 0x04 ? "mol/L" : "kg/L");
        offset += 2;

        o += snprintf(o, out_len - (o - out),"Type: %d\n", in[offset] & 0x0F);
        switch (in[offset] & 0x0F) {
            case 1:
                o += snprintf(o, out_len - (o - out), "\tCapillary Whole blood\n");
                break;
            case 2:
                o += snprintf(o, out_len - (o - out), "\tCapillary Plasma\n");
                break;
            case 3:
                o += snprintf(o, out_len - (o - out), "\tVenous Whole blood\n");
                break;
            case 4:
                o += snprintf(o, out_len - (o - out), "\tVenous Plasma\n");
                break;
            case 5:
                o += snprintf(o, out_len - (o - out), "\tArterial Whole blood\n");
                break;
            case 6:
                o += snprintf(o, out_len - (o - out), "\tArterial Plasma\n");
                break;
            case 7:
                o += snprintf(o, out_len - (o - out), "\tUndetermined Whole blood\n");
                break;
            case 8:
                o += snprintf(o, out_len - (o - out), "\tUndetermined Plasma\n");
                break;
            case 9:
                o += snprintf(o, out_len - (o - out), "\tInterstitial Fluid (ISF)\n");
                break;
            case 10:
                o += snprintf(o, out_len - (o - out), "\tControl Solution\n");
                break;
        }
        o += snprintf(o, out_len - (o - out), "Sample Location: %d\n", (in[offset] >> 4) & 0x0F);
        switch ((in[offset] >> 4) & 0x0F) {
            case 1:
                o += snprintf(o, out_len - (o - out), "\tFinger\n");
                break;
            case 2:
                o += snprintf(o, out_len - (o - out), "\tAlternate Site Test (AST)\n");
                break;
            case 3:
                o += snprintf(o, out_len - (o - out), "\tEarlobe\n");
                break;
            case 4:
                o += snprintf(o, out_len - (o - out), "\tControl solution\n");
                break;
            case 15:
                o += snprintf(o, out_len - (o - out), "\tSample Location value not available\n");
                break;
        }
        offset ++;
    }
    if (in[0] & 0x08) {
        if (in_len - offset < 2) {
            printf("Error not enough Sensor Status Annunciation\n");
            return -1;
        }

        o += snprintf(o, out_len - (o - out),"Sensor Status Annunciation: 0x%04x\n", *((uint16_t*)&in[offset]));

        if (in[offset] & 0x01) {
            o += snprintf(o, out_len - (o - out),"\tDevice battery low at time of measurement\n");
        }

        if (in[offset] & 0x02) {
            o += snprintf(o, out_len - (o - out),"\tSensor malfunction or faulting at time of measurement\n");
        }

        if (in[offset] & 0x04) {
            o += snprintf(o, out_len - (o - out),"\tSample size for blood or control solution insufficient at time of measurement\n");
        }

        if (in[offset] & 0x08) {
            o += snprintf(o, out_len - (o - out),"\tStrip insertion error\n");
        }

        if (in[offset] & 0x10) {
            o += snprintf(o, out_len - (o - out),"\tStrip type incorrect for device\n");
        }

        if (in[offset] & 0x20) {
            o += snprintf(o, out_len - (o - out),"\tSensor result higher than the device can process\n");
        }

        if (in[offset] & 0x40) {
            o += snprintf(o, out_len - (o - out),"\tSensor result lower than the device can process\n");
        }

        if (in[offset] & 0x80) {
            o += snprintf(o, out_len - (o - out),"\tSensor temperature too high for valid test/result at time of measurement\n");
        }
        offset ++;
        if (in[offset] & 0x01) {
            o += snprintf(o, out_len - (o - out),"\tSensor temperature too low for valid test/result at time of measurement\n");
        }

        if (in[offset] & 0x02) {
            o += snprintf(o, out_len - (o - out),"\tSensor read interrupted because strip was pulled too soon at time of measurement\n");
        }

        if (in[offset] & 0x04) {
            o += snprintf(o, out_len - (o - out),"\tGeneral device fault has occurred in the sensor\n");
        }

        if (in[offset] & 0x08) {
            o += snprintf(o, out_len - (o - out),"\tTime fault has occurred in the sensor and time may be inaccurate\n");
        }
    }

    return o - out;
}
static int parse_glucose_measurement_context(const uint8_t *in, int in_len, char *out, int out_len) {

    if (in_len < 3) return -1;

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Flags: 0x%02x\n", in[0]);
    if (in[0] & 0x01) o += snprintf(o, out_len - (o - out),"\tCarbohydrate ID And Carbohydrate Present\n");

    if (in[0] & 0x02) o += snprintf(o, out_len - (o - out),"\tMeal Present\n");

    if (in[0] & 0x04) o += snprintf(o, out_len - (o - out),"\tTester-Health Present\n");

    if (in[0] & 0x08) o += snprintf(o, out_len - (o - out),"\tExercise Duration And Exercise Intensity Present\n");

    if (in[0] & 0x10) o += snprintf(o, out_len - (o - out),"\tMedication ID And Medication Present\n");

    if (in[0] & 0x20) o += snprintf(o, out_len - (o - out),"\tMedication Value Units: kg\n");
    else o += snprintf(o, out_len - (o - out),"\tMedication Value Units: Liters\n");

    if (in[0] & 0x40) o += snprintf(o, out_len - (o - out),"\tHbA1c Present\n");

    if (in[0] & 0x80) o += snprintf(o, out_len - (o - out),"\tExtended Flags Present\n");

    o += snprintf(o, out_len - (o - out),"Sequence Number: 0x%04x\n", *((uint16_t*)&in[1]));
    int offset = 3;

    if (in[0] & 0x80) {
        if (in_len - offset < 1) {
            printf("Error not enough extended Flags\n");
            return -1;
        }
        o += snprintf(o, out_len - (o - out),"Extended Flags (Reserved): 0x%02x\n", in[offset]);
        offset ++;
    }

    if (in[0] & 0x01) {
        if (in_len - offset < 3) {
            printf("Error not enough Carbohydrate ID\n");
            return -1;
        }
        o += snprintf(o, out_len - (o - out),"Carbohydrate ID: 0x%02x\n", in[offset]);
        switch(in[offset]){
            case 1:
                o += snprintf(o, out_len - (o - out),"\tBreakfast\n");
                break;
            case 2:
                o += snprintf(o, out_len - (o - out),"\tLunch\n");
                break;
            case 3:
                o += snprintf(o, out_len - (o - out),"\tDinner\n");
                break;
            case 4:
                o += snprintf(o, out_len - (o - out),"\tSnack\n");
                break;
            case 5:
                o += snprintf(o, out_len - (o - out),"\tDrink\n");
                break;
            case 6:
                o += snprintf(o, out_len - (o - out),"\tSupper\n");
                break;
            case 7:
                o += snprintf(o, out_len - (o - out),"\tBrunch\n");
                break;
        }
        offset ++;

        float carbs = convert_sfloat(*((uint16_t*)&in[offset]));
        o += snprintf(o, out_len - (o - out),"Carbohydrate: %f kg\n", carbs);
        offset += 2;
    }
    if (in[0] & 0x02) {
        if (in_len - offset < 1) {
            printf("Error not enough Meal\n");
            return -1;
        }

        o += snprintf(o, out_len - (o - out),"Meal: 0x%02x\n", in[offset]);
        switch(in[offset]) {
            case 1:
                o += snprintf(o, out_len - (o - out),"\tPreprandial (before meal)\n");
                break;
            case 2:
                o += snprintf(o, out_len - (o - out),"\tPostprandial (after meal)\n");
                break;
            case 3:
                o += snprintf(o, out_len - (o - out),"\tFasting\n");
                break;
            case 4:
                o += snprintf(o, out_len - (o - out),"\tCasual (snacks, drinks, etc.)\n");
                break;
            case 5:
                o += snprintf(o, out_len - (o - out),"\tBedtime\n");
                break;
        }
        offset ++;
    }

    if (in[0] & 0x04) {
        if (in_len - offset < 1) {
            printf("Error not enough Tester and Health\n");
            return -1;
        }

        o += snprintf(o, out_len - (o - out),"Tester: 0x%02x\n", in[offset] & 0x0F);
        switch(in[offset] & 0x0F) {
            case 1:
                o += snprintf(o, out_len - (o - out),"\tSelf\n");
                break;
            case 2:
                o += snprintf(o, out_len - (o - out),"\tHealth Care Professional\n");
                break;
            case 3:
                o += snprintf(o, out_len - (o - out),"\tLab test\n");
                break;
            case 15:
                o += snprintf(o, out_len - (o - out),"\tTester value not available\n");
                break;
        }
        o += snprintf(o, out_len - (o - out),"Health: 0x%02x\n", (in[offset] >> 4) & 0x0F);
        switch((in[offset] >> 4) & 0x0F) {
            case 1:
                o += snprintf(o, out_len - (o - out),"\tMinor health issues\n");
                break;
            case 2:
                o += snprintf(o, out_len - (o - out),"\tMajor health issues\n");
                break;
            case 3:
                o += snprintf(o, out_len - (o - out),"\tDuring menses\n");
                break;
            case 4:
                o += snprintf(o, out_len - (o - out),"\tUnder stress\n");
                break;
            case 5:
                o += snprintf(o, out_len - (o - out),"\tNo health issues\n");
                break;
            case 15:
                o += snprintf(o, out_len - (o - out),"\tHealth value not available\n");
                break;
        }
        offset ++;
    }

    if (in[0] & 0x08) {
        if (in_len - offset < 3) {
            printf("Error not enough Exercise duration and intensity\n");
            return -1;
        }
        o += snprintf(o, out_len - (o - out),"Exercise Duration: %d\n", *((uint16_t*)&in[offset]));
        offset+=2;
        o += snprintf(o, out_len - (o - out),"Exercise Intensity: %d\n", in[offset]);
        offset++;
    }
    if (in[0] & 0x10) {
        if (in_len - offset < 3) {
            printf("Error not enough Mediaction ID\n");
            return -1;
        }

        o += snprintf(o, out_len - (o - out),"Medication ID: 0x%02x\n", in[offset]);
        switch(in[offset]) {
            case 1:
                o += snprintf(o, out_len - (o - out),"\tRapid acting insulin\n");
                break;
            case 2:
                o += snprintf(o, out_len - (o - out),"\tShort acting insulin\n");
                break;
            case 3:
                o += snprintf(o, out_len - (o - out),"\tIntermediate acting insulin\n");
                break;
            case 4:
                o += snprintf(o, out_len - (o - out),"\tLong acting insulin\n");
                break;
            case 5:
                o += snprintf(o, out_len - (o - out),"\tPre-mixed insulin\n");
                break;
        }
        offset ++;

        float medication = convert_sfloat(*((uint16_t*)&in[offset]));
        o += snprintf(o, out_len - (o - out),"Medication: %f %s\n", medication, in[0] & 0x20 ? "Liters" : "kg");
        offset += 2;
    }

    if (in[0] & 0x40) {
        if (in_len - offset < 2) {
            printf("Error not enough Mediaction ID\n");
            return -1;
        }
        float HbA1c = convert_sfloat(*((uint16_t*)&in[offset]));
        o += snprintf(o, out_len - (o - out),"HbA1c: %f%%\n", HbA1c);
        offset += 2;
    }

    return o - out;
}
static int parse_record_access_control_point(const uint8_t *in, int in_len, char *out, int out_len) {

    if (in_len < 10) return -1;

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Op Code: 0x%02x\n", in[0]);
    switch(in[0]) {
        case 1:
            o += snprintf(o, out_len - (o - out),"\tReport stored records (Operator: Value from Operator Table)\n");
            break;
        case 2:
            o += snprintf(o, out_len - (o - out),"\tDelete stored records (Operator: Value from Operator Table)\n");
            break;
        case 3:
            o += snprintf(o, out_len - (o - out),"\tAbort operation (Operator: Null 'value of 0x00 from Operator Table')\n");
            break;
        case 4:
            o += snprintf(o, out_len - (o - out),"\tReport number of stored records (Operator: Value from Operator Table)\n");
            break;
        case 5:
            o += snprintf(o, out_len - (o - out),"\tNumber of stored records response (Operator: Null 'value of 0x00 from Operator Table')\n");
            break;
        case 6:
            o += snprintf(o, out_len - (o - out),"\tResponse Code (Operator: Null 'value of 0x00 from Operator Table')\n");
            break;
    }
    o += snprintf(o, out_len - (o - out),"Operator: 0x%02x\n", in[1]);
    switch(in[1]) {
        case 0:
            o += snprintf(o, out_len - (o - out),"\tNull\n");
            break;
        case 1:
            o += snprintf(o, out_len - (o - out),"\tAll records\n");
            break;
        case 2:
            o += snprintf(o, out_len - (o - out),"\tLess than or equal to\n");
            break;
        case 3:
            o += snprintf(o, out_len - (o - out),"\tGreater than or equal to\n");
            break;
        case 4:
            o += snprintf(o, out_len - (o - out),"\tWithin range of (inclusive)\n");
            break;
        case 5:
            o += snprintf(o, out_len - (o - out),"\tFirst record(i.e. oldest record)\n");
            break;
        case 6:
            o += snprintf(o, out_len - (o - out),"\tLast record (i.e. most recent record)\n");
            break;
    }

    int offset = 2;

    o += snprintf(o, out_len - (o - out),"Operand:\n");

    o += dump_binary(&in[offset], in_len - offset, o, o - out);

    while (offset < in_len) {
        if (in_len - offset < 2) {
            printf("Not enough room for Operand\n");
            return -1;
        }


        o += snprintf(o, out_len - (o - out),"\nOperand Op Code: 0x%02x\n", in[offset]);
        switch(in[offset]) {
            case 1:
                o += snprintf(o, out_len - (o - out),"\tFilter parameters (as appropriate to Operator and Service)\n");
                break;
            case 2:
                o += snprintf(o, out_len - (o - out),"\tFilter parameters (as appropriate to Operator and Service)\n");
                break;
            case 3:
                o += snprintf(o, out_len - (o - out),"\tNot included\n");
                break;
            case 4:
                o += snprintf(o, out_len - (o - out),"\tFilter parameters (as appropriate to Operator and Service)\n");
                break;
            case 5:
                o += snprintf(o, out_len - (o - out),"\tNumber of Records (Field size defined per service)\n");
                break;
            case 6:
                o += snprintf(o, out_len - (o - out),"\tRequest Op Code, Response Code Value\n");
                break;
        }
        offset ++;
        o += snprintf(o, out_len - (o - out),"Operand Response Code Value: 0x%02x\n", in[offset]);
        switch(in[offset]) {
            case 1:
                o += snprintf(o, out_len - (o - out),"\tSuccess\n");
                break;
            case 2:
                o += snprintf(o, out_len - (o - out),"\tOp Code not supported\n");
                break;
            case 3:
                o += snprintf(o, out_len - (o - out),"\tInvalid Operator\n");
                break;
            case 4:
                o += snprintf(o, out_len - (o - out),"\tOperator not supported\n");
                break;
            case 5:
                o += snprintf(o, out_len - (o - out),"\tInvalid Operand\n");
                break;
            case 6:
                o += snprintf(o, out_len - (o - out),"\tNo records found\n");
                break;
            case 7:
                o += snprintf(o, out_len - (o - out),"\tAbort unsuccessful\n");
                break;
            case 8:
                o += snprintf(o, out_len - (o - out),"\tProcedure not completed\n");
                break;
            case 9:
                o += snprintf(o, out_len - (o - out),"\tOperand not supported\n");
                break;
        }
        offset ++;
    }

    return o - out;
}

static int parse_temperature(const uint8_t *in, int in_len, char *out, int out_len) {

    if (in_len < 5) return -1;

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Flags: 0x%02x\n", in[0]);

    if (in[0] & 0x01) o += snprintf(o, out_len - (o - out),"\tFahrenheit\n");
    else o += snprintf(o, out_len - (o - out),"\tCelsius\n");

    if (in[0] & 0x02) o += snprintf(o, out_len - (o - out),"\tTime Stamp Present\n");

    if (in[0] & 0x04) o += snprintf(o, out_len - (o - out),"\tTemperature Type Present\n");


    /* Convert the temperature to float value */
    float temp = convert_float(*((uint32_t*)&in[1]));

    o += snprintf(o, out_len - (o - out),"Temperature: %f %s\n", temp, in[0] & 0x01 ? "F" : "C");

    int offset = 5;

    /* Parse Time */
    if ( in[0] & 0x02) {
        if (in_len - offset < 7){
            printf(" Not enough room to parse time");
            return -1;
        }
        o += parse_time(&in[5], 7, o, out_len - (o - out));
        offset += 7;
    }

    if (in[0] & 0x04) {
        if (in_len - offset < 1) {
            printf("Error not enough Temperature Type\n");
            return -1;
        }

        o += snprintf(o, out_len - (o - out),"Temperature Type: 0x%02x\n", in[offset]);
        switch(in[offset]) {
            case 1:
                o += snprintf(o, out_len - (o - out),"\tArm pit\n");
                break;
            case 2:
                o += snprintf(o, out_len - (o - out),"\tBody (general)\n");
                break;
            case 3:
                o += snprintf(o, out_len - (o - out),"\tEar (usually ear lobe)\n");
                break;
            case 4:
                o += snprintf(o, out_len - (o - out),"\tFinger\n");
                break;
            case 5:
                o += snprintf(o, out_len - (o - out),"\tGastro-intestinal Tract\n");
                break;
            case 6:
                o += snprintf(o, out_len - (o - out),"\tMouth\n");
                break;
            case 7:
                o += snprintf(o, out_len - (o - out),"\tRectum\n");
                break;
            case 8:
                o += snprintf(o, out_len - (o - out),"\tToe\n");
                break;
            case 9:
                o += snprintf(o, out_len - (o - out),"\tTympanum (ear drum)\n");
                break;
        }
    }

    return o - out;
}

/* Courtesy  http://www.nist.gov/pml/wmd/pubs/upload/appc-13-hb44-final.pdf, chapter 4 */
#define LBS_PER_KG    (0.45359237)

static int parse_weight(const uint8_t *in, int in_len, char *out, int out_len) {

    if (in_len < 5) return -1;

    char *o = out;

    o += snprintf(o, out_len - (o - out),"Flags: 0x%02x\n", in[0]);
    o += snprintf(o, out_len - (o - out),"\tStabilized: %s\n", (in[0] & 0x80) ? "Yes":"No");

    /* Convert the weight to float value (always reported in kg X 10) */
    float weight = convert_float(*((uint32_t*)&in[1])) / 10.0;

    o += snprintf(o, out_len - (o - out),"Weight: %fkg (%flb)\n", weight, (weight / LBS_PER_KG) );

    return o - out;
}

/**
 * Internal utility routines.
 */
static float convert_float(uint32_t value) {


    int32_t mant = value & 0x00FFFFFF;
    int32_t exp = ((int32_t) value) >> 24;

    if (mant & 0x00800000) {
        /* convert to C2 negative value */
        mant = mant - FLOAT_MAX_MANTISSA;
    }

    float result = powf(10.0, (float)exp);
    result *= (float)mant;
    return result;
}

static float convert_sfloat(uint16_t value) {


    int16_t mant = value & 0x0FFF;
    int16_t exp = ((int16_t) value) >> 12;

    if (mant & 0x0800) {
        /* convert to C2 negative value */
        mant = mant - SFLOAT_MAX_MANTISSA;
    }

    float result = powf(10.0, (float)exp);
    result *= (float)mant;
    return result;
}

static int dump_binary(const uint8_t *in, int in_len, char *out, int out_len) {
    const uint8_t *p = in;
    char *o = out;
    int po;

    po = 0;

    while (p < in + in_len) {
        o += snprintf(o, out_len - (o - out), "0x%04x: ", (p - in) / LINE_WIDTH);

        /* Dumps the packet in basic ASCII dump format */
        po = 0;
        while (po < LINE_WIDTH) {
            if (p+po < in + in_len)
                o += snprintf(o, out_len - (o - out), "%02x ", p[po]);
            else o += snprintf(o, out_len - (o - out), "   ");
            if (++po == LINE_WIDTH/2) o += snprintf(o, out_len - (o - out), "-- ");
        }
        o += snprintf(o, out_len - (o - out), o, "   ");

        po = 0;
        while (po < LINE_WIDTH) {
            if (p+po < in + in_len)
                o += snprintf(o, out_len - (o - out), "%c", isprint(p[po]) ? p[po] : '.');
            else o += snprintf(o, out_len - (o - out), " ");
            ++po;
        }
        o += snprintf(o, out_len - (o - out), "\n");
        p += po;
    }
    return o - out;
}

/* Remove all non-hex digits to ensure we match successfully */
static char *strip_uuid_128(const char *uuid) {
    if (NULL == uuid)
        return NULL;

    if (strncasecmp(uuid, "0x", 2) == 0)
        uuid += 2;

    char *stripped = strdup(uuid);
    if( NULL == stripped ) {
        return NULL;
    }

    int i = 0;
    while (i < (int)strlen(stripped)) {
        if (isxdigit(stripped[i])) {
            i++;
        } else if (stripped[i] != '-') {
            break;
        } else {
            memmove(&stripped[i], &stripped[i+1], strlen(&stripped[i+1]) + 1);
        }
    }

    /* Only Accept 128-bit */
    if (strlen(stripped) == 32) {
        return stripped;
    } else {
        free(stripped);
        return NULL;
    }
}

/**
 * TI Parsing Routines
 */
static int parse_ti_sensor_humidity(const uint8_t *in, int in_len, char *out, int out_len) {

    if (in_len != 4) return -1;

    char *o = out;

    int16_t raw = (in[0] & 0xff) | ((in[1] << 8) & 0xff00);
    double temperature = -46.85 + 175.72/65536 *(double)raw;

    raw = ((in[2] & 0xff)| ((in[3] << 8) & 0xff00));
    raw &= ~0x0003;

    double humidity = -6.0 + 125.0/65536 * (double)raw;

    o += snprintf(o, out_len - (o - out), "Temperature: %0.0f\n", temperature);
    o += snprintf(o, out_len - (o - out), "Humidity: %0.0f\n", humidity);

    return o - out;
}

/*
 * See:  http://processors.wiki.ti.com/index.php/SensorTag_User_Guide#IR_Temperature_Sensor
 */
static int parse_ti_sensor_temperature(const uint8_t *in, int in_len, char *out, int out_len){

    if( in_len != 4 ) return -1;

    char *o = out;

    int16_t objTemp;
    int16_t ambTemp;
    objTemp = (in[0] & 0xff)| ((in[1] << 8) & 0xff00);
    ambTemp = ((in[2] & 0xff)| ((in[3] << 8) & 0xff00));

    double tmpAmb = (double)((double)ambTemp / (double)128);

    double Vobj2 = (double)objTemp * 0.00000015625;
    double Tdie2 = (double)tmpAmb + 273.15;
    const double S0 = 6.4E-14;
    const double a1 = 1.75E-3;
    const double a2 = -1.678E-5;
    const double b0 = -2.94E-5;
    const double b1 = -5.7E-7;
    const double b2 = 4.63E-9;
    const double c2 = 13.4;
    const double Tref = 298.15;
    double S = S0 * (1 + a1 * (Tdie2 - Tref) + a2 * pow((Tdie2 - Tref),2));
    double Vos = b0 + b1 * (Tdie2 - Tref) + b2 * pow((Tdie2 - Tref),2);
    double fObj = (Vobj2 - Vos) + c2 * pow((Vobj2 - Vos), 2);
    double tObj = pow( pow(Tdie2, 4) + (fObj/S), (float)0.25);
    tObj = (tObj - 273.15);

    o += snprintf(o, out_len - (o - out), "Die Temperature: %0.0f\n", tmpAmb);
    o += snprintf(o, out_len - (o - out), "Target Temperature: %0.0f\n", tObj);

    return o - out;
}

static int parse_ti_sensor_gyroscope(const uint8_t *in, int in_len, char *out, int out_len){

    if( in_len != 6 ) return -1;

    char *o = out;

    int16_t rawX = 0, rawY = 0, rawZ = 0;

    rawX = (in[0] & 0xff) | (in[1] << 8);
    rawY = (in[2] & 0xff) | (in[3] << 8);
    rawZ = (in[4] & 0xff) | (in[5] << 8);

    float vX = (rawX * 1.0) / (65536 / 500);
    float vY = (rawY * 1.0) / (65536 / 500);
    float vZ = (rawZ * 1.0) / (65536 / 500);

    o += snprintf(o, out_len - (o - out), "Gyroscope: X %f deg/s, Y %f deg/s, Z %f deg/s\n", vX, vY, vZ);

    return o - out;
}

static int parse_ti_sensor_accelerometer(const uint8_t *in, int in_len, char *out, int out_len){

    if( in_len != 3 ) return -1;

    char *o = out;

    int8_t x, y,z;
    x = in[0];
    y = in[1];
    z = in[2];

    float Rx = (x * 1.0) / (256/4);
    float Ry = (y * 1.0) / (256/4);
    float Rz = (z * 1.0) / (256/4);

    o += snprintf(o, out_len - (o - out), "Accelerometer: Rx %f G, Ry %f G, Rz %f G\n", Rx, Ry, Rz);

    return o - out;
}

static int parse_ti_sensor_accelerometer_period(const uint8_t *in, int in_len, char *out, int out_len){

    if( in_len != 1 ) return -1;

    char *o = out;

    o += snprintf(o, out_len - (o - out), "Accelerometer period: %dms\n", in[0] * 10);

    return o - out;
}

static int parse_ti_sensor_magnetometer(const uint8_t *in, int in_len, char *out, int out_len){

    if( in_len != 6 ) return -1;

    char *o = out;

    int16_t rawX = (in[0] & 0xff) | ((in[1] << 8) & 0xff00);
    int16_t rawY = (in[2] & 0xff) | ((in[3] << 8) & 0xff00);
    int16_t rawZ = (in[4] & 0xff) | ((in[5] << 8) & 0xff00);

    float vX = (rawX * 1.0) / (65536/2000);
    float vY = (rawY * 1.0) / (65536/2000);
    float vZ = (rawZ * 1.0) / (65536/2000);

    o += snprintf(o, out_len - (o - out), "Magnetometer: X %fuT, Y %fuT, Z %fuT\n", vX, vY, vZ);

    return o - out;
}

static int parse_ti_sensor_magnetometer_period(const uint8_t *in, int in_len, char *out, int out_len){

    if( in_len != 1 ) return -1;

    char *o = out;

    o += snprintf(o, out_len - (o - out), "Magnetometer period: %dms\n", in[0]);

    return o - out;
}

static int parse_ti_sensor_keypressed(const uint8_t *in, int in_len, char *out, int out_len) {

    if( in_len != 1 ) return -1;

    char *o = out;

    o += snprintf(o, out_len - (o - out), o, "Key Pressed: ");
    if (in[0] & 0x04) o += snprintf(o, out_len - (o - out), o, "Side Key ");
    if (in[0] & 0x02) o += snprintf(o, out_len - (o - out), o, "Right Key ");
    if (in[0] & 0x01) o += snprintf(o, out_len - (o - out), o, "Left Key ");
    if (in[0] == 0x00) o += snprintf(o, out_len - (o - out), o, "No Key ");

    o += snprintf(o, out_len - (o - out), o, "\n");

    return o - out;
}

__SRCVERSION( "$URL$ $Rev$" )

