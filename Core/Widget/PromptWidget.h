#ifndef PROMPTWIDGET_H
#define PROMPTWIDGET_H

#include <QWidget>

class PromptWidget : public QWidget
{
    Q_OBJECT
public:
    enum class PromptType
    {
        Alert,
        Prompt
    };

    PromptWidget(const QString &prompt, QWidget *parent);
    PromptWidget(const QString &prompt, QWidget *parent, PromptType type);

    void show(PromptType type = PromptType::Prompt);

protected:
    virtual void buildUI(const QString &prompt);
};

#endif // PROMPTWIDGET_H
