/* Copyright (c) 2012, 2013  Stuart MacMartin.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <bb/cascades/Application>

#include <QtCore/QLocale>
#include <QtCore/QTranslator>

#include "app.h"

using ::bb::cascades::Application;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    QTranslator translator;
    const QString locale_string = QLocale().name();
    const QString filename = QString::fromLatin1("scratchpad_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    App mainApp;

    return Application::exec();
}
