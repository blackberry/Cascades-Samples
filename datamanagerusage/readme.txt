DataManagerUsage

========================================================================
Sample Description.

The Data Manager example shows how the user can make use of the provided default Sql query classes to use as the data source for the ListView. The difference between using these and writing your own DataModel is that they make use of a cache manager and the integrated properties that improve the speed and efficiency of the query results. This in turn allows for the smooth scrolling of the ListView when dealing with large amounts of data.

You will learn how to:
- How to use sql as the data source for ListView
- Usage of the convenience classes such as SqlDataQuery and AsyncDataModel for
  asynchronous data access.
- How to keep the display in sync with data changes(add's, updates, deletes, etc)
  using the SqlDataQuery properties keyColumn and revisionColumn.
- How to improve performance and display accurracy of large data sets.
 
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

