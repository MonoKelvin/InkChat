#ifndef UTILITY_H
#define UTILITY_H

#include <QStyleOption>
#include <QStylePainter>
#include <QWidget>

class QWidget;
class QFont;
class QLabel;

/**
 * @brief 图片处理函数
 * @param pixmap 要处理的图片
 * @param error 错误信息
 */
typedef void (*DealWithPixmapFunc)(const QPixmap& pixmap, const QString& error);

/**
 * @brief 给QWidget控件附加阴影特效
 * @param widget 给定的控件
 * @param xoffset x偏移值
 * @param yoffset y偏移值
 * @param radius 阴影半径
 * @param color 阴影颜色
 * @note 阴影会附加到widget的每一个子控件
 */
void attachShadowEffect(QWidget *widget,
                        double xoffset = 0.0,
                        double yoffset = 0.0,
                        double radius = 30.0,
                        const QColor &color = "#A7ADBD");

/**
 * @brief 获取超出给定最大宽度的省略字符串
 * @param text 给定的字符串
 * @param font 字符串的字体
 * @param maxWidth 最大宽度，当字体像素宽度超过该值后就显示省略号
 * @param mode 省略模式
 * @return 省略处理后的字符串
 */
inline const QString getElidedText(const QString &text,
                                   const QFont &font,
                                   int maxWidth,
                                   Qt::TextElideMode mode = Qt::ElideRight)
{
    return QFontMetrics(font).elidedText(text, mode, maxWidth);
}

/**
 * @brief 获取超出给定最大宽度的省略字符串
 * @param label 给定的QLabel控件
 * @param maxWidth 最大宽度，当控件字体像素宽度超过该值后就显示省略号
 * @param mode 省略模式
 */
void getElidedText(QLabel *label, int maxWidth, Qt::TextElideMode mode = Qt::ElideRight);

/**
 * @brief 更新widget控件的布局，让子控件重新适应布局
 * @param widget 给定的widget控件
 * @note 只针对有子布局的控件可以取到很好的效果，可以很好的取代adjustSize()方法
 */
inline void updateLayout(QWidget *widget)
{
    const auto s = widget->size();
    widget->resize(s.width() + 1, s.height());
    widget->resize(s.width(), s.height());
}

/**
 * @brief 文件夹（路径）是否存在，并提供不存在时创建文件夹的选项
 * @param fullPath 文件夹（路径）全称
 * @param makeIfNull 当路径不存在时则创建路径
 * @param recursion 是否使用递归创建路径。
 *  当为false时，必须保证上一级路径存在；
 *  当为true时，会创建路径中所有不存在的路径文件夹。
 * @return bool 文件夹（路径）存在则返回true，否则返回false。
 *  当makeIfNull为true、recursion为false时：必须保证上一级路径存在才返回true，否则返回false；
 *  当makeIfNull为true、recursion为true时：总是返回true，并创建新指定的路径。
 * @note 该方法总是先判断路径是否存在，存在则返回true，之后不做任何事情。
 * @note isFileExists
 */
bool isDirExists(const QString& fullPath, bool makeIfNull = false, bool recursion = true);

/**
 * @brief 文件是否存在，并提供不存在时创建文件的选项
 * @param fullPath 文件全称，包含路径和后缀名
 * @param makeIfNull 当文件不存在时则创建文件
 * @param recursion 是否使用递归创建文件，即创建路径中不存在的文件夹。
 *  当为false时，必须保证文件所在的文件夹存在；
 *  当为true时，会创建路径中所有不存在的路径文件夹。
 * @return bool 文件存在则返回true，否则返回false。
 *  当makeIfNull为true、recursion为false时：必须保证文件所在的文件夹存在才返回true，否则返回false；
 *  当makeIfNull为true、recursion为true时：总是返回true，并创建新指定的文件。
 * @note 该方法总是先判断文件是否存在，存在则返回true，之后不做任何事情。
 * @see isDirExists
 */
bool isFileExists(const QString& fileName, bool makeIfNull = false, bool recursion = true);

/**
 * @brief 从网络获取图片
 * @param callBack 回调函数，当获取图片或失败后做的事
 * @see DealWithPixmapFunc
 */
void* getPixmapFromNetwork(const QString& url, DealWithPixmapFunc callBack);

/**
 * @brief 继承自QWidget的类如果不实现自己的paintEvent方法，那么该类就无法使用样式表
 * 改变样式。通过该宏可以在样式表中改变当前控件的样式
 * @note 该宏必须放在paintEvent函数中。
 */
#define USE_STYLE_SHEET \
    { \
        QStylePainter _Painter_(this); \
        QStyleOption _Opt_; \
        _Opt_.initFrom(this); \
        _Opt_.rect = rect(); \
        _Painter_.drawPrimitive(QStyle::PE_Widget, _Opt_); \
    }

/** 向上取整，返回的是int类型 */
#define Ceil(x) int(float(x)+0.500000f)

#endif // UTILITY_H
