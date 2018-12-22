#include "task.h"

#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>
#include <QTextStream>

#include <stdexcept>

Task::Task(const QString& access_token,
           const std::vector<std::pair<QString, QString>>& groups,
           const QString& message,
           int interval,
           int period,
           QObject* parent)
    : QObject(parent),
      interval_{interval},
      period_{period},
      accessToken_(access_token),
      message_(message),
      groups_(groups)
{}

void Task::timerEvent(QTimerEvent* event)
{
    if(!active_)
    {
        return;
    }

    for(const auto& group : groups_)
    {
        postToWall(group.first);
        (this->thread())->sleep(static_cast<unsigned long>(interval_));
    }
}

bool Task::attachPhoto(const QString& img_path)
{
    QFile file(img_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    photoAttachment_.reset(new QByteArray(file.readAll()));
    file.close();

    return true;
}

void Task::go()
{    
    this->timerEvent(new QTimerEvent(0));
    startTimer(period_*1000);
}

void Task::postToWall(const QString& group_id) const
{
    try
    {
        if(!photoAttachment_)
        {
            VkQuery::wallPostToGroup(accessToken_, group_id, message_);
            return;
        }

        auto upload_response = uploadPhoto(group_id);
        QString photo = savePhoto(group_id, upload_response);

        VkQuery::wallPostToGroup(accessToken_, group_id, message_, photo);
    }
    catch (const std::exception& e)
    {
        qCritical() << "Error: " << e.what();
    }
}

std::tuple<QString, QString, QString> Task::uploadPhoto(const QString& group_id) const
{
    QJsonObject doc_server =
            (VkQuery::photosGetWallUploadServer(accessToken_, group_id)).object();

    QUrl server_url(((doc_server.take("response")).toObject())["upload_url"].toString());

    std::unique_ptr<QHttpMultiPart> multiPart(new QHttpMultiPart(QHttpMultiPart::FormDataType));

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                        QVariant("image/jpg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("multipart/form-data; name=\"photo\"; filename=\"image.jpg\""));
    imagePart.setRawHeader("Content-Transfer-Encoding", "binary");
    imagePart.setBody(*photoAttachment_);

    multiPart->append(imagePart);

    QJsonObject upload_result =
            (VkQuery::postRequest(QNetworkRequest(server_url), multiPart.get())).object();

    QString server = QString::number(upload_result["server"].toInt());
    QString photo = upload_result["photo"].toString();
    QString hash = upload_result["hash"].toString();

    return std::make_tuple(
                std::move(server),
                std::move(photo),
                std::move(hash));
}

QString Task::savePhoto(const QString& group_id,
                     const std::tuple<QString, QString, QString>& photoDetails) const
{
    QJsonObject save_answer =
            (VkQuery::photosSaveWallPhoto(accessToken_, group_id,
                                                    std::get<0>(photoDetails), // server
                                                    std::get<1>(photoDetails), // photo
                                                    std::get<2>(photoDetails)  // hash
                                                    )).object();

    QJsonArray response_arr = save_answer["response"].toArray();
    if(response_arr.isEmpty())
    {
        throw std::runtime_error{"server hasn't sent response"};
    }

    QJsonObject image = (response_arr.first()).toObject();
    if(image.isEmpty())
    {
        throw std::runtime_error{"photo hasn't been got"};
    }

    QString photo("photo");
    photo += QString::number(image["owner_id"].toInt());
    photo += "_";
    photo += QString::number(image["id"].toInt());
    return photo;
}
