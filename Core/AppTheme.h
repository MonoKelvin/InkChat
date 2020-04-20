#ifndef APPTHEME_H
#define APPTHEME_H

#include <QColor>
#include <QFont>

/**
 * @brief 对齐锚点
 */
enum class EAlignment {
    Top,
    Bottom,
    Left,
    Right,
    Center,
};

/**
 * @brief 方向
 */
enum class EOrientation {
    Vertical,
    Horizontal,
};

/**
 * @brief 间距
 */
enum ESize {
    Tiny = 5,
    Narrow = 10,
    Std = 15,
    Wide = 20,
    Large = 25,
    Extreme = 40,
};

/**
 * @brief 边界样式
 */
enum class EBorderStyle {
    Rectangle,
    Circle,
    Rounded
};

/**
 * @brief 内边距
 */
struct EPaddings {
    int Left;
    int Top;
    int Right;
    int Bottom;

    EPaddings() {}

    EPaddings(int vertical, int horizontal)
        : Left(horizontal)
        , Top(vertical)
        , Right(horizontal)
        , Bottom(vertical)
    {
    }

    EPaddings(int l, int t, int r, int b)
        : Left(l)
        , Top(t)
        , Right(r)
        , Bottom(b)
    {
    }
};

/**
 * @brief 气泡样式
 */
struct SBubbleStyle {
    QColor BubbleColor;
    QColor TextColor;
    EPaddings Paddings;
    int BorderWidth;
    QColor BorderColor;

    SBubbleStyle() {}

    SBubbleStyle(const QColor& background,
        const QColor& text,
        const EPaddings& paddings,
        int borderWidth = 0,
        const QColor& border = Qt::transparent)
        : BubbleColor(background)
        , TextColor(text)
        , Paddings(paddings)
        , BorderWidth(borderWidth)
        , BorderColor(border)
    {
    }
};

class AppTheme {
public:
    // Font
    const QFont SmallFont = QFont(QStringLiteral("Microsoft Yahei"), 8);
    const QFont StdFont = QFont(QStringLiteral("Microsoft Yahei"), 10);
    const QFont SubTitleFont = QFont(QStringLiteral("Microsoft Yahei"), 12, QFont::Bold);
    const QFont TitleFont = QFont(QStringLiteral("Microsoft Yahei"), 13, QFont::Bold);
    const QFont BigFont = QFont(QStringLiteral("Microsoft Yahei"), 14, QFont::Bold);

    // Primary Color
    const QColor PrimayColor1 = QColor("#4D7CFE");
    const QColor PrimayColor2 = QColor("#FFAB2E");
    const QColor PrimayColor3 = QColor("#FF2B52");

    // Text Color
    const QColor MainTextColor = QColor("#2D3135");
    const QColor SubTextColor = MainTextColor.lighter(300);

    // Helper Color
    const QColor TintColor = QColor("#F5F6FA");

    // Default Bubble Style
    const SBubbleStyle LeftBubbleStyle { PrimayColor1, Qt::white, { ESize::Std, ESize::Narrow } };
    const SBubbleStyle RightBubbleStyle { TintColor, MainTextColor, { ESize::Std, ESize::Narrow } };

    /**
     * @brief 获取主色对应的次色
     * @param number 主色编号
     * @param level 亮度等级
     * @return 返回对应的次色
     */
    const QColor getPrimarySubColor(short number = 1, int level = 160) const
    {
        switch (number) {
        case 2:
            return PrimayColor2.lighter(level);
        case 3:
            return PrimayColor3.lighter(level);
        }

        return PrimayColor1.lighter(level);
    }

    // Others
    const int AvatarSize = 46;
};

extern AppTheme XTheme;

#endif // APPTHEME_H
