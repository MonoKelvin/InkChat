﻿#ifndef UTILITY_H
#define UTILITY_H

#include <QCryptographicHash>
#include <QDebug>
#include <QPixmap>

/**
 * @brief 图片处理函数
 * @param pixmap 要处理的图片
 * @param error 错误信息
 */
typedef void (*DealWithPixmapFunc)(const QPixmap& pixmap, const QString& error);

/**
 * @brief 文件夹（路径）是否存在，并提供不存在时创建文件夹的选项
 * @param fullPath 文件夹（路径）全称
 * @param makeIfNull 当路径不存在时则创建路径
 * @param recursion 是否使用递归创建路径。
 *  当为false时，必须保证上一级路径存在；
 *  当为true时，会创建路径中所有不存在的路径文件夹。
 * @return bool 文件夹（路径）存在则返回true，否则返回false。
 *  当makeIfNull为true、recursion为false时：必须保证上一级路径存在才返回true，否则返回false；
 *  当makeIfNull为true、recursion为true时：创建文件夹成功返回true，否则返回false。
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
 *  当makeIfNull为true、recursion为true时：创建文件成功返回true，否则返回false。
 * @note 该方法总是先判断文件是否存在，存在则返回true，之后不做任何事情。
 * @see isDirExists
 */
bool isFileExists(const QString& fileName, bool makeIfNull = false, bool recursion = true);

/**
 * @brief 复制一个文件
 * @param source 源文件。包括路径名称
 * @param newFile 新文件。包括路径名称
 * @param isOverWrite 是否覆盖已经存在的文件
 * @return 复制成功返回true，否则返回false
 * @note 复制文件时会递归创建文件路径
 */
bool copyFile(const QString& source, const QString& newFile, bool isOverWrite = true);

/**
 * @brief 从网络获取图片
 * @param callBack 回调函数，当获取图片或失败后做的事
 * @see DealWithPixmapFunc
 */
void getPixmapFromNetwork(const QString& url, DealWithPixmapFunc callBack);

/**
 * @brief 使用MD5算法生成文本
 * @param source 源字符串
 * @param isHalf 是否只得到16位的字符串
 * @return 返回加密好的字符串
 */
inline const QString encryptTextByMD5(const QString& source, bool isHalf = false)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(source.toLatin1());
    auto result = QString(hash.result().toHex());
    return isHalf ? result.mid(8, 16) : result;
}

/**
 * @brief 获得时间处理后的消息时间
 * @param time 传入的时间，必须比当前时间小
 * @return 格式化后的时间字符串，规则：
 * 当天 => hh:mm
 * 1-7天前 => d天前
 * 大于7天前 => yyyy-MM-dd
 */
const QString GetMessageTime(const QDateTime& time);

/** 向上取整，返回的是int类型 */
#define Ceil(x) int(float(x)+0.500000f)

/** 安全删除指针 */
#define SAFE_DELETE(_Pointer_)   \
    do {                         \
        if (_Pointer_) {         \
            delete _Pointer_;    \
            _Pointer_ = nullptr; \
        }                        \
    } while (0)

/**
 * @brief 选择一个可用的端口号
 * @param defaultPort 默认端口号。如果该端口号没被占用就直接返回，否则从系统中选择一个未被占
 * 用的端口号。
 * @param maxCount 最大重选次数。如果默认端口号已被占用，则每次增加或减少某个特定的数，增加或
 * 减少次数为该值。
 * @return 返回一个未被占用的端口号。如果超过maxCount还是未找到未被占用的端口，则返回0
 */
quint16 selectAvailablePort(quint16 defaultPort = 2020, int maxCount = 20);

/**
 * @brief 获取连接并激活的无线网IP地址
 * @param mac 传出的mac物理地址
 * @param netName 传出的无线网名，比如“wireless_32768”
 * @return 返回连接并激活的无线网IP地址，如果没有连接无线网则返回空字符串
 */
const QString getWirelessAddress(QString* mac = nullptr, QString* netName = nullptr);

/**
 * @brief 检测是否含有文件非法字符
 * @param str 用于检测的字符串
 * @return 如果存在：\ / \ " * ? < > | 者9个非法字符则返回true，否则返回false
 */
bool hasIllegalCharInFile(const QString& str);

#endif // UTILITY_H
