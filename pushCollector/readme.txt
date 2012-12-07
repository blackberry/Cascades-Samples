Push Collector

========================================================================
Sample Description.

The Push Collector example demonstrates how to listen to incoming push
requests and display the delivered data.

You will learn how to:
- Use the PushService and InvokeManager classes
- Use various dialogs
- Store data in a SQLite database
- Use property bindings
- Add notifications to the BlackBerry Hub
- Process received pushes in the background

There is also a very valuable developer guide for this sample push-enabled 
application that you can find at
https://developer.blackberry.com/cascades/documentation/device_platform/push/index.html

The developer guide offers the following topics:

1. An overview of push and the Push Service architecture
2. The requirements for creating a full push solution
4. How to use the sample application when it's loaded on your BlackBerry 10 device 
  (This is also described below.)
5. Code samples to help you write your own push application using the 
   BlackBerry 10 Native SDK

The sample code for this application is Open Source under the Apache 2.0 License
(http://www.apache.org/licenses/LICENSE-2.0.html).

========================================================================
Requirements:

BlackBerry 10 Native SDK

========================================================================
Running the example:

1. Clone the Sample repository.
2. Launch BlackBerry 10 Native SDK, and from the File menu, select Import.
3. Expand General, and select Existing Projects into Workspace. Click Next.
4. Browse to the location of your sample directory, and then click OK.
5. The sample project should display in the Projects section. 
   Click Finish to import the project into your workspace.
6. In the Project Explorer pane, Right-click the project (for example hellocascades) 
   and select Build Project.
7. In the Project Explorer pane, Right-click the project (for example hellocascades) 
   and select Run As > BlackBerry C/C++ Application.
8. The application will now install and launch on your device if not you might
   have to set up your environment: 
   http://developer.blackberry.com/cascades/documentation/getting_started/setting_up.html
9. If you decided to sign your application, you need to make sure your invoke target ID 
   is unique to your application (signing requires that it cannot match the invoke 
   target ID used by someone else who is also attempting to sign the Push Collector 
   sample application):
   a. Change the "com.example.pushCollector.invoke.push" value in bar-descriptor.xml 
      to something unique of your choosing.
   b. Also, change the INVOKE_TARGET_KEY_PUSH constant in PushNotificationService.hpp to 
      have this same value.
   c. Change the "com.example.pushCollector.invoke.open" value in bar-descriptor.xml 
      to something unique of your choosing.
   d. Also, change the INVOKE_TARGET_KEY_OPEN constant in PushNotificationService.hpp to 
      have this same value.
   e. Try Run As > BlackBerry C/C++ Application again
 
   
========================================================================
   
How to send a push
   
In order to be able to send pushes to the Push Collector sample app, you will need to write 
a server-side push application (called a Push Initiator) to send out pushes with.

Luckily, this is fairly easy to do using the Push Service SDK available 
at https://developer.blackberry.com/services/push.

You'll find all the documentation for the Push Service SDK 
at http://docs.blackberry.com/en/developers/subcategories/?userType=21&category=Push+Service.

Note that in order to use the Push Service for developing an application for the general public, 
you will have to first register at https://www.blackberry.com/profile/?eventId=8121.

========================================================================

How to receive a push

1. Start the Push Collector sample application (if you haven't done so already).
2. Tap the Config action at the bottom of the device screen.  The configuration dialog will appear.
3. In the Mode dropdown, choose Public/BIS if the PPG is the BlackBerry Internet Service 
   or Enterprise/BDS if the PPG is the BlackBerry Device Service.
4. Clear the Subscribe with Push Service SDK check box if one of the following is true:
	- You implemented a Push Initiator that does not use the Push Service SDK.
	- Your Push Initiator only uses the low-level APIs without subscription support from the Push Service SDK.
5. If the Subscribe with Push Service SDK check box is selected, in the Application ID field, 
   perform one of the following actions:
	- If you are using the BlackBerry Internet Service as the PPG, type the application ID specified in 
	  the confirmation email message that you received from RIM.
	- If you are using the BlackBerry Device Service as the PPG, type a unique application ID of your choosing.
	  If you clear the Subscribe with Push Service SDK check box, you cannot type an application ID. 
	  In this case, the Push Service APIs create an application ID for you automatically.
6. If you are using the BlackBerry Internet Service as the PPG, in the PPG URL field, type the PPG base URL 
specified in the confirmation email message. The sample application uses this URL to create a channel to the PPG. 
For an evaluation environment, the URL is http://cp{cpid}.pushapi.eval.blackberry.com, where {cpid} is your content
provider ID. For a production environment, the URL is http://cp{cpid}.pushapi.na.blackberry.com.
7. If the Subscribe with Push Service SDK check box is selected, in the Push Initiator URL field, 
type https://{server_address}/pushsdk, where {server_address} is the address of the server where the pushsdk sample 
web application is deployed. The SDK includes the pushsdk sample web application that is deployed on a server, such 
as the Apache Tomcat server. The URL must be accessible from the Internet.  Of course, you can also point to your 
own running Push Initiator application instead of the provided pushsdk sample one.
8. Click the Launch App on New Push check box if you want to start the sample application if it is not already 
running when a new push message arrives. Leave the check box unchecked if you do not want to start the sample 
application when a new push message arrives. Note that if the check box is left unchecked and the sample application 
is not running when a new push message arrives, that push message will never be received by the application (even if 
you manually start the application yourself later on).
9. Click Save.
10. Tap the Register action at the bottom of the device screen.
11. If you had previously checked the Subscribe with Push Service SDK check box, 
    you will be required to enter a username and password. These will be mapped, after authentication, 
    to a subscriber ID in your Push Initiator.  If the Subscribe with Push Service SDK check box 
    had not been checked, then it will jump straight into the register (i.e. create channel) operation.
12. You're all set to receive pushes!


