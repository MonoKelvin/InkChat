#include "MessageList.h"

#include "MessageItem.h"
#include "Avatar.h"

MessageList::MessageList(QWidget *parent)
    : AdvancedList(parent)
{

    addListItem(new MessageItem(1, QPixmap(), "测试", ""));
    addListItem(new MessageItem(1, Avatar::GetDefaultPixmap(), "测试", "沪江词用英语怎么说"));
    addListItem(new MessageItem(1, Avatar::GetDefaultPixmap(), "测试", "沪江词库精选标记为未读用英语怎么说"));
    addListItem(new MessageItem(1, Avatar::GetDefaultPixmap(), "测试", "沪江词库精选沪江词库精选标记为未读用英语怎么说标记为未读用英语怎么说"));
}
