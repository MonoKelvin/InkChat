#ifndef LOADINGINDICATOR_H
#define LOADINGINDICATOR_H

#include <QTimer>
#include <QWidget>

#define INDICATOR_RING_WIDTH 2
#define INDICATOR_SIZE 28
#define INDICATOR_PULLDOWN_LIMIT 100

/**
 * @brief 加载指示器控件
 */
class LoadingIndicator : public QWidget {
    Q_OBJECT

public:
    explicit LoadingIndicator(QWidget* parent);

    inline void start() const noexcept
    {
        mTimer->start(20);
    }

    inline void finish() noexcept
    {
        mTimer->stop();
        setVisible(false);
        deleteLater();
    }

    inline bool isLoading(void) const noexcept
    {
        return mTimer->isActive();
    }

private:
    void paintEvent(QPaintEvent*) override;

private Q_SLOTS:
    inline void updateIndicator() noexcept
    {
        mStartAngle += 16;
        if (mStartAngle > 360) {
            mStartAngle = 0;
        }
        update();
    }

private:
    unsigned short mStartAngle;

    QTimer* mTimer;
};

#endif // LOADINGINDICATOR_H
