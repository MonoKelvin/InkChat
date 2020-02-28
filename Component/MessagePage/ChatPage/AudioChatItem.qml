import QtQuick 2.14
import ChatObject 1.0
import QtMultimedia 5.14
import "qrc:/Element"
import "qrc:/js/js/Utility.js" as Utility

ChatItem {
    id: root
    property alias audioSource: audio.source

    // 播放信号
    signal play
    // 停止信号，包括手动停止和自动播放玩停止
    signal stop

    content: Rectangle {
        id: bubbleRect
        radius: appTheme.bigRadius
        color: appTheme.subColor2
        width: 190
        height: 50
        clip: false

        Rectangle {
            id: corner
            y: 0
            width: bubbleRect.radius
            height: bubbleRect.radius
            color: bubbleRect.color
        }

        Item {
            id: audioContainer
            anchors.fill: parent

            function setWavePlay() {
                for (var i = 0; i < waveRepeater.count; i++)
                    waveRepeater.itemAt(i).yScale = Math.max(0.3, Math.random())
            }

            Audio {
                id: audio
                autoLoad: false

                Component.onCompleted: {
                    if (availability == Audio.ResourceMissing)
                        console.log('音频资源无法找到')

                    durationText.text = Utility.timeLengthFromMilliseconds(
                                duration)
                }

                onPositionChanged: {
                    durationText.text = Utility.timeLengthFromMilliseconds(
                                position)
                }

                onPlaybackStateChanged: {
                    if (playbackState === Audio.PlayingState) {
                        controlWaveTimer.start()
                        root.play()
                    } else {
                        controlWaveTimer.stop()
                        root.stop()
                        if (playbackState === Audio.StoppedState)
                            playButton.checked = false
                    }
                }
            }

            Row {
                id: waveContainer
                height: 28
                width: (parent.spacing + 2) * model
                spacing: 2
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: appTheme.stdSpacing

                Timer {
                    id: controlWaveTimer
                    interval: 300
                    repeat: true
                    onTriggered: audioContainer.setWavePlay()
                }

                Repeater {
                    id: waveRepeater

                    model: 24
                    Rectangle {
                        property alias yScale: scaleTransform.yScale

                        width: 3
                        height: 28
                        radius: 1.5
                        color: appTheme.primaryColor2
                        transformOrigin: Item.Center
                        transform: Scale {
                            id: scaleTransform
                            origin.y: 14
                            yScale: Math.max(0.3, Math.random())
                        }

                        Behavior on yScale {
                            NumberAnimation {
                                duration: 300
                            }
                        }
                    }
                }
            }

            Text {
                id: durationText
                font.pixelSize: appTheme.smallTextSize
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: appTheme.stdSpacing
                color: appTheme.primaryColor2
            }
        }

        IconButton {
            id: playButton
            width: 26
            height: 26
            radius: 13
            normalColor: appTheme.primaryColor2
            clickedColor: appTheme.primaryActiveColor2
            anchors.verticalCenter: parent.bottom
            anchors.margins: appTheme.stdSpacing
            z: 20

            checkable: true
            onCheckedChanged: {
                if (checked)
                    audio.play()
                else
                    audio.pause()
            }
        }
    }

    Component.onCompleted: {
        if (chatItem.sender.roleType === ChatObject.Me) {
            corner.anchors.right = bubbleRect.right
            playButton.anchors.right = bubbleRect.right
            durationText.anchors.right = audioContainer.right
            waveContainer.anchors.left = audioContainer.left
        } else {
            corner.anchors.left = bubbleRect.left
            playButton.anchors.left = bubbleRect.left
            durationText.anchors.left = audioContainer.left
            waveContainer.anchors.right = audioContainer.right
        }
    }
}
