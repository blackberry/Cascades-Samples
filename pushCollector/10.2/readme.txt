Push Collector

========================================================================
The 10.2 folder contains three projects. The projects are bundled together
into one .bar file to make the overall Push Collector application.

- pushCollector: This project is the UI for our sample application, and 
  contains the code which is responsible for configuring our application,
  creating and destroying channels, and displaying push messages.

- pushCollectorCommon: This project builds a shared object library that
  contains common code used by both the UI part and the headless part of 
  our sample application.

- pushCollectorHeadless: This project is the headless application that runs 
  in the background. It contains the logic to create and destroy channels,
  and receive and accept push messages.