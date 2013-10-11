XandOs

========================================================================
Sample Description.

In this example we'll learn how to create a UI app that communicates with a long-running headless service in the background.

Note: This sample requires the headlesservice code to be synced and build in
the same location as headlesserviceui since they are relative to one anothers location.
This has to be done prior to deploying headlesserviceui , otherwise it will fail at the
packaging stage due to missing headlesservice binary. The reason being is that this sample
packages headlesservice in the same bar file. Do not try to deploy headlesservice as a
standalone sample, it could cause some headlesserviceui sample functionality badness.

!!!!!!!!!!!Do not deploy headlesservice as standalone sample!!!!!!!!!!!!!!!!!!!!!!!!


You will learn how to:
- How to launch background service at system startup or first installation and communicated with that service
  using QString instance.

========================================================================
Requirements:

BlackBerry 10.2 Native SDK

========================================================================
Running the example:

1. Clone the Sample repository.
2. Launch BlackBerry 10.2 Native SDK, and from the File menu, select Import.
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
