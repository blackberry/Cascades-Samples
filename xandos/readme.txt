XandOs

========================================================================
Sample Description.

In this example we'll learn how to create a UI app that communicates with a headless service running in the background.

Note: This sample requires the xandosdroid code to be synced and build in
the same location as xandos. This has to be done prior to deploying xandos
, otherwise it will fail at the packaging stage due to missing xandosdroid binary.
The reason being is that this sample packages
xandosdroid in the same bar file. Do not try to deploy xandosdroid as a
standalone sample, it could cause some xandos sample functionality badness.

!!!!!!!!!!!Do not deploy xandosdroid as standalone sample!!!!!!!!!!!!!!!!!!!!!!!!


You will learn how to:
- How to launch background service and communicated with that service

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
8. The application will now install and launch on your device. If not, you might
   have to set up your environment:
   http://developer.blackberry.com/native/documentation/getting_started/setting_up.html
