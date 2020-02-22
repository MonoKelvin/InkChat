import QtQuick 2.14

/** 高级列表控件，提供下拉刷新功能。但不可以定义header了（下拉控件即为header） */
ListView {
    id: listView

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
    // 加载成功显示的文字
    property string loadedString: loadingString
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

    onLoadStateChanged: {
        switch (loadState) {
        case AdvancedList.Ready:
            headerItem.text = refreshString
            break
        case AdvancedList.StartLoad:
            headerItem.text = releaseString
            break
        case AdvancedList.Loading:
            headerItem.text = loadingString
            timer.start()
            break
        case AdvancedList.Loaded:
            headerItem.text = loadedString
            loaded()
            break
        case AdvancedList.LoadFailed:
            headerItem.text = loadFailedString
            loadFailed()
            break
        }
    }

    // 上拉加载
    header: Text {
        id: loadMoreText
        color: appTheme.primaryColor1
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: appTheme.smallTextSize
        width: listView.width
        height: listView.contentY < 0 ? -listView.contentY : 0
        text: refreshString
    }
    headerPositioning: ListView.PullBackHeader

    onContentYChanged: {
        if (contentY < -loadHeight)
            loadState = AdvancedList.StartLoad
        else if (loadState === AdvancedList.StartLoad)
            loadState = AdvancedList.Loading
    }

    onMovementEnded: {
        if (contentY < -loadHeight) {
            loadState = AdvancedList.Loading
            contentYAni.to = -loadHeight
        } else {
            loadState = AdvancedList.Ready
            contentYAni.to = 0
        }

        contentYAni.start()
    }

    // 使用计时器来触发加载后台的任务
    Timer {
        id: timer
        interval: 5
        repeat: false

        onTriggered: {
            listView.loading()
            listView.loadState = AdvancedList.Ready
        }
    }

    NumberAnimation {
        id: contentYAni
        target: listView
        property: "contentY"
        duration: 200
        running: true
    }
}
