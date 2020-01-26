#ifndef CHATINPUTBOX_H
#define CHATINPUTBOX_H

#include <QWidget>

class QPushButton;
class QPlainTextEdit;

class ChatInputBox : public QWidget
{
    Q_OBJECT
public:
    explicit ChatInputBox(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

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
};

#endif // CHATINPUTBOX_H
