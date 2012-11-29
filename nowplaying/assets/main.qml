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

import bb.cascades 1.0
import bb.multimedia 1.0

Page {
    Container {
        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top

            text: qsTr("The Handyman's Lament")
            textStyle {
                base: SystemDefaults.TextStyles.BigText
                color: Color.Gray
            }
        }

//! [0]
        ImageButton {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            defaultImageSource: player.mediaState == MediaState.Started ? "asset:///images/stop_button.png" :
                                player.mediaState == MediaState.Paused  ? "asset:///images/pause_button.png" :
                                                                          "asset:///images/play_button.png"

            onClicked: {
                if (player.mediaState == MediaState.Started)
                    nowPlaying.revoke()
                else
                    nowPlaying.acquire()
            }
        }
//! [0]

//! [1]
        ProgressBar {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            duration: nowPlaying.duration
            position: nowPlaying.position
        }
//! [1]
    }

    attachedObjects: [
//! [2]
        MediaPlayer {
            id: player

            sourceUrl: "music/song.mp3"
        },
//! [2]

//! [3]
        NowPlayingConnection {
            id: nowPlaying

            duration: player.duration
            position: player.position
            iconUrl: "asset:///images/music.png"
            mediaState: player.mediaState

            onAcquired: {
                player.play()

                nowPlaying.mediaState = MediaState.Started
                nowPlaying.metaData = player.metaData
            }

            onPause: {
                player.pause()
            }

            onPlay: {
                player.play()
            }

            onRevoked: {
                player.stop()
            }
        }
//! [3]
    ]
}

