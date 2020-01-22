#include "IChatWidget.h"

IChatWidget::IChatWidget(ESender sender, QWidget *parent)
    : QWidget(parent)
    , mSender(sender)
    , mSendState(ESendState::Sending)
    , mIsUpdatedContents(false)
{

}
