#include "ui/dialog_factory.hpp"
#include <obs-frontend-api.h>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTimer>
#include <QApplication>
#include <QClipboard>
#include "core/qr_generator.hpp"
#include "bilibili_api.hpp"

namespace UI {

static QDialog *createBaseDialog(const QString &title, QWidget *parent)
{
	QDialog *dialog = new QDialog(parent);
	dialog->setWindowTitle(title);
	QVBoxLayout *layout = new QVBoxLayout(dialog);
	dialog->setLayout(layout);
	return dialog;
}

void DialogFactory::message(const QString &msg, const QString &title, QWidget *parent)
{
	QDialog *dialog = createBaseDialog(title, parent ? parent : (QWidget *)obs_frontend_get_main_window());
	QVBoxLayout *layout = (QVBoxLayout *)dialog->layout();
	layout->addWidget(new QLabel(msg));
	QPushButton *confirm = new QPushButton("确认");
	layout->addWidget(confirm);
	QObject::connect(confirm, &QPushButton::clicked, dialog, &QDialog::accept);
	QObject::connect(dialog, &QDialog::finished, dialog, &QDialog::deleteLater);
	dialog->exec();
}

QDialog *DialogFactory::qrLogin(QWidget *parent, const std::string &qrData, std::string &qrKey,
				std::function<void(const std::string &cookies)> onSuccess)
{
	QDialog *dialog = createBaseDialog("Bilibili 登录二维码", parent);
	QVBoxLayout *layout = (QVBoxLayout *)dialog->layout();

	QLabel *qrLabel = new QLabel();
	QPixmap pixmap = Core::QrGenerator::generate(qrData);
	if (pixmap.isNull()) {
		qrLabel->setText("无法生成二维码");
	} else {
		qrLabel->setPixmap(pixmap);
	}
	layout->addWidget(qrLabel);
	layout->addWidget(new QLabel("使用手机扫描二维码登录"));

	QTimer *timer = new QTimer(dialog);
	int retryCount = 0;

	auto timerCallback = [&, retryCount, qrKey, onSuccess, timer, qrLabel, dialog]() mutable {
		if (retryCount > 180) {
			timer->stop();
			qrLabel->setText("二维码已过期，请重新打开");
			return;
		}
		++retryCount;

		std::string cookies, message;
		if (Bili::BiliApi::qrLogin(qrKey, cookies, message)) {
			timer->stop();
			if (onSuccess)
				onSuccess(cookies);
			dialog->accept();
		}
	};

	QObject::connect(timer, &QTimer::timeout, timerCallback);
	QObject::connect(dialog, &QDialog::finished, [timer, dialog]() {
		timer->stop();
		dialog->deleteLater();
	});

	timer->start(1000);
	dialog->exec();
	return dialog;
}

QDialog *DialogFactory::streamStarted(QWidget *parent, const std::string &rtmpAddr, const std::string &rtmpCode,
				      bool obsConfigured)
{
	QDialog *dialog = createBaseDialog("消息", parent);
	QVBoxLayout *layout = (QVBoxLayout *)dialog->layout();

	QLabel *status = new QLabel(obsConfigured ? "Bilibili 已开始直播，OBS 直播设置已自动填入。"
						  : "Bilibili 已开始直播，但 OBS 直播设置未自动填入，请手动复制。");
	status->setWordWrap(true);
	layout->addWidget(status);
	layout->addWidget(new QLabel("若自定义推流失败，请使用预设的 B 站推流并更换推流地址尝试。"));

	QLineEdit *addrInput = new QLineEdit(QString::fromStdString(rtmpAddr));
	addrInput->setReadOnly(true);
	addrInput->setCursorPosition(0);
	QLineEdit *codeInput = new QLineEdit(QString::fromStdString(rtmpCode));
	codeInput->setReadOnly(true);
	codeInput->setCursorPosition(0);

	QHBoxLayout *addrRow = new QHBoxLayout();
	addrRow->addWidget(new QLabel("RTMP 地址:"));
	addrRow->addWidget(addrInput);
	QPushButton *copyAddr = new QPushButton("复制地址");
	addrRow->addWidget(copyAddr);
	layout->addLayout(addrRow);

	QHBoxLayout *codeRow = new QHBoxLayout();
	codeRow->addWidget(new QLabel("推流码:"));
	codeRow->addWidget(codeInput);
	QPushButton *copyCode = new QPushButton("复制推流码");
	codeRow->addWidget(copyCode);
	layout->addLayout(codeRow);

	QPushButton *copy = new QPushButton("复制全部");
	QPushButton *confirm = new QPushButton("确认");
	QHBoxLayout *buttonRow = new QHBoxLayout();
	buttonRow->addWidget(copy);
	buttonRow->addWidget(confirm);
	layout->addLayout(buttonRow);

	QObject::connect(copyAddr, &QPushButton::clicked,
			 [=]() { QApplication::clipboard()->setText(QString::fromStdString(rtmpAddr)); });
	QObject::connect(copyCode, &QPushButton::clicked,
			 [=]() { QApplication::clipboard()->setText(QString::fromStdString(rtmpCode)); });
	QObject::connect(copy, &QPushButton::clicked, [=]() {
		QApplication::clipboard()->setText(
			QString("推流地址: %1\n推流码: %2")
				.arg(QString::fromStdString(rtmpAddr), QString::fromStdString(rtmpCode)));
	});
	QObject::connect(confirm, &QPushButton::clicked, dialog, &QDialog::accept);
	QObject::connect(dialog, &QDialog::finished, dialog, &QDialog::deleteLater);

	dialog->exec();
	return dialog;
}

QDialog *DialogFactory::faceAuth(QWidget *parent, const std::string &faceUrl)
{
	QDialog *dialog = createBaseDialog("实名认证（人脸识别）", parent);
	QVBoxLayout *layout = (QVBoxLayout *)dialog->layout();

	QLabel *qrLabel = new QLabel();
	QPixmap pixmap = Core::QrGenerator::generate(faceUrl);
	if (pixmap.isNull()) {
		qrLabel->setText("二维码生成失败，请检查网络或日志");
	} else {
		qrLabel->setPixmap(pixmap);
	}
	qrLabel->setAlignment(Qt::AlignCenter);

	QLabel *tipLabel = new QLabel("请使用<b>手机 Bilibili App</b> 扫描下方二维码完成人脸认证。<br>"
				      "认证完成后，请重新点击开始直播。");
	tipLabel->setWordWrap(true);
	tipLabel->setAlignment(Qt::AlignCenter);

	QPushButton *closeBtn = new QPushButton("我已完成认证");

	layout->addWidget(tipLabel);
	layout->addWidget(qrLabel);
	layout->addWidget(closeBtn);

	QObject::connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
	QObject::connect(dialog, &QDialog::finished, dialog, &QDialog::deleteLater);

	dialog->exec();
	return dialog;
}

QDialog *DialogFactory::roomSettings(QWidget *parent, const std::string &roomUrl, const std::string &currentTitle,
				     int currentAreaId, int currentPartId,
				     std::function<void(const std::string &title, int areaId, int partId)> onApply)
{
	QDialog *dialog = createBaseDialog("更新直播间信息", parent);
	QVBoxLayout *layout = (QVBoxLayout *)dialog->layout();

	layout->addWidget(
		new QLabel(QString("直播间: https://live.bilibili.com/%1").arg(QString::fromStdString(roomUrl))));

	QLineEdit *titleInput = new QLineEdit(QString::fromStdString(currentTitle));
	QPushButton *confirmTitle = new QPushButton("确认");
	QHBoxLayout *titleRow = new QHBoxLayout();
	titleRow->addWidget(new QLabel("直播间标题:"));
	titleRow->addWidget(titleInput);
	titleRow->addWidget(confirmTitle);
	layout->addLayout(titleRow);

	QComboBox *partCombo = new QComboBox();
	QComboBox *areaCombo = new QComboBox();
	QPushButton *confirmPartition = new QPushButton("确认");
	QHBoxLayout *partitionRow = new QHBoxLayout();
	partitionRow->addWidget(new QLabel("直播间分区:"));
	partitionRow->addWidget(partCombo);
	partitionRow->addWidget(areaCombo);
	partitionRow->addWidget(confirmPartition);
	layout->addLayout(partitionRow);

	struct Part {
		int id;
		std::string name;
		std::vector<json11::Json> list;
	};
	std::vector<Part> parts;
	std::string message;
	auto partitionData = Bili::BiliApi::getPartitionList(message);
	if (partitionData.is_array()) {
		for (const auto &item : partitionData.array_items()) {
			parts.push_back(
				{item["id"].int_value(), item["name"].string_value(), item["list"].array_items()});
		}
	}

	size_t selectedPartIndex = 0;
	for (size_t i = 0; i < parts.size(); ++i) {
		partCombo->addItem(QString::fromStdString(parts[i].name), parts[i].id);
		if (parts[i].id == currentPartId)
			selectedPartIndex = i;
	}
	partCombo->setCurrentIndex(static_cast<int>(selectedPartIndex));

	auto updateAreaCombo = [=](int partIndex) {
		areaCombo->clear();
		if (partIndex >= 0 && static_cast<size_t>(partIndex) < parts.size() && !parts[partIndex].list.empty()) {
			size_t selectedAreaIndex = 0;
			for (size_t i = 0; i < parts[partIndex].list.size(); ++i) {
				int id = std::stoi(parts[partIndex].list[i]["id"].string_value());
				QString name = QString::fromStdString(parts[partIndex].list[i]["name"].string_value());
				areaCombo->addItem(name, id);
				if (id == currentAreaId)
					selectedAreaIndex = i;
			}
			areaCombo->setCurrentIndex(static_cast<int>(selectedAreaIndex));
		} else {
			areaCombo->addItem("英雄联盟", 86);
			if (currentAreaId == 86)
				areaCombo->setCurrentIndex(0);
		}
	};
	updateAreaCombo(static_cast<int>(selectedPartIndex));
	QObject::connect(partCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), updateAreaCombo);

	QObject::connect(confirmTitle, &QPushButton::clicked, [=]() {
		std::string newTitle = titleInput->text().trimmed().toUtf8().constData();
		if (!newTitle.empty() && onApply) {
			onApply(newTitle, areaCombo->currentData().toInt(), partCombo->currentData().toInt());
		}
		dialog->accept();
	});

	QObject::connect(confirmPartition, &QPushButton::clicked, [=]() {
		if (onApply) {
			onApply(titleInput->text().trimmed().toUtf8().constData(), areaCombo->currentData().toInt(),
				partCombo->currentData().toInt());
		}
		dialog->accept();
	});

	QObject::connect(dialog, &QDialog::finished, dialog, &QDialog::deleteLater);
	dialog->exec();
	return dialog;
}

} // namespace UI
