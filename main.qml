import QtQuick 2.0
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.0
import "qrc:/Element/"
import "qrc:/Navigation/"

ApplicationWindow {
    id: window
    title: "InkChat"
    width: 1080
    height: 640
    visible: true
    opacity: 1
    color: appTheme.backgroundColor

    minimumWidth: 800
    minimumHeight: 512

    Component.onCompleted: {
        pageContains.showPage("qrc:/MessagePage/MessagePage.qml");
    }

    AppTheme{
        id: appTheme
    }

    Navigation {
        id: navigation
        anchors.left: parent.left
        height: parent.height
        layer.enabled: true
        layer.effect: DropShadow {
            radius: 20.0
            samples: 17
            color: appTheme.shadowColor
            horizontalOffset: 1
        }

        onNavigate: {
            switch(index)
            {
            case 0:
                pageContains.showPage("qrc:/MessagePage/MessagePage.qml");
                break;
            case 1:
                console.log("好友列表正在开发中...");
                break;
            case 2:
                console.log("我的主页正在开发中...");
                break;
            case 2:
                console.log("系统设置正在开发中...");
                break;
            default:
                break;
            }
        }
    }

    Item {
        id: pageContains
        height: parent.height
        width: parent.width - navigation.width
        anchors.left: navigation.right
        z: -2

        property var pages: new Object

        function showPage(pageQrcLocation) {
            showPageTimer.pageQrcLocation = pageQrcLocation;
            showPageTimer.start();
        }

        Timer {
            id: showPageTimer
            interval: 5
            repeat: false
            running: false

            property string pageQrcLocation

            onTriggered: {
                switch(pageQrcLocation)
                {
                case "": break;
                    // case "notSupport": materialUI.showSnackbarMessage( "此功能暂未开放" ); break;
                default:
                    if (!(pageQrcLocation in pageContains.pages))
                    {
                        var component = Qt.createComponent(pageQrcLocation);

                        if (component.status === Component.Ready) {
                            var page = component.createObject(pageContains);
                            page.anchors.fill = pageContains;
                            pageContains.pages[pageQrcLocation] = page;
                        }
                    }

                    for (var key in pageContains.pages)
                    {
                        pageContains.pages[key].visible = false;
                    }

                    pageContains.pages[pageQrcLocation].visible = true;

                    break;
                }
            }
        }
    }
}
