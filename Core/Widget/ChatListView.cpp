#include "ChatListView.h"

#include "../Configuation.h"

ChatListView::ChatListView(QWidget *parent)
    : QListWidget(parent)
{
    setHorizontalScrollMode(QListView::ScrollPerPixel);
    setVerticalScrollMode(QListView::ScrollPerPixel);
    setEditTriggers(QListView::NoEditTriggers);
    setResizeMode(QListWidget::Adjust);
    setSpacing(ESpacing::Wide);
}
