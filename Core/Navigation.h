#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "Configuation.h"

#include <QWidget>
#include <QIcon>

class QButtonGroup;
class QPushButton;
class Avatar;

/**
 * Normal navigation format for horizontal:
 * logo |------| items |------| widget(avatar)
 * And for vertical:
 *     logo
 *      ---
 *       |
 *       |
 *      ---
 *     items
 *      ---
 *       |
 *       |
 *      ---
 *     widget(avatar)
 */
class Navigation : public QWidget
{
    Q_OBJECT
public:
    explicit Navigation(QWidget *parent = nullptr);
    explicit Navigation(EAlignment alignment, EOrientation orientation, QWidget *parent = nullptr);

    /*********** This Navigation ***********/

    void setPadding(int margin);
    void setPadding(int left, int top, int right, int bottom);

    /*********** Navigation Buttons ***********/

    void addNavButton(const QString& text, const QIcon& icon = QIcon());
    void setNavButtonSize(const QSize& size);
    void setNavButtonsAlignment(const EAlignment &alignment);
    void setNavButtonsSpacing(int spacing);
    void setNavButtonsMargin(int margin);
    void setNavButtonsMargin(int left, int top, int right, int bottom);

    /*********** Logo ***********/

    void addLogo(QPixmap* logo);

    /*********** Custom Widget ***********/

    void addAvatar(Avatar *avatar);

    /**
     * Return the current index of checked button, or -1 if on button is checked.
     */
    int getCurrentNavigationIndex(void);

    inline EAlignment getNavButtonsAlignment() const {
        return mNavButtonsAlignment;
    }

Q_SIGNALS:
    void onNavigated(int index);

private:
    void resizeNavButtonContents(void);

    void init();

private:
    /* Navigation button group. */
    QButtonGroup* mButtonGroup;

    /* The layout for navigation buttons. */
    QWidget* mNavContents;

    EOrientation mOrientation;

    EAlignment mNavButtonsAlignment;
};

#endif // NAVIGATION_H
