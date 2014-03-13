/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 */

#ifndef CERTINFOCONTROL_HPP_
#define CERTINFOCONTROL_HPP_

#include <bb/cascades/QListDataModel>

#include <QtCore/QObject>
#include <QtNetwork/QSslCertificate>

/**
 * The CertInfoControl class is a dialog that shows information
 * about the certificate provided by the server and the
 * certificate chain.
 */
class CertInfoControl: public QObject {
	Q_OBJECT

	// Creates a list of certificates (certificate chain) and
	// makes it available as a DataModel to the UI
	Q_PROPERTY(bb::cascades::DataModel* model READ model CONSTANT)

	// Creates a textual representation of the selected certificate
	// when selected in the UI
	Q_PROPERTY(QString certificateInfo
        READ certificateInfo NOTIFY certificateInfoChanged)

	// Reports whether this dialog is visible or not
	Q_PROPERTY(bool visible
		READ visible WRITE setVisible NOTIFY visibleChanged)

public:
	CertInfoControl();

	// The accessor methods of the properties
	bb::cascades::DataModel* model() const;
	QString certificateInfo() const;
	void setVisible(bool visible);
	bool visible() const;

public Q_SLOTS:
	// Called to show the dialog
	void onViewCertificateChain();

	// Called to set the certificate chain that's displayed
    // in the dialog
	void onCertificateChainChanged(const QList<QSslCertificate> &chain);

	// Called when the user selects a certificate in the ListView
	void setCurrentCertificate(const QVariantList &indexPath);

signals:
	// The change notification signals of the properties
	void certificateInfoChanged();
	void visibleChanged();

private:
	// The certificates of the certificate chain
	QList<QSslCertificate> m_chain;

	// The DataModel that provides the certificate chain
    // to the ListView
	bb::cascades::QListDataModel<QString> m_model;

	// The textual representation of the selected certificate
	QString m_certificateInfo;

	bool m_dialogVisible;
};

#endif /* CERTINFOCONTROL_HPP_ */
