#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>

namespace Ui {
class SettingsPage;
}

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);
    ~SettingsPage();

    void refreshStyleSheetList(const QString& path);

Q_SIGNALS:
    void failed(const QString& msg);

private Q_SLOTS:
    void onStyleSheetChanged(const QString& name);

    /**
     * @brief 提交修改
     */
    void onSubmit();

    /**
     * @brief 消息需要修改时
     */
    void needToChanged();

    /**
     * @brief 还原
     */
    void restore();

private:
    Ui::SettingsPage *ui;
};

#endif // SETTINGSPAGE_H
