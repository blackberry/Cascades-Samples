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
#include "kakelapp.h"

#include <bb/cascades/Container>
#include <bb/cascades/Control>
#include <bb/cascades/ImageView>
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>

KakelApp::~KakelApp()
{
}

KakelApp::KakelApp()
{

  // Create and Load the QMLDocument, using build patterns.
  QmlDocument *qml = QmlDocument::create("asset:///main.qml");

  if (!qml->hasErrors()) {

    setNumMoves(0);
    mNumTiles = 4;

    // Set the context property we want to use from inside the QML file. Functions exposed
    // via Q_INVOKABLE will be found with the property and the name of the function.
    qml->setContextProperty("kakel", this);

    // The application Page is created from the QML file.
    mAppPage = qml->createRootObject<Page>();

    findPlayAreaAndInitialize(mAppPage);

    if (mAppPage) {
      // Finally the main scene for the application is set to the Page.
      Application::instance()->setScene(mAppPage);
    }
  }
}

void KakelApp::resetTiles()
{
  // Reset the tiles.
  setNumMoves(0);
  findPlayAreaAndInitialize(mAppPage);
}

void KakelApp::setNumTiles(int in)
{
  // This function is called from the QML file.
  // The integer passed in to checked for range, in case we're outside the grid.
  if (in > 0 && in < 32) {
    mNumTiles = in;
  }

}

void KakelApp::checkAndChangeTile(int i, int ii, const QString &tileName)
{
  // Check the tile.
  if (i != -1 && ii != -1) {
    if (i != mNumTiles) {
      if (ii != mNumTiles) {

        // Get the object name (actually the image name which is easy to identify).
        QVariant v = mPlayField[i][ii]->objectName();

        if (v.canConvert<QString>()) {
          QString objName = v.value<QString>();

          if (objName.contains(tileName, Qt::CaseInsensitive)) {
            mPlayField[i][ii]->setObjectName("isTurned");
            mPlayField[i][ii]->setImage(Image(QUrl("asset:///images/empty.png")));

            // If we find a tile to change, we check the surrounding tiles also.
            checkNeighboursAndChange(i, ii, tileName);
          }
        }
      }
    }
  }
}

void KakelApp::checkNeighboursAndChange(int i, int ii, const QString &tileName)
{
  // Check the neighbors of the selected tile.
  checkAndChangeTile(i - 1, ii, tileName);
  checkAndChangeTile(i + 1, ii, tileName);
  checkAndChangeTile(i, ii - 1, tileName);
  checkAndChangeTile(i, ii + 1, tileName);
}

void KakelApp::checkPlayAreaAndChange(const QString &msg)
{
  // Loop through the whole play area to check the neighbors of this tile.
  if (mPlayAreaContainer) {
    for (int i = 0; i < mNumTiles; i++) {
      for (int ii = 0; ii < mNumTiles; ii++) {
        if (mPlayField[i][ii]->objectName() == "isTurned") {
          checkNeighboursAndChange(i, ii, msg);
        }
      }
    }
  }
}

void KakelApp::findPlayAreaAndInitialize(Page *appPage)
{
  // Find where we want to add our tiles.
  mPlayAreaContainer = appPage->findChild<Container*>("playArea");

  // Reset the play area by removing all the tile images.
  mPlayAreaContainer->removeAll();

  // As long as nothing is broken, we continue.
  if (mPlayAreaContainer) {

    // Calculate the size of the tiles
    mWantedSize = mPlayAreaContainer->preferredWidth() / mNumTiles;

    // Iterate through all the tiles.
    for (int i = 0; i < mNumTiles; i++) {
      Container* columnContainer = new Container();

      for (int ii = 0; ii < mNumTiles; ii++) {

        // Create an ImageView with a randomized tile image.
        int randCol = (qrand() % 6) + 1;
        ImageView *imageView =
            ImageView::create().bottomMargin(0).topMargin(0).leftMargin(0).rightMargin(0);
        imageView->setPreferredSize(mWantedSize, mWantedSize);

        QString imageSource = QString("asset:///images/tile%1.png").arg(randCol);
        imageView->setImage(Image(imageSource));

        // We are connecting all our tiles to the same slot, we can later identify them by sender().
        connect(imageView, SIGNAL(touch(bb::cascades::TouchEvent *)), this,
            SLOT(onTileTouch(bb::cascades::TouchEvent *)));

        // Keep track of the ImageView.
        mPlayField[i][ii] = imageView;
        mPlayField[i][ii]->setObjectName(imageSource);
        columnContainer->add(imageView);
      }

      mPlayAreaContainer->add(columnContainer);
    }
  }
}

void KakelApp::onTileTouch(bb::cascades::TouchEvent *event)
{
  // If we have a touch-down on a tile.
  if (event->isDown()) {

    // Find who sent it.
    ImageView* senderImage = dynamic_cast<ImageView*>(sender());

    if (senderImage != NULL) {

      // If that container is real, we iterate through all the tiles.
      for (int i = 0; i < mNumTiles; i++) {
        for (int ii = 0; ii < mNumTiles; ii++) {

          // Try and find our saved pointer to a tile.
          if (mPlayField[i][ii] == senderImage) {

            // Get the object name so we can check if it's turned.
            QVariant v = mPlayField[i][ii]->objectName();

            if (v.canConvert<QString>()) {
              QString objName = v.value<QString>();

              // If it's not already turned, let's check the surrounding areas.
              if (!objName.contains("isTurned", Qt::CaseInsensitive)) {
                // Change the object's name and change image to show an empty tile.
                mPlayField[i][ii]->setObjectName("isTurned");
                senderImage->setImage(Image(QUrl("asset:///images/empty.png")));

                // Call the check-around function.
                checkPlayAreaAndChange(objName);

                // A move was made, we're only adding valid moves.
                mNumMoves++;
                emit numMovesChanged();
              }
            }
          }
        } // Inner loop
      } // Outer loop
    } // if statement
  } // if statement
} // onTileTouch

void KakelApp::setNumMoves(const int i)
{
  if (i != mNumMoves) {
    mNumMoves = i;
    emit numMovesChanged();
  }
}

int KakelApp::numMoves()
{
  return mNumMoves;
}
