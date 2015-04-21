IDS_QML_Sample

========================================================================
Sample Description:

 The IDS_QML_Sample sample is an application that is designed to introduce you to a
 number of common development practices that you can use in your own
 applications related to Identity.

 When you run the application, the application displays a personalized welcome line
 at the top of the screen using the user's information from their BlackBerry ID
 account. The button presses are counted and stored in the user's BlackBerry
 Profile so that it is available to this application on all devices that the user
 is logged into.

 Feature summary
 - Display number of times the button has been pressed across all devices
 the BlackBerry ID user is logged into and running this application.
 - Personalized message using the user's first name, or screen name if they
 do not have a first name set in their BlackBerry ID account.
 - Status of what is happening displayed at the bottom of the screen.
 - Each time the button is pressed, the value is updated in the user's 
 BlackBerry Profile, and can be retrieved and updated from all devices that 
 the same user is logged into using this application.


========================================================================
Requirements:

 - BlackBerry® 10 Native SDK
 - One of the following:
   - BlackBerry® 10 device
   - BlackBerry® 10 simulator

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
8. The application will now install and launch on your device. If not, you might
   have to set up your environment: 
   http://developer.blackberry.com/native/documentation/getting_started/setting_up.html
