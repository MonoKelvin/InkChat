#include "IChatItem.h"

IChatItem::IChatItem(ESender sender, QWidget *parent)
    : QWidget(parent)
    , mSender(sender)
    , mSendState(ESendState::Sending)
    , mIsUpdatedContents(false)
{

}
