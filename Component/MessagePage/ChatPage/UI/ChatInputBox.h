#ifndef CHATINPUTBOX_H
#define CHATINPUTBOX_H

#include <QWidget>

class QPushButton;
class QPlainTextEdit;

class ChatInputBox : public QWidget
{
    Q_OBJECT

    friend class ChatViewWidget;

public:
    explicit ChatInputBox(QWidget *parent = nullptr);

Q_SIGNALS:
    /**
     * @brief 信号：当折叠或展开输入面板时发送的信号
     * @param enabled 是否折叠，true表示展开，否则表示收起
     */
    void expand(bool enabled);

    /**
     * @brief 当发送消息时
     * @param type 消息类型
     * @param msg 数据
     */
    void send(int type, const QVariant& data);

public Q_SLOTS:
    /**
     * @brief 展开消息输入框
     * @param enabled 开关，传入true表示展开，false则表示收起
     */
    void onExpand(bool enabled);

protected:
    void moveEvent(QMoveEvent *event) override;
    void paintEvent(QPaintEvent*) override;

private:
    /** 发送图片的按钮 */
    QPushButton *mBtnText;

    /** 发送视频的按钮 */
    QPushButton *mBtnEmoji;

    /** 发送文件的按钮 */
    QPushButton *mBtnFile;

    /** 文本输入框 */
    QPlainTextEdit *mChatInputer;

    /** 发送按钮 */
    QPushButton* mBtnSend;

private:
    /** 在收起之前的高度 */
    int _oldH;
};

#endif // CHATINPUTBOX_H
