#include "task.h"

#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>

Task::Task(const QString& access_token,
           const QStringList& groups,
           const QString& message,
           int interval,
           int period,
           QObject* parent)
    : QObject(parent),
      active_{true},
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
        postToWall(groupId);
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

void Task::setImgPath(const QString& img_path)
{
    imgPath_ = img_path;
    qDebug() << "imgPath_: " << imgPath_;
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

QJsonDocument Task::loadImage(const QString &group_id) const
{
    QJsonDocument result;

    QMap<QString, QString> params;
    params["group_id"] = group_id;
    params["access_token"] = accessToken_;
    params["v"] = "5.52";

    QJsonDocument doc = VkQuery::makeGetRequest(QUrl("https://api.vk.com/method/photos.getWallUploadServer"), params);
    QString upload_url = ((doc["response"])["upload_url"]).toString();
    if(upload_url.isEmpty())
    {
        return result;
    }

    QFile* img = new QFile(imgPath_);
    if(!img->open(QIODevice::ReadOnly))
    {
        return result;
    }

    QByteArray array = img->readAll();
    img->close();

    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("multipart/form-data; name=\"photo\"; filename=\"image.jpg\""));
    imagePart.setRawHeader("Content-Transfer-Encoding", "binary");
    imagePart.setBody(array);

    multiPart->append(imagePart);

    result = VkQuery::postRequest(QNetworkRequest(QUrl(upload_url)), multiPart);
    return result;
}

QJsonDocument Task::saveImage(const QString &group_id) const
{
    QJsonDocument load_ans = loadImage(group_id);

    QMap<QString, QString> params;
    params["group_id"] = group_id;
    params["photo"] = load_ans["photo"].toString();
    params["server"] = QString::number(load_ans["server"].toInt());
    params["hash"] = load_ans["hash"].toString();
    params["access_token"] = accessToken_;
    params["v"] = "5.52";

    QJsonDocument result = VkQuery::makeGetRequest(QUrl("https://api.vk.com/method/photos.saveWallPhoto"), params);

    qDebug() << result;

    return result;
}

void Task::postToWall(const QString& group_id) const
{
    QJsonDocument save_ans = saveImage(group_id);

    QJsonArray response = save_ans["response"].toArray();
    if(response.isEmpty())
    {
        return;
    }

    QJsonObject image = (response.first()).toObject();

    QUrl url("https://api.vk.com/method/wall.post");
    QUrlQuery query(url);

    query.addQueryItem("owner_id", QString("-").append(group_id));
    query.addQueryItem("friends_only", "1");
    query.addQueryItem("message", message_);
    query.addQueryItem("attachments", QString("photo") + QString::number(image["owner_id"].toInt()).append("_") + QString::number(image["id"].toInt()));
    query.addQueryItem("access_token", accessToken_);
    query.addQueryItem("test_mode", "1");
    query.addQueryItem("v", "5.52");

    url.setQuery(query);
    VkQuery::getRequest(QNetworkRequest(url));
}
