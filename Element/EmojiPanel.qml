import QtQuick 2.14
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.14 as Controls2_14
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.0

Controls2_14.Popup {
    id: root
    z: 200
    width: 0
    height: 192
    modal: true
    dim: false
    padding: 0

    property bool isFixed: false

    // 当点击选中一个表情符号时
    signal selected(string emoji)

    // TODO: 从文件加载
    readonly property var yanList: [{
            "name": qsTr("常用"),
            "emoji": ["O(∩_∩)O", "（。＾▽＾）", "(￣▽￣)\"", "(●ˇ∀ˇ●)", "(。・∀・)ノ", "(~￣▽￣)~", "(*^_^*)", "o(*￣▽￣*)o", "(ToT)", "(′д｀σ)σ", "(*￣3￣)╭", "§(*￣▽￣*)§", "~(￣▽￣)~*", "(≧∇≦)ﾉ", "（*＾-＾*）", "(*^▽^*)", "`(*>﹏<*)′", "(✿◡‿◡)", "<(￣︶￣)↗", "φ(゜▽゜*)♪", "ψ(｀∇´)ψ", "φ(*￣0￣)", "( $ _ $ )", "(ง •_•)ง"]
        }, {
            "name": qsTr("卖萌"),
            "emoji": ["(￣y▽,￣)╭ ", "(o|o) ", "(^人^)", "§(*￣▽￣*)§", "ψ(._. )>", "(/▽＼)", "(o′┏▽┓｀o) ", "◑﹏◐", "(○｀ 3′○)", "(ಥ _ ಥ)", "(⓿_⓿)", "(❤´艸｀❤)", "(ง •_•)ง", "（〃｀ 3′〃）", "(●'◡'●)", "ˋ( ° ▽、° ) ", "(*/ω＼*)", "=￣ω￣=", "(～o￣3￣)～", "(✿◕‿◕✿)", "╰(￣ω￣ｏ)", "~(￣▽￣)~*", "ԅ(¯﹃¯ԅ)", "(っ´Ι`)っ", "(～o￣3￣)～", "(❤ ω ❤)", "(★ ω ★)", "ヽ(￣ω￣〃)ゝ", "*(੭*ˊᵕˋ)੭*", "U•ェ•*U", "（￣。。￣）", "(°°)～", "o(=•ェ•=)m", "\(0^◇^0)/", "(￣(工)￣)", "(:◎)≡", "^(*￣(oo)￣)^", "( ¯(∞)¯ )"]
        }, {
            "name": qsTr("难过"),
            "emoji": ["/_ \\", "（；´д｀）", "＞﹏＜", "(っ °Д °;)っ", "(￣ ‘i ￣;)", "( *^-^)ρ", "(*╯^╰)", "＞︿＜", "o(￣┰￣*)ゞ", "(ノへ￣、)", "(ノへ￣、)", "<(＿　＿)>", "(#｀-_ゝ-)", "（＞人＜；）", "{{{(>_<)}}}", "≡(▔﹏▔)≡", "(#_<-)>)", "⊙﹏⊙", "/(ㄒoㄒ)/~~", "╯︿╰", "(┬┬﹏┬┬)", "(；′⌒`)", "≧ ﹏ ≦〒▽〒", "(*′д｀)", "(T_T)", "(≧﹏ ≦)", "(′д｀ )", "<( _ _ )>", "o(TヘTo)", "~~>_<~~", "┗( T﹏T )┛", "╥﹏╥...", "(。﹏。*)", "┭┮﹏┭┮X﹏X"]
        }]

    readonly property var emojiList: [{
            "name": qsTr("常用"),
            "emoji": ['😀', '😁', '😂', '🤣', '😃', '😄', '😅', '😆', '😉', '😊', '😋', '😎', '😍', '😘', '😗', '🙂', '🤔', '😐', '🤨', '😶', '🙄', '😏', '😣', '😥', '😮', '😫', '😴', '😛', '😒', '🙃', '😲', '🤑', '🙁', '😞', '😟', '😭', '😨', '😡', '😱', '😈']
        }, {
            "name": qsTr("动物"),
            "emoji": ['👽', '🤖', '💩', '🙈', '🙉', '🙊', '🐶', '🐺', '🐱', '🦁', '🐯', '🦒', '🦊', '🐮', '🐷', '🐗', '🐭', '🐹', '🐰', '🐻', '🐨', '🐼', '🐸', '🦓', '🐴', '🦄', '🐔', '🐲', '🐇', '🐍', '🐟', '🐬', '🐤', '🕷', '🦇', '🐌', '🦑', '🐙', '🐘', '🦉']
        }, {
            "name": qsTr("人"),
            "emoji": ['👩', '👨', '🧑', '👧', '👦', '🧒', '👶', '👱‍', '👸', '🤴', '👵', '👴', '👳‍', '👳‍', '🎅', '💪', '👈', '👉', '👆', '👇', '🤞', '🖖', '🤘', '🤙', '🖐', '✋', '👌', '👍', '👎', '✊', '👊', '🤛', '🤜', '🤚', '🤟', '👏', '🤲', '🙏', '🤝']
        }, {
            "name": qsTr("水果植物"),
            "emoji": ['🥝', '🍇', '🍉', '🍌', '🍍', '🍎', '🍑', '🍓', '🍅', '🥔', '🍒', '🍊', '🍆', '🍄', '🥒', '🥦', '🥕', '🌱', '🥥', '🍈', '🍏', '🍐', '🥑', '🌽', '💐', '🍁', '🌹', '🌻', '🌷', '🥀', '🌲', '🌳', '🌴', '🌵', '🌾', '🌿', '🍀', '🍁', '🍂', '🍃']
        }, {
            "name": qsTr("食物"),
            "emoji": ['🍕', '🍔', '🍟', '🌭', '🍿', '🍤', '🥘', '🍡', '🍳', '🍞', '🧀', '🌮', '🍖', '🍗', '🥩', '🥟', '🍘', '🍙', '🍚', '🥣', '🍦', '🍩', '🍰', '🍬', '🍭', '🍺', '🍜', '🍛', '🍥', '🍝', '🎂', '🍼', '🍯', '🍵', '🍸', '🥄', '🥠', '🍱', '🥪', '🥗']
        }, {
            "name": qsTr("其他"),
            "emoji": ['🚽', '🛁', '⛅', '🌜', '⭐', '🌂', '🌈', '🔥', '🌀', '⛄', '🚛', '🚓', '🚒', '🛵', '🛴', '🚠', '🚇', '🚀', '⛵', '🛬', '🚥', '🚩', '🌍', '🗻', '🏡', '🏥', '💙', '💛', '💚', '💔', '💨', '💢', '❓', '❌', '⭕', '➰', '💲', '➕', '➖', '🔴']
        }]

    Behavior on width {
        NumberAnimation {
            duration: 300
            easing.type: Easing.OutQuart
        }
    }

    onWidthChanged: {
        if (width + x > window.width)
            x = window.width - width
    }

    Component.onCompleted: {
        if (height + y > window.height)
            y = window.height - height
        paginationLayout.opacity = 1
    }

    onVisibleChanged: if(!visible) destroy()

    background: Rectangle {
        width: parent.width
        height: parent.height
        color: appTheme.backgroundColor
        radius: appTheme.stdRadius
        border.color: appTheme.tintColor
        layer.enabled: true
        layer.effect: DropShadow {
            id: dropShadow
            radius: 22
            samples: 20
            verticalOffset: 10
            color: appTheme.shadowColor
        }
    }

    TabView {
        id: tableView
        clip: true
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: paginationLayout.top
            margins: appTheme.narrowSpacing
        }

        property int currentPage: 0

        onCurrentIndexChanged: {
            const i = getTab(currentIndex).item
            root.width = i.implicitWidth + 2 * anchors.margins
            pagination.model = i.model
        }

        Tab {
            title: qsTr("表情")

            EmojiGridList {
                anchors.fill: parent
                columns: 10
                emojiSize: Qt.size(32, 32)
                contentTextSize: appTheme.stdTextSize
                emojis: emojiList
                rightInsert: 4
                bottomInsert: 4
                onSelected: {
                    root.selected(emoji)
                    if (!isFixed)
                        root.destroy()
                }

                Component.onCompleted: {
                    root.width = implicitWidth + 2 * tableView.anchors.margins
                    pagination.model = model
                }

                onCurrentIndexChanged: tableView.currentPage = currentIndex
            }
        }
        Tab {
            title: qsTr("颜文字")

            EmojiGridList {
                anchors.fill: parent
                columns: 4
                emojiSize: Qt.size(80, 32)
                emojis: yanList
                onSelected: {
                    root.selected(emoji)
                    if (!isFixed)
                        root.destroy()
                }

                onCurrentIndexChanged: tableView.currentPage = currentIndex
            }
        }

        style: TabViewStyle {
            frameOverlap: 1
            tab: Item {
                implicitWidth: 64
                implicitHeight: 30
                Rectangle {
                    color: styleData.selected ? appTheme.primaryColor1 : appTheme.tintColor
                    anchors.fill: parent
                    anchors.leftMargin: appTheme.tinySpacing
                    anchors.rightMargin: appTheme.tinySpacing
                    anchors.bottomMargin: appTheme.tinySpacing
                    radius: appTheme.stdRadius
                    Text {
                        id: text
                        anchors.centerIn: parent
                        text: styleData.title
                        font.pixelSize: appTheme.smallTextSize
                        color: styleData.selected ? appTheme.backgroundColor : appTheme.subTextColor
                    }
                }
            }

            frame: Rectangle {
                color: "transparent"
            }
        }
    }

    Row {
        id: paginationLayout
        opacity: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: appTheme.narrowSpacing
        spacing: appTheme.tinySpacing

        Repeater {
            id: pagination

            Rectangle {
                width: 6
                height: 6
                radius: 3
                color: (tableView.currentPage
                        === index) ? appTheme.primaryColor1 : appTheme.tintColor
            }
        }

        Behavior on opacity {
            NumberAnimation {
                easing.type: Easing.InCubic
                duration: 500
            }
        }
    }

    // TODO : replace text with icon
    StyleButton {
        checkable: true
        radius: appTheme.stdRadius

        anchors.right: tableView.right
        anchors.top: tableView.top

        onToggled: isFixed = !isFixed
    }
}
