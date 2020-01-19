#ifndef ADVANCEDLIST_H
#define ADVANCEDLIST_H

#include "../Configuation.h"

#include <QListView>
#include <QMenu>

class QPushButton;

class AdvancedList : public QWidget
{
    Q_OBJECT
public:
    explicit AdvancedList(QWidget *parent = nullptr);

    /****************************/
    /******* 工具栏相关方法 *******/
    /****************************/

    void setMenuVisiable(bool enabled);

    void setMenu(const QMenu &menu);

    void addActionToMenu(const QAction &action);

    void addWidgetToTopContainer(QWidget *widget);

    /*****************************/
    /******* 回到顶部相关方法 *******/
    /*****************************/

    /**
     * @brief 设置回到顶部按钮的对齐方式
     * @param alignment 对齐方式
     * @note 该对齐方式只有底部左边、底部中间和底部右边三种，分别对应：
     * EAlignment::Left、EAlignment::Center和EAlignment::Right
     */
    void setBackToTopAlignment(EAlignment alignment);

    void setCanBackToTop(bool enabled);

    /*****************************/
    /******* 列表视图相关方法 *******/
    /*****************************/

    inline QListView *getListView(void) const { return mListView; }

Q_SIGNALS:
    void onSearched(const QString &text);
    void onBackToTop();

protected slots:
    void closeSearchBox(void);
    void expandSearchBox(void);

    virtual void search(const QString &key, bool isRealTime = false);
    virtual bool refresh();

protected:
    virtual void resizeEvent(QResizeEvent *e) override;

protected:
    /** 列表显示控件 */
    QListView *mListView;

    /** 搜索栏 */
    QLineEdit *mSearchBox;

    /** 菜单按钮 */
    QPushButton *mBtnMenu;

    /** 回到顶部按钮 */
    QPushButton *mBtnBackToTop;

    /** 点击菜单按钮显示的菜单 */
    QMenu mMenu;

private:
    /** 顶部搜索框和菜单等控件的容器 */
    QWidget *mToopContainer;
};

#endif // ADVANCEDLIST_H
