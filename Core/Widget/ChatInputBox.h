#ifndef CHATINPUTBOX_H
#define CHATINPUTBOX_H

#include <QWidget>

class QPushButton;
class QPlainTextEdit;

class ChatInputBox : public QWidget
{
    Q_OBJECT

    friend class ChatListView;
public:
    explicit ChatInputBox(QWidget *parent = nullptr);

    /**
     * @brief 显示模式
     */
    enum EDisplayMode
    {
        Expand, // 展开
        Foldup, // 收起
        Simple, // 精简 TODO: next version
    };

Q_SIGNALS:
    void onFoldup(bool enabled);

public slots:
    /**
     * @brief 收起消息输入框
     * @param enabled 开关，传入true表示收起，false则表示展开
     */
    void foldup(bool enabled);

protected:
    void moveEvent(QMoveEvent *event) override;

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

    /** 显示模式 */
    EDisplayMode mDisplayMode;

private:
    /** 在收起之前的高度 */
    int _oldH;
};

#endif // CHATINPUTBOX_H
