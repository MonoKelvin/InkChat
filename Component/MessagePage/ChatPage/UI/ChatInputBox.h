#ifndef CHATINPUTBOX_H
#define CHATINPUTBOX_H

#include <ChatItem.h>

#include <QWidget>

class QPushButton;
class QPlainTextEdit;

class ChatInputBox : public QWidget
{
    Q_OBJECT

    friend class ChatListView;
public:
    explicit ChatInputBox(QWidget *parent = nullptr);

Q_SIGNALS:
    /**
     * @brief 信号：当折叠或展开输入面板时发送的信号
     * @param enabled 是否折叠，true表示折叠，否则表示展开
     */
    void foldup(bool enabled);

    /**
     * @brief 当发送（普通文本）消息时
     * @param content 文本内容
     */
    void send(const QString& content);

public Q_SLOTS:
    /**
     * @brief 收起消息输入框
     * @param enabled 开关，传入true表示收起，false则表示展开
     */
    void onFoldup(bool enabled);

protected:
    void moveEvent(QMoveEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    /** 输入文本的按钮，默认 */
    QPushButton *mBtnText;

    /** 发送图片的按钮 */
    QPushButton *mBtnImage;

    /** 发送视频的按钮 */
    QPushButton *mBtnVideo;

    /** 发送文件的按钮 */
    QPushButton *mBtnFile;

    /** 文本输入框 */
    QPlainTextEdit *mChatInputer;

    /** 发送按钮 */
    QPushButton *mBtnSend;

    /** 收回\展开 输入框按钮 */
    QPushButton *mBtnExpand;

private:
    /** 在收起之前的高度 */
    int _oldH;
};

#endif // CHATINPUTBOX_H
