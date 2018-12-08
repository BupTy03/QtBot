#include "task.h"

#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>
#include <QTextStream>

#include <exception>

#include "myutils.h"

Task::Task(const QString& access_token,
           const QStringList& groups,
           const QString& message,
           int interval,
           int period,
           QObject* parent)
    : QObject(parent),
      interval_{interval},
      period_{period},
      accessToken_(access_token),
      message_(message),
      groupsIds_(groups)
{}

void Task::timerEvent(QTimerEvent* /*event*/)
{
    if(!active_)
    {
        return;
    }

    for(const auto& groupId : groupsIds_)
    {
        try
        {
            postToWall(groupId);
        }
        catch (const std::exception& e)
        {
            Log::toFile(e.what());
        }
        (this->thread())->msleep(static_cast<unsigned long>(interval_*1000));
    }
}

const QStringList& Task::groupsIds() const
{
    return groupsIds_;
}

const QString& Task::getMessage() const
{
    return message_;
}

int Task::getInterval() const
{
    return interval_;
}

int Task::getPeriod() const
{
    return period_;
}

bool Task::attachPhoto(const QString& img_path)
{
    auto file = std::make_unique<QFile>(img_path);
    if(!file->open(QIODevice::ReadOnly))
    {
        return false;
    }

    photoAttachment_ = std::make_unique<QByteArray>(file->readAll());
    file->close();

    return true;
}

void Task::start()
{
    active_ = true;
}

void Task::stop()
{
    active_ = false;
}

void Task::go()
{
    this->timerEvent(new QTimerEvent(0));
    startTimer(period_*1000);
}

void Task::postToWall(const QString& group_id) const
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

std::tuple<QString, QString, QString> Task::uploadPhoto(const QString& group_id) const
{
    QJsonDocument doc_server =
            VkQuery::photosGetWallUploadServer(accessToken_, group_id);

    QUrl server_url(((doc_server["response"])["upload_url"]).toString());

    auto multiPart = std::make_unique<QHttpMultiPart>(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                        QVariant("image/jpg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("multipart/form-data; name=\"photo\"; filename=\"image.jpg\""));
    imagePart.setRawHeader("Content-Transfer-Encoding", "binary");
    imagePart.setBody(*photoAttachment_);

    multiPart->append(imagePart);

    QJsonDocument upload_result =
            VkQuery::postRequest(QNetworkRequest(server_url), multiPart.get());

    QString server = QString::number(upload_result["server"].toInt());
    QString photo = upload_result["photo"].toString();
    QString hash = upload_result["hash"].toString();

    return std::make_tuple(
                std::move(server),
                std::move(photo),
                std::move(hash)
                );
}

QString Task::savePhoto(const QString& group_id,
                     const std::tuple<QString, QString, QString>& photoDetails) const
{
    QJsonDocument save_answer =
            VkQuery::photosSaveWallPhoto(accessToken_, group_id,
                                                    std::get<0>(photoDetails), // server
                                                    std::get<1>(photoDetails), // photo
                                                    std::get<2>(photoDetails)  // hash
                                                    );

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
