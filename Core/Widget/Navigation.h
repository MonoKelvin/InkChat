#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "../Configuation.h"

#include <QWidget>
#include <QIcon>

class QButtonGroup;
class QPushButton;
class User;

/**
 * 一般的垂直导航形式为：
 * logo |------| 导航按钮 |------| 用户头像
 * 而水平形式的导航形式为：
 *     logo
 *      ---
 *       |
 *       |
 *      ---
 *    导航按钮
 *      ---
 *       |
 *       |
 *      ---
 *    用户头像
 */
class Navigation : public QWidget
{
    Q_OBJECT
public:
    explicit Navigation(QWidget *parent = nullptr);
    explicit Navigation(EAlignment alignment, EOrientation orientation, QWidget *parent = nullptr);

    /**
     * @brief 按钮样式定制枚举
     */
    enum ENavButtonStyle
    {
        IconsAndText,
        IconsOnly,
        TextOnly,
    }; // TODO: 下个版本加入

    /**
     * @brief 按钮选择时样式枚举
     */
    enum ENavSelectionStyle
    {
        BarMoving,
        BackgroundMoving,
        BarChange,
        BackgroundChange,
    }; // TODO: 下个版本加入

    /********************************/
    /*********** 导航类方法 ***********/
    /********************************/

    /**
     * @brief 设置导航条内边距
     * @param margin 内边距大小
     */
    void setPadding(int margin);

    /**
     * @brief 设置导航条内边距
     * @param left   左边内边距
     * @param top    上边内边距
     * @param right  右边内边距
     * @param bottom 下边内边距
     */
    void setPadding(int left, int top, int right, int bottom);

    /**********************************/
    /*********** 导航按钮相关 ***********/
    /**********************************/

    /**
     * @brief 导航动作
     * @param index 按钮索引
     */
    void navigate(int index);

    /**
     * @brief 添加一个导航按钮
     * @param text 按钮显示的文字
     * @param icon 按钮显示的图标
     */
    void addNavButton(const QString &text, const QIcon &icon = QIcon());

    /**
     * @brief 设置导航按钮的尺寸
     * @param size 要设置的尺寸大小
     * @note 尽量使用样式表来设置按钮的大小，比如在qss中导航按钮：
     * #navBtnsContents QPushButton {
     *      border: none;
     *      max-width: 50px;
     *      max-height: 50px;
     * }
     */
    void setNavButtonSize(const QSize &size);

    /**
     * @brief 设置导航按钮对齐方式
     * @param alignment 对齐方式枚举
     */
    void setNavButtonsAlignment(const EAlignment &alignment);

    /**
     * @brief 设置导航按钮之间的空隙
     * @param spacing 空隙大小
     */
    void setNavButtonSpacing(int spacing);

    /**
     * @brief 设置导航按钮的外边距
     * @param margin 外边距大小
     * @note 该外边距是按钮整体的外边距，而不是每个按钮的外边距
     */
    void setNavButtonsMargin(int margin);

    /**
     * @brief 设置导航按钮的外边距
     * @param left   左边外边距
     * @param top    上边外边距
     * @param right  右边外边距
     * @param bottom 下边外边距
     */
    void setNavButtonsMargin(int left, int top, int right, int bottom);

    /*************************************/
    /*********** Logo 和头像相关 ***********/
    /*************************************/

    /**
     * @brief 添加一个软甲LOGO标志，一般在导航条的头部（垂直形式为顶部，水平形式为最左部）
     * @param logo logo图标
     * @note 若不想添加到默认位置，可以不设置该值，而是用@see pushWidget()添加到尾部。
     */
    void addLogo(const QPixmap &logo);

    /**
     * @brief 设置一个用户到导航栏尾部
     * @param user 用户
     * @note 若不想添加到默认位置，可以不设置该值，而是用@see pullWidget()添加到头部。
     */
    void setUser(QScopedPointer<User> user);

    /*******************************/
    /*********** 其他方法 ***********/
    /*******************************/

    /**
     * @brief 像头部（垂直形式为顶部，水平形式为最左部）添加一个控件
     * @param widget 要添加的控件
     */
    void pullWidget(QWidget *widget);

    /**
     * @brief 像尾部（垂直形式为底部，水平形式为最右部）追加一个控件
     * @param widget 要添加的控件
     */
    void pushWidget(QWidget *widget);

    /**
     * 返回当前选中的导航按钮索引，如果没有选中则返回-1
     * @note 索引顺序是从上到小（如果是水平形式，则是从左到右），从0开始
     */
    int getCurrentNavigationIndex(void);

    inline EAlignment getNavButtonsAlignment(void) const { return mNavButtonsAlignment; }
    inline EOrientation getOrientation(void) const { return mOrientation; }

Q_SIGNALS:
    /**
     * @brief 当点击导航按钮进行导航时发送的信号
     * @param index 导航按钮的索引
     */
    void onNavigated(int index);

private:
    void resizeNavButtonContents(void);

    void init();

private:
    /** 管理导航按钮的按钮组 */
    QButtonGroup *mButtonGroup;

    /** 导航按钮的内容容器 */
    QWidget *mNavContents;

    /** 导航方向，只有水平和垂直两种形式，一般使用垂直 */
    EOrientation mOrientation;

    /** 导航按钮的对齐形式 */
    EAlignment mNavButtonsAlignment;
};

#endif // NAVIGATION_H
