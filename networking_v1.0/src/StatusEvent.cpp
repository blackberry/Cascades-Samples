/* Copyright (c) 2013 BlackBerry Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "StatusEvent.hpp"
#include <bps/bps.h>
#include <bps/netstatus.h>

StatusEvent::StatusEvent ()
{
    subscribe(netstatus_get_domain());

    bps_initialize();

    // Request all network status events.
    netstatus_request_events(0);

    info = NULL;
}

StatusEvent::~StatusEvent ()
{
    bps_shutdown();
}

void StatusEvent::event ( bps_event_t *event )
{
    bool status = false;
    const char* interface = "";
    const char* type = "asset:///images/unknown.gif";
    netstatus_interface_details_t* details = NULL;

    // Verify that the event coming in is a network status event.
    if (bps_event_get_domain(event) == netstatus_get_domain())
    {
        // Using the BPS event code of the network status event,
        // verify that the event is a network information event.
        if (NETSTATUS_INFO == bps_event_get_code(event))
        {
            // Retrieve the network status information, and verify
            // that the procedure is successful.
            if (BPS_SUCCESS == netstatus_get_info(&info))
            {
                status = netstatus_info_get_availability(info);
                interface = netstatus_info_get_default_interface(info);
                int success = netstatus_get_interface_details(interface, &details);

                if (success == BPS_SUCCESS)
                {
                    switch (netstatus_interface_get_type(details))
                    {
                        case NETSTATUS_INTERFACE_TYPE_WIRED:
                            type = "asset:///images/wired.gif";
                            break;

                        case NETSTATUS_INTERFACE_TYPE_WIFI:
                            type = "asset:///images/wifi.gif";
                            break;

                        case NETSTATUS_INTERFACE_TYPE_BLUETOOTH_DUN:
                            type = "asset:///images/bluetooth.gif";
                            break;

                        case NETSTATUS_INTERFACE_TYPE_USB:
                        case NETSTATUS_INTERFACE_TYPE_BB:
                            type = "asset:///images/usb.gif";
                            break;

                        case NETSTATUS_INTERFACE_TYPE_VPN:
                            type = "asset:///images/vpn.gif";
                            break;


                        case NETSTATUS_INTERFACE_TYPE_CELLULAR:
                            type = "asset:///images/cellular.gif";
                            break;

                        case NETSTATUS_INTERFACE_TYPE_P2P:
                        case NETSTATUS_INTERFACE_TYPE_UNKNOWN:
                            type = "asset:///images/unknown.gif";
                            break;
                    }
                    netstatus_free_info(&info);
                }
            }

            // Emit the signal to trigger networkStatusUpdated slot.
            emit networkStatusUpdated(status, type);
        }
    }
}
