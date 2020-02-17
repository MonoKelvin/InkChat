#ifndef SINGLETON_H
#define SINGLETON_H
#include <QPointer>
/**
 * @brief 单例基类
 * @note 继承该类可以实现单例模式，并且是线程安全的，可自动释放资源
 */
template <typename T>
class Singleton {
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    virtual ~Singleton() {}

    inline static QPointer<T> Instance()
    {
        // 使用静态局部对象。
        static QPointer<T> mInstance = QPointer<T>(new T);
        return mInstance;
    }

protected:
    Singleton() = default;
};

#endif // SINGLETON_H
