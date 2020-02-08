function createToast(text, parent, duration = 2500, showType = Toast.EmergeFromBottom) {
    var component = Qt.createComponent("qrc:/Element/Toast.qml");
    if (component.status === Component.Ready) {
        var toast = component.createObject(parent);
        toast.show(text, duration, showType);
    }
}

/**
 * 获得一个新颜色，改颜色的alpha来自原颜色的alpha通道，但不会改变原颜色值
 * @param color|string sourceColor 源色
 * @param real a 透明通道，范围是0.0-1.0，0.0表示全透明；1.0表示不透明
 * @return color 返回新的颜色（不会影响源色）
 */
function alpha(sourceColor, a) {
    var res = Qt.darker(sourceColor, 1);
    res.a = a;
    return res;
}
