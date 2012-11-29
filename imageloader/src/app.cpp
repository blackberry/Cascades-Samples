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

/*
 * Legal Information for images used.
 *
 * Image: http://upload.wikimedia.org/wikipedia/commons/6/62/Peace_riding_in_a_triumphal_chariot_Bosio_Carrousel_-_2012-05-28.jpg
 * Attribution: By PierreSelim (Own work) [CC-BY-3.0 (http://creativecommons.org/licenses/by/3.0)], via Wikimedia Commons
 *
 * Image: http://upload.wikimedia.org/wikipedia/commons/a/af/Crepuscular_rays_with_reflection_in_GGP.jpg
 * Attribution: By Brocken Inaglory (Own work) [CC-BY-SA-3.0 (http://creativecommons.org/licenses/by-sa/3.0) or GFDL (http://www.gnu.org/copyleft/fdl.html)], via Wikimedia Commons
 *
 * Image: http://upload.wikimedia.org/wikipedia/commons/2/2a/Anodorhynchus_hyacinthinus_-Hyacinth_Macaw_-side_of_head.jpg
 * Attribution: By Randy (originally posted to Flickr as Hyacinth) [CC-BY-2.0 (http://creativecommons.org/licenses/by/2.0)], via Wikimedia Commons
 *
 * Image: http://upload.wikimedia.org/wikipedia/commons/2/29/Bataille_Waterloo_1815_reconstitution_2011_cuirassier.jpg
 * Attribution: By Myrabella (Own work) [CC-BY-SA-3.0 (http://creativecommons.org/licenses/by-sa/3.0)], via Wikimedia Commons
 *
 * Image: http://upload.wikimedia.org/wikipedia/commons/e/ec/Armadillo_Aerospace_Pixel_Hover.jpg
 * Attribution: By Armadillo Aerospace/Matthew C. Ross [CC-BY-3.0-us (http://creativecommons.org/licenses/by/3.0/us/deed.en)], via Wikimedia Commons
 *
 * Image: http://upload.wikimedia.org/wikipedia/commons/f/f5/A_sculpture_at_the_entrance_to_the_palace_of_Versailles.jpg
 * Attribution: By Chris Robinson [CC-BY-2.0 (http://creativecommons.org/licenses/by/2.0)], via Wikimedia Commons
 *
 * Image: http://upload.wikimedia.org/wikipedia/commons/6/6d/Firehole_river_at_Upper_Geyser_Basin-2008-june.jpg
 * Attribution: By Brocken Inaglory (Own work) [CC-BY-SA-3.0 (http://creativecommons.org/licenses/by-sa/3.0) or GFDL (http://www.gnu.org/copyleft/fdl.html)], via Wikimedia Commons
 *
 * Image: http://upload.wikimedia.org/wikipedia/commons/7/7c/Peugeot_206_WRC.jpg
 * Attribution: By Christopher Batt ( http://www.flickr.com/people/christopherbatt/ ) [CC-BY-SA-2.0 (http://creativecommons.org/licenses/by-sa/2.0)], via Wikimedia Commons
 *
 * Image: http://upload.wikimedia.org/wikipedia/commons/9/97/Toda_Hut.JPG
 * Attribution: Pratheep P S [CC-BY-SA-2.5 (http://creativecommons.org/licenses/by-sa/2.5)], via Wikimedia Commons
 *
 * Image: http://upload.wikimedia.org/wikipedia/commons/d/dc/Marriott_Center_1.JPG
 * Attribution: By Mark A. Philbrick (Brigham Young University) [CC-BY-SA-3.0 (http://creativecommons.org/licenses/by-sa/3.0)], via Wikimedia Commons
 */

#include "app.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

/**
 * This app demonstrates the techniques how to use multi-threads to handle network activities
 * and sync the results with the main thread.
 *
 * In this app, you will learn:
 *     -how to use QNetworkAccessManager to perform asynchronous network requests.
 *     -how to use QThread to perform time-consuming operations in their own threads.
 *
 */
//! [0]
App::App(QObject *parent)
    : QObject(parent)
    , m_model(new QListDataModel<QObject*>())
{
    // Register custom type to QML
    qmlRegisterType<ImageLoader>();

    m_model->setParent(this);

    // Fill the model with data
    m_model->append(new ImageLoader("http://upload.wikimedia.org/wikipedia/commons/6/62/Peace_riding_in_a_triumphal_chariot_Bosio_Carrousel_-_2012-05-28.jpg", this));
    m_model->append(new ImageLoader("http://upload.wikimedia.org/wikipedia/commons/a/af/Crepuscular_rays_with_reflection_in_GGP.jpg", this));
    m_model->append(new ImageLoader("http://upload.wikimedia.org/wikipedia/commons/2/2a/Anodorhynchus_hyacinthinus_-Hyacinth_Macaw_-side_of_head.jpg", this));
    m_model->append(new ImageLoader("http://upload.wikimedia.org/wikipedia/commons/2/29/Bataille_Waterloo_1815_reconstitution_2011_cuirassier.jpg", this));
    m_model->append(new ImageLoader("http://upload.wikimedia.org/wikipedia/commons/e/ec/Armadillo_Aerospace_Pixel_Hover.jpg", this));
    m_model->append(new ImageLoader("http://upload.wikimedia.org/wikipedia/commons/f/f5/A_sculpture_at_the_entrance_to_the_palace_of_Versailles.jpg", this));
    m_model->append(new ImageLoader("http://upload.wikimedia.org/wikipedia/commons/6/6d/Firehole_river_at_Upper_Geyser_Basin-2008-june.jpg", this));
    m_model->append(new ImageLoader("http://upload.wikimedia.org/wikipedia/commons/7/7c/Peugeot_206_WRC.jpg", this));
    m_model->append(new ImageLoader("http://upload.wikimedia.org/wikipedia/commons/9/97/Toda_Hut.JPG", this));
    m_model->append(new ImageLoader("http://upload.wikimedia.org/wikipedia/commons/d/dc/Marriott_Center_1.JPG", this));

    // Create the UI
    QmlDocument* qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_app", this);

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}
//! [0]

//! [1]
void App::loadImages()
{
    // Call the load() method for each ImageLoader instance inside the model
    for (int row = 0; row < m_model->size(); ++row) {
        qobject_cast<ImageLoader*>(m_model->value(row))->load();
    }
}
//! [1]

bb::cascades::DataModel* App::model() const
{
    return m_model;
}
