#include "Util.hpp"

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const char* Util::parse_uuid(const char *uuid, const uuidnames_t uuidNames[])
{
    uint16_t id = 0;

    if (0 == uuid)
        return "UNKNOWN";

    if (strncasecmp(uuid, "0x", 2) == 0)
        uuid += 2;

    if (strlen(uuid) != 4)
        return uuid;

    if (sscanf(uuid, "%04hx", &id) != 1)
        return uuid;

    for (int i = 0; uuidNames[i].uuid != -1; i++) {
        if (uuidNames[i].uuid == id) {
            return uuidNames[i].name;
        }
    }

    return uuid;
}

const char* Util::parse_characteristic_uuid(const char *uuid)
{
    const uuidnames_t characteristicUUID[] = {
            { 0x2a43, "ALERT_CATEGORY_ID" },
            { 0x2A42, "ALERT_CATEGORY_ID_BIT_MASK" },
            { 0x2A06, "ALERT_LEVEL" },
            { 0x2A44, "ALERT_NOTIFICATION_CONTROL_POINT" },
            { 0x2A3F, "ALERT_STATUS" },
            { 0x2A01, "GAP_APPEARANCE" },
            { 0x2A19, "BATTERY_LEVEL" },
            { 0x2A49, "BLOOD_PRESSURE_FEATURE" },
            { 0x2A35, "BLOOD_PRESSURE_MEASUREMENT" },
            { 0x2A38, "BODY_SENSOR_LOCATION" },
            { 0x2A22, "BOOT_KEYBOARD_INPUT_REPORT" },
            { 0x2A32, "BOOT_KEYBOARD_OUTPUT_REPORT" },
            { 0x2A33, "BOOT_MOUSE_INPUT_REPORT" },
            { 0x2A2B, "CURRENT_TIME" },
            { 0x2A08, "DATE_TIME" },
            { 0x2A0A, "DAY_DATE_TIME" },
            { 0x2A09, "DAY_OF_WEEK" },
            { 0x2A00, "GAP_DEVICE_NAME" },
            { 0x2A0D, "DST_OFFSET" },
            { 0x2A0C, "EXACT_TIME_256" },
            { 0x2A26, "FIRMWARE_REVISION_STRING" },
            { 0x2A51, "GLUCOSE_FEATURE" },
            { 0x2A18, "GLUCOSE_MEASUREMENT" },
            { 0x2A34, "GLUCOSE_MEASUREMENT_CONTEXT" },
            { 0x2A27, "HARDWARE_REVISION_STRING" },
            { 0x2A39, "HEART_RATE_CONTROL_POINT" },
            { 0x2A37, "HEART_RATE_MEASUREMENT" },
            { 0x2A4C, "HID_CONTROL_POINT" },
            { 0x2A4A, "HID_INFORMATION" },
            { 0x2A2A, "IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST" },
            { 0x2A36, "INTERMEDIATE_BLOOD_PRESSURE" },
            { 0x2A1E, "INTERMEDIATE_TEMPERATURE" },
            { 0x2A0F, "LOCAL_TIME_INFORMATION" },
            { 0x2A29, "MANUFACTURER_NAME_STRING" },
            { 0x2A21, "MEASUREMENT_INTERVAL" },
            { 0x2A24, "MODEL_NUMBER_STRING" },
            { 0x2A46, "NEW_ALERT" },
            { 0x2A04, "GAP_PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS" },
            { 0x2A02, "GAP_PERIPHERAL_PRIVACY_FLAG" },
            { 0x2A50, "PNP_ID" },
            { 0x2A4E, "PROTOCOL_MODE" },
            { 0x2A03, "GAP_RECONNECTION_ADDRESS" },
            { 0x2A52, "RECORD_ACCESS_CONTROL_POINT" },
            { 0x2A14, "REFERENCE_TIME_INFORMATION" },
            { 0x2A4D, "REPORT" },
            { 0x2A4B, "REPORT_MAP" },
            { 0x2A40, "RINGER_CONTROL_POINT" },
            { 0x2A41, "RINGER_SETTING" },
            { 0x2A4F, "SCAN_INTERVAL_WINDOW" },
            { 0x2A31, "SCAN_REFRESH" },
            { 0x2A25, "SERIAL_NUMBER_STRING" },
            { 0x2A05, "GATT_SERVICE_CHANGED" },
            { 0x2A28, "SOFTWARE_REVISION_STRING" },
            { 0x2A47, "SUPPORTED_NEW_ALERT_CATEGORY" },
            { 0x2A48, "SUPPORTED_UNREAD_ALERT_CATEGORY" },
            { 0x2A23, "SYSTEM_ID" },
            { 0x2A1C, "TEMPERATURE_MEASUREMENT" },
            { 0x2A1D, "TEMPERATURE_TYPE" },
            { 0x2A12, "TIME_ACCURACY" },
            { 0x2A13, "TIME_SOURCE" },
            { 0x2A16, "TIME_UPDATE_CONTROL_POINT" },
            { 0x2A17, "TIME_UPDATE_STATE" },
            { 0x2A11, "TIME_WITH_DST" },
            { 0x2A0E, "TIME_ZONE" },
            { 0x2A07, "TX_POWER" },
            { 0x2A45, "UNREAD_ALERT_STATUS" },
            { -1, 0 },
    };

    return parse_uuid(uuid, characteristicUUID);
}

const char* Util::parse_service_uuid(const char *uuid)
{
    const uuidnames_t serviceUUID[] = {
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
            { -1, 0 },
    };

    return parse_uuid(uuid, serviceUUID);
}

const char* Util::parse_descriptor_uuid(const char *uuid)
{
    const uuidnames_t descriptorUUID[] = {
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

    return (parse_uuid(uuid, descriptorUUID));
}

