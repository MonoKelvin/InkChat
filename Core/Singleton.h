#ifndef SINGLETON_H
#define SINGLETON_H

#include <QDebug>
#include <QPointer>

#define SINGLETON_STATIC_INITIALIZA(_ClassName_) \
    template <>                                  \
    Singleton<_ClassName_>::_MemeryCollection Singleton<_ClassName_>::_GC

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

    inline static QPointer<T>& Instance()
    {
        // 使用静态局部对象。
        static QPointer<T> mInstance(new T);
        return mInstance;
    }

protected:
    Singleton() = default;

    // 内存回收
    class _MemeryCollection {
    public:
        _MemeryCollection() = default;
        ~_MemeryCollection()
        {
            qDebug() << "memery collected";
            Instance().clear();
        }
    };

    static _MemeryCollection _GC;
};

#endif // SINGLETON_H
