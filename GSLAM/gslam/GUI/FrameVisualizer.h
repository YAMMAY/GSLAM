#ifndef FRAMEVISUALIZER_H
#define FRAMEVISUALIZER_H
#include "../../core/GSLAM.h"
#include "../../core/Svar.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTableWidget>

namespace GSLAM
{
class GImageWidget;

class InfomationViewer : public QTableWidget
{
    Q_OBJECT
public:
    InfomationViewer(QWidget* parent);
    QTableWidgetItem* setValue(int row,int col,QString val);
    QTableWidgetItem* setValue(int row,int col,double  val);
    void              update(const FramePtr& frame,bool flush=true);
    std::map<QString,QString>   vars;
    double                      lastUpdateTime;
};

class FrameVisualizer: public QWidget,public GObjectHandle
{
    Q_OBJECT
public:
    FrameVisualizer(QWidget* parent=NULL):QWidget(parent){
        _splitter=new QSplitter(Qt::Vertical,this);
        _imageLayout=new QVBoxLayout(this);
        _imageLayout->addWidget(_splitter);
        _infos=new InfomationViewer(this);
        _splitter->addWidget(_infos);
        connect(this,SIGNAL(signalFrameUpdated()),this,SLOT(slotFrameUpdated()));
    }
    virtual ~FrameVisualizer(){}

    void showFrame(const FramePtr& frame){
        if(!frame) return;

        {
            GSLAM::WriteMutex lock(_mutex);
            _curFrame=frame;
            if(frame->cameraNum())
                _lastImageFrame=frame;
        }

        emit signalFrameUpdated();
    }

signals:
    void signalFrameUpdated();
public slots:
    void slotFrameUpdated();

protected:
    GSLAM::MutexRW              _mutex;
    FramePtr                    _curFrame,_lastImageFrame;
    std::vector<GImageWidget*>  _images;
    InfomationViewer*           _infos;
    QVBoxLayout*                _imageLayout;
    QSplitter*                  _splitter;
};

}

#endif
