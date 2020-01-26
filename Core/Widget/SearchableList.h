#ifndef ADVANCEDLIST_H
#define ADVANCEDLIST_H

#include <QListWidget>

class QPushButton;
enum class EAlignment;

class SearchableList : public QListWidget
{
    Q_OBJECT
public:
    explicit SearchableList(QWidget *parent = nullptr);

    /**
     * @brief 设置回到顶部按钮的对齐方式
     * @param alignment 对齐方式
     * @note 该对齐方式只有底部左边、底部中间和底部右边三种，分别对应：
     * EAlignment::Left、EAlignment::Center和EAlignment::Right
     */
    inline void setBackTopAlignment(const EAlignment &alignment)
    {
        mBackTopAlignment = alignment;
        resizeEvent(nullptr);
    }

    /**
     * @brief 设置回到顶部按钮是否可见
     * @param enabled 开关
     */
    void setBackTopVisiable(bool enabled);

public slots:
    virtual void search(const QString &key);
//    virtual bool refresh();

protected:
    virtual void resizeEvent(QResizeEvent *e) override;

    virtual bool searchDelegate(const QString &, QListWidgetItem *) { return true; }

protected:
    /** 回到顶部按钮 */
    QPushButton *mBackTop;

    /** 回到顶部按钮的对齐方式
     * @note 该对齐方式只有底部左边、底部中间和底部右边三种，分别对应：
     * EAlignment::Left、EAlignment::Center和EAlignment::Right
     */
    EAlignment mBackTopAlignment;
};

#endif // ADVANCEDLIST_H
