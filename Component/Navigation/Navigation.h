#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QIcon>
#include <QWidget>

namespace Ui {
class Navigation;
}

class User;
class QButtonGroup;

class Navigation : public QWidget
{
    Q_OBJECT

public:
    explicit Navigation(QWidget *parent = nullptr);
    ~Navigation();

    /**
     * @brief 添加一个导航按钮
     * @param icon 按钮显示的图标
     * @param text 按钮显示的文字
     */
    void addNavButton(const QIcon& icon = QIcon(), const QString& text = QString());

    /**
     * @brief 设置一个用户到导航栏尾部
     * @param user 用户
     * @note 若不想添加到默认位置，可以不设置该值，而是用@see pullWidget()添加到头部。
     */
    void setUser(User* user) const;

    /**
     * 返回当前选中的导航按钮索引，如果没有选中则返回-1
     * @note 索引顺序是从上到小（如果是水平形式，则是从左到右），从0开始
     */
    int getCurrentNavigationIndex(void);

public Q_SLOTS:
    /**
     * @brief 导航动作
     * @param index 导航按钮的索引
     * @see navigated
     */
    void navigate(int index);

    void selectAvatarFile();

Q_SIGNALS:
    /**
     * @brief 信号：当点击导航按钮或手动触发navigate方法时进行导航时发送的信号
     * @param index 导航按钮的索引
     * @see navigate 
     */
    void navigated(int index);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Ui::Navigation *ui;

    /** 管理导航按钮的按钮组 */
    QButtonGroup* mButtonGroup;
};

#endif // NAVIGATION_H
