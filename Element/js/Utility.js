function createToast(text, parent, duration = 2500, showType = Toast.EmergeFromBottom) {
    var component = Qt.createComponent("qrc:/Element/Toast.qml");
    if (component.status === Component.Ready) {
        var toast = component.createObject(parent);
        toast.show(text, duration, showType);
    }
}
