Push Collector Headless

========================================================================
Description.

Push Collector Headless is the headless part of the Push Collector
application. It is responsible for handling all push-related logic,
including:

- Creating and destroying channels
- Processing incoming push notifications
- Adding notifications to the BlackBerry Hub
- Handling a push transport error
- Handling a lost connection to the PNS Agent
- Handling a SIM change

The sample code for this application is Open Source under the Apache 2.0 License
(http://www.apache.org/licenses/LICENSE-2.0.html).

========================================================================
Requirements:

BlackBerry 10.2 Native SDK

========================================================================
Running the example:

The headless application cannot be launched directly by the user. To
launch it, run the Push Collector application. The Push Collector uses
the invocation framework to trigger the launching of the headless application.

See the readme for the Push Collector application for instructions on 
how to build and launch the Push Collector application.