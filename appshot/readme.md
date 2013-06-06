basicscreenshot - AppShot
 
================================================================
This small sample illustrates how take programatic screen shots of your running app. It contains AppShot which is a helper class for doing this from e.g. QML:

What needs to be done:

- Add libscreen and libbbdevice (for ScreenSize class) libraries by right
  clicking you project and selecting add Configure -> "Add Library"
- Give the app permission to write to the shared files folder, add
  <permission>access_shared</permission>, to the bar-descriptor.xml
 - Register the AppShot type to use it in qml before creating your qml document
   ex: qmlRegisterType < AppShot > ("com.appshot", 1, 0, "ScreenShot");
 - Register the AppShot type to use it in qml before creating your qml document

C++
 ```
 #include "appshot/appshot.h"
 
 ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{
    // Always register custom types before you create your qml document
    qmlRegisterType < AppShot > ("com.appshot", 1, 0, "AppShot");

    // Create document, root control and set screne
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    app->setScene(root);
}
 ```

QML
 ```
import bb.cascades 1.0
import com.appshot 1.0

Page {
   Container {
       Button {
           text: "App Shot"
           onClicked: {
               appShot.captureScreen("test.jpg")
           }

       }
   }

   attachedObjects: AppShot {
       id: appShot
   }
}
```

================================================================

========================================================================
Requirements:

BlackBerry 10 Native SDK 

========================================================================
Running the example:

1. From the Sample apps page, download and extract one of the sample applications.
2. Launch BlackBerry 10 Native SDK, and from the File menu, select Import.
3. Expand General, and select Existing Projects into Workspace. Click Next.
4. Browse to the location where you extracted the sample file, and then click OK.
5. The sample project should display in the Projects section. 
   Click Finish to import the project into your workspace.
6. In the Project Explorer pane, Right-click the project (for example hellocascades) 
   and select Build Project.
7. In the Project Explorer pane, Right-click the project (for example hellocascades) 
   and select Run As > BlackBerry C/C++ Application.
8. The application will now install and launch on your device. If it doesent you might
   have to set up your environment: 
   http://developer.blackberry.com/cascades/documentation/getting_started/setting_up.html