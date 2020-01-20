#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include "../Component/AdvancedList.h"

class MessageList : public AdvancedList
{
    Q_OBJECT
public:
    explicit MessageList(QWidget *parent = nullptr);
};

#endif // MESSAGELIST_H
