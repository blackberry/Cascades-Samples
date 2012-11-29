/* Copyright (c) 2012 Stuart MacMartin.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef APP_HPP
#define APP_HPP

#include <QtCore/QObject>

#include <bb/cascades/Image>

class App : public QObject
{
    Q_OBJECT

    // The object that should be painted (either file name of 'circle' or 'square')
    Q_PROPERTY(QString object READ object WRITE setObject NOTIFY objectChanged)

    // The number of replicant rows
    Q_PROPERTY(QString rows READ rows WRITE setRows NOTIFY rowsChanged)

    // The number of replicant columns
    Q_PROPERTY(QString columns READ columns WRITE setColumns NOTIFY columnsChanged)

    // The final rendered image
    Q_PROPERTY(bb::cascades::Image image READ image NOTIFY imageChanged)

public:
    App();

Q_SIGNALS:
    // The change notification signals for the properties
    void objectChanged();
    void rowsChanged();
    void columnsChanged();
    void imageChanged();

private:
    // The accessor methods for the properties
    void setObject(const QString &object);
    QString object() const;
    void setRows(const QString &rows);
    QString rows() const;
    void setColumns(const QString &columns);
    QString columns() const;
    bb::cascades::Image image() const;

    // A helper method that is called whenever an input parameter has changed
    void updateImage();

    // A helper method to calculate the size of a single replicant image
    QSize desiredReplicantSize() const;

private:
    QString m_object;
    int m_rows;
    int m_columns;
    bb::cascades::Image m_image;
};

#endif
