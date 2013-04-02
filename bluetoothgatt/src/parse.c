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
static int parse_appearance(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_blood_pressure_measurement(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_heartrate_measurement(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_glucose_measurement(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_glucose_measurement_context(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_record_access_control_point(const uint8_t *in, int in_len, char *out, int out_len);
static int parse_temperature(const uint8_t *in, int in_len, char *out, int out_len);

static float convert_float(uint32_t value);
static float convert_sfloat(uint16_t value);
static int dump_binary(const uint8_t *in, int in_len, char *out, int out_len);

typedef int (*parse_api)( const uint8_t *in, int in_len, char *out, int out_len );

typedef struct  {
    int uuid;
    parse_api parser;
} parser_entry_t;

parser_entry_t parser_list[] = {
    { 0x2A43, NULL },        //"ALERT_CATEGORY_ID"
    { 0x2A42, NULL },        //"ALERT_CATEGORY_ID_BIT_MASK"
    { 0x2A06, NULL },        //"ALERT_LEVEL"
    { 0x2A44, NULL },        //"ALERT_NOTIFICATION_CONTROL_POINT"
    { 0x2A3F, NULL },        //"ALERT_STATUS"
    { 0x2A01, parse_appearance },    //"GAP_APPEARANCE"
    { 0x2A19, NULL },        //"BATTERY_LEVEL"
    { 0x2A49, NULL },        //"BLOOD_PRESSURE_FEATURE"
    { 0x2A35, parse_blood_pressure_measurement },    //"BLOOD_PRESSURE_MEASUREMENT"
    { 0x2A38, NULL },        //"BODY_SENSOR_LOCATION"
    { 0x2A22, NULL },        //"BOOT_KEYBOARD_INPUT_REPORT"
    { 0x2A32, NULL },        //"BOOT_KEYBOARD_out_REPORT"
    { 0x2A33, NULL },        //"BOOT_MOUSE_INPUT_REPORT"
    { 0x2A2B, NULL },        //"CURRENT_TIME"
    { 0x2A08, NULL },        //"DATE_TIME"
    { 0x2A0A, NULL },        //"DAY_DATE_TIME"
    { 0x2A09, NULL },        //"DAY_OF_WEEK"
    { 0x2A00, NULL },        //"GAP_DEVICE_NAME"
    { 0x2A0D, NULL },        //"DST_OFFSET"
    { 0x2A0C, NULL },        //"EXACT_TIME_256"
    { 0x2A26, NULL },        //"FIRMWARE_REVISION_STRING"
    { 0x2A51, NULL },        //"GLUCOSE_FEATURE"
    { 0x2A18, parse_glucose_measurement },    //"GLUCOSE_MEASUREMENT"
    { 0x2A34, parse_glucose_measurement_context },    //"GLUCOSE_MEASUREMENT_CONTEXT"
    { 0x2A27, NULL },        //"HARDWARE_REVISION_STRING"
    { 0x2A39, NULL },        //"HEART_RATE_CONTROL_POINT"
    { 0x2A37, parse_heartrate_measurement },    //"HEART_RATE_MEASUREMENT"
    { 0x2A4C, NULL },        //"HID_CONTROL_POINT"
    { 0x2A4A, NULL },        //"HID_INFORMATION"
    { 0x2A2A, NULL },        //"IEEE_11073_20601_REGULATORY_CERTIFICATION_in_LIST"
    { 0x2A36, parse_blood_pressure_measurement },    //"INTERMEDIATE_BLOOD_PRESSURE"
    { 0x2A1E, parse_temperature },    //"INTERMEDIATE_TEMPERATURE"
    { 0x2A0F, NULL },        //"LOCAL_TIME_INFORMATION"
    { 0x2A29, NULL },        //"MANUFACTURER_NAME_STRING"
    { 0x2A21, NULL },        //"MEASUREMENT_INTERVAL"
    { 0x2A24, NULL },        //"MODEL_NUMBER_STRING"
    { 0x2A46, NULL },        //"NEW_ALERT"
    { 0x2A04, NULL },        //"GAP_PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS"
    { 0x2A02, NULL },        //"GAP_PERIPHERAL_PRIVACY_FLAG"
    { 0x2A50, NULL },        //"PNP_ID"
    { 0x2A4E, NULL },        //"PROTOCOL_MODE"
    { 0x2A03, NULL },        //"GAP_RECONNECTION_ADDRESS"
    { 0x2A52, parse_record_access_control_point },    //"RECORD_ACCESS_CONTROL_POINT"
    { 0x2A14, NULL },        //"REFERENCE_TIME_INFORMATION"
    { 0x2A4D, NULL },        //"REPORT"
    { 0x2A4B, NULL },        //"REPORT_MAP"
    { 0x2A40, NULL },        //"RINGER_CONTROL_POINT"
    { 0x2A41, NULL },        //"RINGER_SETTING"
    { 0x2A4F, NULL },        //"SCAN_TERVAL_WINDOW"
    { 0x2A31, NULL },        //"SCAN_REFRESH"
    { 0x2A25, NULL },        //"SERIAL_NUMBER_STRING"
    { 0x2A05, NULL },        //"GATT_SERVICE_CHANGED"
    { 0x2A28, NULL },        //"SOFTWARE_REVISION_STRING"
    { 0x2A47, NULL },        //"SUPPORTED_NEW_ALERT_CATEGORY"
    { 0x2A48, NULL },        //"SUPPORTED_UNREAD_ALERT_CATEGORY"
    { 0x2A23, NULL },        //"SYSTEM_ID"
    { 0x2A1C, parse_temperature },    //"TEMPERATURE_MEASUREMENT"
    { 0x2A1D, NULL },        //"TEMPERATURE_TYPE"
    { 0x2A12, NULL },        //"TIME_ACCURACY"
    { 0x2A13, NULL },        //"TIME_SOURCE"
    { 0x2A16, NULL },        //"TIME_UPDATE_CONTROL_POINT"
    { 0x2A17, NULL },        //"TIME_UPDATE_STATE"
    { 0x2A11, NULL },        //"TIME_WITH_DST"
    { 0x2A0E, NULL },        //"TIME_ZONE"
    { 0x2A07, NULL },        //"TX_POWER"
    { 0x2A45, NULL },        //"UNREAD_ALERT_STATUS"
    { -1, NULL }
};

/**
 * MAIN PARSING ROUTINE.
 */
int parse_characteristic_uuid_buffer(const char *uuid, const uint8_t *in, int in_len, char *out, int out_len) {

    uint16_t id = 0;

    if (NULL == uuid) return -1;

    if (strncasecmp(uuid, "0x", 2) == 0) uuid += 2;

    if (strlen(uuid) != 4) return -1;

    if (sscanf(uuid, "%04hx", &id) != 1) return -1;

    int i;
    for( i = 0; parser_list[i].uuid >= 0; i++ ) {
        if( parser_list[i].uuid == id ) {
            if( NULL == parser_list[i].parser ) {
                return -1;
            } else {
                return parser_list[i].parser( in, in_len, out, out_len );
            }
        }
    }

    return -1;
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
            printf("Error not enough Sensor Status Annuciation\n");
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

        o += snprintf(o, out_len - (o - out), o, "0x%04x: ", (p - in) / LINE_WIDTH);

        /* Dumps the packet in basic ASCII dump format */
        po = 0;
        while (po < LINE_WIDTH) {
            if (p+po < in + in_len)
                o += snprintf(o, out_len - (o - out), o, "%02x ", p[po]);
            else o += snprintf(o, out_len - (o - out), o, "   ");
            if (++po == LINE_WIDTH/2) o += snprintf(o, out_len - (o - out), o, "-- ");
        }
        o += snprintf(o, out_len - (o - out), o, "   ");

        po = 0;
        while (po < LINE_WIDTH) {
            if (p+po < in + in_len)
                o += snprintf(o, out_len - (o - out), o, "%c", isprint(p[po]) ? p[po] : '.');
            else o += snprintf(o, out_len - (o - out), o, " ");
            ++po;
        }
        o += snprintf(o, out_len - (o - out), "\n");
        p += po;
    }

    return o - out;
}

__SRCVERSION( "$URL$ $Rev$" )
