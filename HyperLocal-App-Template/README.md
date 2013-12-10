# HyperLocal App Template

This is a template application for building apps out of websites. It has built in functionality for BlackBerry Maps, BBM, BBM Channels, Twitter, Facebook, and Foursquare.

The application is customizable via the *settings.json* file, or by using the web-form.

If you need to override any of the website's CSS, you can do so in the *settings.css* file.

The web-form outputs a *settings.json*, and a *settings.css* file (if needed) which you simply place in the assets folder of your project.

**Roadmap**

1. Major code clean-up
2. Dependency removal(s)
3. Better documentation

**Applies To**

* [HyperLocal Template](https://github.com/blackberry/#)

**Author(s)**

* [Chad Tetreault](http://www.twitter.com/chadtatro)

**Web-Form Dependencies**

1. [underscore.js](http://underscorejs.org/) is [licensed](https://github.com/jashkenas/underscore/blob/master/LICENSE) under the MIT license.
2. [bootstrap](https://github.com/twbs/bootstrap) is [licensed](https://github.com/twbs/bootstrap/blob/master/LICENSE-MIT) under the MIT license.
3. [filesaver.js](https://github.com/eligrey/FileSaver.js) is [licensed](https://github.com/eligrey/FileSaver.js/blob/master/LICENSE.md) under the MIT license.
4. [jQuery](http://jquery.com/) is [licensed](https://github.com/jquery/jquery/blob/master/MIT-LICENSE.txt) under the MIT license.

**Icons**

The [Liz Myers](http://www.myersdesign.com) Icon set and are [licensed](http://creativecommons.org/licenses/by/3.0/) under the CC-BY-3.0 license.

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
