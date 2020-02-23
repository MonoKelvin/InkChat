import QtQuick 2.14

/** 高级列表控件，提供下拉刷新功能。*/
ListView {
    id: listView
    topMargin: 0

    enum ELoadState {
        Ready,
        StartLoad,
        Loading,
        Loaded,
        LoadFailed
    }

    // 刷新文字
    property string refreshString: qsTr("下拉刷新")
    // 释放时显示的文字
    property string releaseString: qsTr("释放加载更多")
    // 加载时显示的文字
    property string loadingString: qsTr("正在加载...")
    // 加载失败时显示的文字
    property string loadFailedString: qsTr("加载失败！")

    // 最大加载时下拉文字高度，只有超出该值才会进行加载
    property int loadHeight: 40
    // 超时间隔，毫秒为单位，当超过这个值都没有加载完成就加载失败
    //property int timeout: loadFailedString
    // 加载状态
    property int loadState: AdvancedList.Ready

    // 正在加载信号
    signal loading
    // 加载完成信号
    signal loaded
    // 加载失败信号
    signal loadFailed

    // 更新加载文本的内容
    function _updateLoadContent() {
        if (contentY < -loadHeight - topMargin)
            loadState = AdvancedList.StartLoad
        else if (contentY < -topMargin && loadState === AdvancedList.StartLoad)
            loadState = AdvancedList.Loading
        else if (dragging)
            loadState = AdvancedList.Ready
    }

    Component.onCompleted: {
        contentYChanged.connect(_updateLoadContent)
    }

    onLoadStateChanged: {
        switch (loadState) {
        case AdvancedList.Ready:
            loadText.text = refreshString
            break
        case AdvancedList.StartLoad:
            loadText.text = releaseString
            break
        case AdvancedList.Loading:
            loadText.text = loadingString

            contentYAni.to = -loadHeight - topMargin
            contentYAni.start()

            if (footerItem)
                footerItem.y = height - footerItem.height

            timer.start()
            break
        case AdvancedList.Loaded:
            contentY = -topMargin
            loaded()
            loadState = AdvancedList.Ready
            break
        case AdvancedList.LoadFailed:
            loadText.text = loadFailedString
            loadFailed()
            break
        }
    }

    Text {
        id: loadText
        color: appTheme.subTextColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: appTheme.smallTextSize
        width: listView.width
        height: contentY < -topMargin ? -contentY - topMargin : topMargin
        text: refreshString
        y: topMargin
        z: -1
        visible: contentY < -topMargin
    }

    boundsBehavior: Flickable.DragOverBounds

    // 使用计时器来触发加载后台的任务
    Timer {
        id: timer
        interval: 5
        repeat: false

        onTriggered: {
            listView.loading()
        }
    }

    NumberAnimation {
        id: contentYAni
        property: "contentY"
        duration: 200
    }
}
