#include "blocksdisplay.h"
#include "ui_blocksdisplay.h"

#include <QLabel>

extern "C" {
#define this self
#include <mkd64/block.h>
#undef this
}

BlocksDisplay::BlocksDisplay(QWidget *parent) :
    QMainWindow(parent),
    nextTrack(0),
    ui(new Ui::BlocksDisplay),
    statusIcons(new QPixmap[3])
{
    ui->setupUi(this);
    statusIcons[0].load(":/gfx/blFree.png");
    statusIcons[1].load(":/gfx/blReserved.png");
    statusIcons[2].load(":/gfx/blAllocated.png");
}

BlocksDisplay::~BlocksDisplay()
{
    delete[] statusIcons;
    delete ui;
}

void BlocksDisplay::addTrack(Track *t)
{
    QLabel *l = new QLabel(this);
    l->setFixedWidth(32);
    l->setFixedHeight(16);
    l->setText(QString::number(nextTrack+1).append(':'));
    ui->gridLayout->addWidget(l, nextTrack, 0);
    int numSectors = Track_numSectors(t);
    for (int i = 0; i < numSectors;)
    {
        BlockStatus s = Track_blockStatus(t, i);
        int status = 0;
        if (s & BS_ALLOCATED) status = 2;
        else if (s & BS_RESERVED) status = 1;

        l = new QLabel(this);
        l->setFixedWidth(16);
        l->setFixedHeight(16);
        l->setPixmap(statusIcons[status]);
        ui->gridLayout->addWidget(l, nextTrack, ++i);
    }
    ++nextTrack;
}

void BlocksDisplay::setBlock(int track, int sector, int status)
{
    QLayoutItem *i = ui->gridLayout->itemAtPosition(track-1, sector+1);
    if (i)
    {
        QLabel *l = (QLabel *)i->widget();
        l->setPixmap(statusIcons[status]);
    }
}

void BlocksDisplay::setStatusMsg(const QString &text)
{
    ui->statusBar->showMessage(text);
}
