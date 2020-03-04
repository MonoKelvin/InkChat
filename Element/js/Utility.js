
/**
 * 创建一个提示控件
 * @param {string} text 提示文本内容
 * @param {object} parent 父对象，即要显示在那个控件的中心
 * @param {int} duration 显示时长
 * @param {Toast.EShowType} 显示类型，目前只支持Toast.EmergeFromBottom类型
 */
function createToast(text, parent, duration, showType) {
    if (text.trim() === '')
        return
    var component = Qt.createComponent("qrc:/Element/Toast.qml")
    if (component.status === Component.Ready) {
        var toast = component.createObject(parent)
        toast.show(text, duration, showType)
    }
}


/**
 * 创建一个emoji输入面板
 * @param x 面板的x坐标
 * @param y 面板的y坐标，该坐标不是最后面板显示的真实坐标，会减去面板的height
 * @param parent 要显示在哪个父对象里
 * @param callBack 回调函数，即输入面板中点击某个emoji时响应的事件函数，其格式为：
 *  function callBack(string emoji)
 * @note 一般x、y为鼠标点击时的全局坐标（相对于Window.window）
 */
function createEmojiPanel(x, y, parent, callBack) {
    var component = Qt.createComponent("qrc:/Element/EmojiPanel.qml")
    if (component.status === Component.Ready) {
        var emojiPanel = component.createObject(parent)
        emojiPanel.x = x
        emojiPanel.y = y - emojiPanel.height
        emojiPanel.open()
        emojiPanel.selected.connect(callBack)
    }
}


/**
 * 获得一个新颜色，改颜色的alpha来自原颜色的alpha通道，但不会改变原颜色值
 * @param color|string sourceColor 源色
 * @param real a 透明通道，范围是0.0-1.0，0.0表示全透明；1.0表示不透明
 * @return color 返回新的颜色（不会影响源色）
 */
function alpha(sourceColor, a) {
    var res = Qt.darker(sourceColor, 1)
    res.a = a
    return res
}


/**
 * 获得当前时间戳的数量
 */
function currentTimeNumber() {
    var time = new Date()
    return Number(time.getTime())
}


/**
 * 从毫秒数获得时间长，格式为 [hh:]mm:ss
 * @param milliseconds 毫秒数
 * @param includeHours 是否包括小时
 */
function timeLengthFromMilliseconds(milliseconds, includeHours) {
    const total = milliseconds / 1000
    const m = parseInt(total / 60)
    const ss = parseInt(total % 60)

    let result = ''
    // 小时
    if (includeHours === true) {
        const h = parseInt(total / 3600)
        if (h < 10) {
            result += '0'
        }
        result += h + ':'
    }

    // 分钟
    if (m < 10) {
        result += '0'
    }
    result += m + ':'
    // 秒数
    if (ss < 10) {
        result += '0'
    }
    result += ss

    return result
}
