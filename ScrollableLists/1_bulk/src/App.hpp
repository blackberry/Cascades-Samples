/* Copyright (c) 2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef APP_H
#define APP_H

#include <QtCore/QObject>
#include <bb/cascades/Application>
#include <bb/cascades/QListDataModel>

using namespace bb::cascades;

namespace bb
{
namespace cascades
{
class AbstractPane;
class ListView;
class NavigationPane;
class Page;
}
}

/**
 * Scrollbar application
 *
 */
class App : public QObject
{
    Q_OBJECT

public:
    App();

private slots:

    /**
     * Slot function for selection in the cpp list.
     *
     * @param indexPath Index path to the item.
     * @param active True if the new state is selected, otherwise false.
     */
    void onSelectionChanged(const QVariantList indexPath, bool selected);

    /**
     * Slot function that receives signals when the top Control has changed in the
     * NavigationPane.
     */
    void onTopChanged(bb::cascades::Page* page);

private:

    /**
     * Function that finds the container in the QML that the list view
     * will be added to. Creates the inner containers, and inserts the list
     * view into it.
     */
    void setupCppListView();

    /**
     * Function that initializes the list
     */
    ListView* createCppListView();

    // Model for the list.
    bb::cascades::QVariantListDataModel m_listDataModel;

    // A navigation pane is used to navigate the list
    NavigationPane* m_navPane;

    // The list.
    ListView* m_listView;
};

#endif // ifndef APP_H
