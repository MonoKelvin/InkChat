﻿import QtQuick 2.0
/**
 * - 亮色主题配色表 -
 * 背景色：#ffffff
 * 主蓝色：#4D7BF5
 * 次蓝色：#D9E8FF
 * 主橘色：#FDAA2A
 * 次橘色：#FFEDD5
 * 主红色：#FE2A52
 * 次红色：#FFD6DE
 * 主字色：#3B4151
 * 次字色：#A7ADBD
 * 浅蓝色：#F5F6FA
 * 主绿色：#2AFE75
 *
 * - 字体 -
 * 字体： 微软雅黑
 * 主要字体样式：14px 主字色
 * 次要字体样式：12px 次字色
 * 标题字体样式：16px 次字色 加粗
 * 用户名样式：16px 主字色
 *
 * !间距全用 5的倍数，控件大小全部偶数
 *
 * - 间距 -
 * 小间距：5px
 * 窄间距：10px
 * 标准间距：15px
 * 大间距：20px
 *
 * - 头像 -
 * 尺寸：46px * 46px
 *
 * - 按钮 -
 * 圆形按钮：40px * 40px
 * 长方形按钮：auto * 36px
 * 导航按钮：40px * 40px
 *
 * - 图标 -
 * 标准图标：16px * 16px
 * 消息控件中特殊图标：26px * 26px
 *
 * - 图标样式 -
 * 未激活样式：次字色
 * 激活样式：主蓝色
 * 圆形图标按钮激活：原有色加深
 *
 * - 圆角 -
 * 标准圆角：6px
 * 聊天气泡圆角：10px
 *
 * !设置固定宽使用：min-width + max-width = x
 * !单纯使用width无效，height也是一样
 *
 */

QtObject {
    property color primaryColor1: "#4D7CFE"
    property color primaryActiveColor1: Qt.darker(primaryColor1, 1.3)
    property color subColor1: Qt.lighter(primaryColor1, 1.6)

    property color primaryColor2: "#FFAB2E"
    property color primaryActiveColor2: Qt.darker(primaryColor2, 1.3)
    property color subColor2: Qt.lighter(primaryColor2, 1.6)

    property color primaryColor3: "#ff2b52"
    property color primaryActiveColor3: Qt.darker(primaryColor3, 1.3)
    property color subColor3: Qt.lighter(primaryColor3, 1.6)

    property color primaryColor4: "#2bff75"

    property color mainTextColor: "#333b50"
    property color subTextColor: Qt.lighter(mainTextColor, 2.5)

    property color tintColor: "#f5f6fa"
    property color backgroundColor: "#ffffff"
    property color widgetColor: tintColor
    property color shadowColor: "#1f5a6169"
    property color borderColor: Qt.darker(backgroundColor, 1.2)

    property color leftBubbleColor: primaryColor1
    property color leftBubbleTextColor: backgroundColor
    property color rightBubbleColor: tintColor
    property color rightBubbleTextColor: mainTextColor

    readonly property int tinySpacing: 5
    readonly property int narrowSpacing: 10
    readonly property int stdSpacing: 15
    readonly property int wideSpacing: 20
    readonly property int largeSpacing: 25
    readonly property int extremeSpacing: 40

    readonly property int smallRadius: 4
    readonly property int stdRadius: 6
    readonly property int bigRadius: 10

    readonly property int titleTextSize: 16
    readonly property int stdTextSize: 14
    readonly property int smallTextSize: 12
    readonly property int tinyTextSize: 10

    property color onlineStateColor: primaryColor4;
    property color offlineStateColor: subTextColor;
    property color busyStateColor: primaryColor2;

    property int stdWidgetHeight: 32
    property int stdBadgeHeight: 18

    function getDefaultUrl() {
        return "qrc:/AppResource/Icon/default_avatar.png";
    }
}