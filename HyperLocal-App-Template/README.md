# HyperLocal App Template

This is a template application for building apps out of websites. It has built in functionality for BlackBerry Maps, BBM, BBM Channels, Twitter, Facebook, and Foursquare.

The application is customizable via the *assets/settings.json* file.

If you need to override any of the website's CSS, you can do so in the *settings.css* file just as you would for any webapp, or website.

**Author(s)**

* [Chad Tetreault](http://www.twitter.com/chadtatro)
* [Erik Oros](http://www.twitter.com/waterlooerik)
* [Isaac Gordezky](https://github.com/igordezky)

**Icons**

The [Liz Myers](http://www.myersdesign.com) Icon set and are [licensed](http://creativecommons.org/licenses/by/3.0/) under the CC-BY-3.0 license.


###How to configure

Setup is pretty simple if you're farmiliar with JSON, and even if you're not. :)

All of the settings for the app are configured via the **assets/settings.json
** file. 

    "name": "BlackBerry Dev",
    "bbmUUID": "712f3fb1-2be6-48e2-83e1-321e84772ed3",
    "bbmChannel": "C000D71DD",
    "facebook": "103837544053",
    "twitter": "@BlackBerryDev",
    "shareText": "Check out the HyperLocal hybrid app template for #BlackBerry10! http://bit.ly/L5XO6g",

    "locations": [
        {
            "name": "Venue 1",
            "phone": "519-555-5551",
            "address": "2220 University Ave E, Waterloo, Ontario",
            "foursquare": "4db08eb293a061576843c452",
            "coords": {
                "lat": "43.4635577",
                "lng": "-80.52239550000002"
            }
        },
        {
            "name": "Venue 2",
            "phone": "519-555-5552",
            "address": "2220 University Ave E, Waterloo, Ontario",
            "foursquare": "4db08eb293a061576843c452",
            "coords": {
                "lat": "43.4024011",
                "lng": "-80.32614699999999"
            }
        },
        {
            "name": "Venue 3",
            "phone": "519-555-5553",
            "address": "2220 University Ave E, Waterloo, Ontario",
            "foursquare": "4db08eb293a061576843c452",
            "coords": {
                "lat": "43.0245829",
                "lng": "-81.28965470000003"
            }
        }
    ],


    "tabs": [
        {
            "name": "Developer Blog",
            "icon": "asset:///images/blog.png",
            "url": "http://devblog.blackberry.com/",
            "actions": []
        },
        {
            "name": "Native Development",
            "icon": "asset:///images/dev.png",
            "url": "https://developer.blackberry.com/native/",
            "actions": []
        }
    ],

    "settings": {
        "orientation": "portrait",
        "pinchToZoom": "false"
    }
    
**Here's a breakdown of each section:**

##General

`name`: Business name, and the name of the application on the device

`bbmUUID`: A unique UUID used for BBM registration

`bbmChannel`: The ID for BBM Channel

`facebook`: The ID for Facebook page

`twitter`: Twitter handle

`shareText`: The text that will be used when the user "shares" the app socially


##Settings

`orientation`: Which way you want the orientation locked. **portrait** or **landscape**

`pinchToZoom`: Enable pinch-to-zoom. **true** or **false**

##Locations

This lastest version accepts multiple venues! Just add them to the locations array.

`name`: Venue name

`phone`: Venue phone number

`address`: Venue address

`foursquare`: Venue Foursquare ID

`coords`: Venue Lattitude/Longitude

##Tabs

`name`: Name of the Tab

`icon`: Icon for the Tab

`url`: URL the Tab will load

`actions`: Not currently in use, but will be for specifying unique actions for individual Tabs.

##Configuring the app

There are a few things you'll need to modify in the **bar-descriptor.xml** file to customize it.

- Package Name
- Application Name
- Application Description

##Utilities to make things easier

There are a few websites we frequent to make building these apps easier. 

- [JSON Lint](www.jsonlint.com) - After configuring the settings.json file, paste it into JSONLint.com and make sure it's valid. If it's not, the app will not load.
- [Find my Facebook ID](http://findmyfacebookid.com/) - This website retrieves a Facebook Page ID from a URL.
- [GPS Coords from Address](http://www.characterscounter.com/get-coordinates-google-maps.php) - Type in an address, and get its GPS Coordinates.
- [UUID Generator](http://www.famkruithof.net/uuid/uuidgen) - Easily generate UUIDs for use with BBM.


##Application Assets

Here's a list of assets that you'll need to create an application, and the resolutions they should be.

####For BlackBerry World

Vendor Icon:

- Represents the company and/or brand
- 480x480


Application Icon:

- Shows up in the BlackBerry World storefront
- 480x480

Feature Image:

- Shows up in BlackBerry World during times when the application is "featured"
- 1920x1186



####For the application

Splash Screens:

- Displayed when launching the app
- 720x720
- 768x1280
- 720x1280


Application Icon:

- Displayed on the home screen of the phone
- 114x114

Window Covers

- Displayed when the app is minimized
- 319x437
- 344x396
- 310x211

## How To Build

1. Clone the repo to your computer.
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
   have to [set up your environment](http://developer.blackberry.com/cascades/documentation/getting_started/setting_up.html).


## More Info

* [BlackBerry Cascades & NDK](https://developer.blackberry.com/native) - Downloads, Getting Started guides, samples, code signing keys.
* [Native Development Guides](https://developer.blackberry.com/native/documentation/cascades/)
* [BlackBerry Native Development Forums](https://developer.blackberry.com/native/documentation/cascades/)


## Disclaimer

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.








