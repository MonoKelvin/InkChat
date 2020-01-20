#ifndef ADVANCEDLIST_H
#define ADVANCEDLIST_H

#include "../Configuation.h"

#include <QListWidget>
#include <QMenu>

class QPushButton;

/**
 * @brief 高级列表类，提供可搜索的列表容器，可添加自定义菜单动作、回到顶部按钮等。
 */
class AdvancedList : public QWidget
{
    Q_OBJECT
public:
    explicit AdvancedList(QWidget *parent = nullptr);

    /****************************/
    /******* 工具栏相关方法 *******/
    /****************************/

    void setMenuVisiable(bool enabled);

#if QT_CONFIG(menu)
    void setToolMenu(QMenu *menu);

    void addActionToToolMenu(const QAction &action);
#endif

    void addWidgetToTool(QWidget *widget);

    /*****************************/
    /******* 回到顶部相关方法 *******/
    /*****************************/

    /**
     * @brief 设置回到顶部按钮的对齐方式
     * @param alignment 对齐方式
     * @note 该对齐方式只有底部左边、底部中间和底部右边三种，分别对应：
     * EAlignment::Left、EAlignment::Center和EAlignment::Right
     */
    void setBackTopAlignment(EAlignment alignment);

    void setBackTopVisiable(bool enabled);

    /*****************************/
    /******* 列表视图相关方法 *******/
    /*****************************/

    inline const QListWidget *getListWidget(void) const { return mListWidget; }

    void addListItem(QWidget *widget);

    void clearList(void);

protected slots:
    void closeSearchBox(void);
    void expandSearchBox(void);

    virtual void search(const QString &key, bool isRealTime = false);
    virtual bool refresh();

protected:
    virtual void resizeEvent(QResizeEvent *e) override;

protected:
    /** 列表显示控件 */
    QListWidget *mListWidget;

    /** 搜索栏 */
    QLineEdit *mSearchBox;

    /** 刷新按钮 */
    QPushButton *mBtnRefresh;

#if QT_CONFIG(menu)
    /** 菜单按钮 */
    QPushButton *mBtnMenu;
#endif

    /** 回到顶部按钮 */
    QPushButton *mBtnBackTop;

    /** 回到顶部按钮对齐方式 */
    EAlignment mBackTopAlignment;

private:
    /** 顶部搜索框和菜单等控件的容器 */
    QWidget *mToopContainer;
};

#endif // ADVANCEDLIST_H
