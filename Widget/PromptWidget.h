#ifndef PROMPTWIDGET_H
#define PROMPTWIDGET_H

#include <QWidget>

/**
 * @brief 提示控件类。
 */
class PromptWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 提示控件的类型
     */
    enum EPromptType {
        Alert, // 警告
        Prompt // 普通的提示
    };

    /**
     * @brief 构造函数
     * @param prompt 文本内容
     * @param parent 要显示在那个父对象上
     * @param type 提示控件的类型 @see EPromptType
     * @note 一般在构建时只要通过new构建出来即可，不许过多步骤，显示完后会自动释放
     * @example
     * 
     * QWidget mainWindow;
     * mainWindow.show();
     * 
     * new PromptWidget("This is an alert!", &mainWindow, EPromptType::Alert);
     * 
     */
    PromptWidget(const QString& content, QWidget* parent, EPromptType type = Prompt);

protected:
    /**
     * @brief 构建UI方法
     * @param prompt 文本内容
     */
    virtual void buildUI(const QString& prompt);

    /**
     * @brief 显示方法
     */
    virtual void show();

    void paintEvent(QPaintEvent*) override;
};

#endif // PROMPTWIDGET_H
